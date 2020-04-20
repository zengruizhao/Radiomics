

//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeContainerToVolumeFeatures.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkComposeImageFilter.h> // this itk function does what we need
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer VolumeContainerToVolumeFeatures(
  const VolumeConstImagesContainer & images )
{

  // create our filter
  typedef itk::ComposeImageFilter<
    VolumeImageType,
    VolumeFeaturesType
  > ComposeImageFilter;

  ComposeImageFilter::Pointer composer = ComposeImageFilter::New();

  // input everything into the composer
  const size_t numberOfImages = images.size();
  for ( unsigned int imageIndex = 0; imageIndex < numberOfImages; ++imageIndex )
    composer->SetInput( imageIndex, images[ imageIndex ] );

  // update and output
  using std::cerr;
  using std::endl;
  try
    {
    composer->Update();
    return composer->GetOutput();
    }
  catch( itk::ExceptionObject & exception )
    {
    cerr << endl << "Error in VolumeContainerToVolumeFeatures:" <<
      endl << exception << endl;
    return VolumeFeaturesPointer();
    }
  catch( ... )
    {
    cerr << endl << "Unknown error in VolumeContainerToVolumeFeatures." << endl;
    return VolumeFeaturesPointer();
    }

} // VolumeContainerToVolumeFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesConstPointer VolumeContainerToVolumeFeaturesConst(
  const VolumeConstImagesContainer & images )
{

  return static_cast< VolumeFeaturesConstPointer >(
    VolumeContainerToVolumeFeatures( images ) );

} // VolumeContainerToVolumeFeaturesConst
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
