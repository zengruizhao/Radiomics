


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMModel.h"
#include "ccipdCore.h"
#include "ccipdSIMStorage.h"
#include "ccipdPatients.h"       // our training data
#include "ccipdPatient.h"        // our training data
#include "ccipdStudy.h"          // our training data
#include "ccipdSIMFeaturePreparerUsingExtractors.h"
#include "ccipdSIMEstimator.h"
#include "ccipdSIMStudiesAligner.h"
#include "ccipdSIMBoundingBoxExtractor.h"
#include "ccipdEnableSharedFromThis.hxx"

// boost includes
#include <boost/foreach.hpp>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/serialization/nvp.hpp>
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/vector.hpp>
  #include "ccipdSIMFeatureExtractor.h"
  #include "ccipdSharedPointerSerialization.hxx"
  #include "ccipdDistribution.h"
  #include "ccipdMultivariateGaussianDistribution.h"
  #include <fstream>
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
class SIMModel::SecretData
{

public:

  SecretData(); ///< Constructor
  
  bool
    m_Verbose,
    m_ExcludeTemplateStudy;
  SIMVolumeStoragePointer
    m_TrainedSIM;
  SIMStudiesAlignerPointer
    m_StudiesAligner;
  SIMBoundingBoxExtractorPointer
    m_BoundingBoxExtractor;
  SIMFeatureExtractorCollection
    m_Extractors;
  PatientIDType
    m_AlignmentPatient;
    
}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMModel::SecretData::SecretData() :
m_Verbose( true ),
m_ExcludeTemplateStudy( false ),
m_TrainedSIM( SIMVolumeStorage::New() )
{
} // SecretData Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMModel::SIMModel() :
ccipdNewPIMPLMacro
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMModel::~SIMModel()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool SIMModel::Train( const Patients & trainingData )
{

  // let the child class determine the best way to get the training studies
  const StudyCollectionType trainingStudies = 
    this->GetTrainingStudies( trainingData );

  // you need some training studies!
  if ( trainingStudies.empty() )
    {
    if ( this->m_SecretData->m_Verbose )
      cerr << "No studies to train SIM!" << endl;
    return false;
    }

  // you need to give the alignment patient to the study aligner
  const bool setAlignmentPatient = this->UpdateStudiesAligner( trainingData );

  if ( !setAlignmentPatient )
    {
    if ( this->m_SecretData->m_Verbose )
      cerr << "Unable to update studies aligner." << endl;
    return false;
    }

  // now try to train with our studies
  const bool trained = this->TrainStudies( trainingStudies );

  return trained;
} // Train
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool SIMModel::UpdateStudiesAligner( const Patients & trainingData )
{
  const bool verbose = this->m_SecretData->m_Verbose;
  SIMStudiesAlignerPointer & studiesAligner = this->m_SecretData->m_StudiesAligner;
  studiesAligner->SetVerbose( verbose );

  const PatientPointer alignmentPatient = 
    trainingData.GetPatientWithID( this->m_SecretData->m_AlignmentPatient );

  // if we cannot find the patient return false
  if ( !alignmentPatient )
    {
    if ( verbose ) 
      cerr << "No patient for alignment template" << endl;
    return false;
    }

  // we found the patient so extract a study and return true.
  studiesAligner->SetTemplateStudy( alignmentPatient->GetOneStudy() );

  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool SIMModel::TrainStudies( const StudyCollectionType & inputStudies )
{

  const bool verbose = this->m_SecretData->m_Verbose;

  // extract the mask aligner
  SIMStudiesAlignerPointer & studiesAligner = this->m_SecretData->m_StudiesAligner;
  studiesAligner->SetVerbose( verbose );

  // optionally remove the template study
  // from the training studies
  
  StudyCollectionType studies;
  const bool excludeTemplateStudy = this->GetExcludeTemplateStudy();
  if ( excludeTemplateStudy )
    {

    const StudyConstPointer templateStudy =
      studiesAligner->GetTemplateStudy();
    if ( templateStudy )
      {

      BOOST_FOREACH( const StudyPointer study, inputStudies )
        {

        // if it's not equal
        if ( study && !( *study == *templateStudy ) )
          {
          studies.insert( study );
          }

        } // for each study
      } // templateStudy
    else
      {
      // include all
      studies = inputStudies;
      }  

    } // exclude
  else
    {
    // include all
    studies = inputStudies;
    }

  // give it all the studies
  studiesAligner->ClearStudies();
  studiesAligner->AddStudies( studies );

  // now register
  if ( verbose ) cout << "Registering all studies:" << endl;
  studiesAligner->RegisterAllStudies();
  if ( verbose ) cout << "Registering all studies done." << endl;
  
  // prepare to extract the features
  const SIMFeaturePreparerUsingExtractors::Pointer
    featurePreparerPointer = SIMFeaturePreparerUsingExtractors::New();
  SIMFeaturePreparerUsingExtractors
    & featurePreparer = *featurePreparerPointer;

  featurePreparer.SetVerbose( verbose );
  featurePreparer.SetStudyAligner( studiesAligner );
  featurePreparer.SetBoundingBoxExtractor( this->m_SecretData->m_BoundingBoxExtractor );
  featurePreparer.AddExtractors( this->m_SecretData->m_Extractors );

  // prepare for an "SIM estimator" to receive the features
  const SIMEstimator::Pointer estimatorPointer = SIMEstimator::New();
  SIMEstimator & estimator = *estimatorPointer;

  estimator.SetVerbose( verbose );
  estimator.SetWhichFeatures( featurePreparer.GetWhichFeatures() );

  // loop through each study again
  BOOST_FOREACH( const StudyPointer & study, studies )
    {
    
    // give the features for this study
    // to the estimator
    if ( verbose ) cout <<
      "Adding training features to estimator from study " <<
      study->GetDescription() << ":" << endl;
    estimator.AddTrainingFeatures( featurePreparer.GetFeatures( study ) );
    if ( verbose ) cout <<
      "Adding training features to estimator from study " <<
      study->GetDescription() << " done." << endl;

    } // for each study

  // generate the SIM from the training features
  if ( verbose ) cout << "Generating SIM:" << endl;
  const SIMVolumeStoragePointer newSIM = estimator.CalculateSIM();
  if ( verbose ) cout << "Generating SIM done." << endl;

  if ( !newSIM )
    {
    cerr << "Error: No SIM trained." << endl;
    return false;
    }

  // output

  // keep the original output folder
  if ( this->m_SecretData->m_TrainedSIM )
    {
    newSIM->SetOutputFolder( this->m_SecretData->m_TrainedSIM->GetOutputFolder() );
    }
  
  // assign it and we're done
  this->m_SecretData->m_TrainedSIM = newSIM;
  return true;

} // TrainStudies
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool SIMModel::operator==( const Hashable & other ) const
{

  // cast and confirm
  const SIMModel & otherModel =
    dynamic_cast< const SIMModel & >( other );
  if ( &otherModel == this )
    return true;

  // check our variables
  //return *( this->m_SecretData ) == *( otherModel.m_SecretData );

  // TODO
  return false;

} // operator==
//////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
HashValueType SIMModel::ReHash() const
{

  // TODO
  return 1;

} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( SIMModel, Verbose             , bool )
ccipdSetByvalImplementMacro( SIMModel, ExcludeTemplateStudy, bool )
ccipdGetConstImplementMacro( SIMModel, ExcludeTemplateStudy, bool )
ccipdSetByrefImplementMacro( SIMModel, AlignmentPatient, PatientIDType )
ccipdSetByrefImplementMacro( SIMModel, 
  BoundingBoxExtractor, SIMBoundingBoxExtractorPointer )
ccipdSetByrefImplementMacro( SIMModel, 
  StudiesAligner      , SIMStudiesAlignerPointer       )
ccipdSetByrefImplementMacro( SIMModel, 
  Extractors          , SIMFeatureExtractorCollection  )
ccipdGetConstImplementMacro( SIMModel, 
  AlignmentPatient    , PatientIDType )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMVolumeStoragePointer SIMModel::GetTrainedSIM() const
{
  // make sure we've trained
  if ( !this->GetIsTrained() )
    return SIMVolumeStoragePointer();

  // get our trained model
  return m_SecretData->m_TrainedSIM;

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool SIMModel::GetIsTrained() const
{
  return static_cast< bool >( m_SecretData->m_TrainedSIM );
}
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

namespace ccipd
{

ccipdSerializationImplementationHeadingMacro( SIMModel )
{

  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  archive
    & make_nvp( "Verbose"                , this->m_SecretData->m_Verbose                 )
    & make_nvp( "AligmentPatient"        , this->m_SecretData->m_AlignmentPatient        )
    & make_nvp( "StudiesAligner"         , this->m_SecretData->m_StudiesAligner          )
    & make_nvp( "BoundingBoxExtractor"   , this->m_SecretData->m_BoundingBoxExtractor    )
    & make_nvp( "TrainedSIM"             , this->m_SecretData->m_TrainedSIM              );

  if ( version >= 2 )
    {
    archive
      & make_nvp( "ExcludeTemplateStudy", this->m_SecretData->m_ExcludeTemplateStudy )
      & make_nvp( "Extractors"          , this->m_SecretData->m_Extractors           );
    }

} // serialization

ccipdSerializationXMLMacro( SIMModel )

} // namespace ccipd

#endif // Boost_SERIALIZATION_FOUND
