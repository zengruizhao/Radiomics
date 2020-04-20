


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFACrossValidationTraining.h"
#include "ccipdValidationTypes.h"               // for defining our validation types
#include "ccipdValidateSubDirectories.h"        // for getting the next subdirectory
#include "ccipdMFAModel.h"                      // what we are training
#include "ccipdPatients.h"                      // what we are training with
#include "ccipdSIMStorage.h"                    // to update the directory before saving

// std includes
#include <iostream> // for cout, etc.

// boost includes
#include <boost/filesystem.hpp>
#include <boost/assign/list_of.hpp> // for our collections
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// "using" shortcuts
using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_regular_file;
using boost::filesystem::remove;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
bool RunMFACrossValidationTraining(
  const string & crossValidationRootDirectory,
  const bool     verbose )
{

  try
    {

    // make sure that the model template
    // is in the root directory
    const string modelFileName = "MFAModel.xml";
    const path modelTemplatePath(
      path( crossValidationRootDirectory) / modelFileName );
    const bool modelTemplateExists = exists( modelTemplatePath )
      && is_regular_file( modelTemplatePath );
    if ( !modelTemplateExists )
      {
      if ( verbose )
        cerr << "Error: " << modelTemplatePath << " does not exist!" << endl <<
          "You need to create a template model with the MFA parameters " <<
          "to train the MFA." << endl;
      return false;
      } // modelTemplatePathExists

	
    // initialize the directoryNavigator
    const ValidateSubDirectories::Pointer
      directoryNavigatorPointer = ValidateSubDirectories::New();
    ValidateSubDirectories & directoryNavigator = *directoryNavigatorPointer;
	
	  directoryNavigator.SetRootDirectory ( crossValidationRootDirectory );
	  directoryNavigator.SetVerbose       ( verbose );
	  directoryNavigator.SetDoneFileName  ( "trained" );

    // what file names will we be using to define
    // the state of each subdirectory?
    const string
      trainingPatientsShortFileName = "TrainingPatients.xml",
	  lockedShortFileName           = directoryNavigator.GetLockedFileName(),
	  trainedShortFileName          = directoryNavigator.GetDoneFileName();

    // okay, now get the next subdirectory which is ready
    // for training, and not locked by any other process,
    // and not already trained
    using boost::assign::list_of;
    const FileNameCollection
      requiredFiles  = list_of( trainingPatientsShortFileName ),
      forbiddenFiles = list_of( lockedShortFileName )( trainedShortFileName );

	directoryNavigator.SetForbiddenFiles( forbiddenFiles );
	directoryNavigator.SetRequiredFiles ( requiredFiles );

    // get next directory and lock it by creating a "locked" file
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
        cerr << "Error: no subdirectories exist for training!" << endl;
      return false;
      }

    // extract the relevant paths
    const path
      subDirectoryPath( subDirectoryName ),
      lockedPath( subDirectoryPath / lockedShortFileName );

    // load the patients
    const string trainingPatientsFullFileName =
      ( subDirectoryPath / trainingPatientsShortFileName ).string();
    if ( verbose )
      cout << "Loading patients from " << trainingPatientsFullFileName << ":" << endl;

    const bool returnNULLOnError = true;
    const PatientsConstPointer patients =
      Patients::ConstNew( trainingPatientsFullFileName, returnNULLOnError );

    // make sure some patients were loaded
    if ( !patients || patients->GetNumberOfStudies() == 0 )
      {

      cerr << "Error: unable to load any patients from " <<
        trainingPatientsFullFileName << endl;

      remove( lockedPath );
      return false;
      } // has patients
    if ( verbose )
      cout << "Loading patients from " << trainingPatientsFullFileName << " done." << endl;

    // load some MFA parameters
    if ( verbose )
      cout << "Loading model parameters from " << modelTemplatePath << ":" << endl;

    const MFAModelPointer model = MFAModel::New();
    const bool loadedModelFile = model->Load( modelTemplatePath.string() );

    if ( !loadedModelFile )
      {
      if ( verbose )
        cerr << "Warning: Unable to load model parameters " <<
          "from " << modelTemplatePath << endl;

      remove( lockedPath );
      return false;

      } // loadedModelFile

    if ( verbose )
      cout << "Loading model parameters from " << modelTemplatePath << " done." << endl;

    // okay, train!
    if ( verbose ) cout << "Training MFA:" << endl;
    const bool trained = model->Train( *patients );

    if ( !trained )
      {
      if ( verbose )
        cout << "Error: unable to train." << endl;

      remove( lockedPath );
      return false;
      } // trained

    if ( verbose ) cout << "Training MFA done." << endl;

    // we need to update the file name to the desired subdirectory
    model->GetTrainedSIM()->SetOutputFolder( subDirectoryName );

    // now we can save
    const string modelFullFileName = ( subDirectoryPath / modelFileName ).string();
    if ( verbose ) cout << "Saving " << modelFullFileName << ":" << endl;
    const bool savedModel = model->Save( modelFullFileName );

    if ( !savedModel )
      {
      if ( verbose )
        cerr << "Error: unable to save " << modelFullFileName << endl;

      remove( lockedPath );
      return false;

      } // savedModel

    if ( verbose ) cout << "Saving " << modelFullFileName << " done." << endl;

    // now save a file indicating that we are done training
    const bool savedTrainedFile( directoryNavigator.CreateDoneFile() );

    if ( !savedTrainedFile )
      {

      if ( verbose )
        cerr << "Error: unable to create " << trainedShortFileName << endl;

      remove( lockedPath );
      return false;

      } // savedTrainedFile

    // alright, we've trained!
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

} // RunMFACrossValidationTraining
//////////////////////////////////////////////////////////////////////////////////////////




} // namespace ccipd
