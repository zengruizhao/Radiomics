

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUpdateVTKPropTransparency.h"

// std includes
#include <iostream> // for cout, etc.

// vtk includes
#include <vtkSmartPointer.h>
#include <vtkActor.h>         // the actual actor prop
#include <vtkImageSlice.h>    // the actual image prop
#include <vtkProperty.h>      // stores the transparency
#include <vtkImageProperty.h> // stores the transparency
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKPropTransparency(
  const vtkPropPointer & prop,
  const double           opacity,
  const bool             verbose )
{

  // some error checking
  if ( !prop )
    {
    if ( verbose ) cerr << "Error: no prop to update the transparency!" << endl;
    return;
    }

  if ( opacity < 0.0 || opacity > 1.0 )
    {
    if ( verbose )
      cerr << "Error: The opacity was given as " << opacity <<
      ". However, it must be between 0 and 1 (inclusive)." << endl;
    return;
    } // opacity

  // try casting it to an image slice
  vtkImageSlice * const imageSlice = 
    dynamic_cast< vtkImageSlice * >( prop.GetPointer() );

  // if successful, update the transparency
  if ( imageSlice )
    {
    if ( verbose ) cout << "Updating the transparency of the image slice." << endl;
    imageSlice->GetProperty()->SetOpacity( opacity );
    return;
    } // imageSlice

  // try an actor
  vtkActor * const actor =
    dynamic_cast< vtkActor * >( prop.GetPointer() );

  // if successful, update the transparency
  if ( actor )
    {
    if ( verbose ) cout << "Updating the transparency of the actor." << endl;
    actor->GetProperty()->SetOpacity( opacity );
    return;
    } // actor

  // if we haven't cast it to something,
  // we are at a loss!
  if ( verbose ) cerr << "Error: Unknown prop type." << endl;
  return;

} // UpdateVTKPropTransparency
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKPropTransparency(
  const vtkPropPointer & prop,
  const unsigned int     opacity,
  const bool             verbose )
{

  if ( opacity > 100 ) return;

  return UpdateVTKPropTransparency(
    prop,
    static_cast< double >( opacity ) / 100.0,
    verbose );

} // UpdateVTKPropTransparency
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
