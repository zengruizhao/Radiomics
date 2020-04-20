


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFACrossValidationFileNames.h"


#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// std includes
#include <string>

// boost includes
#include <boost/filesystem.hpp>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// "using" shortcuts
using std::string;
using boost::filesystem::exists;
using boost::filesystem::is_regular_file;
using boost::filesystem::path;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
string GetMFASegmentationParametersFileName()
{
  return "MFASegmenter.xml";
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
string GetTrainedMFAModelFileName()
{
  return "MFAModel.xml";
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
string GetTrainedFileName()
{
  return "trained";
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
string GetSegmentedFileName()
{
  return "segmented";
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
string GetLockedFileName()
{
  return "locked";
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool CreateEmptyFile(
  const string & directory,
  const string & shortFileName )
{

  try
    {

    // concatenate the file name
    const string fullFileName =
      ( path( directory ) / shortFileName ).string();

    // call the correct function with the
    // full file name
    return CreateEmptyFile( fullFileName );

    }
  catch( ... )
    {

    // return false on any errors
    return false;

    } // catch

} // CraeteEmptyFile
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool CreateEmptyFile( const string & fullFileName )
{

  try
    {

    // just open and close the file
    fclose( fopen( fullFileName.c_str(), "w" ) );

    // make sure the file exists
    const path fullFilePath( fullFileName );
    const bool fileExists =
      exists( fullFilePath ) &&
      is_regular_file( fullFilePath );

    // if it doesn't, return false
    return fileExists;

    } // try
  catch( ... )
    {

    // return false on any errors
    return false;

    } // catch

} // CreateEmptyFile
//////////////////////////////////////////////////////////////////////////////////////////



}