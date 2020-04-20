

////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdHistogram.hxx"
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
ccipd::HistogramDPointer Get1DHistogram(
	const VolumeImageType * const inImage,
	const int                     binNumber )
{
  return Get1DHistogram<VolumeImageType>(inImage, binNumber);
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
void SaveHist(  
  const HistogramTypeD * const histogram, 
  const std::string         & outFilename)
{
  FILE * file = fopen (outFilename.c_str(), "w");
  if (file==NULL)
    {
    cerr << "Unable to write " << outFilename << endl;
    return;
    }

  unsigned int totalBins = 1;
  for(unsigned int i = 0; i < 1; ++i)
    totalBins *= static_cast< unsigned int >( histogram->GetSize()[i] );

  float sum = 0.0;
  for(unsigned int i = 0; i < totalBins; ++i)
    sum += histogram->GetFrequency(i);

  for(unsigned int i = 0; i < totalBins; ++i)
    fprintf(
      file, "%17.10f %17.10f\n",
      histogram->GetMeasurementVector(i)[0], histogram->GetFrequency(i)/sum);

  fclose(file);

} // SaveHist
////////////////////////////////////////////////////////////////////////////////////////////////////



} //namespace ccipd