


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h" // to disable warnings and errors

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageFileReader.h>
  #include <itkImageFileWriter.h>
  #include <itkGradientRecursiveGaussianImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile   outputVectorImageFile   sigma" << std::endl;
    return EXIT_FAILURE;
    }
  const unsigned int  Dimension = 2;
  typedef    float    InputPixelType;
  typedef    float    OutputComponentPixelType;

  typedef itk::CovariantVector<
    OutputComponentPixelType, Dimension > OutputPixelType;
  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;
  typedef itk::ImageFileReader< InputImageType >  ReaderType;
  typedef itk::GradientRecursiveGaussianImageFilter<
                        InputImageType, OutputImageType >  FilterType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  const double sigma = atof( argv[3] );
  filter->SetSigma( sigma );
  filter->Update();

  typedef itk::ImageFileWriter< OutputImageType >  WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( filter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}

