


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAModel.h"
#include "ccipdCore.h"
#include "ccipdSIMStorage.h"
#include "ccipdPatients.h"       // our training data
#include "ccipdPatient.h"        // our training data
#include "ccipdStudy.h"          // our training data
#include "ccipdMFAInvalidStudies.h" // displaying info
#include "ccipdMFAMaskAligner.h"
#include "ccipdMFALevelsetExtractor.h"
#include "ccipdMFATextureExtractor.h"
#include "ccipdSIMFeaturePreparerUsingExtractors.h"
#include "ccipdSIMEstimator.h"
#include "ccipdMFABoundingBoxTraining.h"
#include "ccipdDistribution.h"
#include "ccipdMultivariateGaussianDistribution.h"

// std includes
  #include <fstream>

// boost includes
#include <boost/foreach.hpp>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/serialization/nvp.hpp>
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include "ccipdSharedPointerSerialization.hxx"
#endif // Boost_SERIALIZATION_FOUND

#ifdef _MSC_VER
#pragma warning( disable: 4355 )
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
class MFAModel::SecretData
{

public:
  SecretData( );
  
  bool
    m_Verbose;

  unsigned int
    m_MinimumBoundingBoxes;
  MFAMaskAlignerPointer
    m_MaskAligner;
  MFALevelsetExtractorPointer
    m_LevelsetExtractor;
  MFATextureExtractorPointer
    m_TextureExtractor;
  AffineDistributionConstPointer
    m_BoundingBoxDistribution;
}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAModel::MFAModel() :
ccipdNewPIMPLMacro
{

  // set items for our SIMModel
  this->Superclass::SetBoundingBoxExtractor( this->m_SecretData->m_MaskAligner );
  this->Superclass::SetStudiesAligner( this->m_SecretData->m_MaskAligner );
  this->Superclass::SetVerbose( this->m_SecretData->m_Verbose );

  SIMFeatureExtractorCollection extractors;
  extractors.push_back( this->m_SecretData->m_LevelsetExtractor );
  extractors.push_back( this->m_SecretData->m_TextureExtractor  );
  this->Superclass::SetExtractors( extractors );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAModel::~MFAModel()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAModel::SecretData::SecretData():
m_Verbose( true ),
m_MinimumBoundingBoxes( 3 ),
m_MaskAligner      ( MFAMaskAligner::New() ),
m_LevelsetExtractor( MFALevelsetExtractor::New() ),
m_TextureExtractor ( MFATextureExtractor::New() ),
m_BoundingBoxDistribution( AffineGaussianDistributionType::New() )
{
  this->m_LevelsetExtractor->SetAligner             ( this->m_MaskAligner );
  this->m_LevelsetExtractor->SetBoundingBoxExtractor( this->m_MaskAligner );
  this->m_TextureExtractor ->SetAligner             ( this->m_MaskAligner );
  this->m_TextureExtractor ->SetBoundingBoxExtractor( this->m_MaskAligner );
  
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
bool MFAModel::UpdateStudiesAligner( const Patients & trainingData )
{
  // super class only tries to find the alignment patient
  const bool hasAlignmentPatient = this->Superclass::UpdateStudiesAligner( trainingData );

  // if we do not have the alignment patient just pick a random study
  if (! hasAlignmentPatient )
    {
    const StudyPointer alignmentStudy =  *(this->GetTrainingStudies( trainingData )).cbegin();
    
    MFAMaskAlignerPointer maskAligner = this->m_SecretData->m_MaskAligner;
    maskAligner->SetTemplateStudy( alignmentStudy );
    }

  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType MFAModel::GetTrainingStudies( const Patients & trainingData )
{
  // extract our objects
  const MaskObjectCollectionType
    oldObjects =
      this->m_SecretData->m_LevelsetExtractor->GetWhichLevelsetObjects(),
    objectNames =
      oldObjects.empty()               ? // Do we have a list?
      trainingData.GetAllMaskObjects() : // If not, use all objects.
      oldObjects                       ; // If so, use the list.

  const bool verbose = this->m_SecretData->m_Verbose;

  // you need some masks!
  if ( objectNames.empty() )
    {
    if ( verbose )
      cerr << "No mask objects to train MFA!" << endl;
    return StudyCollectionType();
    }
  
  // extract the mask aligner
  const MFAMaskAlignerPointer
    maskAligner = this->m_SecretData->m_MaskAligner;

  // update our lists
  this->m_SecretData->m_LevelsetExtractor->SetWhichLevelsetObjects( objectNames );
  maskAligner->SetBoundingBoxMaskObjects( objectNames );

  // which object will we be aligning?
  const MaskObjectType
    oldAlignmentObject = maskAligner->GetObjectToAlign(),
    alignmentObject =
      objectNames.find( oldAlignmentObject ) == objectNames.end() ?
      *objectNames.cbegin() : // If it's an invalid object, just pick a random one.
      oldAlignmentObject    ; // Use the input alignment object.

  // update our stored alignment object
  maskAligner->SetObjectToAlign( alignmentObject );

  // extract studies with the desired objects
  const StudyCollectionType trainingStudies =
    trainingData.GetStudiesWithMaskObjects( objectNames );

  // display info about which studies are invalid
  if ( verbose )
    {
    MFADisplayInvalidStudies( trainingData, trainingStudies );
    } // verbose

  return trainingStudies;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool MFAModel::TrainStudies( const StudyCollectionType & studies )
{
  
  // extract the mask aligner
  const MFAMaskAlignerPointer maskAligner = this->m_SecretData->m_MaskAligner;

  std::cout << "Randomize = " << maskAligner->GetRandomize() << std::endl;
  
  // now train the studies
  const bool isTrained = this->Superclass::TrainStudies( studies );

  std::cout << "Randomize = " << maskAligner->GetRandomize() << std::endl;
  
  // do some MFA specific training
  const bool verbose = this->m_SecretData->m_Verbose;

  // get the bounding box distribution
  if ( verbose ) cout << "Calculating bounding box distribution:" << endl;
  const unsigned int
    minimumBoundingBoxes = this->m_SecretData->m_MinimumBoundingBoxes,
    numberOfStudies      = static_cast< unsigned int >( studies.size() ),
    repeatsPerStudy      = minimumBoundingBoxes > 0 ?
      ( minimumBoundingBoxes - 1 ) / numberOfStudies + 1 : 0;

  if ( verbose )
    cout <<
      "Minimum bounding boxes = " << minimumBoundingBoxes << endl <<
      "# of studies = " << numberOfStudies << endl <<
      "Repeats per study = " << repeatsPerStudy << endl;
      
  
  std::cout << "Randomize = " << maskAligner->GetRandomize() << std::endl;
  
  maskAligner->VerboseOff();
  
  std::cout << "Randomize = " << maskAligner->GetRandomize() << std::endl;
  
  this->m_SecretData->m_BoundingBoxDistribution = GetAffineDistribution(
      studies, *maskAligner, *maskAligner, repeatsPerStudy );
  if ( verbose ) cout << "Calculating bounding box distribution done." << endl;
  
  maskAligner->SetVerbose( verbose );
  
  return isTrained;
} // TrainStudies
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool MFAModel::operator==( const Hashable & other ) const
{

  // cast and confirm
  const MFAModel & otherModel =
    dynamic_cast< const MFAModel & >( other );
  if ( &otherModel == this )
    return true;

  // check our variables
  //return *( this->m_SecretData ) == *( otherModel.m_SecretData );

  // TODO
  return false;

} // operator==
//////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
HashValueType MFAModel::ReHash() const
{

  // TODO
  return 1;

} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxExtractorConstPointer MFAModel::GetBoundingBoxExtractor() const
{
  return this->m_SecretData->m_MaskAligner;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdGetConstImplementMacro( MFAModel,
  LevelsetExtractor, MFALevelsetExtractorConstPointer )
ccipdGetConstImplementMacro( MFAModel,
  TextureExtractor, MFATextureExtractorConstPointer )
ccipdGetConstImplementMacro( MFAModel,
  BoundingBoxDistribution, AffineDistributionConstPointer )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

namespace ccipd
{

ccipdSerializationImplementationHeadingMacro( MFAModel )
{

  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  // these have not changes between the version
  archive
    & make_nvp( "Verbose"                , this->m_SecretData->m_Verbose                 )
    & make_nvp( "MinimumBoundingBoxes"   , this->m_SecretData->m_MinimumBoundingBoxes    )
    & make_nvp( "MaskAligner"            , this->m_SecretData->m_MaskAligner             )
    & make_nvp( "LevelsetExtractor"      , this->m_SecretData->m_LevelsetExtractor       )
    & make_nvp( "TextureExtractor"       , this->m_SecretData->m_TextureExtractor        )
    & make_nvp( "BoundingBoxDistribution", this->m_SecretData->m_BoundingBoxDistribution )
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass );

  // update our superclass
  this->Superclass::SetBoundingBoxExtractor( this->m_SecretData->m_MaskAligner );
  this->Superclass::SetStudiesAligner      ( this->m_SecretData->m_MaskAligner );
  
  SIMFeatureExtractorCollection extractors;
  extractors.push_back( this->m_SecretData->m_LevelsetExtractor );
  extractors.push_back( this->m_SecretData->m_TextureExtractor  );
  this->Superclass::SetExtractors( extractors );
  
} // serialization

ccipdSerializationXMLMacro( MFAModel )

} // namespace ccipd

#endif // Boost_SERIALIZATION_FOUND
