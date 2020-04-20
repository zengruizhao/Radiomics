


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAPrepareFilter.h"
#include "ccipdRegisterMFAAlgorithm.h"
#include "ccipdRegisterMFAMetric.h"
#include "ccipdSIMFeaturePreparerInterface.h"
#include "ccipdSIMStorage.h"
#include "ccipdApplyTransformation.h"
#include "ccipdVolumeTypes.h"

// itk includes
#include "itkMFAReconstructionFilter.h"
#include "itkMFAReconstructionMetric.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
MFAVolumeReconstructionFilterPointer PrepareReconstructionFilter(
  const RegisterMFAAlgorithm        & algorithm,
  const VolumeTransformConstPointer & finalTransform )
{

  const MFAVolumeReconstructionFilterPointer badOutput;

  // extract everything from the algorithm
  const bool verbose = algorithm.SuperclassMetric::GetVerbose();

  // get the trained MFA
  const MFAVolumeBaseConstPointer trainedMFA = algorithm.GetTrainedMFA();
  if ( !trainedMFA ) return badOutput;

  // get the MFA metric
  const RegisterMFAMetricPointer registerMetric = 
    std::dynamic_pointer_cast < RegisterMFAMetric >( algorithm.GetMetric() );
  
  const RegisterMFAMetric::MFAMetricPointer metric(
    registerMetric->GetMFAMetric() );

  if ( !metric ) return badOutput;

  // get the input features
  const FeatureIDConstCollection whichFeatures = 
    metric->GetInputFeatureIDs();
  if ( whichFeatures.empty() ) return badOutput;

  typedef MFAVolumeReconstructionFilter::VectorImageType::ConstPointer
    VectorImageConstPointer;

  // we must input the features
  // and then transform them now
  // in the coordinate space of the MFA using
  // the final transform after registering
  const VectorImageConstPointer
    untransformedFeatures( metric->GetInputFeatures() ),
    transformedFeatures( ApplyTransform(
      finalTransform, untransformedFeatures,
      trainedMFA->GetMeanAndEigenvectors( *whichFeatures.cbegin() ) ) );

  // we can possibly use the sample locations
  // from the last iteration
  const MFAVolumeReconstructionFilter::SampleLocationsContainer
    sampleLocations = metric->GetSampleLocations();

  // create a filter
  const MFAVolumeReconstructionFilterPointer
    outputFilter = MFAVolumeReconstructionFilter::New();

  // give everything to the filter
  outputFilter->SetVerbose( verbose );
  outputFilter->SetInputFeatures(
    transformedFeatures,
    whichFeatures );
  outputFilter->SetTrainedMFA( *trainedMFA );
  outputFilter->SetUseVarianceWeights( registerMetric->GetUseVarianceWeights() );
  outputFilter->SetMaxEigenvectors( registerMetric->GetMaxEigenvectors() );
  
  // optionally use the sample locations
  // if they exist
  if ( sampleLocations.empty() )
    {
    outputFilter->UseExistingLocationsOff();
    }
  else
    {
    outputFilter->UseExistingLocationsOn();
    outputFilter->SetExistingLocations( sampleLocations );
    } // empty
  
  // just give it the correctly-sized fixed image as the input

  outputFilter->SetInput( algorithm.GetFixedImage()->GetImage() );

  // output
  return outputFilter;

  /*
  // extract the data for this function

  const StudyConstPointer             study           =  this->m_Study;
  const FeatureIDConstCollection    & drivingFeatures =  this->m_DrivingFeatures;
  const MFABaseType                 & trainedMFA      = *this->m_TrainedModel->GetTrainedMFA();
  const VolumeImageConstPointer     & fixedImage      =  this->m_FixedImage;
  const SampleLocationsContainer    & sampleLocations =  this->m_SampleLocations;

  // create a new filter
  typedef itk::SmartPointer< FilterType > FilterPointer;
  const FilterPointer filter = FilterType::New();

  // give all the parameters to the filter

  // first, we must transform the features to be
  const VolumeFeaturesConstPointer
    & movedFeatures = this->m_CurrentMovedFeatures;

  // give it the features
  filter->SetInputFeatures(
    movedFeatures, 
    drivingFeatures );

  // give it the trained MFA
  filter->SetTrainedMFA( trainedMFA );

  // show progress?
  filter->SetVerbose( verbose );

  // give it the sample locations
  if ( sampleLocations.empty() )
    {
    filter->UseExistingLocationsOff();
    }
  else
    {
    filter->UseExistingLocationsOn();
    filter->SetExistingLocations( sampleLocations );
    }

  // give it the last weights (or a null pointer)
  filter->SetUseVarianceWeights( this->m_UseVarianceWeights );

  // we will use this later as the template basis
  // image to size our resulting reconstruction result
  filter->SetInput( fixedImage );

#ifdef NDEBUG
  // actually do the calculation
  if ( verbose ) cout << "Updating reconstruction filter:" << endl;
  filter->Update();
  if ( verbose ) cout << "Updating reconstruction filter done." << endl;
#endif // NDEBUG
  
  // remember this filter
  this->m_ReconstructionFilters[ study ] = filter;
  */

} // PrepareReconstructionFilter
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace 
