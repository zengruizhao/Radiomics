/* 
*  This program cuts out a region from a volume; everything outside the region becomes 0
*  
*/



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h" 
#include "ccipdLoadVolume.h"
#include "ccipdSaveImage.h"


#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageSliceIteratorWithIndex.h>  // to access voxels
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
typedef itk::ImageSliceIteratorWithIndex< ccipd::VolumeImageType > SliceIteratorType;

int main( int argc, char *argv[] )
{

// make sure an image has been given 
if (argc < 5)
  {
  std::cout << argv[0] << " inputVolume axis(xyz) min max outputVolume" << std::endl;
  return EXIT_FAILURE;
  }
 
  // set variables
  const std::string
    inputFile = argv[1];

  const std::string
    axis = argv[2];
    
  const float 
    min = static_cast< float >( atof(argv[3]) ),
    max = static_cast< float >( atof(argv[4]) ); 

  // set variables
  const std::string
    outputFile = argv[5];

  // load in the image
  ccipd::VolumeImagePointer vol = ccipd::LoadVolume( inputFile );

  std::cout << "Size:" << vol->GetLargestPossibleRegion().GetSize() << std::endl;

  SliceIteratorType it ( vol,  vol->GetLargestPossibleRegion() );
  

  it.SetFirstDirection(0); // x?
  it.SetSecondDirection(1); // y?

  it.GoToBegin();
  while( !it.IsAtEnd() )
  {
    while( !it.IsAtEndOfSlice() )
    {
      while( !it.IsAtEndOfLine() )
      {
        if ((axis.compare("x")==0 && it.GetIndex()[0]>=min && it.GetIndex()[0]<=max)||
            (axis.compare("y")==0 && it.GetIndex()[1]>=min && it.GetIndex()[1]<=max)||
            (axis.compare("z")==0 && it.GetIndex()[2]>=min && it.GetIndex()[2]<=max))
          it.Set( 0 );
        ++it;
      }
      it.NextLine();
    }
    it.NextSlice();
  }


  ccipd::SaveImage( vol.GetPointer(), outputFile ,false );
  std::cout<< "done." << std::endl;

  return EXIT_SUCCESS;
}


