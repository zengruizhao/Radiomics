


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMFASegmenter.h"
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdRegisterMFAAlgorithm.h"
#include "ccipdRegisterMFARegularizedMetric.h"
#include "ccipdMFAGetReconstruction.h"
#include "ccipdMFATransformedReconstruction.h"
#include "ccipdMFALevelsetExtractor.h" // for feature names
#include "ccipdMFAPrepareFilter.h"
#include "ccipdMFAModel.h"           // our trained model
#include "ccipdSIMStorage.h"            // our trained model
#include "ccipdStudy.h"              // for our new study to be segmented
#include "ccipdFeatureID.h"          // for defining a feature to reconstruct
#include "ccipdLevelsets.h"          // for converting masks to levelsets
#include "ccipdSIMBoundingBoxExtractor.h"
#include "ccipdRegisterBoundingBoxes.h"
#include "ccipdDistribution.h"
#include "ccipdDuplicateTransform.h"

// itk includes
#include "itkMFAReconstructionFilter.h"

// eigen includes
#include <Eigen/Core>

// serialization includes
#include "ccipdSegmentationOptions.h"

#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/serialization/nvp.hpp>
  #include <boost/serialization/vector.hpp>
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include "ccipdSharedPointerSerialization.hxx"
  #include <fstream>
#endif // Boost_SERIALIZATION_FOUND

// boost includes
#include <boost/foreach.hpp>

#include "ccipdDisableWarningsMacro.h"
  #include <itkTransform.h>
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( disable: 4355 )
#pragma warning( disable: 4482 )
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
class MFASegmenter::SecretData
{

public:
  SecretData(); ///< Constructor

  MFAModelConstPointer
    m_TrainedModel;

  MFADrivingFeaturePreparerPointer
    m_DrivingFeaturePreparer;

  MaskObjectType
    m_SegmentationObject;

  string
    m_TrainedMFAFile;

  bool
    m_ReconstructFeaturesInMoved,
    m_Initialized,    ///< For loading a model from a file.
    m_HasBoundingBox, ///< Whether a bounding box was assigned.
    m_Verbose;

  MFABoundingBoxType
    m_BoundingBox;

  RegisterMFAAlgorithmCollection
    m_RegistrationAlgorithms;

  // okay this is a little confusing because the RegisterMFAAlgorithmTransformType is now our RegisterTransformLinearCombo and m_FinalTransform should be an itk::Transform...
  typedef RegisterMFAAlgorithm::TransformType::TransformType 
    TransformType;

  typedef RegisterMFAAlgorithm::TransformType::TransformConstPointer
    TransformConstPointer;
  
  TransformConstPointer
    m_FinalTransform;

  MFAVolumeReconstructionFilterPointer
    m_ReconstructionFilter;

  HashValueType
    m_SegmentedStudyHash;
  
  StudyPointer
    m_MovedStudy;

