


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdLandmarkTypes.h"    // for image types
#include "ccipdConcatenateLandmarks.h"

#include "ccipdDisableWarningsMacro.h"
  //ITK includes
  #include <itkImage.h>
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char * argv[] )
{

  if ( argc == 1 )
    {
    std::cout << std::endl << "Usage:" << 
      argv[ 0 ] << " landmarkSetInput1(vtk format) landmarkSetInput2(vtk"
                << "format) landmarkSetOutput  " << std::endl;
    return EXIT_SUCCESS;
    }

  ccipd::LandmarkPointType point;// = LandmarkType::New();
  point[0] = 1;
  point[1] = 1;
  point[2] = 1;

  const auto newList = ccipd::ConcatenateLandmarks(point, point);
 // const auto newList2 = ccipd::ConcatenateLandmarkSets( newList, newList );

  return EXIT_SUCCESS;

} // main
