#include "ccipdCore.h"

#include "ccipdHistogram.h"


#include "ccipdStandardizeImage.h"
#include "ccipdDisableWarningsMacro.h"


#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>


#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>


using namespace ccipd;

#include <string>
#include <iostream>


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

  VolumeImageType::Pointer pStandardizedOutputImage = ccipd::StandardizeImage<VolumeImageType, HistogramTypeF>(pInputImage, pReferenceHistogram);

  SaveImage<VolumeImageType>(pStandardizedOutputImage, argv[3]);

//=================================================================================
  // Convert Histogram to a vtkTable
  unsigned int histogramSize = pReferenceHistogram->GetSize()[0];

  typedef itk::Statistics::ImageToHistogramFilter<VolumeImageType> HistogramFilterType;
  HistogramFilterType::Pointer pHistogramFilter = HistogramFilterType::New();
  pHistogramFilter->SetHistogramSize(pReferenceHistogram->GetSize());
  pHistogramFilter->SetAutoMinimumMaximum(true);
  pHistogramFilter->SetInput(pInputImage);
  pHistogramFilter->Update();
  HistogramTypeD * pSourceHistogram = pHistogramFilter->GetOutput();

  HistogramFilterType::Pointer pHistogramFilter2 = HistogramFilterType::New();
  pHistogramFilter2->SetHistogramSize(pReferenceHistogram->GetSize());
  pHistogramFilter2->SetAutoMinimumMaximum(true);
  pHistogramFilter2->SetInput(pStandardizedOutputImage);
  pHistogramFilter2->Update();
  HistogramTypeD * pStandardizedHistogram = pHistogramFilter2->GetOutput();

//=================================================================================

// Display the two histograms
  // Create a table with some points in it
  vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
 
  vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
  arrX->SetName("X Axis");
  table->AddColumn(arrX);
 
  vtkSmartPointer<vtkFloatArray> arrC = vtkSmartPointer<vtkFloatArray>::New();
  arrC->SetName("Reference Histogram"); // red
  table->AddColumn(arrC);
 
  vtkSmartPointer<vtkFloatArray> arrS = vtkSmartPointer<vtkFloatArray>::New();
  arrS->SetName("Source Histogram"); // green
  table->AddColumn(arrS);

  vtkSmartPointer<vtkFloatArray> arrT = vtkSmartPointer<vtkFloatArray>::New();
  arrT->SetName("Standardized Histogram"); // blue
  table->AddColumn(arrT);
 
  // Fill in the table with histograms' values
  table->SetNumberOfRows(histogramSize);
  for (unsigned int bin = 0; bin < histogramSize; bin++ )
  {
    table->SetValue(bin, 0, bin);
    table->SetValue(bin, 1, pReferenceHistogram->GetFrequency(    bin, 0 ));
    table->SetValue(bin, 2, pSourceHistogram->GetFrequency(       bin, 0 ));
    table->SetValue(bin, 3, pStandardizedHistogram->GetFrequency( bin, 0 ));
  }
 
  // Set up the view
  vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
 
  // Add multiple line plots, setting the colors etc
  vtkSmartPointer<vtkChartXY> chart = vtkSmartPointer<vtkChartXY>::New();
  view->GetScene()->AddItem(chart);
  
  vtkPlot *line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, 1);
  line->SetColor(255, 0, 0, 255);
  line->SetWidth(5.0);
  //line->GetPen()->SetLineType(2);//For dotted line, can be from 2 to 5 for different dot patterns
  
  line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, 2);
  line->SetColor(0, 255, 0, 255);
  line->SetWidth(1.0);
  
  line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, 3);
  line->SetColor(0, 0, 255, 255);
  line->SetWidth(1.0);


  //line->GetPen()->SetLineType(2);//For dotted line, can be from 2 to 5 for different dot patterns


  // Start interactor
  view->GetInteractor()->Initialize();
  view->GetInteractor()->Start();


  return EXIT_SUCCESS;
}
