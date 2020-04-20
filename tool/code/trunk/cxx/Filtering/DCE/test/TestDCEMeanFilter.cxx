#include "ccipdCore.h"
#include "ccipdDisableWarningsMacro.h"
#include "itkDCEMeanFilter.h"
#include <itkImageRegionIterator.h>
#include "ccipdEnableWarningsMacro.h"


template <typename TImage>
void CreateVectorImage(TImage* const image, unsigned imDim)
{
  // Create an image with 2 connected components
  typename TImage::IndexType index = {{0,0,0}};
  typename TImage::SizeType size = {{imDim, imDim, imDim}};

  typename TImage::RegionType region(index, size);

  image->SetVectorLength(4);
  image->SetRegions(region);
  image->Allocate();

  typename TImage::InternalPixelType n = 1;

  for (unsigned int z = 0; z < imDim; z++ )
  { 
    for (unsigned int j = 0; j < imDim; j++)
    {
      for (unsigned int i = 0; i < imDim; i++, n++)
      {
        typename TImage::IndexType index = {{i,j,z}};
        
		typename TImage::PixelType pixel = image->GetPixel(index);
		
		pixel[0] = n;
		pixel[1] = n+1;
		pixel[2] = n+2;
		pixel[3] = n+3;

		image->SetPixel(index, pixel);
      }
    }
  }
}

template <class TImage>
void DisplayVectorImage(TImage* const image)
{
	unsigned int nChannels = image->GetNumberOfComponentsPerPixel();

	
	itk::ImageRegionIterator<TImage>  it( image, image->GetBufferedRegion() );
    it.GoToBegin();
	
	    std::cout << "\n===========\n";
		while(!it.IsAtEnd())
		{
			std::cout << "\n";
			for (int c = 0; c < nChannels; c++)  std::cout << it.Get()[c] << "  " ;

			++it;
		}
}

template <class TImage>
void DisplayScalarImage(TImage* const image)
{
	itk::ImageRegionIterator<TImage>  it( image, image->GetBufferedRegion() );
    it.GoToBegin();
	
	std::cout << "\n===========\n";
	while(!it.IsAtEnd())
	{
		std::cout << "\n";
		std::cout << it.Get() << "  " ;

		++it;
	}
}

int main(int, char*[])
{

	typedef itk::VectorImage<float, 3> VectorImageType;
    typedef itk::Image<float, 3>       ScalarImageType;
    typedef itk::DCEMeanFilter<VectorImageType, ScalarImageType>  FilterType;
   
    FilterType::Pointer filter = FilterType::New();


//////////////////////////////////////////////////////////////////////////

// Fill in a test image
       VectorImageType::Pointer    inputImage = VectorImageType::New();
       unsigned int imageDimensionsSize = 2; // 2 x 2 x 2
       CreateVectorImage<VectorImageType>(inputImage, imageDimensionsSize);
	   DisplayVectorImage<VectorImageType>(inputImage);

       filter->SetInput(inputImage);
       filter->Update();
	   filter->GetOutput();

	   DisplayScalarImage<ScalarImageType>(filter->GetOutput());


       return EXIT_SUCCESS;
}


 
