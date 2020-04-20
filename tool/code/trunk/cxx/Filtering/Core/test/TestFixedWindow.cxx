


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"                // for our image types
#include "ccipdLoadVolume.h"
#include "ccipdSaveImage.h"

// our own itk includes
#include "itkRangeImageFilter.h"     // to get the filter the image

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageFileWriter.h>      // to write out the data
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char *argv[] )
{
//////////////////////////////////////////////////////////////////////////////////////////
  // use the input file
  const std::string
	inputMRI    = argc > 1 ? argv[1]:" ";

  //prepare image types
  ccipd::VolumeImageConstPointer  MRIImage;

  try 
    {
    //get the output
    MRIImage  = ccipd::LoadConstVolume( inputMRI, false  );
    std::cout << "Success!" << std::endl;
    } //try
  catch (itk::ExceptionObject & err)
    {
        std::cerr << "ExceptionObject caught!" << std::endl;
		std::cerr << err << std::endl;
		return EXIT_FAILURE;
    } 

  typedef ccipd::InternalPixelType                                          OutputPixelType;
  typedef itk::Image< OutputPixelType, ccipd::VolumeDimension >             OutputImageType;

  typedef itk::RangeImageFilter<ccipd::VolumeImageType, OutputImageType>    RangeFilterType;

  RangeFilterType      ::Pointer     myFilter=RangeFilterType::New();
  myFilter->SetInput(MRIImage);
  myFilter->SetRadius(1);
  
  const std::string   outputName = "range_test.mha";
  ccipd::SaveImage( myFilter->GetOutput(), outputName, true );
  
  return 0;
}