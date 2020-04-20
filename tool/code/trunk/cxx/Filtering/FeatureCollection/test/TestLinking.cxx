
// ccipd includes
#include "ccipdTextureFeatureExtractor.h"
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"



int main()
{
ccipd::TextureFeatureExtractor extractor;
extractor.SetVolume( ccipd::VolumeImageType::New() );
return 0;
}