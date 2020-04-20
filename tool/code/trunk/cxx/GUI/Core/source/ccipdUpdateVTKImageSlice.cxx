


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdUpdateVTKImageSlice.h"

// std includes
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkSmartPointer.h>
  #include <vtkImageSlice.h>       // the actual image prop
  #include <vtkImageSliceMapper.h> // for changing slices
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageSlice(
        vtkPropPointer & imageProp,
  const unsigned int     slice,
  const bool             verbose )
{

  // simple error checking
  if ( !imageProp )
    {
    if ( verbose ) cout << "Error: No image prop to update!" << endl;
    return;
    } // imageProp

  // first, try to cast it 
  if ( verbose ) cout << "Casting to image slice:" << endl;

  vtkImageSlice * const imageSlice =
    dynamic_cast< vtkImageSlice * >( imageProp.GetPointer() );

  if ( !imageSlice )
    {
    if ( verbose ) cerr << "Error: Unable to cast to image slice." << endl;
    return;
    } // imageSlice

  if ( verbose ) cout << "Casting to image slice done." << endl;

  // now, extract the mapper
  if ( verbose ) cout << "Extracting mapper:" << endl;

  vtkImageSliceMapper * const mapper =
    dynamic_cast< vtkImageSliceMapper * >( imageSlice->GetMapper() );

  if ( !mapper )
    {
    if ( verbose ) cerr << "Unable to extract mapper!" << endl;
    return;
    } // mapper

  if ( verbose ) cout << "Extracting mapper done." << endl;

  // now, update the slice

  if ( verbose ) cout << "Changing mapper slice:" << endl;
  mapper->SetSliceNumber( slice );
  if ( verbose ) cout << "Changing mapper slice done." << endl;

  // all done!

} // UpdateVTKImageSlice
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd