




//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFASegmentAndSave.h"
#include "ccipdStudy.h"                           // the actual study to segment
#include "ccipdMFASegmenter.h"                    // what we use to segment
#include "ccipdSaveImage.h"                       // for saving the output
#include "ccipdFeatureCollection.h"               // for knowing which features were used
#include "ccipdFeatureID.h"                       // for getting info on a feature
#include "ccipdQuantitativeSegmentationResults.h" // for saving quantitative results
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdMFALevelsetExtractor.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#pragma warning( disable: 4701 )
#pragma warning( disable: 4702 )
#endif // _MSC_VER

// itk includes
#include <itkVectorImage.h>

// std includes
#include <iostream>

// boost includes
#include <boost/foreach.hpp>      // for loops
#include <boost/lexical_cast.hpp> // for casting to strings

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
using boost::lexical_cast;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
bool MFASegmentAndSave(
  const StudyPointer & study,
        MFASegmenter & segmenter,
  const string       & outputDirectory,
  const string       & outputPrefix,
  const bool           verbose )
{

  // if no segmentation object has been specified, then
  // we will segment all objects
  const string segmentationObject = segmenter.GetSegmentationObject();
  const bool noSegmentationObject = segmentationObject.empty();

  if ( noSegmentationObject && verbose )
    {
    // Give some information to the user.
    cout << endl << "No specific segmentation object " <<
      "has been specified. " << 
      "Therefore, all objects will be reconstructed and " <<
      "segmented. If you want to segment a specific object, then " <<
      "specify one in the parameters." << endl;
    } // noSegmentationObject

  // This is a bit silly, be we will hard code
  // the fact to use the "Prostate" object
  // for the midgland. If no "Prostate" object
  // exists, the midgland won't be calculated.
  const string midglandObject = "Prostate";
  const VolumeMaskImageConstPointer
    midglandTemplateMask = study->GetMask( midglandObject );

  // segment
  cout << "Segmenting:" << endl;
  const bool segmented = segmenter.Segment( study );

  // segmented might be false if you don't
  // specify a specific segmentation object,
  // but that only matters if you aren't
  // reconstructing everything

  cout << "Segmenting done." << endl;

  const string
    extension = ".mha",
    prefix    = outputDirectory + "/" + outputPrefix;

  // just a temporary variable to say whether or not we've successfully saved
  bool saved = true;

  if ( !noSegmentationObject )
    {

    if ( !segmented )
      {
      // the user had specified (in the xml file)
      // that a specific object should be segmented
      if ( verbose ) 
        cerr << "Error: " << segmentationObject << " not segmented." << endl;
      return false;
      } // segmented

    // save the result
    const string
      segmentationFileName =
      prefix +
      "segmentation_" +
      segmentationObject +
      extension;

    const VolumeMaskImageConstPointer segmentation(
      segmenter.GetSegmentation( segmentationObject, study ) );

    saved = SaveImage(
      segmentation.GetPointer(),
      segmentationFileName,
      true );

    // output whether or not we successfully saved
    return saved;

    } // noSegmentationObject

  // at this point, all segmentations will be saved

  if ( verbose )
    cout << "Saving all segmentation results:" << endl;

  if ( study->HasVolume() )
    {

    // save the original
    const string originalFileName = prefix + "Original" + extension;

    // extract the original
    const FeatureIDConstCollection drivingFeatures =
      segmenter.GetDrivingFeaturePreparer()->GetWhichFeatures();
    const VolumeFeaturesConstPointer originalFeatures(
      study->GetFeatureCollection()->GetFeatures( drivingFeatures ) );
        
    saved = saved && SaveImage( originalFeatures.GetPointer(), originalFileName, true );

    } // has volume

  // save each feature's reconstruction
  const FeatureIDConstCollection featureIDs = segmenter.GetAllFeatures();

  // loop through each feature
  BOOST_FOREACH( const FeatureIDConstPointer & featureID, featureIDs )
    {

    if ( !featureID )
      {
      cerr << "Error: NULL Feature!" << endl;
      continue;
      }
      
    const string levelsetObject =
      MFALevelsetExtractor::ExtractObjectNameFromFeatureID( *featureID );

    // if we have a description, that's the levelset object
    const bool isLevelset = !levelsetObject.empty();

    const string
      hashString =
        lexical_cast< string >( featureID->GetHash() ) +
          string( isLevelset ? string( "_" + levelsetObject ) : "" ),
      reconstructionFileName =
        prefix + "reconstruction_" + hashString;

    if ( verbose )
      cout << "Calculating reconstructions for " << hashString << ":" << endl;

    // get the reconstruction of that feature
    const VolumeImageConstPointer
      reconstructedFeature(
        segmenter.GetReconstruction( featureID ) ),
      reconstructedTransformedFeature(
        segmenter.GetTransformedReconstruction( featureID ) );

    if ( verbose )
      cout << "Calculating reconstructions for " << hashString << " done." << endl;

    // save them
    saved =
      saved
      &&
      SaveImage( reconstructedFeature.GetPointer(), reconstructionFileName + extension, true )
      &&
      SaveImage( reconstructedTransformedFeature.GetPointer(),
        reconstructionFileName + "_transformed" + extension, true );

    // save the mask
    if ( isLevelset )
      {
      const VolumeMaskImageConstPointer segmentation(
        segmenter.GetSegmentation( levelsetObject, study ) );

      const string segmentationFileName = 
        prefix + "segmentation_" + hashString + extension;

      saved = saved && SaveImage( segmentation.GetPointer(), segmentationFileName, true );

      // if there's a ground truth, save that too
      const VolumeMaskImageConstPointer groundTruth(
        study->GetMask( levelsetObject ) );

      if ( groundTruth )
        {
        const string groundTruthFileName =
          prefix + "ground_truth_" + hashString + extension;
        saved = saved && SaveImage( groundTruth.GetPointer(), groundTruthFileName, true );

        cout << "Saving quantitative results:" << endl;

        // quantitative parameters
        const string
          quantitativeFileName = prefix + "quantitative_" + levelsetObject + ".txt";
        const float
          midglandSlicesPercentage = 1.0f / 3.0f;
        const bool
          verbose = true;

        // save quantitative results
        saved = saved && SaveQuantitativeSegmentationResults(
          segmentation, groundTruth, quantitativeFileName,
          midglandTemplateMask, midglandSlicesPercentage, verbose );
        cout << "Saving quantitative results done." << endl;

        } // groundTruth

      } // isLevelset

    } // for each feature id

  if ( verbose )
    {
    if ( saved )
      cout << "Saving all segmentation results done." << endl;
    else
      cerr << "Error: Unable to save at least one of the results." << endl;
    } // verbose

  // return whether or not could save all the results
  return saved;

} // MFASegmentAndSave
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
