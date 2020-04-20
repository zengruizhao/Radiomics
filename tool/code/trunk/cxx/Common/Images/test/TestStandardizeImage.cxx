#include "ccipdCore.h"
#include "ccipdHistogram.h"

#include "ccipdStandardizeImage.h"
#include "ccipdDisableWarningsMacro.h"
  #include <itkImageFileReader.h>
  #include <itkImageFileWriter.h>
#include "ccipdEnableWarningsMacro.h"
#include <string>
#include <iostream>
using namespace ccipd;

template<class TImageType>
void SaveImage(typename TImageType::Pointer image, std::string fileName)
{
  typedef itk::ImageFileWriter<TImageType> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetInput(image); 
  writer->SetFileName(fileName);
  writer->Update();
}

template <class TImageType>
typename TImageType::Pointer LoadImage(std::string fileName)
{
  typedef itk::ImageFileReader<TImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName);
  reader->Update();
  return reader->GetOutput();
}

int main( int argc, char *argv[] )
{
   if(argc < 4)
  {
    cout << "Argument 1 is the .csv file from which you want to load the Reference histogram\n"
         << "Argument 2 is the .mha or .mhd file from which you want to load the input volume image\n"
         << "Argument 3 is the .mha or .mhd file to which you want to write the output standardized volume image\n";
    return EXIT_FAILURE;
  }

  HistogramFPointer pReferenceHistogram = ccipd::LoadHistogram<HistogramTypeF>(argv[1]);
  VolumeImageType::Pointer pInputImage = LoadImage<VolumeImageType>(argv[2]);

  VolumeImageType::Pointer pStandardizedOutputImage = StandardizeImage<VolumeImageType, HistogramTypeF>(pInputImage, pReferenceHistogram);

  SaveImage<VolumeImageType>(pStandardizedOutputImage, argv[3]);


  return EXIT_SUCCESS;
}
