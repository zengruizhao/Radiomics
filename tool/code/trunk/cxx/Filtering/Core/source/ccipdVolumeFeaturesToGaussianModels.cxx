//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeFeaturesToGaussianModels.h"

// our own itk includes
#include "itkVectorImageGaussianModelEstimator.h"
//////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{
typedef itk::VectorImageGaussianModelEstimator <
  VolumeFeaturesType,
  VolumeFeaturesGaussianFunctionType, 
  VolumeMaskImageType
> FeatureVolumeGaussianModelType;

const VolumeFeaturesGuassianFunctionContainer	
  VolumeFeaturesToGaussianModels (
  VolumeFeaturesType  * const features,
  VolumeMaskImageType * const mask,
  const unsigned int    numberOfClasses )
{
FeatureVolumeGaussianModelType::Pointer 
  featuresModel = FeatureVolumeGaussianModelType::New();

featuresModel->SetInputImage     ( features );
featuresModel->SetTrainingImage  ( mask );
featuresModel->SetNumberOfModels ( numberOfClasses );
featuresModel->SetNumberOfModelSamples( 1000 );

featuresModel->Update();

return featuresModel->GetMembershipFunctions();

}

} //ccipd