


//////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadVolume.h"

// std includes
#include <string>

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"

// boost includes
#include <boost/lexical_cast.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char * argv[] )
{

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::string;
  using boost::lexical_cast;

  if ( argc < 4 )
    {
    cout << "Usage: " << argv[ 0 ] <<
      "image index_x index_y index_z" << endl;
    return EXIT_SUCCESS;
    } // argc

  const string fileName = argv[ 1 ];

  const ccipd::VolumeImageConstPointer volume(
    ccipd::LoadVolume( fileName ) );

  if ( !volume )
    {
    cerr << "Error: Unable to load " << fileName << "!" << endl;
    return EXIT_FAILURE;
    } // volume

  
  // try to load parameters from the command line
  ccipd::VolumeImageBase::IndexType index;
  typedef ccipd::VolumeImageBase::IndexValueType IndexValueType;

  try
    {
    index[ 0 ] = lexical_cast< IndexValueType >( argv[ 2 ] );
    }
  catch( ... )
    {
    cerr << "Error: Unable to read in the index." << endl;
    return EXIT_FAILURE;
    }
    
  try
    {
    index[ 1 ] = lexical_cast< IndexValueType >( argv[ 3 ] );
    }
  catch( ... )
    {
    cerr << "Error: Unable to read in the index." << endl;
    return EXIT_FAILURE;
    }
    
  try
    {
    index[ 2 ] = lexical_cast< IndexValueType >( argv[ 4 ] );
    }
  catch( ... )
    {
    cerr << "Error: Unable to read in the index." << endl;
    return EXIT_FAILURE;
    }
    
  // get the point
  ccipd::VolumeImageType::PointType point;

  volume->TransformIndexToPhysicalPoint( index, point );

  // output
  cout << "Index = " << index << endl << "Point = " << point << endl;

  return EXIT_SUCCESS;

} // main
//////////////////////////////////////////////////////////////////////////////////////////////////
    