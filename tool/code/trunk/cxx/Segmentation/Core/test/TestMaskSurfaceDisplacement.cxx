


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadVolume.h"
#include "ccipdSaveImage.h"
#include "ccipdResizeImage.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdLevelsetToSurface.h"
#include "ccipdMaskImage.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <cstdlib>
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] )
{

  if ( argc == 1 )
    {

    // display help message
    cout << endl << endl <<
      "Usage: " << argv[ 0 ] << " fixedMask movedMask [outputmask = output.mha]" <<
      endl << endl << "This will calculate the Dice " <<
      "value between 2 masks.  There must be " <<
      "exactly 2 parameters, where each parameter " <<
      "is a 3D mask file." << endl << endl;

    return EXIT_SUCCESS;
    }
  else if ( argc < 3 )
    {
    cerr << "You must supply at least 2 mask files." << endl << endl;
    return EXIT_FAILURE;
    }
  else if ( argc > 4 )
    {
    cerr << "Warning: Arguments 5 through " << argc << " ignored." << endl;
    }
    
  // input our mask files
  const string
    fixedMaskFile( argv[ 1 ] ),
    movedMaskFile( argv[ 2 ] ),
    outputFile( argc > 3 ? argv[ 3 ] : "output.mha" );

  // load our masks
  ccipd::VolumeMaskImageConstPointer
    fixedMask( ccipd::LoadVolumeMask( fixedMaskFile ) ),
    movedMask( ccipd::LoadVolumeMask( movedMaskFile ) );

  if ( !fixedMask || !movedMask )
    {
    cerr << "Unable to load either fixed or moved masks." << endl;
    return EXIT_FAILURE;
    }

  // now we want to make the fixed mask isotropic
  // by converting to a levelset
  ccipd::VolumeImageConstPointer
    fixedLevelset( ccipd::MaskToLevelset< ccipd::InternalPixelType >( fixedMask.GetPointer(), false, false, 4.0, 0.0, false ) ),
    movedLevelset( ccipd::MaskToLevelset< ccipd::InternalPixelType >( movedMask.GetPointer(), false, false, 4.0, 0.0, false ) );

  // resize isotropically
  fixedLevelset = ccipd::ResizeImageIsotropically( fixedLevelset.GetPointer() );
  
  if ( !fixedLevelset )
    {
    cerr << "Error: No fixedLevelset." << endl;
    return EXIT_FAILURE;
    }

  // resize the moved to the fixed
  const bool extrapolate = true;
  movedLevelset = ccipd::ResizeImage( movedLevelset, fixedLevelset, extrapolate );
  
  if ( !movedLevelset )
    {
    cerr << "Error: No movedLevelset." << endl;
    return EXIT_FAILURE;
    }

  // now we need to create a surface out of the fixed levelset
  // such that pixels on the surface get a value of 1
  // while pixels away from the surface get a value of 0
  const ccipd::VolumeMaskImageConstPointer
    fixedSurfaceMask ( ccipd::GetSurfaceImage<
      ccipd::InternalPixelType, ccipd::MaskPixelType >( fixedLevelset.GetPointer() ) );

  if ( !fixedSurfaceMask )
    {
    cerr << "Error: No fixed surface mask." << endl;
    return EXIT_FAILURE;
    }

  // threshold the moved levelset so that the surface pixels
  // have a value representing their distance, and
  // non-surface pixels have a value of 0
  const ccipd::VolumeImageConstPointer
    maskedMovedLevelset ( ccipd::MaskImage<
      ccipd::InternalPixelType, ccipd::MaskPixelType >(
        movedLevelset.GetPointer(),
        fixedSurfaceMask.GetPointer() ) );
  
  if ( !maskedMovedLevelset )
    {
    cerr << "Error: No maskedMovedLevelset." << endl;
    return EXIT_FAILURE;
    }

  // save the output
  const bool saved = ccipd::SaveImage(
    maskedMovedLevelset.GetPointer(), outputFile, true );

  if ( !saved )
    {
    cerr << "Error: Unable to save " << outputFile << endl;
    return EXIT_FAILURE;
    } // saved

  cout << "All done!" << endl;
  return EXIT_SUCCESS;

} // main
//////////////////////////////////////////////////////////////////////////////////////////
