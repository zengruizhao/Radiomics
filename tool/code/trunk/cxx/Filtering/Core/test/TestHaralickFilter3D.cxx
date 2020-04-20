#include "ccipdCore.h"
#include "ccipdDisableWarningsMacro.h"
#include "itkHaralickFilter.h"
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "ccipdEnableWarningsMacro.h"
#include <ctime>
#include <iostream>




int main(int argc, char *argv[])
{
  // First argument is the executable name
  // Second argument argv[1] is the full path of the input image
  // Third argument argv[2] is the full path of the output folder
  
      if(argc < 3)
      {
        std::cerr << "Usage of : " << argv[0] << " requires the following parameters: "<< std::endl;
        std::cerr << "argv[1]: Name/Path of the Input Volume Image File (Should be of a supported format like .mha) " << std::endl;
        std::cerr << "argv[2]: Name/Path of the Output Directory (where the Output Image Volume Files will be created. " << std::endl;
        return EXIT_FAILURE;
      }
 
        std::string fileName =  argv[1];
        std::string outputFolder = argv[2];

   //int main(int /*argc*/, char /**argv[]*/)
   //{
   //std::string fileName =  "G:\\Data\\T2AXIALFSE_4_exampleDataProvidedBySatish\\COH833.mha";
   //std::string outputFolder = "G:\\Data\\T2AXIALFSE_4_exampleDataProvidedBySatish";



     typedef itk::Image<float, 3> ImageType;
     typedef ImageType::Pointer   ImagePointerType;

     typedef itk::HaralickFilter<ImageType, ImageType>  FilterType;
   
     typedef itk::ImageFileReader<ImageType>  ReaderType;
     typedef itk::ImageFileWriter<ImageType>  WriterType;


     ReaderType::Pointer reader = ReaderType::New();
     WriterType::Pointer writer = WriterType::New();
     FilterType::Pointer filter = FilterType::New();

     
// Load Test Image

     reader->SetFileName(fileName);
     filter->SetInput( reader->GetOutput() );


     //ImageType::SizeType imSize;
     //imSize.Fill(2);
     //filter->SetRadius(imSize); // Radius will be defaulted to 1,1,1 if not set by user
     
     //filter->SetInputImageMinimum(0.0f);
     //filter->SetInputImageMaximum(1237.0f); // Min and max will be defaulted to those of the input image of not set by user

     //filter->SetNumberOfBinsPerAxis(64); // will be defaulted to 16 if not set by user
     
     
     //ImageType::OffsetType offset = {{0, 1, 0}};
     //filter->SetOffset(offset); // Offset will be defaulted to 1,0,0 if not set by user

     //filter->SetNumberOfThreads(32); // Severely affects computational time - 
     //filter->Print(std::cout);

     time_t now = time(0); char* dt = ctime(&now); std::cout << "date/time before: " << dt << std::endl;
     filter->Update();
     now = time(0); dt = ctime(&now); std::cout << "date/time after: " << dt << std::endl;


     writer->SetInput(filter->GetEnergyOutput()); 
     writer->SetFileName(outputFolder + "/Feature_Energy.mha" );
     writer->Update();


     writer->ResetPipeline();
     writer->SetInput(filter->GetEntropyOutput()); 
     writer->SetFileName(outputFolder + "/Feature_Entropy.mha" );
     writer->Update();

     writer->ResetPipeline();
     writer->SetInput(filter->GetCorrelationOutput()); 
     writer->SetFileName(outputFolder + "/Feature_Correlation.mha" );
     writer->Update();

     writer->ResetPipeline();
     writer->SetInput(filter->GetInverseDifferenceMomentOutput());
     writer->SetFileName(outputFolder + "/Feature_InverseDifferenceMoment.mha" );
     writer->Update();
     
     writer->ResetPipeline();
     writer->SetInput(filter->GetInertiaOutput());
     writer->SetFileName(outputFolder + "/Feature_Inertia.mha" );
     writer->Update();

     writer->ResetPipeline();
     writer->SetInput(filter->GetClusterShadeOutput());
     writer->SetFileName(outputFolder + "/Feature_ClusterShade.mha" );
     writer->Update();

     writer->ResetPipeline();
     writer->SetInput(filter->GetClusterProminenceOutput());
     writer->SetFileName(outputFolder + "/Feature_ClusterProminence.mha" );
     writer->Update();

     writer->ResetPipeline();
     writer->SetInput(filter->GetHaralickCorrelationOutput());
     writer->SetFileName(outputFolder + "/Feature_HaralickCorrelation.mha" );
     writer->Update();

  return EXIT_SUCCESS;
}
