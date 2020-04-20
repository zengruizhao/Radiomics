#ifndef __ccipdInsertImage_hxx
#define __ccipdInsertImage_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdInsertImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegion.h>
  #include <itkImage.h>
  #include <itkFixedArray.h>
  #include <itkRegionOfInterestImageFilter.h>

  #include <itkPasteImageFilter.h>


#include "ccipdEnableWarningsMacro.h"

// std includes
#include <cmath> // for floor and ceil
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

template <class TInputImage, class TOutputImage>
typename TOutputImage::Pointer 
InsertRegion(  typename TInputImage::Pointer    pSourceImage
             , typename TOutputImage::Pointer   pDestinationImage
             , typename TOutputImage::IndexType destinationIndex 
             , typename TInputImage::RegionType sourceRegion       
		    )
{
	// Check if pointers are valid
	if(!pSourceImage || !pDestinationImage)
	{
		std::cerr << "\nError: Invalid input Images ... \n"
		          << __FILE__ << "  " << __LINE__ ;
		return NULL;
	}

    // Check if sourceRegion (if not null) lies inside the pSourceImage 
	if(!sourceRegion.GetSize()[0])
	{
		sourceRegion = pSourceImage->GetLargestPossibleRegion();
	}
	else
	{
	    if(!sourceRegion.IsInside(pSourceImage->GetBufferedRegion()))
		{
			std::cerr << "\\nError: Source region is not totally inside the source image ... \n"
		              << __FILE__ << "  " << __LINE__ ;
		    return NULL;
		}
	}

	// Check if destinationIndex lies inside the pDestinationImage

	// Check if source region is inside the destination image at the given index


	typedef itk::PasteImageFilter <TInputImage, TOutputImage > PasteImageFilterType;
    typename PasteImageFilterType::Pointer pasteFilter = PasteImageFilterType::New ();
    pasteFilter->SetSourceImage(pSourceImage);
    pasteFilter->SetDestinationImage(pDestinationImage);
	pasteFilter->SetDestinationIndex(destinationIndex);
	pasteFilter->SetSourceRegion(sourceRegion);
	pasteFilter->Update();
	return pasteFilter->GetOutput();
}

template <class TInputImageType, class TOutputImage>
typename TOutputImage::Pointer 
InsertImage(  typename TInputImageType::Pointer    pSourceImage
            , typename TOutputImage::Pointer       pDestinationImage
            , typename TOutputImage::IndexType     destinationIndex 
		   )
{
	// Check only if source image pointer is valid, additional checks will be applied in the InsertRegion function 
	if(!pSourceImage || !pDestinationImage)
	{
		std::cerr << "\nError: Invalid input Images ... \n"
		          << __FILE__ << "  " << __LINE__ ;
		return NULL;
	}

	return InsertRegion<TInputImageType, TOutputImage>(pSourceImage, pDestinationImage, destinationIndex, pSourceImage->GetBufferedRegion());
}

} // namespace
#endif // __ccipdInsertImage_hxx
