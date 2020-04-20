/* 
*  This program tests convolution filters
*
*/



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h" 
#include "ccipdLoadVolume.h"
#include "ccipdSaveImage.h"
#include "ccipdCreateGaborKernel.hxx"


#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkConvolutionImageFilter.h>
  #include <itkGaborImageSource.h>
  #include <itkImage.h>
  #include <itkImageBase.h>
#include "ccipdEnableWarningsMacro.h"

#if ( __GNUC__ >= 5 || ( __GNUC__ == 4 && __GNUC_MINOR__ >= 7) )
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#endif // GNUC
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char *argv[] )
{

// make sure an image has been given 
if (argc < 1)
  {
  std::cout << "Warning must give at least one input" << std::endl;
  return EXIT_FAILURE;
  }
 
// set variables
const std::string
  imageFile = argv[1];

// define the radius
const double radius = 1;
// load in the image
std::cout << "Begin loading image..." << std::endl;
ccipd::VolumeImagePointer inputImage = ccipd::LoadVolume( imageFile );
std::cout<< "done." << std::endl;

// prepare the kernel type
std::cout << "Begin creating kernel..." << std::endl;

const float freq = static_cast<float>(radius)/100.0f;

ccipd::VolumeImagePointer kernel;
kernel = ccipd::CreateGaborKernel< ccipd::VolumeImageType > (freq, 0.785f);

ccipd::SaveImage( kernel.GetPointer(), "D:/code/GaborKernel.mhd", false );
std::cout << "done." << std::endl;


typedef itk::ConvolutionImageFilter< ccipd::VolumeImageType > FilterType;
FilterType::Pointer filter = FilterType::New();

std::cout << "Begin convolution" << std::endl;
filter->SetInput( inputImage );
filter->SetKernelImage( kernel);


ccipd::SaveImage( filter->GetOutput(), "D:/code/FilteredImageSlice_1_45.mhd",false );
std::cout<< "done." << std::endl;



return EXIT_SUCCESS;
}


