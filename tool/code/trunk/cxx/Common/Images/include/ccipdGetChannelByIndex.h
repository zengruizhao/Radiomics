#ifndef __ccipdGetChannelByIndex_h__
#define __ccipdGetChannelByIndex_h__

#include "ccipdVolumeTypes.h"
#include "ccipdGetChannelByIndex.hxx"


namespace ccipd
{

	

/**
 * Gets an indexed component from a given vector image
 * 
 * \author  Ahmad
 * \ingroup Common Images
 */
template< class TVectorImage, class TScalarImage >
typename TScalarImage::Pointer 
GetChannelByIndex(  const typename TVectorImage::Pointer input
                  , const unsigned int                   index 
				 );

}




#endif