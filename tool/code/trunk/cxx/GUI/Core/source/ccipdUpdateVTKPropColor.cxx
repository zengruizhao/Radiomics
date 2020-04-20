

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUpdateVTKPropColor.h"
#include "ccipdDisableWarningsMacro.h"

// std includes
#include <iostream> // for cout, etc.

//Qt includes
#include <QColorDialog>

// vtk includes
#include <vtkSmartPointer.h>
#include <vtkActor.h>         // the actual actor prop
#include <vtkImageSlice.h>    // the actual image prop
#include <vtkProperty.h>      // stores the color
#include <vtkImageProperty.h> // stores the color
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKPropColorGUI(const vtkPropPointer & prop)
{

  // some error checking
  if ( !prop )
  {
     cerr << "Error: no prop to update the color!" << endl;
     return;
  }

  // Let the user selects the desired color
  QColor color = QColorDialog::getColor();
  

  // try casting it to an image slice
  vtkImageSlice * const imageSlice = dynamic_cast< vtkImageSlice * >( prop.GetPointer() );

  // if successful, update the color
  if ( imageSlice )
    {
		cerr << "Error: prop to update is an image slice ... not relevant type !" << endl;
		//imageSlice->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
    return;
    } // imageSlice

  // try an actor
  vtkActor * const actor = dynamic_cast< vtkActor * >( prop.GetPointer() );

  // if successful, update the color
  if ( actor )
    {
		actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
    return;
    } // actor

  // if we haven't cast it to something,
  // we are at a loss!
  cerr << "Error: Unknown prop type." << endl;
  return;

} // UpdateVTKPropColor
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
