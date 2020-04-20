#ifndef __ccipdHistogram_h
#define __ccipdHistogram_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
#include "ccipdHistogramTypes.h"
#include "ccipdHistogram.hxx"
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{

/**
 * Create 1D histogram from an image (templated class)
 * \param inImage the image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
HistogramDPointer Get1DHistogram(
	const TInputImage * const inImage,
	const int                 binNumber = 24 );

/**
 * Create 1D histogram from an VolumeImageType
 * \param inImage the image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
HistogramDPointer Get1DHistogram(
	const VolumeImageType * const inImage,
	const int                     binNumber = 24 );

/**
 * Write histogram to file
 * \param hist who
 * \param filename where to write
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
void SaveHist(  
  const HistogramTypeD * const histogram, 
  const std::string         & outFilename
  );


/** \brief  : This function Loads a templated histogram from a .csv file
  * \Created: Ahmad Algohary 9/16/2013
  * \Updated:  
  * \ingroup: Filtering Core
  * \param  : fileName: string holding the file name, this must have a ".csv" extension
  * \param  :   
  * \return : smart Pointer to a templated Histogram type (itk::Statistics::Histogram)
  * \Notes  : This function assumes a .csv file with data either coming in 1 column of frequencies for each histogram bin
              All decimal points of the given frequencies will be ignored - frequencies' type are restricted to unsigned int
  */
template <class THistogramType>
typename THistogramType::Pointer LoadHistogram(const std::string &fileName);

/** \brief  : This function Writes a templated histogram to a .csv file
  * \Created: Ahmad Algohary 9/17/2013
  * \Updated:  
  * \ingroup: Filtering Core
  * \param  : fileName: string holding the file name, this must have a ".csv" extension
  * \param  : pHistogram: Smart Pointer to a templated Histogram type (itk::Statistics::Histogram)  
  * \return : bool: true if writing was successful, otherwise: false
  * \Notes  : This function assumes a .csv file with data coming in 1 column of frequencies for each histogram bin
  */
template <class THistogramType>
bool WriteHistogram(const typename THistogramType::Pointer &pHistogram, const std::string &fileName);



} //namespace ccipd

#endif //__ccipdHistogram_h
