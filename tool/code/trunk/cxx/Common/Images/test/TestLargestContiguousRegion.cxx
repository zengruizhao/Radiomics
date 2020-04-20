


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdGetLargestContiguousRegion.h"
#include "ccipdLoadVolume.h"
#include "ccipdSaveImage.h"

// std includes
#include <string>

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using ccipd::GetLargestContiguousRegion;
using ccipd::GetLargestContiguousRegions;
using ccipd::LoadVolumeMask;
using ccipd::SaveImage;
using ccipd::VolumeMaskImageConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char * argv[])
{

  if ( argc < 2 )
    {
    cout << "Usage: " << argv[ 0 ] << " maskFile" << endl;
    return EXIT_SUCCESS;
    }

  // try to load a mask
  const string inputMaskFile( argv[ 1 ] );
  cout << "Loading " << inputMaskFile << ":" << endl;
  const VolumeMaskImageConstPointer inputMask(
    LoadVolumeMask( inputMaskFile ) );
  if ( inputMask.IsNull() )
    {
    cerr << "No mask loaded." << endl;
    return EXIT_FAILURE;
    } 
  cout << "Loading " << inputMaskFile << " done." << endl;

  cout << "Getting 1 largest region:" << endl;
  const VolumeMaskImageConstPointer
    largestRegion1( GetLargestContiguousRegion( inputMask.GetPointer() ) );
  if ( largestRegion1.IsNull() )
    {
    cerr << "Largest region null!" << endl;
    return EXIT_FAILURE;
    }
  
  cout << "Getting 2 largest regions:" << endl;
  const VolumeMaskImageConstPointer
    largestRegion2( GetLargestContiguousRegions( inputMask.GetPointer() ) );
  if ( largestRegion2.IsNull() )
    {
    cerr << "Largest 2 regions null!" << endl;
    return EXIT_FAILURE;
    }

  const bool
    saved1 = SaveImage(
      largestRegion1.GetPointer(), "largestRegion1.mha", true ),
    saved2 = SaveImage(
      largestRegion2.GetPointer(), "largestRegion2.mha", true );

  if ( !saved1 || !saved2 )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;

} // main
