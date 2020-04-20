#ifndef __ccipdHistogram_hxx
#define __ccipdHistogram_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
#include "ccipdHistogramTypes.h"
#include "ccipdHistogram.h"

// std includes
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkHistogram.h>
  #include <itkCSVArray2DFileReader.h>
  #include <itkCSVNumericObjectFileWriter.h>
  #include <itkCSVArray2DDataObject.h>
  #include <itkImageToHistogramFilter.h>
  #include <itkMinimumMaximumImageCalculator.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage>
HistogramDPointer Get1DHistogram(
	const TInputImage * const inImage,
	const int                 binNumber )
{

  typedef itk::MinimumMaximumImageCalculator < TInputImage >     ImageCalculatorFilterType;
 
  typename ImageCalculatorFilterType::Pointer imageCalculatorFilter
          = ImageCalculatorFilterType::New ();
  imageCalculatorFilter->SetImage(inImage);
  imageCalculatorFilter->Compute();

  double 
    min = imageCalculatorFilter->GetMinimum(),
    max = imageCalculatorFilter->GetMaximum();

  cout << "Computing histogram with " << binNumber << " bins in [" << min << ","<< max << "]" << endl;  
  const unsigned int MeasurementVectorSize = 1; // gray levels
 
  typedef itk::Statistics::ImageToHistogramFilter< TInputImage > ImageToHistogramFilterType;
 
  typename ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(MeasurementVectorSize);
  typename ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(MeasurementVectorSize);
  lowerBound.Fill(min);
  upperBound.Fill(max) ;
 
  typename ImageToHistogramFilterType::HistogramType::SizeType size(MeasurementVectorSize);
  size.Fill(binNumber);
 
  typename ImageToHistogramFilterType::Pointer imageToHistogramFilter = ImageToHistogramFilterType::New();
  imageToHistogramFilter->SetInput                (inImage);
  imageToHistogramFilter->SetHistogramBinMinimum  (lowerBound);
  imageToHistogramFilter->SetHistogramBinMaximum  (upperBound);
  imageToHistogramFilter->SetHistogramSize        (size);
  imageToHistogramFilter->SetAutoMinimumMaximum   (false);
  imageToHistogramFilter->Update();
 
  const HistogramDPointer histogram = imageToHistogramFilter->GetOutput();
  
  return histogram;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

template <class THistogramType>
typename THistogramType::Pointer LoadHistogram(const std::string &fileName)
{
  // Check if the fileName's extension == .csv
  //std::string::size_type pos = filename.rfind( "." );
  //std::string            filepre = std::string( filename, 0, pos );
  //std::string            extension;
  //if( pos != std::string::npos )
  //  {
  //  extension = std::string( filename, pos, filename.length() - 1);
  //  if( extension == std::string(".gz") )
  //    {
  //    pos = filepre.rfind( "." );
  //    extension = std::string( filepre, pos, filepre.length() - 1 ) + extension;
  //    filepre = std::string( filepre, 0, pos );
  //    }
  //  }

  typedef itk::CSVArray2DFileReader< typename THistogramType::MeasurementType> CSVReaderType; 
  typename CSVReaderType::Pointer reader = CSVReaderType::New();
  reader->SetFileName( fileName ); 
  reader->SetFieldDelimiterCharacter( ',' ); 
  //reader->SetStringDelimiterCharacter( '"' ); 
  reader->HasColumnHeadersOff(); 
  reader->HasRowHeadersOff(); 
  //reader->UseStringDelimiterCharacterOn();
  reader->Update();
  typename itk::CSVArray2DDataObject<typename THistogramType::MeasurementType>::MatrixType matrix = reader->GetOutput()->GetMatrix() ;
  
  unsigned int rows = matrix.rows();
  unsigned int cols = matrix.cols();
  
  if(0 == rows || 0 == cols || cols > 2 || (1 == rows && 1 == cols))
  {
    // Error
    return NULL;
  }

  const unsigned int numberOfComponents = 1; //cols == 1 ? cols : cols - 1;
  typename THistogramType::Pointer pHistogram = THistogramType::New();
  pHistogram->SetMeasurementVectorSize(numberOfComponents);
  typename THistogramType::SizeType histogramSize(numberOfComponents);
  histogramSize.Fill(rows);

  pHistogram->Initialize(histogramSize);
  pHistogram->SetBinMax(0, 0, 1);

  for(unsigned int index = 0; index < histogramSize[0] ; index++)
  {
    // Histogram may be just a list of frequencies
    if(!pHistogram->SetFrequency(index, (unsigned int)matrix(index, 0))) 
    {
      // Error
      std::cerr << "\nError(138-262): Histogram Loading error ....";
      return NULL;
    }
  }

  std::cout << "\nNumber of bins  = " << pHistogram->Size()
            << "\nTotal frequency = " << pHistogram->GetTotalFrequency()
            << "\nDimension sizes = " << pHistogram->GetSize() << std::endl;


  //for(unsigned int index = 0; index < histogramSize[0] ; index++)
  //{
  //  std::cout << pHistogram->GetFrequency(index) << "\n";
  //}

  return pHistogram;
}

template <class THistogramType>
bool WriteHistogram(const typename THistogramType::Pointer &pHistogram, const std::string &fileName)
{
  // Check if the fileName's extension == .csv

  unsigned int histogramSize = (unsigned int)pHistogram->GetSize()[0];

  const unsigned int ARows = (unsigned int)pHistogram->GetSize()[0];
  const unsigned int ACols = 1;

  typedef itk::Array2D<typename THistogramType::MeasurementType> ArrayType;
  ArrayType array(ARows, ACols);

  for(unsigned int index = 0; index < histogramSize; index++)
  {
    array[index][0] = (typename THistogramType::MeasurementType)pHistogram->GetFrequency(index);
  }
  

  typedef itk::CSVNumericObjectFileWriter<typename THistogramType::MeasurementType/*, ARows, ACols*/> CSVWriterType; 
  typename CSVWriterType::Pointer pWriter = CSVWriterType::New();
  pWriter->SetFileName( fileName ); 
  pWriter->SetFieldDelimiterCharacter( ',' );
  pWriter->SetInput(&array);

  try
  {
    pWriter->Write();
  }
  catch (itk::ExceptionObject& exp)
  {
    std::cerr << "Exception caught!" << std::endl;
    std::cerr << exp << std::endl;
    return false;
  }

  return true;
}

} //namespace ccipd
#endif //__ccipdHistogram_hxx
