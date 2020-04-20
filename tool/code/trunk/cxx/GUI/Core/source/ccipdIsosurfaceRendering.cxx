


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdIsosurfaceRendering.h"
#include "ccipdITKImageToVTKImage.h" // so we can use ITK images
#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkVersion.h>
  #include <vtkMarchingCubes.h>  // the isosurface algorithm
  #include <vtkPolyDataMapper.h> // how to map it to the actor
  #include <vtkImageData.h>      // the input type
  #include <vtkActor.h>          // the output type
  #include <vtkProperty.h>
  #include <vtkSmartPointer.h>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer GenerateVTKActorFromIsosurface(
  const VolumeImageType * const image,
  const InternalPixelType       isosurfaceValue )
{

  const vtkImageDataPointer
    imageAsVTKImage = ConvertITKImageToVTKImage( image ),
    duplicateImage  = vtkImageDataPointer::New();

  // we must duplicate the image because
  // if the original, input ITK image was
  // deleted, and we are still trying to
  // render it, we will get "access violation"
  // errors
  duplicateImage->DeepCopy( imageAsVTKImage );

  return GenerateVTKActorFromIsosurface(
    duplicateImage, isosurfaceValue );

} // GenerateVTKActorFromIsosurface
//////////////////////////////////////////////////////////////////////////////////////////

vtkActorPointer GenerateVTKActorFromIsosurface(  const RGBVolumeType    * const image
											   , const InternalPixelType        isosurfaceValue
											  )
{
	const vtkImageDataPointer imageAsVTKImage = ConvertITKImageToVTKImage(image);
	const vtkImageDataPointer  duplicateImage = vtkImageDataPointer::New();
	duplicateImage->DeepCopy(imageAsVTKImage);
	return GenerateVTKActorFromIsosurface(duplicateImage, isosurfaceValue);
} // GenerateVTKActorFromIsosurface


//////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer GenerateVTKActorFromIsosurface(  const vtkImageDataPointer & image
											   , const InternalPixelType     isosurfaceValue 
											  )
{
  // create a marching cubes algoritm
  typedef vtkSmartPointer< vtkMarchingCubes > vtkMarchingCubesPointer;
  vtkMarchingCubesPointer marchingCubes = vtkMarchingCubesPointer::New();

  // give the data to the marching cubes
#if VTK_MAJOR_VERSION >= 6
  marchingCubes->SetInputData(image);
#else
  marchingCubes->SetInput( image );
#endif
    
  marchingCubes->SetValue( 0,  static_cast< double >( isosurfaceValue ) ); // it needs a double

  // create a mapper for the marching cubes data
  const vtkPolyDataMapperPointer mapper = vtkPolyDataMapperPointer::New();
  mapper->SetInputConnection( marchingCubes->GetOutputPort() );

  // don't use the mapper to display colors
  mapper->ScalarVisibilityOff();

  // create an actor
  const vtkActorPointer actor = vtkActorPointer::New();

  // give the maper to the actor
  actor->SetMapper( mapper );

  const double
    surfaceR       = 0.31,
    surfaceG       = 0.31,
    surfaceB       = 1.00,
    surfaceOpacity = 1.00;
  actor->GetProperty()->SetColor( surfaceR, surfaceG, surfaceB );
  actor->GetProperty()->SetOpacity( surfaceOpacity );
  
  // output
  return actor;

} // GenerateVTKActorFromIsosurface
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
