#ifndef __ccipdExtractFeatureImage_h
#define __ccipdExtractFeatureImage_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdExtractFeatureImage.hxx"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will take one of the
 * components from a feature image
 * and copy it into its own new image.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeImagePointer ExtractFeatureImage(  const VolumeFeaturesType * featureImage
                                       , const unsigned int         whichComponent 
                                      );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This templated function actually performs the calculation.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template< class TPixelType, unsigned int Dimension >
typename itk::Image< TPixelType, Dimension >::Pointer 
ExtractFeatureImage(  const itk::VectorImage< TPixelType, Dimension > * featureImage
                    , const unsigned int                                whichComponent 
                   );
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Removes an indexed component from a given vector image
 * 
 * \author  Ahmad
 * \ingroup Common Images
 */
template< class TPixelType, unsigned int Dimension >
typename itk::VectorImage< TPixelType, Dimension >::Pointer 
RemoveFeatureImage(  const itk::VectorImage< TPixelType, Dimension > * featureImage
                   , const unsigned int                                componentIndex 
                  );




} // namespace
#endif // __ccipdExtractFeatureImage_h
