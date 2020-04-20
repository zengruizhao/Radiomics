//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCalculateProbabilityImage.h"

// our own itk includes
#include "itkVectorImageToProbabilityImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVectorImage.h>
  #include <itkImage.h>
  #include <itkMultiplyImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{

VolumeImageConstPointer CalculateProbabilityImage (
	const VolumeFeaturesType * featureImage,
	const MemberType         *  model)
{

  typedef itk::VectorImageToProbabilityImage<
    VolumeFeaturesType, 
	VolumeImageType, 
	MemberType
  > ProbabilityFilterType;

  ProbabilityFilterType::Pointer 
    probabilityFilter= ProbabilityFilterType::New();

  probabilityFilter->SetInput              ( featureImage );
  probabilityFilter->SetMembershipFunction ( model        );
  probabilityFilter->Update(); 

  return probabilityFilter->GetOutput();
}

VolumeImageConstPointer CalculateProbabilityImage (
	const VolumeFeaturesType * featureImage,
	const MemberType         *  model,
	const VolumeImageType    * spatialPrior)
{

	typedef itk::MultiplyImageFilter<
	  VolumeImageType,
	  VolumeImageType,
	  VolumeImageType
	> VolumeImageMultiplier;
	
  VolumeImageConstPointer 
    probImage = CalculateProbabilityImage (
	featureImage, model);

  VolumeImageMultiplier::Pointer 
    probMultipler = VolumeImageMultiplier :: New();

  probMultipler->SetInput1( probImage    );
  probMultipler->SetInput2( spatialPrior );
  probMultipler->Update();

  return probMultipler->GetOutput();
}


VolumeImageConstPointer CalculateProbabilityImage (
	const VolumeFeaturesType * featureImage,
	const MemberType         *  model,
	const VolumeImageType    * spatialPrior,
	const double                spatialWeight )
{
	typedef itk::MultiplyImageFilter<
	  VolumeImageType,
	  VolumeImageType,
	  VolumeImageType
	> VolumeImageMultiplier;
  
  VolumeImageMultiplier::Pointer 
    probMultipler = VolumeImageMultiplier :: New();

  probMultipler->SetInput1    ( spatialPrior );
  probMultipler->SetConstant2 ( static_cast< InternalPixelType >( spatialWeight ) );
  probMultipler->Update();

  VolumeImageConstPointer weightedPrior = probMultipler->GetOutput();

  return CalculateProbabilityImage( featureImage, model, weightedPrior );
}
} // namespace ccipd