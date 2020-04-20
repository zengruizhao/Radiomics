


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFACrossValidationSegmenting.h"
#include "ccipdMFACrossValidationFileNames.h"
#include "ccipdValidationTypes.h"               // for defining our validation types
#include "ccipdValidateSubDirectories.h"        // for getting the next subdirectory
#include "ccipdPatients.h"                      // what we are segmenting
#include "ccipdPatient.h"                       // what we are segmenting
#include "ccipdMFASegmenter.h"                  // for performing the segmentation
#include "ccipdMFASegmentAndSave.h"             // for performing the segmentation

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// std includes
#include <iostream> // for cout, etc.

// boost includes
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>        // for loops
#include <boost/assign/list_of.hpp> // for assigning the required/forbidden files

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// "using" shortcuts
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_regular_file;
using boost::filesystem::remove;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
bool RunMFACrossValidationSegmenting(
  const string & crossValidationRootDirectory,
  const bool     verbose )
{

  try
    {

    // the root directory
    const path crossValidationRootPath( crossValidationRootDirectory );

    // make sure that the segmentation parameters
    // is in the root directory
    const string templateParametersFileName = GetMFASegmentationParametersFileName();
    const path templateParametersPath(
      crossValidationRootPath / templateParametersFileName );
    const bool templateParametersExists = exists( templateParametersPath )
      && is_regular_file( templateParametersPath );
    if ( !templateParametersExists )
      {
      if ( verbose )
        cerr << "Error: " << templateParametersPath << " does not exist!" << endl <<
          "You need to create a template MFA parameters file " <<
          "to train the MFA." << endl;
      return false;
      } // templateParametersExists
	
	
    // initialize the directoryNavigator
    const ValidateSubDirectories::Pointer
      directoryNavigatorPointer = ValidateSubDirectories::New();
    ValidateSubDirectories & directoryNavigator = *directoryNavigatorPointer;

    directoryNavigator.SetRootDirectory ( crossValidationRootDirectory );
    directoryNavigator.SetVerbose       ( verbose );
    directoryNavigator.SetDoneFileName  ( "segmented" );

    // what file names will we be using to define
    // the state of each subdirectory?
    const string
		lockedShortFileName          = directoryNavigator.GetLockedFileName(),
      trainedShortFileName         = "trained",
      testingPatientsShortFileName = "TestingPatients.xml",
      segmentedShortFileName       =  directoryNavigator.GetDoneFileName();
    
    // okay, now get the next subdirectory which is ready
    // for training, and not locked by any other process,
    // and not already trained
    using boost::assign::list_of;
    const FileNameCollection
      requiredFiles  = list_of( trainedShortFileName )( testingPatientsShortFileName ),
      forbiddenFiles = list_of( lockedShortFileName  )( segmentedShortFileName       );

	directoryNavigator.SetRequiredFiles ( requiredFiles );
	directoryNavigator.SetForbiddenFiles( forbiddenFiles );
    
    // get next directory and lock it
	const bool createdLockFile( directoryNavigator.CreateLockedFile() );

    if ( !createdLockFile )
      {
      if ( verbose )
        cerr << "Error: unable to create lock file " << 
          lockedShortFileName << endl;
      return false;
      } // createdLockFile

	const string subDirectoryName( directoryNavigator.GetSubDirectory() );

    // do we have a subdirectory?
    if ( subDirectoryName.empty() )
      {
      if ( verbose )
        cerr << "Error: no subdirectories exist for segmenting!" << endl;
      return false;
      }

    // extract the relevant paths
    const path
      subDirectoryPath( subDirectoryName ),
      lockedPath( subDirectoryPath / lockedShortFileName );

    // load the patients
    const string testingPatientsFullFileName =
      ( subDirectoryPath / testingPatientsShortFileName ).string();
    if ( verbose )
      cout << "Loading patients from " << testingPatientsFullFileName << ":" << endl;
    
    const bool returnNULLOnError = true;
    const PatientsConstPointer patients =
      Patients::ConstNew( testingPatientsFullFileName, returnNULLOnError );

    // make sure some patients were loaded
    if ( !patients || patients->GetNumberOfStudies() == 0 )
      {
      if ( verbose )
        cerr << "Error: unable to load any patients from " <<
          testingPatientsFullFileName << endl;

      remove( lockedPath );
      return false;
      } // has patients

    if ( verbose )
      cout << "Loading patients from " << testingPatientsFullFileName << " done." << endl;

    // okay, go through each patient
    BOOST_FOREACH( const PatientConstPointer & patient, patients->GetAllPatients() )
      {

      // extract which patient it is
      const PatientIDType patientID = patient->GetPatientID();
      if ( verbose )
        cout << "Segmenting patient " << patientID << ":" << endl;

      const std::size_t numberOfStudies = patient->GetNumberOfStudies();
      if ( numberOfStudies == 0 )
        {
        if ( verbose )
          cerr << "Skipping " << patientID << " because there are no studies!" << endl;
        } // no studies

      // check if specific parameters for this patient exists
      const string specificParametersShortFileName = patientID + ".xml";
      const path specificParametersPath =
        crossValidationRootPath / specificParametersShortFileName;
      const bool useSpecificParameters =
        exists( specificParametersPath ) &&
        is_regular_file( specificParametersPath );
      const path parametersPath = useSpecificParameters ?
        specificParametersPath : templateParametersPath;

      // create an object to segment this patient
      const MFASegmenter::Pointer segmenterPointer = MFASegmenter::New();
      MFASegmenter & segmenter = *segmenterPointer;

      // load the parameters
      if ( verbose )
        cout << "Loading segmenter parameters from " <<
          parametersPath << ":" << endl;

      const bool loadedParameters = segmenter.Load( parametersPath.string() );

      if ( !loadedParameters )
        {
        if ( verbose )
          cerr << "Error: unable to load any parameters from " <<
            parametersPath << endl;

        remove( lockedPath );
        return false;
        } // loadedParameters
      
      // we will use the model from the current directory
      const string trainedModelShortFileName = GetTrainedMFAModelFileName();
      const path trainedModelPath = subDirectoryPath / path( trainedModelShortFileName );
      segmenter.SetTrainedMFAFile( trainedModelPath.string() );

      if ( verbose )
        cout << "Loading segmenter parameters from " <<
          parametersPath << " done." << endl;

      // segment a study
      const StudyPointer study = patient->GetOneStudy();

      // warn if we have multiple studies
      if ( numberOfStudies > 1 && verbose )
        {
        cout << "Warning: " << patientID << " has " <<
          numberOfStudies << "." << endl <<
          "Picking one study to segment." << endl;
        } // >1 studies

      // okay, segment and save
      if ( verbose ) cout << "Segmenting " << patientID << ":" << endl;
      const bool segmented = MFASegmentAndSave(
        study,
        segmenter,
        subDirectoryPath.string(),
        patientID + "_",
        verbose );

      // was it successful?
      if ( !segmented )
        {

        if ( verbose )
          cerr << "Error: unable to segment " << patientID << endl;

        remove( lockedPath );
        return false;
        } // segmented

      if ( verbose ) cout << "Segmenting " << patientID << " done." << endl;

      } // for each patient

    // say that we're done segmenting in this directory
	const bool createdSegmentedFile( directoryNavigator.CreateDoneFile() );

    if ( !createdSegmentedFile )
      {

      if ( verbose )
        cerr << "Error: Unable to create " <<
          segmentedShortFileName << " file." << endl;

      remove( lockedPath );
      return false;

      } // createdSegmentedFile

    remove( lockedPath );
    return true;

    } // try
  catch( std::exception & error )
    {

    if ( verbose ) cerr << "Error: " << error.what() << endl;

    // on errors return false
    return false;

    } // catch
  catch( ... )
    {
    if ( verbose ) cerr << "Error." << endl;
    return false;
    }

} // RunMFACrossValidationSegmenting
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
