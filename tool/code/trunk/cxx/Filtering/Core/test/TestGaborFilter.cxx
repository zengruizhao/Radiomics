#include "ccipdCore.h"
#include "ccipdDisableWarningsMacro.h"
  #include "itkGaborFilter.h"
  #include "itkImageFileReader.h"
  #include "itkImageFileWriter.h"
#include "ccipdEnableWarningsMacro.h"

template<class TImageType>
void
SaveImage(  typename TImageType::Pointer image
          , std::string fileName
         )
{
  typedef itk::ImageFileWriter<TImageType> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetInput(image); 
  writer->SetFileName(fileName);
  writer->Update();
}

template <class TImageType>
typename TImageType::Pointer 
LoadImage(std::string fileName)
{
  typedef itk::ImageFileReader<TImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName);
  reader->Update();
  return reader->GetOutput();
}

  int main( int argc, char *argv[] )
  {
    if(argc < 5)
    {
      cout << "Please, Enter the following parameters:\n";
      cout << "argv[1]: Path/Name of the Input Volume Image\n";
      cout << "aggv[2]: Path/Name of the Output Volume Image\n";
      cout << "aggv[3]: Input Frequency value to the Gabor Filter\n";
      cout << "aggv[4]: Input Rotation angle value (around z-axis) in radians\n";
      return EXIT_FAILURE;
    }

    typedef itk::Image<float, 3> VolumeImageType;
    typedef itk::GaborFilter<VolumeImageType> GaborFilterType;

    GaborFilterType::Pointer gaborFilter = GaborFilterType::New();
    

    gaborFilter->SetInput(LoadImage<VolumeImageType>(argv[1]));
    gaborFilter->SetFrequency(atof(argv[3]));
    gaborFilter->SetRotationZ(atof(argv[4]));
    gaborFilter->Update();
    SaveImage<VolumeImageType>(gaborFilter->GetOutput(), argv[2]);

    //VolumeImageType::Pointer pInputImage = LoadImage<VolumeImageType>("J:\\Data\\T2AXIALFSE_4_exampleDataProvidedBySatish\\COH833.mha");
    //gaborFilter->SetInput(pInputImage);
    //gaborFilter->SetFrequency((float)(1.0f/45.2548));
    //gaborFilter->SetRotationZ(2.7489);
    //gaborFilter->Update();
    //SaveImage<VolumeImageType>(gaborFilter->GetOutput(), "FFTConvolutedImage_Wavelength_45.2548_Orientation_2.7489.mha");
    
    return EXIT_SUCCESS;
  }