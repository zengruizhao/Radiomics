

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUpdateVTKImageBlinds.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageBase.h>

  // vtk includes
  #include <vtkImageSlice.h>
  #include <vtkImageProperty.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void UpdateVTKImageBlinds(
  const itk::ImageBase< TDimension > & image,
        vtkImageSlice                & imageSlice,
  const unsigned int                   horizontalBlinds,
  const unsigned int                   verticalBlinds )
{

  // which blinds are we displaying?
  const bool
    hasHorizontalBlinds = horizontalBlinds > 1,
    hasVerticalBlinds   = verticalBlinds   > 1;

  std::cout << "Horizontal blinds = " << horizontalBlinds << std::endl;
  std::cout << "Vertical blinds = "   << verticalBlinds   << std::endl;

  // turn on the checkerboard
  vtkImageProperty * const imageProperty = imageSlice.GetProperty();

  if ( !hasHorizontalBlinds && !hasVerticalBlinds )
    {

    // no blinds should be shown
    imageProperty->CheckerboardOff();
    return;

    }

  // some blinds should be shown
  imageProperty->CheckerboardOn();

  // image type
  typedef itk::ImageBase< TDimension > ImageType;

  // how many squares?
  const typename ImageType::RegionType::SizeType
    imageSize = image.GetBufferedRegion().GetSize();
  const typename ImageType::SpacingType
    imageSpacing = image.GetSpacing();
  const double
    imageSpacingX        = static_cast< double >( imageSpacing [ 0 ] ),
    imageSizeX           = static_cast< double >( imageSize    [ 0 ] ),
    imageSpacingY        = static_cast< double >( imageSpacing [ 1 ] ),
    imageSizeY           = static_cast< double >( imageSize    [ 1 ] ),
    imageWidth           = imageSpacingX * imageSizeX,
    imageHeight          = imageSpacingY * imageSizeY,
    checkerboardSpacingX = !hasVerticalBlinds ? imageWidth  * 2 :
      imageWidth / verticalBlinds,
    checkerboardSpacingY = !hasHorizontalBlinds ? imageHeight * 2 :
      imageHeight / horizontalBlinds;
  
  std::cout << "checkerboardSpacingX = " << checkerboardSpacingX << std::endl;
  std::cout << "checkerboardSpacingY = " << checkerboardSpacingY << std::endl;

  // set the checkerboard properties
  imageProperty->SetCheckerboardSpacing(
    checkerboardSpacingX, checkerboardSpacingY );
        
  // create a random offset between -0.25 and +0.25
  // the 0.5 is so there isn't a big line down the center of the image
  const double
    randomIntegerX = static_cast< double >( rand() ),
    randomDoubleX  = randomIntegerX / static_cast< double > ( RAND_MAX ),
    randomOffsetX  = !hasVerticalBlinds ? 0.5 : randomDoubleX / 2.0 - 0.25,
    randomIntegerY = static_cast< double >( rand() ),
    randomDoubleY  = randomIntegerY / static_cast< double > ( RAND_MAX ),
    randomOffsetY  = !hasHorizontalBlinds ? 0.5 : randomDoubleY / 2.0 - 0.25;
  
  std::cout << "checkerboardOffsetX = " << randomOffsetX << std::endl;
  std::cout << "checkerboardOffsetY = " << randomOffsetY << std::endl;

  imageProperty->SetCheckerboardOffset( randomOffsetX, randomOffsetY );

} // UpdateVTKImageBlinds
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void UpdateVTKImageBlinds(
  const itk::ImageBase< TDimension > & image,
        vtkProp                      & imageProp,
  const unsigned int                   horizontalBlinds,
  const unsigned int                   verticalBlinds )
{
  // try to cast it as an image slice
  try
    {
    vtkImageSlice & imageSlice = 
      dynamic_cast< vtkImageSlice & >( imageProp );

    return UpdateVTKImageBlinds(
      image,
      imageSlice,
      horizontalBlinds,
      verticalBlinds );
    }
  catch( ... )
    {
    // ignore errors
    }

} // UpdateVTKImageBlinds
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
