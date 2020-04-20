

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdLoadVolume.h"               // for loading
#include "ccipdIntensityStandardization.h" // for standardizing
#include "ccipdSaveImage.h"                // for saving

// std includes
#include <iostream> // for cout
#include <string>

// boost includes
#include <boost/lexical_cast.hpp>
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using ccipd::LoadVolume;
using ccipd::VolumeImageConstPointer;
using ccipd::VolumeImagePointer;
using ccipd::StandardizeImage;
using ccipd::SaveImage;
using boost::lexical_cast;
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{

  // set up default parameters
  unsigned int
    histogramBins        = 100,
    histogramMatchPoints = 15;

  // display usage
  if ( argc < 3 )
    {
    cout << "Usage: " << argv[ 0 ] <<
       " templateImage unstandardizedImage " <<
        "	[outputImage = standardized.mha] " <<
        " [histogramBins = " << histogramBins << "] " <<
        " [histogramMatchPoints = " << 
        histogramMatchPoints << "] " << endl;
    return EXIT_SUCCESS;
    } // argc
    
  // try to load parameters from the command line
  if ( argc > 4 )
    try
      {
      histogramBins = lexical_cast< unsigned int >( argv[ 4 ] );
      }
    catch( ... ) {}
    
  if ( argc > 5 )
    try
      {
      histogramMatchPoints = lexical_cast< unsigned int >( argv[ 5 ] );
      }
    catch( ... ) {}

  const string
    templateImageFile       = argv[ 1 ],
    unstandardizedImageFile = argv[ 2 ],
    standardizedImageFile   = argc > 3 ? argv[ 3 ] : "standardized.mha";

  // output parameters
  cout << "Template image file       = " << templateImageFile       << endl;
  cout << "Unstandardized image file = " << unstandardizedImageFile << endl;
  cout << "Standardized image file   = " << standardizedImageFile   << endl;
  cout << "Histogram bins            = " << histogramBins           << endl;
  cout << "Histogram match points    = " << histogramMatchPoints    << endl;
  
  // load the images
  cout << "Loading volumes:" << endl;
  const VolumeImagePointer
    templateImage       = LoadVolume( templateImageFile       ).GetPointer(),
    unstandardizedImage = LoadVolume( unstandardizedImageFile ).GetPointer();
    
  // was it successful?
  if ( !templateImage  )
    {
    cerr << "Error: unable to load " << templateImageFile << "!" << endl;
    return EXIT_FAILURE;
    }
  if ( !unstandardizedImage  )
    {
    cerr << "Error: unable to load " << unstandardizedImageFile << "!" << endl;
    return EXIT_FAILURE;
    }

    
  cout << "Loading volumes done." << endl;
  
  // standardize
  cout << "Standardizing intensities:" << endl;
  const VolumeImageConstPointer standardizedImage(  
                 StandardizeImage(unstandardizedImage, templateImage, histogramBins, histogramMatchPoints) 
				                                 );
  cout << "Standardizing intensities done." << endl;
  
  // try to save the output
  cout << "Saving standardized image:" << endl;
  const bool saved = SaveImage(
    standardizedImage.GetPointer(),
    standardizedImageFile,
    true /* verbose */ );
    
  if ( !saved )
    {
    cerr << "Error saving standardized image!" << endl;
    return EXIT_FAILURE;
    }
    
  cout << "Saving standardized image done." << endl;
    
  return EXIT_SUCCESS;
  
} // main
