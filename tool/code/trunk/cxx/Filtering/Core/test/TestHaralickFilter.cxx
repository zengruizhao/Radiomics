#include "ccipdCore.h"
#include "ccipdDisableWarningsMacro.h"
#include "itkHaralickFilter.h"
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "ccipdEnableWarningsMacro.h"


template <typename TImage>
void CreateImage(TImage* const image, unsigned imDim)
{
  // Create an image with 2 connected components
  typename TImage::IndexType index = {{0,0,0}};
  typename TImage::SizeType size = {{imDim, imDim, imDim}};

  typename TImage::RegionType region(index, size);

  image->SetRegions(region);
  image->Allocate();

  typename TImage::InternalPixelType n = 0;

  for (unsigned int z = 0; z < imDim; z++ )
  { 
    for (unsigned int j = 0; j < imDim; j++)
    {
      for (unsigned int i = 0; i < imDim; i++, n++)
      {
        typename TImage::IndexType index = {{i,j,z}};
        image->SetPixel(index, n);
      }
    }
  }
}


template <class TInputImageType>
typename TInputImageType::Pointer LoadImage(std::string fileName)
{
  typedef itk::ImageFileReader<TInputImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName);
  reader->Update();
  return reader->GetOutput();
}



int main(int, char*[])
{

     typedef itk::Image<float, 3> ImageType;
     typedef itk::HaralickFilter<ImageType, ImageType>  FilterType;
   
     typedef itk::ImageFileReader<ImageType>  ReaderType;
     typedef itk::ImageFileWriter<ImageType>  WriterType;


     ReaderType::Pointer reader = ReaderType::New();
     WriterType::Pointer writer = WriterType::New();
     FilterType::Pointer filter = FilterType::New();

     
// Load Test Image

     //reader->SetFileName( "J:/aa.vtk" );
     //filter->SetInput( reader->GetOutput() );

//////////////////////////////////////////////////////////////////////////

// Fill in a test image
       ImageType::Pointer    inputImage = ImageType::New();
       unsigned int imageDimensionsSize = 5; // Image can be 5 x 5 x 5
      CreateImage<ImageType>(inputImage, imageDimensionsSize);

       // Cout the image
       for (unsigned int z = 0; z < imageDimensionsSize; z++ )
       { 
         std::cout <<"\nSlice\n";
         for (unsigned int j = 0; j < imageDimensionsSize; j++)
         {
           std::cout <<"\n";
           for (unsigned int i = 0; i < imageDimensionsSize; i++)
           {
             ImageType::IndexType index;
             index[2] = z; index[1] = j; index[0] = i; 
             float pixel = inputImage->GetPixel( index );
             std::cout << pixel << " ";
           }
         }
       }



       filter->SetInput(inputImage);
//////////////////////////////////////////////////////////////////////////


     ImageType::SizeType imSize;
     imSize.Fill(2);
     filter->SetRadius(imSize); // Radius will be defaulted to 1,1,1 if not set by user
     
    // filter->SetInputImageMinimum(0.0f);
     //filter->SetInputImageMaximum(1237.0f); // Min and max will be defaulted to those of the input image of not set by user

     // This significantly affects the speed of computation
     filter->SetNumberOfBinsPerAxis(32); // will be defaulted to 16 if not set by user
     
     
     ImageType::OffsetType offset = {{0, 1, 0}};
     filter->SetOffset(offset); // Offset will be defaulted to 1,0,0 if not set by user

    // ImageType::


     filter->Update();


     // To get the energy output feature image, user may call either filter->GetOutput(0); or filter->GetEnergyOutput();
     // To get the Entropy output feature image, user may call either filter->GetOutput(1); or filter->GetEntropyOutput();
     // See itkHaralickFilter.h for the full list of output Features
    
     for(unsigned int op = 0; op < filter->GetNumberOfOutputs(); op++)
     {
       std::cout <<"\nFeature Image: " << op << "\n";
     ImageType::Pointer outputEnergyFeatureImage = filter->GetOutput(op); // This call will get the primary output Feature which is Energy
     // Cout the image
     for (unsigned int z = 0; z < imageDimensionsSize; z++ )
     { 

       std::cout <<"\nSlice\n";
       for (unsigned int j = 0; j < imageDimensionsSize; j++)
       {
         std::cout <<"\n";
         for (unsigned int i = 0; i < imageDimensionsSize; i++)
         {
           ImageType::IndexType index;
           index[2] = z; index[1] = j; index[0] = i; 
           float pixel = outputEnergyFeatureImage->GetPixel( index );
           std::cout << pixel << " ";
         }
       }
     }

     }

  return EXIT_SUCCESS;
}


 
