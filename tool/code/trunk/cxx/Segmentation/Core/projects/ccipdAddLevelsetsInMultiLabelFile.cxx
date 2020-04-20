//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdSaveImage.h"      // for saving results
#include "ccipdLoadVolume.h"     // for loading images
#include "ccipdVolumeTypes.h"    // for image types
#include "ccipdLevelsets.h"

#include "ccipdDisableWarningsMacro.h"
#include <itkAddImageFilter.h>
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <iostream>             // for cout
#include <string>               // for filenames
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string; 


using ccipd::VolumeMaskImageType;
using ccipd::VolumeMaskImageConstPointer;
using ccipd::VolumeImageType;
using ccipd::VolumeMaskImageType;
using ccipd::VolumeImagePointer;
using ccipd::VolumeImageConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



#define epsilon 1e-6; 

int main( int argc, char * argv[] )
{

  if ( argc < 3 )
  {
    cout << endl << "Usage:" << 
      argv[ 0 ] << " levelsetFile1 levelSetFile2 outFile " <<
      endl << endl;
    return EXIT_SUCCESS;
  }

  // extract the inputs
  const string
    imageFileName1( argv[ 1 ] ),
    imageFileName2( argv[ 2 ] ),
    outputFileName( argv[ 3 ] );
 
  // load the image
  cout << "Loading File 1: " << imageFileName1 << ":" << endl;
  const VolumeImageConstPointer vol1  = 
            static_cast<const VolumeImageType *>(ccipd::LoadVolume( imageFileName1 ));
  cout << "Loading File 2: " << imageFileName2 << ":" << endl;
  const VolumeImageConstPointer vol2  = 
            static_cast<const VolumeImageType *>(ccipd::LoadVolume( imageFileName2 ));

  const VolumeMaskImageConstPointer vol1_mask  = 
    static_cast<const VolumeMaskImageType *>(ccipd::CalculateMaskFromLevelset( vol1 )); 
  const VolumeMaskImageConstPointer vol2_mask  = 
    static_cast<const VolumeMaskImageType *>(ccipd::CalculateMaskFromLevelset( vol2 )); 

  typedef itk::AddImageFilter<VolumeMaskImageType, VolumeMaskImageType, VolumeMaskImageType> 
    AddImageFilterType;

  AddImageFilterType::Pointer adder = AddImageFilterType::New();
  adder->SetInput1(vol1_mask);
  adder->SetInput2(vol2_mask);
  adder->Update();
  
  VolumeMaskImageConstPointer addedVol = adder->GetOutput();

  // save the result
  cout << "Saving extraction result:" << endl;
  const bool savedSuccessfully =
    ccipd::SaveImage( addedVol.GetPointer(), outputFileName, true );
  if ( !savedSuccessfully )
    {
    std::cerr << "Saving extraction result failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Saving extraction result done." << endl;

  return EXIT_SUCCESS;

} // main
