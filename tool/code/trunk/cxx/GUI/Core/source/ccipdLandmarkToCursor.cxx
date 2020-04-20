


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdLandmarkToCursor.h"

// vtk includes
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCursor3D.h>
#include <vtkPolyDataMapper.h>

#include "ccipdDisableWarningsMacro.h"
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer CreateCursorAtLandmark(
  const LandmarkPointType & landmark,
  const LandmarkType        xRadius,
  const LandmarkType        yRadius,
  const LandmarkType        zRadius )
{

  // make sure we are the correct type
  // and compute the bounds
  const double
    centerX = static_cast< double >( landmark[ 0 ] ),
    centerY = static_cast< double >( landmark[ 1 ] ),
    centerZ = static_cast< double >( landmark[ 2 ] ),
    minX = centerX - static_cast< double >( xRadius ),
    minY = centerY - static_cast< double >( yRadius ),
    minZ = centerZ - static_cast< double >( zRadius ),
    maxX = centerX + static_cast< double >( xRadius ),
    maxY = centerY + static_cast< double >( yRadius ),
    maxZ = centerZ + static_cast< double >( zRadius );

  // create a cursor
  typedef vtkSmartPointer< vtkCursor3D > vtkCursor2DPointer;
  const vtkCursor2DPointer cursor = vtkCursor2DPointer::New();

  // give it the coordinates
  cursor->SetModelBounds( minX, maxX, minY, maxY, minZ, maxZ );
  cursor->SetFocalPoint ( centerX, centerY, centerZ );

  // set up the properties to create a crosshair
  cursor->AllOff();
  cursor->AxesOn();
  cursor->Update();

  // create a mapper for the cursor
  const vtkPolyDataMapperPointer cursorMapper = vtkPolyDataMapperPointer::New();
  cursorMapper->SetInputConnection( cursor->GetOutputPort() );
    
  // create an actor for the cursor
  const vtkActorPointer cursorActor = vtkActorPointer::New();
  cursorActor->SetMapper( cursorMapper );

  // output
  return cursorActor;

} // CreateCursorAtLandmark
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
