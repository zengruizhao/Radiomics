/* 
*  This program tests moving window filters
*
*/


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"                               // for our image types
#include "ccipdLoadTRUS.h"                             // to read in a test image

// our own itk includes
#include "itkRayleighHistogram.h"                     // determines the type of statisitic calculated
#include "itkNakagamiHistogram.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkMovingHistogramImageFilter.h>			  // to get the filtered image
  #include <itkFlatStructuringElement.h>				   // to determine the neighborhood considered in the filter	
  #include <itkCastImageFilter.h>
  #include <itkImageFileWriter.h>                       // to write out the data
  #include <itkNeighborhoodOperator.h>
  #include <itkAddImageFilter.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char *argv[] )
{
//////////////////////////////////////////////////////////////////////////////////////////
  // use the input file
  const std::string
	inputTRUS    = argc > 1 ? argv[1]:" ";
  const int
	  radius= argc > 2 ? atoi(argv[2]):1;

  //prepare image type
  ccipd::TRUSImageConstPointer TRUSImage(
    ccipd::LoadTRUSFromGIPL( inputTRUS  ) );

  // add one to the image since zeros are causing all sorts of problems to the nakagami distro
  typedef itk::AddImageFilter< 
    ccipd::VolumeImageType, 
	ccipd::VolumeImageType,
	ccipd::VolumeImageType
  > AddFilterType;

  AddFilterType::Pointer addFilter = AddFilterType::New();
  addFilter->SetInput1   ( TRUSImage );
  addFilter->SetConstant2( 1 );
  addFilter->Update();

  typedef itk::NakagamiHistogram< ccipd::InternalPixelType >                     NakagamiHistogramType;
  typedef itk::FlatStructuringElement<3> StructuringElementType;
  StructuringElementType::RadiusType elementRadius;
  
  elementRadius.Fill(radius); // a 7x7 kernel
  StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);
  
  typedef itk::MovingHistogramImageFilter<
    ccipd::TRUSImageType,
    ccipd::TRUSImageType,
    StructuringElementType,
    NakagamiHistogramType
  > NakagamiFilterType;

  //create the moving histogram
  NakagamiFilterType :: Pointer filterTRUS        = NakagamiFilterType ::New();
  filterTRUS->SetInput( addFilter->GetOutput() );
  filterTRUS->SetKernel(structuringElement);

  typedef itk::CastImageFilter<
    ccipd::TRUSImageType,
    ccipd::VolumeImageType
  > TRUSCasterType;
  TRUSCasterType::Pointer TRUSCastImage	= TRUSCasterType::New();

  TRUSCastImage->SetInput(filterTRUS->GetOutput());

  typedef itk::ImageFileWriter< ccipd::VolumeImageType > ImageWriterType;
  ImageWriterType::Pointer imageWriterTRUSFull = ImageWriterType  ::New();

  const std::string   outputName = "NakagamiTest.mhd";
  imageWriterTRUSFull->SetFileName(outputName);
  imageWriterTRUSFull->SetInput(TRUSCastImage->GetOutput());
  
  try
    {
    imageWriterTRUSFull->Update();
    std::cout << "Success!" << std::endl;
    }
  catch(itk::ExceptionObject & err)
    {
    std::cerr << "ExceptionObject caught!" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

return 0;
}