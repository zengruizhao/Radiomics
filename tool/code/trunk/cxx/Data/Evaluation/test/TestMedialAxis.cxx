

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"          // for mask types
#include "ccipdLandmarkTypes.h"        // for mask types
#include "ccipdLoadMRI.h"              // for loading masks
#include "ccipdLevelsets.h"            // for dice calculation
#include "ccipdCalculateSimpleMedialAxesDistance.h"        // for dice calculation

// std includes
#include <cstdlib>
#include <cstdio> // for sscanf
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkRegionOfInterestImageFilter.h> // for cropping
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace ccipd;
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] )
{
  if ( argc == 1 )
    {

    // display help message
    cout << endl << endl <<
      "Usage: " << argv[ 0 ] << " levelset1  levelset2 " <<
      endl << endl << "This will calculate the Medial Axis of a levelset" << endl << endl;
    return EXIT_SUCCESS;
    }
  else if ( argc < 2 )
    {
    cerr << "You must supply two levelset." << endl << endl;
    return EXIT_FAILURE;
    }
  else if ( argc > 2 )
    {
    cerr << "Warning: Arguments 3 through " << argc << " ignored." << endl;
    }
        
  // input our level set
  const string 
    fileName1( argv[ 1 ] ),
    fileName2( argv[ 2 ] );

  // load our levelset
  const auto 
    levelset1 = LoadMRI( fileName1 ),
    levelset2 = LoadMRI( fileName2 );

  if ( !levelset1 ||  !levelset2 )
    {
    return EXIT_FAILURE;
    }

  // display some progress
  cout << "Calculating Difference beween the Medial Axis of " << fileName1 << " and " << fileName2 << endl;
  VolumeImagesContainer levelsets;
  levelsets.push_back( levelset1 );
  levelsets.push_back( levelset2 );
  //auto medialAxis = 
  cout<<  CalculateSimpleMedialAxesDistance( levelsets ) << endl;;

  return EXIT_SUCCESS;

} // main
//////////////////////////////////////////////////////////////////////////////////////////
