


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFACrossValidationTraining.h"
#include "ccipdMFACrossValidationSegmenting.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#pragma warning( disable: 4701 )
#pragma warning( disable: 4702 )
#endif // _MSC_VER

// itk
#include <itkMultiThreader.h>

// std includes
#include <string>
#include <iostream>

#ifdef _MSC_VER
#pragma warning( default: 4701 )
#pragma warning( default: 4702 )
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



/** \ingroup MFACrossValidation */
int main( int argc, char *argv[] )
{

  // "using" shortcuts
  using std::string;
  using std::cerr;
  using std::cout;
  using std::endl;
  using ccipd::RunMFACrossValidationTraining;
  using ccipd::RunMFACrossValidationSegmenting;

  if ( argc < 2 )
    {
    cout << "Usage: " << argv[ 0 ] <<
      " Cross_Validation_Root_Directory [type]" << endl;
    cout << "where type can be train to only train, " <<
      "or segment to only segment. If you input " <<
      "anything else or omit it, both training " <<
      "and segmentation will occur." << endl;

    return EXIT_SUCCESS;
    }

  if ( itk::MultiThreader::GetGlobalDefaultNumberOfThreads() == 1 )
    {
    itk::MultiThreader::SetGlobalDefaultNumberOfThreads( 32 );
    }
    
  cout << "Number of theads = " <<
    itk::MultiThreader::GetGlobalDefaultNumberOfThreads() << endl;
  
  const bool
    onlyTrain   = argc > 2 && string( argv[ 2 ] ).compare( "train"   ) == 0,
    onlySegment = argc > 2 && string( argv[ 2 ] ).compare( "segment" ) == 0;

  // get the files/directories
  const string crossValidationDirectory = argv[ 1 ];
  
  // initialize
  bool
    successfulTraining   = true,
    successfulSegmenting = true;

  if ( !onlySegment )
    {
    
    // run the cross validation training
    cout << "Cross validation training in " << crossValidationDirectory << ":" << endl;
    successfulTraining = RunMFACrossValidationTraining( crossValidationDirectory, true );

    if ( !successfulTraining )
      {
      cerr << "Error: unsuccessful MFA training." << endl;
      }
    else
      {
      cout << "Cross validation training in " << crossValidationDirectory << " done." << endl;
      }

    } // onlySegment
  else
    {
    cout << "Not training; only segmenting." << endl;
    }

  if ( !onlyTrain )
    {

    // run the cross validation segmentation
    cout << "Cross validation segmenting in " << crossValidationDirectory << ":" << endl;
    successfulSegmenting = RunMFACrossValidationSegmenting( crossValidationDirectory, true );

    if ( !successfulSegmenting )
      {
      cerr << "Error: unsuccessful MFA segmenting." << endl;
      }
    else
      {
      cout << "Cross validation segmenting in " << crossValidationDirectory << " done." << endl;
      }

    } // onlyTrain
  else
    {
    cout << "Not segmenting; only training." << endl;
    }

  if ( !( successfulTraining || successfulSegmenting ) )
    {
    cerr << "Error: no training or segmenting done!" << endl;
    return EXIT_FAILURE;
    }
  else
    {
    return EXIT_SUCCESS;
    }

} // main
