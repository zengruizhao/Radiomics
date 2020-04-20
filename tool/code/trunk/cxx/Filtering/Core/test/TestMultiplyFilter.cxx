


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h" 
#include "ccipdLoadVolume.h"
#include "ccipdSaveImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkMultiplyImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



/* 
*  This program tests multiply by constant filter
*/
int main( int argc, char *argv[] )
{
  // make sure an image has been given 
  if (argc < 4)
    {
    std::cout << "Usage: " << std::endl;
    std::cout << "   " << argv[0] << " inputVol scalingFactor outputVol" << std::endl;
    return EXIT_FAILURE;
    }
   
  // set variables
  const std::string
    imageFile = argv[1],
    outImageFile = argv[3];
  const double
    scalingFactor = atof(argv[2]);
  // load in the image
  std::cout << "Begin loading image..." << std::endl;
  ccipd::VolumeImagePointer inputImage = ccipd::LoadVolume( imageFile );
  std::cout<< "done." << std::endl;


  typedef itk::MultiplyImageFilter< ccipd::VolumeImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( inputImage );
  filter->SetInput2( static_cast< ccipd::InternalPixelType >( scalingFactor ) );
  
  ccipd::SaveImage( filter->GetOutput(), outImageFile, false );
  std::cout<< "done." << std::endl;

  return EXIT_SUCCESS;
}


