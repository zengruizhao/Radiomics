#ifndef __ccipdImageToProp_hxx
#define __ccipdImageToProp_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdImageToProp.h"
#include "ccipdITKImageToVTKImage.h"

#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkVersion.h>
  #include <vtkImageData.h>
  #include <vtkSmartPointer.h>
  #include <vtkImageSliceMapper.h>
  #include <vtkImageSlice.h>
  #include <vtkImageMagnitude.h>         // for collapsing components
  #include <vtkImageShiftScale.h>        // for rescaling from 0 to 255
  #include <vtkImageAppendComponents.h>  // for appending an alpha channel
  #include <vtkImageExtractComponents.h> // for just extracting RGB
  #include <vtkImageCast.h>              // for casting back from unsigned char to float
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TImageType >
vtkPropPointer CreatePropFromImage(  const TImageType * const image
	                               , const unsigned int       slice
								   , const bool               makeZeroTransparent 
								  )
{

  // make sure we have an image
  if ( !image )
    return vtkPropPointer();

  // convert it to a vtk image for viewing
  typedef vtkSmartPointer< vtkImageData > vtkImageDataPointer;
  const vtkImageDataPointer vtkImageVolumeOriginal = ConvertITKImageToVTKImage(image);
  const vtkImageDataPointer vtkImageVolume = vtkImageDataPointer::New(); // duplicated

  // we must duplicate it in case the ITK image ever gets deleted
  vtkImageVolume->DeepCopy( vtkImageVolumeOriginal );

  // create an actor for the image
  // TODO: use vtkImageResliceMapper
  typedef vtkSmartPointer< vtkImageSliceMapper > vtkImageSliceMapperPointer;
  typedef vtkSmartPointer< vtkImageSlice       > vtkImageSlicePointer; 

  // create the vtk objects 
  const vtkImageSliceMapperPointer imageMapper = vtkImageSliceMapperPointer::New();
  const vtkImageSlicePointer       imageSlice  = vtkImageSlicePointer      ::New();

  if ( makeZeroTransparent )
    {

    // first, take the magnitude of the RGB,
    // so that a 0 magnitude means that R, G, and B were all 0
    typedef vtkSmartPointer< vtkImageMagnitude > vtkImageMagnitudePointer;
    const vtkImageMagnitudePointer magnitude = vtkImageMagnitudePointer::New();

    magnitude->SetInputData(vtkImageVolume);



    // We then multiply every pixel by 255,
    // and set anything > 255 to be 255.
    // The only thing remaining **should** be 0 and 255 values
    typedef vtkSmartPointer< vtkImageShiftScale > vtkImageShiftScalePointer;
    const vtkImageShiftScalePointer scale = vtkImageShiftScalePointer::New();
    scale->SetInputConnection( magnitude->GetOutputPort() );
    scale->ClampOverflowOn();
    scale->SetScale( 255 );
    scale->SetOutputScalarTypeToUnsignedChar(); // so it clamps at 255

    // now that it's been clamped to 255, we
    // actually need to back into our original data type
    typedef vtkSmartPointer< vtkImageCast > vtkImageCastPointer;
    const vtkImageCastPointer caster = vtkImageCastPointer::New();
    caster->SetInputConnection( scale->GetOutputPort() );
    caster->SetOutputScalarType( vtkImageVolume->GetScalarType() );

    // now, we have to take our 0 - 255 values
    // and make it the alpha channel
    typedef vtkSmartPointer< vtkImageAppendComponents > vtkImageAppendComponentsPointer;
    const vtkImageAppendComponentsPointer appender = vtkImageAppendComponentsPointer::New();

    // the image might already have an alpha channel, so we must remove it
    const int scalarComponents = vtkImageVolume->GetNumberOfScalarComponents();
    if ( scalarComponents  > 3 )
      {
      typedef vtkSmartPointer< vtkImageExtractComponents > vtkImageExtractComponentsPointer;
      const vtkImageExtractComponentsPointer extractor = vtkImageExtractComponentsPointer::New();
      extractor->SetComponents( 0, 1, 2 ); // R, G, B


      extractor->SetInputData( vtkImageVolume );
         

      // give the extracted RGB channels to the appender
      appender->AddInputConnection( extractor->GetOutputPort() );

      // give the alpha channel to the appender
      appender->AddInputConnection( caster->GetOutputPort() );

      } // >3 components
    else
      {
      // okay, we have enough for an alpha channel
      
      // add in our image

      // in case we have < 3
      for( unsigned int component = scalarComponents; component < 4; ++component )
        {
          appender->AddInputData( vtkImageVolume );
        } // for component

      // add in 1 alpha channel
      appender->AddInputConnection( caster->GetOutputPort() );

      } // if >3 components

    imageMapper->SetInputConnection( appender->GetOutputPort() );

    } // makeZeroTransparent
  else
    {
      // just use the raw image
      imageMapper->SetInputData( vtkImageVolume );  
    }
  
  // connect the components
  imageSlice->SetMapper( imageMapper );

  // which slice do we want?
  imageMapper->SetSliceNumber( static_cast< int >( slice ) );

  // output
  return imageSlice;

} // CreatePropFromImage
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdImageToProp_hxx
