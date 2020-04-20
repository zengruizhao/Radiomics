


////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCohenMRIBiasCorrection.h"
#include "itkCohenBiasCorrectionFilter.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd{
  
  
  
////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer CohenCorrectBiasField(
  const VolumeImageType * const inputImage,
  const float sigma )
{
  /*
  typedef itk::MRIBiasFieldCorrectionFilter<
    MRIImageType,
    MRIImageType,
    MRIMaskImageType
  > BuiltinBiasCorrector;
  BuiltinBiasCorrector::Pointer biasCorrector = BuiltinBiasCorrector::New();
  BuiltinBiasCorrector->SetInput( inputImage );

  // we need to initialize these
  // but I'm not sure how sensitive it is to these
  const unsigned int numberOfClasses = 3;
  itk::Array< double >
    classMeans ( numberOfClasses ),
    classSigmas( numberOfClasses );
  classMeans .Fill( 1 );
  classSigmas.Fill( 1 );

  biasCorrector->SetTissueClassStatistics( classMeans, classSigmas );

  try
    {
    temp2->Update();
    }
  catch( itk::ExceptionObject & exception )
    {
    std::cerr << "Error: " << exception << std::endl;
    return MRIImagePointer();
    }
  return biasCorrector->GetOutput();
  */
const itk::FixedArray<InternalPixelType,VolumeDimension> sigmaArray(sigma);
return CohenCorrectBiasField( inputImage, sigmaArray);

} // CohenCorrectBiasField
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer CohenCorrectBiasField(
  const VolumeImageType * const inputImage,
  const itk::FixedArray<InternalPixelType,VolumeDimension> & sigmaArray )
{

	  // create a new bias field corrector
  typedef itk::CohenBiasCorrectionFilter<
    VolumeImageType > BiasCorrector;
  BiasCorrector::Pointer biasCorrector = BiasCorrector::New();
  
  // input everything
  biasCorrector->SetInput( inputImage );
  biasCorrector->SetSigmaArray( sigmaArray ); // this is the only parameter
  
  // calculate and output
  biasCorrector->Update();
  return biasCorrector->GetOutput();
}
} // namspace
