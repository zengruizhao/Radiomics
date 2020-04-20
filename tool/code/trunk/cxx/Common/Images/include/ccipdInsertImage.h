#ifndef __ccipdInsertImage_h
#define __ccipdInsertImage_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdInsertImage.hxx"
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
#include "ccipdSliceTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

/** \brief  : Takes a section of one image and paste into another image.
  * \Created: Ahmad 1/17/2014
  * \Updated:  
  * \ingroup: Common Images
  * \param  : pDestinationImage: The destinationtion image into which a region will be inserted
  * \param  : pSourceImage     : The source image from which a region will be copied
  * \param  : destinationIndex : The index of the destination image at which insertion starts
  * \param  : sourceRegion     : The region to be copied from the source image
  * \return : A smart pointer to the modified destination image
  * \Note(s): If the sourceRegion is NULL, then the whole sourceImage will be inserted into 
  *           the destinationImage at the given destinationIndex
  */
template <class TInputImage, class TOutputImage>
typename TOutputImage::Pointer 
InsertRegion(  typename TInputImage::Pointer    pSourceImage
             , typename TOutputImage::Pointer   pDestinationImage
             , typename TOutputImage::IndexType destinationIndex 
             , typename TInputImage::RegionType sourceRegion       //= typename TInputImage::RegionType()
		    );

/** \brief  : A special case of InsertRegion where the whole source image is inserted into the destination image.
  * \Created: Ahmad 1/17/2014
  * \Updated:  
  * \ingroup: Common Images
  * \param  : pDestinationImage: The destinationtion image into which a region will be inserted
  * \param  : pSourceImage     : The source image from which a region will be copied
  * \param  : destinationIndex : The index of the destination image at which insertion starts
  * \return : A smart pointer to the modified destination image
  */
template <class TInputImageType, class TOutputImage>
typename TOutputImage::Pointer 
InsertImage(  typename TInputImageType::Pointer    pSourceImage
            , typename TOutputImage::Pointer       pDestinationImage
            , typename TOutputImage::IndexType     destinationIndex 
		   );




} // namespace
#endif // __ccipdInsertImage_h

