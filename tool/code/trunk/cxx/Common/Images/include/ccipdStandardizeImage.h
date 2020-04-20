#ifndef __ccipdStandardizeImage_h
#define __ccipdStandardizeImage_h

//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdStandardizeImage.hxx"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

template<class TImageType, class THistogramType>
typename TImageType::Pointer
StandardizeImage(  typename TImageType::Pointer pInputImage
                 , typename THistogramType::Pointer pReferenceHistogram
                );

}

#endif