  /// Return a template image from the algorithms.
  VolumeImageBaseConstPointer GetTemplateImage() const;
  
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenter::MFASegmenter() :
ccipdNewPIMPLMacro
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenter::~MFASegmenter()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenter::SecretData::SecretData() :
m_DrivingFeaturePreparer( MFADrivingFeaturePreparer::New() ),
m_ReconstructFeaturesInMoved( true  ),
m_HasBoundingBox            ( false ),
m_Verbose                   ( true  ),
m_SegmentedStudyHash( 12345 ) // random value
{
  this->m_RegistrationAlgorithms.push_back(
    RegisterMFAAlgorithmPointer( RegisterMFAAlgorithm::New() ) );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationMacro( MFASegmenter )
ccipdSerializationXMLMacro           ( MFASegmenter )

ccipdSerializationImplementationHeadingMacro( MFASegmenter::SecretData )
{

  using boost::serialization::make_nvp;

  if ( version < 8 )
    {
    cerr << "Error: Unable to serialize versions prior to 8." << endl;
    return;
    }

  archive
    & make_nvp( "TrainedMFAFile"        , this->m_TrainedMFAFile         )
    & make_nvp( "SegmentationObject"    , this->m_SegmentationObject     )
    & make_nvp( "RegistrationAlgorithms", this->m_RegistrationAlgorithms )
    & make_nvp( "DrivingFeatures"       , this->m_DrivingFeaturePreparer );

  if ( version >= 9 )
    {
    archive & make_nvp(
      "ReconstructFeaturesInMoved", this->m_ReconstructFeaturesInMoved );
    }

  // no longer initialized (just to be safe)
  this->m_Initialized = false;

} // serialize

ccipdSerializationXMLMacro( MFASegmenter::SecretData )

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool MFASegmenter::Segment( const StudyConstPointer & inputStudy )
{

  this->Initialize();
  const SegmentationModelConstPointer
    model( this->m_SecretData->m_TrainedModel );

  if ( model )
    return this->Segment( inputStudy, model );
  else
    return false;
  
} // Segment
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool MFASegmenter::Resegment( const StudyConstPointer & inputStudy )
{

  this->Initialize();
  const SegmentationModelConstPointer
    model( this->m_SecretData->m_TrainedModel );

  if ( model )
    return this->Resegment( inputStudy, model );
  else
    return false;
  
} // Resegment
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenter::Initialize()
{

  // try to load a mfa model
  const bool verbose = this->GetVerbose();

  // do we need to initialize?
  if ( this->m_SecretData->m_Initialized )
    {
    cout << "Note: MFA segmenter already initialized." << endl;
    return;
    } // initialized

  const string & fileName = this->GetTrainedMFAFile();

  if ( verbose )
    cout << "Loading trained MFA model from " << fileName << ":" << endl;

  // load a model from the file
  const bool returnNULLOnError = true;
  const MFAModelPointer model =
    MFAModel::New( fileName, returnNULLOnError );

  if ( model )
    {
    if ( verbose )
      cout << "Loading trained MFA model from " << fileName << " done." << endl;
    }
  else
    {
    if ( verbose )
      cout << "Loading trained MFA model from " << fileName << " failed." << endl;
    }

  this->m_SecretData->m_TrainedModel = model;

  this->m_SecretData->m_Initialized = true;

} // Segment
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool MFASegmenter::Segment(
  const StudyConstPointer             & inputStudy,
  const SegmentationModelConstPointer & trainedModel )
{
  
  // do we already have reconstructions calculated?
  const MaskObjectType segmentationObject = this->GetSegmentationObject();
  const bool alreadySegmented =
    this->GetSegmentation( segmentationObject, inputStudy ).IsNotNull();

  if ( alreadySegmented )
    {
    return true;
    }
  else
    {
    return this->Segment( inputStudy, trainedModel );
    }

} // Segment
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool MFASegmenter::Resegment(
  const StudyConstPointer             & inputStudy,
  const SegmentationModelConstPointer & trainedModel )
{

  const bool verbose = this->GetVerbose();

  // cast it to the correct type
  const MFAModelConstPointer trainedMFAModel =
    std::dynamic_pointer_cast< const MFAModel >( trainedModel );

  // check to make sure we actually had input an MFA model type
  // using the dynamic cast
  if ( !trainedMFAModel )
    return false;
    
  // extract our trained MFA
  const MFAVolumeBaseConstPointer trainedMFA = trainedMFAModel->GetTrainedSIM();

  if ( !trainedMFA )
    return false;

  // make sure we have some features
  const FeatureIDConstCollection allFeatures = trainedMFA->GetFeatureIDs();
  if ( allFeatures.empty() )
    return false;

  // cache it for other functions
  this->m_SecretData->m_TrainedModel = trainedMFAModel;

  const MFADrivingFeaturePreparerPointer & drivingFeatures =
    this->m_SecretData->m_DrivingFeaturePreparer;

  drivingFeatures->SetMFAModel( trainedMFAModel );

  // we must perform a full segmentation on this study
    
  // start with the bounding box for this study
  MFABoundingBoxType boundingBox;

  typedef MFABoundingBoxExtractor::BoundingBoxImageConstPointer
    BoundingBoxImageConstPointer;

  BoundingBoxImageConstPointer
    boundingBoxImage;

  // see if one has been assigned to the segmenter
  const bool hasInputBoundingBox = this->m_SecretData->m_HasBoundingBox;

  if ( hasInputBoundingBox )
    {
    boundingBox = this->m_SecretData->m_BoundingBox;
    if ( verbose )
      cout << "Note: Using manually input bounding box." << endl <<
      "Bounding box = " << endl << boundingBox << endl;
    boundingBoxImage = inputStudy->GetVolume();
    }
  else // !hasInputBoundingBox
    {

    // see if we have an extractor
    const MFABoundingBoxExtractorConstPointer boundingBoxExtractor =
      trainedMFAModel->GetBoundingBoxExtractor();

    if ( boundingBoxExtractor )
      {
      if ( verbose ) cout << "Getting bounding box from extractor:" << endl;
      boundingBox      = boundingBoxExtractor->GetBoundingBox     ( inputStudy );
      boundingBoxImage = boundingBoxExtractor->GetBoundingBoxImage( inputStudy );
      if ( verbose ) cout << "Getting bounding box done." << endl;
      }

    } // hasInputBoundingBox

  // were we able to find a bounding box?
  const bool hasBoundingBox =
    boundingBoxImage && boundingBox.GetNumberOfPixels() > 0;
    
  if ( hasBoundingBox && verbose )
    cout << "Bounding box to use = " << endl << boundingBox << endl;
      
  // start with an empty transform
  typedef SecretData::TransformConstPointer TransformConstPointer;
  this->m_SecretData->m_FinalTransform = TransformConstPointer();
    
  // if we have a bounding box, we can use it to initialize the transform
  VolumeAffineTransformConstPointer intermediateTransform;

  // if the study has a transform, we can use it to initialize
  if ( inputStudy->HasAffineTransform() )
    {
      
    this->m_SecretData->m_FinalTransform =
      inputStudy->GetAffineTransform();

    if ( verbose )
      cout << "Note: Using study's affine transform to initialize." << endl <<
      "Initial transform = " << endl << this->m_SecretData->m_FinalTransform << endl;
      
    }
  else if ( hasBoundingBox )
    {

    // if no affine transform is used, try the bounding box
      
    // extract the full "moving" bounding box
    // which is just the bounding box
    // of the entire, buffered, trained MFA
    const FeatureIDConstPointer anyFeature = *allFeatures.cbegin();
      
    // they are all the same so just pick one
    const BoundingBoxImageConstPointer
      movingImage( trainedMFA->GetMeanAndEigenvectors( anyFeature ) ),
      fixedImage ( boundingBoxImage );

    if ( movingImage && fixedImage )
      {

      // extract the bounding boxes
      const MFABoundingBoxType
        movingBoundingBox = movingImage->GetBufferedRegion(),
        fixedBoundingBox  = boundingBox;

      if ( verbose )
        cout << "Using full MFA bounding box." << endl <<
        "Moving bounding box = " << endl << movingBoundingBox << endl <<
        "Fixed bounding box = "  << endl << fixedBoundingBox  << endl;
          
      // extract the transform mapping the moving (MFA's)
      // bounding box to the fixed (new study's) bounding box
      TransformConstPointer intermediateTransformTemp( GetTransform(
        fixedBoundingBox, movingBoundingBox, *fixedImage, *movingImage ) );

      // try to cast it to the correct type-- this needs to be based on what the user input
      intermediateTransform = VolumeAffineTransformConstPointer(
        dynamic_cast< const VolumeAffineTransformType * >(
          intermediateTransformTemp.GetPointer() ) );

      if ( verbose &&  intermediateTransform )
        cout << "Calculated intermediate transform = " << endl <<
        intermediateTransform << endl;
          
      } // movingImage && fixedImage

    } // hasBoundingBox

  // if we have an intermediate transform and
  // a distribution, we can use it to initialize
  // our transformation
  const AffineDistributionConstPointer boundingBoxDistribution(
    trainedMFAModel->GetBoundingBoxDistribution() );
    
  if ( intermediateTransform )
    {
    // just start with the bounding boxes
    // lining up
    this->m_SecretData->m_FinalTransform = intermediateTransform.GetPointer();
    } // intermediateTransform 

  if ( boundingBoxDistribution && intermediateTransform )
    {

    // we have T_actual( point ) = T_calculated( T_intermediate( point ) )

    // we also have a distribution of T_calculated
    // and T_intermediate

    // get the most likely calculated transform
    const auto meanCalculatedTransformParametersVector =
      boundingBoxDistribution->GetMode();

    if( meanCalculatedTransformParametersVector )
      {

      if ( verbose )
        cout << "Mean transform distribution parameters = " << endl <<
        meanCalculatedTransformParametersVector << endl;
        
      // we now must apply T_calculated( T_intermediate )

      // create some parameters for T_calculated
      VolumeAffineTransformType::ParametersType
        calculatedTransformParameters( AffineParametersCount );
      calculatedTransformParameters.SetData(
        meanCalculatedTransformParametersVector->data() );

      // create a transform T_calculated
      const VolumeAffineTransformPointer
        calculatedTransform = VolumeAffineTransformType::New();
      calculatedTransform->SetParametersByValue(
        calculatedTransformParameters );

      if ( verbose )
        cout << "Calculated transform = " << endl << calculatedTransform << endl;
        
      // initialize with the calculated mean transform
      const VolumeAffineTransformPointer
        actualInitialTransform( dynamic_cast< VolumeAffineTransformType * >(
          DuplicateTransform( calculatedTransform ).GetPointer() ) );

      // we first want to apply calculatedTransform
      // so therefore we don't want to "pre"-compose it
      // with the mean transform

      const bool preCompose = false;
      actualInitialTransform->Compose( intermediateTransform, preCompose );
            
      if ( verbose )
        cout << "Actual initial transform = " << endl << actualInitialTransform << endl;
          
      // set it
      this->m_SecretData->m_FinalTransform =
        actualInitialTransform.GetPointer();

#ifndef NDEBUG
      // create a random point
      VolumePointType randomPoint;
      randomPoint[ 0 ] = 1;
      randomPoint[ 1 ] = 3;
      randomPoint[ 2 ] = 4;

      // try to transform
      const VolumePointType
        transformedPoint1 =
          intermediateTransform->TransformPoint(
            calculatedTransform->TransformPoint( randomPoint ) ),
        transformedPoint2 =
          actualInitialTransform->TransformPoint( randomPoint );

      assert( std::abs( transformedPoint1[ 0 ] - transformedPoint2[ 0 ] ) < 1e-4 );

#endif // NDEBUG

      } // meanCalculatedTransformParametersVector

    } // initializing transform

  RegisterMFAAlgorithmPointer lastAlgorithm;

  // loop through the algorithms
  BOOST_FOREACH( const RegisterMFAAlgorithmPointer & algorithm,
    this->m_SecretData->m_RegistrationAlgorithms )
    {

    // give everything to the algorithm
    algorithm->SuperclassMetric::SetVerbose  ( verbose );
    algorithm->SetMovingStudy                ( inputStudy );
    algorithm->SetDrivingFeatures      ( this->GetDrivingFeaturePreparer() );

    // get and set metric specific measures
    const RegisterMFARegularizedMetricPointer registerMetric = 
      std::dynamic_pointer_cast < RegisterMFARegularizedMetric >( algorithm->GetMetric() );
    registerMetric->SetDistribution         ( boundingBoxDistribution );
    registerMetric->SetIntermediateTransform( intermediateTransform );
      

  RegisterMFAAlgorithm::OutputPointer studyOutput = algorithm->GetStudyOutput();
    studyOutput->SetReconstructAllFeatures(
      this->GetReconstructFeaturesInMoved() );

  typedef RegisterMFAAlgorithm::OptimizerType AlgorithmOptimizerType;
  RegisterMFAAlgorithm::OptimizerPointer registerOptimizer = 
    std::dynamic_pointer_cast< AlgorithmOptimizerType > ( algorithm->GetOptimizer() );

    registerOptimizer->SetWhichParticleSwarm(
      AlgorithmOptimizerType::WhichParticleSwarm::InitializationBiased );
      
    // initialize the transform if we have one
    const TransformConstPointer
      currentTransform = this->m_SecretData->m_FinalTransform;
    
    if ( currentTransform )
      {
      RegisterMFAAlgorithm::TransformPointer transform(
        std::dynamic_pointer_cast< RegisterMFAAlgorithm::TransformType > ( 
          algorithm->SuperclassMetric::GetTransform() ) );

      if ( transform ) // make sure we have a transform class
        transform->SetInitialTransform( currentTransform );
      }
    // register
    algorithm->Register();

    // update the final transform
    // but casted as a linear transform
    this->m_SecretData->m_FinalTransform =
      dynamic_cast< const SecretData::TransformType * >(
      algorithm->SuperclassMetric::GetFinalTransform().GetPointer() ); // this should go away once output is transformed over

    lastAlgorithm = algorithm;

    } // for each algorithm

  if ( verbose )
    {
    cout << "Segmentation for study " << inputStudy->GetHash() <<
      " complete." << endl;
    cout << "Final transform = " << endl <<
      this->m_SecretData->m_FinalTransform << endl;
    } // verbose
      
  // update the study hash
  this->m_SecretData->m_SegmentedStudyHash =
    inputStudy->GetHash();
    
  // create a filter with the final output
  SecretData::TransformConstPointer finalTransform = this->m_SecretData->m_FinalTransform;

  if ( lastAlgorithm && finalTransform )
    {
    if ( verbose ) cout << "Preparing reconstruction filter:" << endl;
    this->m_SecretData->m_ReconstructionFilter =
      PrepareReconstructionFilter(
        *lastAlgorithm, finalTransform.GetPointer() );
    if ( verbose ) cout << "Preparing reconstruction filter done." << endl;
    } // preparing filter

  if ( this->m_SecretData->m_ReconstructionFilter )
    {
    if ( verbose ) cout << "Updating reconstruction filer:" << endl;
    this->m_SecretData->m_ReconstructionFilter->Update();
    if ( verbose ) cout << "Updating reconstruction filer done." << endl;
    } // reconstruction filter

  // update the moved study
  // now that we have the filter
  // and transform prepared
  if ( verbose ) cout << "Calculating moved study:" << endl;
  
 RegisterMFAAlgorithm::OutputPointer studyOutput = lastAlgorithm->GetStudyOutput();

 studyOutput->SetFinalTransform( lastAlgorithm->GetFinalTransform() );

  studyOutput->UpdateMovedStudy( *this );
  this->m_SecretData->m_MovedStudy = studyOutput->GetMovedStudy();
  
  if ( verbose ) cout << "Calculating moved study done." << endl;
    
  return true;

} // Resegment
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageBaseConstPointer MFASegmenter::SecretData::GetTemplateImage() const
{

  const VolumeImageBaseConstPointer badOutput;
  
  // get the template image
  const RegisterMFAAlgorithmCollection
    & algorithms = this->m_RegistrationAlgorithms;
  if ( algorithms.empty() ) return badOutput;
  
  const RegisterMFAAlgorithmConstPointer
    singleAlgorithm = *( algorithms.cbegin() );
  if ( !singleAlgorithm ) return badOutput;
  
  // the moving image was actually
  // the template of the original image
  const RegisterMFAAlgorithm::MovingImageType::ConstPointer
    movingImage = std::dynamic_pointer_cast< const RegisterMFAAlgorithm::MovingImageType > ( singleAlgorithm->GetMovingImage() );

  const VolumeImageConstPointer
    templateImage = movingImage->GetImage();
    
  return templateImage.GetPointer();
  
} // GetTemplateImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenter::GetReconstruction(
  const FeatureIDConstPointer & whichFeature ) const
{

  // make sure we have everything we need
  const MFAVolumeReconstructionFilterPointer
    filter = this->m_SecretData->m_ReconstructionFilter;
  
  const VolumeImagePointer badOutput;
  if ( !filter )
    {
    std::cerr << "Error: No reconstruction filter!" << std::endl;
    return badOutput;
    }
    
  return ::ccipd::GetReconstruction(
    *filter,
    whichFeature,
    this->m_SecretData->GetTemplateImage(),
    this->m_SecretData->m_FinalTransform.GetPointer() );
    
} // GetReconstruction
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenter::GetTransformedReconstruction(
  const FeatureIDConstPointer & whichFeature ) const
{

  // make sure we have everything we need
  const MFAVolumeReconstructionFilterPointer
    filter = this->m_SecretData->m_ReconstructionFilter;
  
  const VolumeImagePointer badOutput;
  if ( !filter )
    {
    std::cerr << "Error: No reconstruction filter!" << std::endl;
    return badOutput;
    }
    
  return ::ccipd::GetTransformedReconstruction(
    *filter, whichFeature );

} // GetTransformedReconstruction
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenter::GetReconstructedLevelset(
  const MaskObjectType & whichObject ) const
{

  // as per our convention, the object name is the description of the feature ID
  const FeatureIDConstPointer whichFeature =
    MFALevelsetExtractor::CreateFeatureIDWithObject( whichObject );
  
  // Calculate the reconstruction for that feature
  return this->GetReconstruction( whichFeature );

} // GetReconstructedLevelset
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenter::GetReconstructedTransformedLevelset(
  const MaskObjectType & whichObject ) const
{
  
  // as per our convention, the object name is the description of the feature ID
  const FeatureIDConstPointer whichFeature =
    MFALevelsetExtractor::CreateFeatureIDWithObject( whichObject );

  // reconstruct and transform
  return this->GetTransformedReconstruction( whichFeature );

} // GetReconstructedTransformedLevelset
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenter::GetReconstructedTexture(
  const TextureFeatureEnum whichTexture ) const
{

  // create a feature with that texture
  const FeatureIDConstPointer whichFeature( FeatureID::New( whichTexture ) );

  // reconstruct it
  return this->GetReconstruction( whichFeature );

} // GetReconstructedTexture
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenter::GetReconstructedTransformedTexture(
  const TextureFeatureEnum whichTexture ) const
{

  // create a feature with that texture
  const FeatureIDConstPointer whichFeature( FeatureID::New( whichTexture ) );

  // reconstruct and transform
  return this->GetTransformedReconstruction( whichFeature );

} // GetReconstructedTransformedTexture
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer MFASegmenter::GetSegmentation(
  const MaskObjectType    & whichObject,
  const StudyConstPointer & whichStudy ) const
{

  // see if we segmented this study
  const VolumeMaskImagePointer badOutput;
  
  if ( whichStudy->GetHash() != this->m_SecretData->m_SegmentedStudyHash )
    {
    std::cerr << "You have not segmented study " <<
      whichStudy->GetHash() << "!" << std::endl;
    return badOutput;
    }

  return this->GetSegmentation( whichObject );

} // GetSegmentation (with study )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer MFASegmenter::GetSegmentation(
  const MaskObjectType & whichObject ) const
{
  
  // see if we segmented this study
  const VolumeMaskImagePointer badOutput;
  
  // get the levelset
  const VolumeImageConstPointer levelset(
    this->GetReconstructedLevelset( whichObject ) );

  if ( levelset )
    {
    // in case the image had some 0's padded 
    const float threshold = -std::numeric_limits< float >::epsilon();
    return CalculateMaskFromLevelset( levelset, threshold );
    }
  else
    {
    std::cerr << "Error: No reconstructed levelset for " <<
      whichObject << "!" << std::endl;
    return badOutput;
    }
  
} // GetSegmentation (no study)
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer MFASegmenter::GetTransformedSegmentation(
  const MaskObjectType    & whichObject,
  const StudyConstPointer & whichStudy ) const
{

  // see if we segmented this study
  const VolumeMaskImagePointer badOutput;
  
  if ( whichStudy->GetHash() != this->m_SecretData->m_SegmentedStudyHash )
    {
    std::cerr << "You have not segmented study " <<
      whichStudy->GetHash() << "!" << std::endl;
    return badOutput;
    }

  return this->GetTransformedSegmentation( whichObject );

} // GetTransformedSegmentation (with study)
//////////////////////////////////////////////////////////////////////////////////////////


  
//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer MFASegmenter::GetTransformedSegmentation(
  const MaskObjectType & whichObject ) const
{
  
  // see if we segmented this study
  const VolumeMaskImagePointer badOutput;
  
  // get the levelset
  const VolumeImageConstPointer levelset(
    this->GetReconstructedTransformedLevelset( whichObject ) );

  if ( levelset )
    {
    // in case the image had some 0's padded 
    const float threshold = -std::numeric_limits< float >::epsilon();
    return CalculateMaskFromLevelset( levelset, threshold );
    }
  else
    {
    std::cerr << "Error: No reconstructed levelset for " <<
      whichObject << "!" << std::endl;
    return badOutput;
    }

} // GetTransformedSegmentation (no study)
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectCollectionType MFASegmenter::GetAllSegmentationObjects() const
{

  // prepare the output
  MaskObjectCollectionType segmentationObjects;

  // start with all the features
  const FeatureIDConstCollection allFeatures = this->GetAllFeatures();

  // loop through
  BOOST_FOREACH( const FeatureIDConstPointer whichFeature, allFeatures )
    {

    if ( !whichFeature ) continue;

    // ignore textures
    if ( whichFeature->HasTextureFeature() ) continue;

    // try to convert to a levelset
    const MaskObjectType segmentationObject =
      MFALevelsetExtractor::ExtractObjectNameFromFeatureID( *whichFeature );

    // insert it if we found a levelset object
    if ( !segmentationObject.empty() )
      segmentationObjects.insert( segmentationObject );

    } // for each feature

  return segmentationObjects;

} // GetAllFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection MFASegmenter::GetAllFeatures() const
{

  // first get the model
  const MFAModelConstPointer & model = this->m_SecretData->m_TrainedModel;
  if ( !model ) return FeatureIDConstCollection();

  // then get the actual trained MFA
  const MFAVolumeBaseConstPointer trainedMFA = model->GetTrainedSIM();
  if ( !trainedMFA ) return FeatureIDConstCollection();

  // then extract the features from it
  return trainedMFA->GetFeatureIDs();

} // GetAllFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenter::SetTrainedMFAFile( const string & variable )
{

  const bool differentFileName =
    this->m_SecretData->m_TrainedMFAFile.compare( variable ) != 0;

  if ( differentFileName )
    {

    ccipdSetBodyMacro( TrainedMFAFile )

    // we are no longer initialized
    this->m_SecretData->m_Initialized = false;

    } // differentFileName

} // SetTrainedMFAFile
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenter::SetBoundingBox( const MFABoundingBoxType & variable )
{
  this->m_SecretData->m_HasBoundingBox = true;
  ccipdSetBodyMacro( BoundingBox )
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenter::ClearBoundingBox()
{
  this->m_SecretData->m_HasBoundingBox = false;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// implementations of all the get/set functions
ccipdSetByrefImplementMacro( MFASegmenter,
  SegmentationObject, MaskObjectType )
ccipdSetByvalImplementMacro( MFASegmenter, Verbose, bool )
ccipdSetByvalImplementMacro( MFASegmenter, ReconstructFeaturesInMoved, bool )
ccipdGetConstImplementMacro( MFASegmenter, ReconstructFeaturesInMoved, bool )
ccipdSetByrefImplementMacro( MFASegmenter,
  RegistrationAlgorithms, RegisterMFAAlgorithmCollection )

ccipdGetConstImplementMacro( MFASegmenter, TrainedModel          , MFAModelConstPointer             )
ccipdGetConstImplementMacro( MFASegmenter, MovedStudy            , StudyPointer                     )
ccipdGetConstImplementMacro( MFASegmenter, TrainedMFAFile        , string                           )
ccipdGetConstImplementMacro( MFASegmenter, DrivingFeaturePreparer, MFADrivingFeaturePreparerPointer )
ccipdGetConstImplementMacro( MFASegmenter, SegmentationObject    , MaskObjectType              )
ccipdGetConstImplementMacro( MFASegmenter, Verbose               , bool                        )
ccipdGetConstImplementMacro( MFASegmenter,
  RegistrationAlgorithms, RegisterMFAAlgorithmCollection )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Which serialization version are we on?
 * 
 * History:
 *   - 2012-06-26 version 1 added support for separate affine + deformable parameters
 *   - 2012-06-27 version 2 made the spline points equal for all resolutions
 *   - 2012-07-01 version 3 added support for cropping 
 *   - 2012-07-01 version 4 added per-dimension affine segmentation padding
 *   - 2012-07-01 version 5 added per-dimension deformable segmentation padding
 *   - 2012-08-18 version 6 added regularization weighting
 *   - 2012-08-27 version 7 added the # of particles
 *   - 2013-01-16 version 8 completely refactored the old code
 *   - 2013-03-25 version 9 added reconstruct all features
 */
#ifdef Boost_SERIALIZATION_FOUND
BOOST_CLASS_VERSION( ccipd::MFASegmenter::SecretData, 9 )
#endif
//////////////////////////////////////////////////////////////////////////////////////////
