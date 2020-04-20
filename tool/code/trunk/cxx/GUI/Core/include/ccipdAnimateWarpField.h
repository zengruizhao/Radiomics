#ifndef __ccipdAnimateWarpField_h
#define __ccipdAnimateWarpField_h


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdRGBSliceTypes.h"

// std includes
#include <string>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Function to slowly warp and image and save frames.
 * 
 * A warp field will be scaled by a factor
 * using VectorScalePixelAccessor()
 * for each frame, and the scaled warp field
 * will be applied to the image.
 * 
 * The resulting frames will be saved to the disk.
 * 
 * \param[in] warpField            The full warp field from moving to moved.
 * \param[in] fixedImage           The fixed image.
 * \param[in] movingImage          The moving image.
 * \param[in] numberOfFrames       The warp field will be scaled this many increments.
 * \param[in] movingImageOpacity   255 is fully opaque, 0 is fully transparent.
 * \param[in] makeBlackTransparent If true, all RGB(0,0,0) pixels will be transparent.
 * \param[in] outputDirectory      The frames will be saved in this directory as images.
 * \param[in] fileNameFormat       sprintf style format for file names.
 * 
 * \date    2012-05-09
 * \author  Rob Toth
 * \ingroup Registration
 */
void GenerateWarpFieldAnimationFrames(
  const SliceWarpFieldType * const warpField,
  const RGBSliceType       * const fixedImage,
  const RGBSliceType       * const movingImage,
  const unsigned int               numberOfFrames,
  const MaskPixelType              movingImageOpacity   = 128,
  const bool                       makeBlackTransparent = true,
  const std::string              & outputDirectory      = ".",
  const std::string              & fileNameFormat       = "%05d.png" );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class to multiply a vector image by a scalar.
 * 
 * http://bit.ly/KERIc6
 * 
 * \date    2012-05-09
 * \author  Rob Toth
 * \ingroup Registration
 */
template< class TPixelType, unsigned int TDimension >
class VectorScalePixelAccessor
{
public:
   typedef itk::Vector< TPixelType, TDimension > VectorPixelType;
   typedef VectorPixelType InternalType;
   typedef VectorPixelType ExternalType;

   VectorScalePixelAccessor();
   VectorScalePixelAccessor( const TPixelType scale );
   void operator=( const VectorScalePixelAccessor & other );
   void SetScale( const TPixelType scale );
   VectorPixelType Get( const VectorPixelType & input ) const;

private:
   TPixelType m_Scale;
}; // class VectorScalePixelAccessor
//////////////////////////////////////////////////////////////////////////////////////////




} // namespace ccipd
#endif // __ccipdAnimateWarpField_h 
