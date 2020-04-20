


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
#include "ccipdSaveImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkFlatStructuringElement.h>
  #include <itkImage.h>
  #include <itkRandomImageSource.h>  // to generate test image
  #include <itkTimeProbe.h>           // to time each filter
  #include <itkMeanImageFilter.h>     // for iterating filter type
  #include <itkMeanHistogram.h>       //for moving histogram filter type
  #include <itkFlatStructuringElement.h>

#ifdef _MSC_VER
#pragma warning( disable: 4244 ) // itkFlatStrutctingElement conversion from SizeValueType to float
#endif // _MSC_VER

  #include <itkMovingHistogramImageFilter.h>
#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( disable: 4244 ) // itkFlatStrutctingElement conversion from SizeValueType to float
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/*
 * This program evaluates the time it takes to perform a mean image filter for images of 
 * different sizes, where each image is generated to contain random pixels from 0 to 100.
 *
 * The input is a text file where the results for how long it takes to calculate each filter 
 * are stored. The default is compareFilterTypes.txt which will be output to the current 
 * directory.
 */
//////////////////////////////////////////////////////////////////////////////////////////
int main ( int argc, char * argv[] )
{

//////////////////////////////////////////////////////////////////////////////////////////
  if ( argc == 1 )
    {
    std::cout << std::endl << "Usage:" << std::endl <<
     argv[ 0 ] << "[outputFile = compareFilterTypes.txt] " <<   
	 std::endl << std::endl;
    return EXIT_SUCCESS;
    } // 1 argument
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////  
const std::string writeFile = argc > 1 ? argv[1] : "compareFilterTypes.txt";

FILE * file;
file = fopen(writeFile.c_str(), "w"); 

//how big to make the image
for (unsigned int i=1;i<11;++i)
  {

//////////////////////////////////////////////////////////////////////////////////////////
  // prepare random image generator
  typedef itk::RandomImageSource<
    ccipd::VolumeImageType
  > VolumeRandomImageType;
  
  VolumeRandomImageType::Pointer imageGenerator = VolumeRandomImageType::New();

  //scale image appropriately
  ccipd::VolumeImageType::SizeType   imageSize;
  imageSize[0]=100*i,
  imageSize[1]=100*i,
  imageSize[2]=10*i;

  //generate image
  imageGenerator->SetMin(0);
  imageGenerator->SetMax(100);
  imageGenerator->SetSize(imageSize);
  imageGenerator->Update();
  ccipd::VolumeImagePointer testImage = imageGenerator->GetOutput();
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  //FixedWindowFilter
  typedef itk::MeanImageFilter<
    ccipd::VolumeImageType,
	ccipd::VolumeImageType
  > MeanFilterType1;
  
  MeanFilterType1::Pointer filter1 = MeanFilterType1::New();
  filter1->SetInput(testImage);
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  //MovingWindowFilter
  typedef itk::MeanHistogram<
    ccipd::InternalPixelType 
  > MeanHistogramType;
  
  typedef itk::FlatStructuringElement<
    ccipd::VolumeDimension
  > StructuringElementType;
  
  typedef itk::MovingHistogramImageFilter<
	ccipd::VolumeImageType,
	ccipd::VolumeImageType,
    StructuringElementType,
    MeanHistogramType
  > MeanFilterType2;

  MeanFilterType2 :: Pointer filter2 = MeanFilterType2 ::New();
  filter2->SetInput(testImage);
 //////////////////////////////////////////////////////////////////////////////////////////



  for (unsigned int j=1; j<11;++j)
    {
    
	//prepare my clocks
    itk::TimeProbe clock1;
	itk::TimeProbe clock2;
	
	//filter 1
	filter1->SetRadius(j);
	  
	clock1.Start();
	filter1->Update();
	clock1.Stop();

	//filter 2
    StructuringElementType::RadiusType elementRadius;
    elementRadius.Fill(j);
    
	StructuringElementType
      structuringElement = StructuringElementType::Box(elementRadius);
	filter2->SetKernel(structuringElement);
  
	clock2.Start();
	filter2->Update();
	clock2.Stop();

	fprintf(file, "Image Size %d  WindowSize %d ", i*100,2*j+1);
	fprintf(file, " FixedWindowFilter %f  MovingWindowFilter %f \n", clock1.GetTotal(), clock2.GetTotal());
    } // for j

  }// for i

fclose(file);
return EXIT_SUCCESS;
}