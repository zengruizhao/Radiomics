




//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"              // disable warnings
#include "ccipdMFASegmentAndSave.h" // for actually running it
#include "ccipdMFASegmenter.h"      // for segmenting a new study
#include "ccipdPatientTypes.h"      // for our study types
#include "ccipdStudy.h"             // for our study

#ifdef _MSC_VER
#pragma warning( push, 0 )
#pragma warning( disable: 4701 )
#pragma warning( disable: 4702 )
#endif // _MSC_VER

// itk
#include <itkMultiThreader.h>

// std includes
#include <string>
#include <vector>
#include <cfloat>

#ifdef _MSC_VER
#pragma warning( default: 4701 )
#pragma warning( default: 4702 )
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using ccipd::MFASegmenter;
using ccipd::StudyPointer;
using ccipd::MFASegmentAndSave;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \defgroup MFASegment MFASegment
 * \brief Program to segment an image using a trained MFA. 
 * \ingroup  MFA
 * 
 * There is one required input and one
 * optional input.
 * 
 * The required input is an xml file
 * containing the study you wish to
 * segment.
 * 
 * An example is as follows:
 *    <code>
 *    \verbatim
      <description>My Study Name</description>
      <modality>MRI (T2-FSE)</modality>
      <date>2010-01-01</date>
      <volume>/my/dicom/directory/</volume>
      <mask>/my/dicom/directory/</volume>
      <mask>
         <object>Prostate</object>
         <file>/my/prostate/mask.nrrd</file>
       </mask>
      <mask>
         <object>Bladder</object>
         <file>/my/bladder/mask.nrrd</file>
      </mask>\endverbatim
 *    </code>
 * 
 * The second input is an xml file containing
 * the parameters used for segmentation.
 * The default name (if none is given) is
 * MFASegmenter.xml in the current directory.
 * 
 * If no second file is given, or the program
 * is unable to read the file, then
 * one will be created with default parameters.
 * 
 * You can run it like <code>./MFASegment study.xml options.xml</code>
 * 
 * The parameters in the xml file are as follows:
 *   - <b>DisplayProgress:</b>
 *       A float between 0 and 1 indicating
 *       firstly whether or not to even show
 *       verbose output (>0), and second
 *       how frequently to display the registration
 *       iterations as it moves the MFA over the
 *       new image. It is recommended to keep this at "1" because
 *       the MFA usually converges in about 20 iterations.
 *       The output will show the following output:
 *       - <i>Iteration:</i> Which iteration.
 *       - <i>Parameters:</i>
 *           This will contain 12 parameters representing
 *           the current affine transformation.
 *           Given a 3x4 affine transformation matrix,
 *           this set of parameters is essentially
 *           reading down each column. Therefore, the last
 *           3 parameters represent translation.
 *       - <i>Metric:</i>
 *          Either the NCC or SSD value averaged
 *          over all features.
 *   - <b>Initial??_Or_nan:</b>
 *       The initial translation/scalerotation
 *       in x/y/z to use when starting
 *       the segmentation process. Sometimes the 
 *       MFA starts too far from the object of interest
 *       and can't "latch on" to the correct area of the
 *       image, and in those cases some manual intervention
 *       is required. Alternatively, you can input "nan"
 *       (no quotes) in which case it will pick a default
 *       reasonable value. The other Initial parameters are similar.
 *   - <b>Range??:</b>
 *       The optimizer needs to know how important translation
 *       is relative to scale, and rotation. So you must
 *       play around with these parameters to make sure
 *       that the optimizer is moving well throughout the
 *       search space. To disable rotation over the x
 *       and y axes, for example, set RangeRy to 0.
 *   - <b>UseNCC:</b>
 *       If true (or "1"), NCC between the feature
 *       and its reconstruction is averaged
 *       over all features and maximized. Alternatively,
 *       the SSD may be minimized. I have found NCC
 *       to be more robust.
 *   - <b>TrainedMFAFile:</b>
 *       Specify the file path and nane of the
 *       MFA model you wish to use. See #MFATrain
 *       for a description of the format of that xml file.
 *   - <b>SampledPixels:</b>
 *       This is a collection of what percentage
 *       of pixels to sample at each resolution.
 *       To have a multi-resolution search, in which
 *       2 resolutions are used, set the "count" tag
 *       to 2, and make two "item" tags. Each item
 *       is a float ranging from 0 to 1, representing
 *       what percent of the pixels in the cropped image
 *       to sample and reconstruct.
 *       The way the multi-resolution collections work,
 *       is that if you have only two "item"s in the
 *       SampledPixels tag, but another multi-resolution
 *       tag has four items, then the second item
 *       in SampledPixels is also used for the
 *       third and fourth resolution.
 *   - <b>SegmentationPadding:</b>
 *       At each resolution, a bounding box is
 *       created around the mean levelset "0" values
 *       and expanded by this many mm to create a new field of view.
 *       What that means is that if you trained your
 *       MFA, you end up with the average (or mean)
 *       levelset for a given object after the objects
 *       were aligned. This can be thresholded at
 *       0 to yield a "mean segmentation". The MFA
 *       segmenter will create a bounding box encompassing
 *       this mean segmentation and expand it a bit.
 *       Then, any pixels which fall outside this bounding
 *       box in the MFA are discounted.
 *       If you are using some levelsets to drive
 *       the MFA (see below), then the union of the
 *       mean segmentations
 *       of those levelsets is used to create the bounding
 *       box. If you are not using any levelsets
 *       to drive the MFA, then the union of the mean
 *       segmentations of <b>all</b> trained levelsets
 *       is used to create the bounding box for
 *       the field of view.
 *   - <b>MaxIterations:</b>
 *       This represents the maximum number of
 *       iterations to perform at each resolution.
 *       It's pretty deprecated, as the optimizer
 *       usually converges in around 20 iterations
 *       or so.
 *   - <b>DrivingLevelsetObjects</b>
 *       In your trained MFA, you may have
 *       trained the intensities (as your texture),
 *       the levelsets of the prostate, and the
 *       levelsets of the bladder. Normally, you can
 *       segment out both the prostate and the
 *       bladder using just the intensities.
 *       This parameter, however, lets you use an
 *       existing segmentation to drive the other.
 *       For example, in a new image, you may
 *       already have the bladder segmentation and
 *       want to use it to help drive the segmentation
 *       of the prostate. You simply specify the
 *       object name of any mask you already have
 *       in your study xml file, that you wish
 *       to use to drive the segmentation
 *       of your other objects.
 *   - <b>SegmentationObject:</b>
 *       This parameter lets you specify which
 *       object to segment. If you specify a
 *       specific object to segment, then
 *       only that object will be segmented
 *       and the output saved as "MFA_segmentation_<i>object</i>.mha".
 *       If you specify no object, then much more is output.
 *       Specifically:
 *       - The segmentation of each possible object
 *       - The reconstruction of each feature
 *       - The reconstruction of each feature in the MFA's coordinate frame.
 * 
 * \note
 *   All the outputs are saved in the current directory. Therefore,
 *   if you want to save the outputs to /this/output/folder
 *   but the MFASegment command is in /this/executable/folder
 *   and your xml files are in /this/xml/folder,
 *   then run the following:
 *   <code>\n\n
 *     cd /this/output/folder\n\n
 *     /this/executable/folder/MFASegment
 *     /this/xml/folder/study.xml
 *     /this/xml/folder/options.xml\n
 *   </code>
 * 
 * \author Rob Toth
 */
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup MFASegment
int main( int argc, char *argv[] )
{

  if ( argc == 1 )
    {
    cout << "Usage: " << argv[ 0 ] <<
      " study.xml [MFASegmenter.xml]" << endl << endl <<
      "where study.xml is an xml file containing an ccipd::Study " <<
      "and MFASegmenter.xml is an optional file containing " <<
      "the segmentation options (including the location " <<
      "of the trained MFA). If you don't specify MFASegmenter.xml, " <<
      "then a default one will be created in the current directory." << endl;

    return EXIT_SUCCESS;

    } // argc == 1

  if ( itk::MultiThreader::GetGlobalDefaultNumberOfThreads() == 1 )
    {
    itk::MultiThreader::SetGlobalDefaultNumberOfThreads( 32 );
    }
    
  cout << "Number of theads = " <<
    itk::MultiThreader::GetGlobalDefaultNumberOfThreads() << endl;
    
  const string
    inputStudyFile = argv[ 1 ],
    inputSegmenterFile = argc > 2 ? argv[ 2 ] : "MFASegmenter.xml";

  // load an image to segment
  cout << "Loading patient to segment:" << endl;

  // load a study using the xml file provided
  
  const bool returnNULLOnError = true;
  const StudyPointer newStudy =
    ccipd::Study::New( inputStudyFile, returnNULLOnError );

  if ( !(newStudy) )
    {
    cout << "Loading study to segment failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Loading study to segment done." << endl;
  cout << "Study = " << newStudy->GetDescription() << endl;

  // load some parameters
  const MFASegmenter::Pointer segmenterPointer = MFASegmenter::New();
  MFASegmenter & segmenter = *segmenterPointer;
  cout << "Loading segmenter parameters from " << inputSegmenterFile << ":" << endl;
  
  const bool loadedSegmenterFile = segmenter.Load( inputSegmenterFile );

  if ( !loadedSegmenterFile )
    {
    cerr << "Warning: Unable to load segmenter parameters " <<
      "from " << inputSegmenterFile << ". However, we will continue " <<
      "with the default parameters." << endl;
    }
  else
    cout << "Loading segmenter parameters from " << inputSegmenterFile << " done." << endl;

  // save an xml file now so the user can see all the settings
  cout << "Saving initial MFA parameters to " << inputSegmenterFile << ":" << endl;
  bool saved = segmenter.Save( inputSegmenterFile );

  if ( saved )
    cout << "Saving initial MFA parameters to " << inputSegmenterFile <<
      " done. " << endl << "Please manually edit this file and rerun " <<
      argv[ 0 ] << " again to use the new parameters." << endl;
  else
    cerr << "Warning: Saving initial MFA parameters to "
      << inputSegmenterFile << " failed. Continuing anyway." << endl;

  // if no segmentation object has been specified, then
  // we will segment all objects
  const string segmentationObject = segmenter.GetSegmentationObject();
  const bool noSegmentationObject = segmentationObject.empty();

  if ( noSegmentationObject )
    {
    // Give some information to the user.
    cout << endl << "No specific segmentation object " <<
      "has been specified in " << inputSegmenterFile <<
      ". Therefore, all objects will be reconstructed and " <<
      "segmented. If you want to segment a specific object, then " <<
      "specify one in " << inputSegmenterFile << "." << endl;
    }

  // segment and save the results
  const string
    outputDirectory = ".",
    outputPrefix = "MFA_";
  const bool
    verbose = true;

  cout << "Segmenting and saving:" << endl;
  const bool segmented = MFASegmentAndSave(
    newStudy,
    segmenter,
    outputDirectory,
    outputPrefix,
    verbose );

  if ( segmented )
    {
    cout << "Segmenting and saving done." << endl;
    return EXIT_SUCCESS;
    }
  else
    {
    cerr << "Segmenting and saving failed." << endl;
    return EXIT_FAILURE;
    }
  
} // main
//////////////////////////////////////////////////////////////////////////////////////////
