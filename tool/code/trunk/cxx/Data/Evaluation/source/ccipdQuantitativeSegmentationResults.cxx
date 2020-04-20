


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdQuantitativeSegmentationResults.h"
#include "ccipdCalculateDICE.h"
#include "ccipdCalculateMAD.h" 
#include "ccipdExtractSubMask.h"  // for just getting the midgland

// std includes
#include <iostream>
#include <fstream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::ofstream;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
bool SaveQuantitativeSegmentationResults(
  const VolumeMaskImageType * const segmentation,
  const VolumeMaskImageType * const groundTruth,
  const string              &       fileName,
  const VolumeMaskImageType * const midglandTemplateMask,
  const float                       midglandSlicesPercentage,
  const bool                        verbose )
{

  // error checking
  if ( !segmentation || !groundTruth )
    {
    if ( verbose )
      cerr << "Error: You need both the segmentation and " <<
        "ground truth masks!" << endl;
    return false;
    }

  // calculate dice
  if ( verbose ) cout << "Calculating dice:" << endl;
  const EvaluationValueType dice = CalculateDice( segmentation, groundTruth );
  if ( verbose )
    {
    cout << "Calculating dice done." << endl;
    cout << "Dice = " << dice << endl;
    } // verbose

  // optionally calculate dice for midgland
  const bool doMidgland =
    midglandSlicesPercentage > 0.0f &&
    midglandSlicesPercentage < 1.0f;
  if ( verbose ) cout << "Calculating midgland = " << doMidgland << endl;
  EvaluationValueType midglandDice = -1.0f;
  if ( doMidgland )
    {

    // extract the template
    const VolumeMaskImageConstPointer
      actualMidglandTemplateMask = midglandTemplateMask ?
        midglandTemplateMask : groundTruth;

    // extract the midgland from the masks
    const VolumeMaskImageConstPointer
      midglandSegmentation( ExtractMidglandMask(
        segmentation, actualMidglandTemplateMask, midglandSlicesPercentage ) ),
      midglandGroundTruth( ExtractMidglandMask(
        groundTruth, actualMidglandTemplateMask, midglandSlicesPercentage ) );

    // calculate dice
    midglandDice = CalculateDice( midglandSegmentation, midglandGroundTruth ); 

    if ( verbose ) cout << "Midgland Dice = " << midglandDice << endl;

    } // doMidgland


  // calculate MAD
  if ( verbose ) cout << "Calculating MAD:" << endl;
  const EvaluationValueType MAD = CalculateMAD( segmentation, groundTruth );
  if ( verbose )
    {
    cout << "Calculating MAD done." << endl;
    cout << "MAD = " << MAD << " mm" << endl;
    } // verbose

  try
    {

    // try to open a file
    if ( verbose ) cout << "Opening " << fileName << ":" << endl;

    ofstream outputFile;
    outputFile.open( fileName );

    if ( !outputFile.is_open() )
      {
      if ( verbose ) cerr << "Opening " << fileName << " failed." << endl;
      return false;
      } // isopen

    if ( verbose ) cout << "Opening " << fileName << " done." << endl;

    if ( verbose ) cout << "Saving results to file:" << endl;
    outputFile << "Dice = " << dice << endl;

    if ( midglandDice >= 0.0f )
      outputFile << "Midgland Dice = " << midglandDice << endl;

    outputFile << "MAD = " << MAD << " mm" << endl;
    if ( verbose ) cout << "Saving results to file done." << endl;

    // close the file
    outputFile.close();

    // success!
    return true;

    } // try
  catch( std::exception & exception )
    {
    if ( verbose ) cerr << "Error: " << endl << exception.what() << endl;
    return false;
    }
  catch( ... )
    {
    if ( verbose ) cerr << "Unknown error." << endl;
    return false;
    }

} // SaveQuantitativeSegmentationResults
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
