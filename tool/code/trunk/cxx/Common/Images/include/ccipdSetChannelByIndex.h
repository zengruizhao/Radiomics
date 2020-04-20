#ifndef __ccipdSetChannelByIndex_h__
#define __ccipdSetChannelByIndex_h__

#include "ccipdVolumeTypes.h"
#include "ccipdSetChannelByIndex.hxx"


namespace ccipd
{	

/**
 * Sets an indexed component of a given vector image
 * 
 * \author  Ahmad
 * \ingroup Common Images
 */
template< class TVectorImage, class TScalarImage >
void
SetChannelByIndex(  typename TVectorImage::Pointer & vectorImage
                  , typename TScalarImage::Pointer & scalarImage
                  , const    unsigned int            index 
			     );


}




#endif