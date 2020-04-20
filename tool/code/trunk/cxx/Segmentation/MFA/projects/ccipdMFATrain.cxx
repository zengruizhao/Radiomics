


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMFAModel.h"
#include "ccipdPatientTypes.h"
#include "ccipdPatients.h"
#include "ccipdFeatureID.h"

// std includes
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk
  #include <itkMultiThreader.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// "using" shortcuts
using std::cerr;
using std::cout;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \defgroup MFATrain MFATrain
 * \brief Program to train an MFA. 
 * \ingroup  MFA
 * 
 * This program is used to train an MFA.
 * There is one required input and one
 * optional input.
 * 
 * The required input is an xml file
 * containing the patients you wish to
 * use to train.
 * 
 * An example file can be found at http://bit.ly/L1l8M7
 * 
 * The second input is an xml file containing
 * the parameters used to train the MFA.
 * The default name (if none is given) is
 * MFAModel.xml in the current directory.
 * 
 * If no second file is given, or the program
 * is unable to read the file, then
 * one will be created with default parameters.
 * 
 * You can run it like <code>./MFATrain patients.xml model.xml</code>
 * 
 * The parameters in the xml file are as follows:
 *   - <b>TrainingPadding:</b>
 *       When one has a bunch of masks,
 *       they will first be aligned, and then
 *       they will be "OR"ed to create a union
 *       of masks. A bounding box will be created
 *       around that union, and expanded by some
 *       size (in mm). Each feature will then
 *       be cropped to this expanded bounding
 *       box. TrainingPadding specifies the size
 *       of this expansion.
 *   - <b>MaxComponents:</b>
 *       The maximum number of eigenvectors to keep.
 *   - <b>Verbose:</b>
 *       To display progress messages during training.
 *   - <b>NormalizeLevelset:</b>
 *       Normally, a levelset is represented by a signed
 *       distance function, in which each pixel outside
 *       the object of interest has a value of +mm, and each
 *       pixel inside the object of interest has a value
 *       of -mm. If this is set to true, then each pixel
 *       won't be represented by mm, but rather the entire
 *       levelset image will be normalized to have unit variance.
 *   - <b>AlignmentObject:</b>
 *       Which object's masks should be aligned? For example,
 *       if you have a bunch of prostate masks and a bunch
 *       of bladder masks, which set of masks should define
 *       the alignment to combine all the training studies?
 *   - <b>AlignmentPatient:</b>
 *       Of all the patients in your input file,
 *       which should serve as the "fixed" or "template" patients
 *       to which all others are aligned?
 *   - <b>TextureFeatures:</b>
 *       This is a collection of texture features to extract.
 *       To add a new texture feature, change the count
 *       tag, and add another "<item></item>" tag.
 *       The texture features are defined in #ccipd::TextureFeatureEnum
 *   - <b>LevelsetObjects:</b>
 *       This is another collection, defining which
 *       levelset/mask objects to train. It's possible your patient
 *       xml file has masks for objects that you don't want
 *       to train the MFA for.
 *   
 * \note
 *   The MFA tag in the XML file contains the actual
 *   trained MFA and should not be modified.
 * 
 * \author Rob Toth
 */
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup MFATrain
int main( int argc, char * argv[] )
{

  if ( argc == 1 )
    {
    cout << endl << "Usage: " << endl <<
      argv[ 0 ] << " Patients.xml [ModelOptions.xml] " <<
      "where Patients contains an xml file of ccipd::Patients " <<
      "as training data, " <<
      "and ModelOptions.xml is an optional XML file containing " <<
      "MFAModel parameters saved using ccipd::MFAModel::Save." << endl;

    return EXIT_SUCCESS;
    }
    
  if ( itk::MultiThreader::GetGlobalDefaultNumberOfThreads() == 1 )
    {
    itk::MultiThreader::SetGlobalDefaultNumberOfThreads( 32 );
    }
    
  cout << "Number of theads = " <<
    itk::MultiThreader::GetGlobalDefaultNumberOfThreads() << endl;

  // load some training data
  const string patientsFile( argv[ 1 ] );
  cout << "Loading patients from " << patientsFile << ":" << endl;
  
  const bool returnNULLOnError = true;
  const ccipd::PatientsPointer patients =
    ccipd::Patients::New( patientsFile, returnNULLOnError );

  if ( !patients || patients->GetNumberOfStudies() == 0 )
    {

    cerr << "Error: unable to load any patients from " <<
      patientsFile << endl;

    return EXIT_FAILURE;
    }
  else
    cout << "Loading patients from " << patientsFile << " done." << endl;

  // what file will we be using to load/save parameters?
  const string modelFile = argc > 2 ? argv[ 2 ] : "MFAModel.xml";

  // load some parameters
  ccipd::MFAModelPointer model = ccipd::MFAModel::New();
  cout << "Loading model parameters from " << modelFile << ":" << endl;

  bool loadedModelFile = model->Load( modelFile );

  model.reset();
  model = ccipd::MFAModel::New();
  loadedModelFile = model->Load( modelFile );

  if ( !loadedModelFile )
    {
    cerr << "Warning: Unable to load model parameters " <<
      "from " << modelFile << ". However, we will continue " <<
      "with the default parameters." << endl;
    }
  else
    cout << "Loading model parameters from " << modelFile << " done." << endl;

  // save an xml file now so the user can see all the settings
  cout << "Saving initial MFA parameters to " << modelFile << ":" << endl;
  bool saved = model->Save( modelFile );

  if ( saved )
    cout << "Saving initial MFA parameters to " << modelFile <<
      " done. " << endl << "Please manually edit this file and rerun " <<
      argv[ 0 ] << " again to use the new parameters." << endl;
  else
    cerr << "Warning: Saving initial MFA parameters to "
      << modelFile << " failed. Continuing anyway." << endl;

  // train
  cout << "Training MFA:" << endl;

  const bool trained = model->Train( *patients );
  cout << endl;
  if ( trained )
    {
    cout << "Training MFA done." << endl;
    }
  else
    {
    cerr << "Training MFA failed." << endl;
    return EXIT_FAILURE;
    }

  // save the trained MFA
  cout << "Saving trained MFA to " << modelFile << ":" << endl;

  saved = model->Save( modelFile );

  if ( saved )
    {
    cout << "Saving trained MFA to " << modelFile << " done." << endl;
    }
  else
    {
    cout << "Saving trained MFA to " << modelFile << " failed." << endl;
    return EXIT_FAILURE;
    }

  cout << endl << "MFA training completed!" << endl << endl;

  return EXIT_SUCCESS;

} // MFATrain
//////////////////////////////////////////////////////////////////////////////////////////
