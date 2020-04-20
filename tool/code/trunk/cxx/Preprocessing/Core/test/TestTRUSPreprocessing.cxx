


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdPatient.h"
#include "ccipdStudy.h"
#include "ccipdDuplicateImage.h"
#include "ccipdSaveImage.h"
#include "ccipdPreprocessGIPL.h"
#include "ccipdFeatureTypes.h"
#include "ccipdFeatureID.h"
#include "ccipdFeatureCollection.h"
#include "ccipdVolumeFeaturesToGaussianModels.h"
#include "ccipdCalculateProbabilityImage.h"
#include "ccipdTextureFeatureExtractor.h"
#include "ccipdCalculateDICE.h"
#include "ccipdTextureFeatureGaborParameters.h"
// boost includes
#include <boost/math/constants/constants.hpp>

// std includes
#include <climits>                          // for defining min and max pixel values            
#include <iostream>                         // for outputting text
#include <string>                           // for file names
#include <vector>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkThresholdLabelerImageFilter.h>
  #include <itkAddImageFilter.h>
  #include <itkSubtractImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



/*
 * Test function for correction of TRUS artifacts.
 * In this function tissue attenuation is corrected.
 * For both corrected and uncorrected images a set of 
 * features is extracted and used to build a Gaussian model.
 * Both models have are then compared for accuracy.
 */

// argc is the count of the number of arguments given to the program.
// argv is an array of characters for each argument.
int main( int argc, char *argv[] )
{



//////////////////////////////////////////////////////////////////////////////////////////
	if ( argc == 1 )
	{
		
    // display help message
    std::cout << std::endl << std::endl <<
      "Usage: " << argv[ 0 ] <<
      " patient xml " <<
      "write directory[ =./] " <<
    std::endl << std::endl << 
	  "This will calculate an image model for give patient TRUS study. "
	<< std::endl << std::endl;

	return EXIT_SUCCESS;
	}

const std::string   xmlFile        = argv[1],
	                directoryWrite = argc > 2 ?     argv[2]  : "./";
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Load in the Study
std::cout << "Begin Loading Volume." << std::endl;

const bool returnNULLOnError = false;
const ccipd::Patient::ConstPointer patient = ccipd::Patient::ConstNew( xmlFile, returnNULLOnError );

if (!patient )
  {
  std::cout << "Unable to load patient from file " << xmlFile.c_str() ;
  return EXIT_FAILURE;
  }
ccipd::Study::Pointer  fixedTRUS( *(patient->GetStudiesWithModality("TRUS" ).cbegin()) );
ccipd::Study::ConstPointer spatialPrior( *(patient->GetStudiesWithModality("Spatial Prior").cbegin()) );

// Check whether or not I am preprocessing
const std::string fileDescriptor =
  ( fixedTRUS->GetBiasCorrectionSigma() ) ? directoryWrite +"/Processed" :
  directoryWrite+"/";

ccipd::SaveImage( fixedTRUS->GetVolume().GetPointer(), fileDescriptor + "Image.mhd", true );

std::cout << "Done Loading Volume." << std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
//calculate a set of features for the image 
std::cout << "Begin Calculating Features." << std::endl;

ccipd::FeatureIDPointer
  meanID     ( ccipd::FeatureID::New( ccipd::TextureFeatureMean                  ) ),
  skewID     ( ccipd::FeatureID::New( ccipd::TextureFeatureSkew                  ) ),
  medianID   ( ccipd::FeatureID::New( ccipd::TextureFeatureMedian                ) ),
  rangeID    ( ccipd::FeatureID::New( ccipd::TextureFeatureRange                 ) ),
  stdID      ( ccipd::FeatureID::New( ccipd::TextureFeatureStandardDeviation     ) ),
  nakagamiID ( ccipd::FeatureID::New( ccipd::TextureFeatureNakagami              ) ),
  rayleighID ( ccipd::FeatureID::New( ccipd::TextureFeatureRayleigh              ) );
ccipd::FeatureIDPointer gabor1( ccipd::FeatureID::New( ccipd::TextureFeatureGabor, ccipd::TextureFeatureGaborParameters::New(.1, 0 ) ) ); 

float pi = boost::math::constants::pi<float>();
ccipd::FeatureIDPointer gabor2( ccipd::FeatureID::New( ccipd::TextureFeatureGabor, ccipd::TextureFeatureGaborParameters::New(.1, pi/2 ) ) ); 

ccipd::FeatureIDCollection ids;
  ids.push_back( medianID );
  ids.push_back( rangeID );
  ids.push_back( rayleighID );
  ids.push_back( nakagamiID );
ccipd::FeatureCollectionPointer collection( ccipd::FeatureCollection::New() );

collection->SetVolume ( fixedTRUS->GetVolume() );
collection->SetVerbose( true  );
  
ccipd::SaveImage( collection->GetFeatures( ids ).GetPointer(), fileDescriptor + "VectorImage.mhd", true );

std::cout << "Done Calculating Features." << std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// add a 1 to the mask so I can calculate inside and outside probabilities
typedef itk::AddImageFilter<
  ccipd::VolumeMaskImageType, 
  ccipd::VolumeMaskImageType,
  ccipd::VolumeMaskImageType
> AddFilterMaskType;

AddFilterMaskType::Pointer addMask = AddFilterMaskType::New();
addMask->SetConstant2( 1 );
addMask->SetInput1( fixedTRUS->GetMask("Prostate") );
addMask->Update();
 
ccipd::VolumeMaskImageType::ConstPointer testMask = fixedTRUS->GetMask("Prostate");
ccipd::VolumeMaskImageType::Pointer testAdder = addMask->GetOutput();

ccipd::SaveImage( addMask->GetOutput(), fileDescriptor + "Mask.mhd", true );

const ccipd::VolumeFeaturesPointer  vectorImage =
    const_cast< ccipd::VolumeFeaturesType * >(
      collection->GetFeatures(ids).GetPointer() );

const ccipd::VolumeFeaturesGuassianFunctionContainer imageModel 
  ( ccipd::VolumeFeaturesToGaussianModels (
    collection->GetFeatures( ids ).GetPointer(), addMask->GetOutput(), 2 
  ) );


const ccipd::VolumeImageConstPointer
  ProbModel1 ( 
    ccipd::CalculateProbabilityImage(
      collection->GetFeatures( ids ).GetPointer(), 
      imageModel[0] 
    ) ),
  ProbModel2 ( 
    ccipd::CalculateProbabilityImage(
      collection->GetFeatures( ids ).GetPointer(), 
      imageModel[1]
    ) );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
typedef itk::SubtractImageFilter<
  ccipd::VolumeImageType,
  ccipd::VolumeImageType
> SubtractImageType;

SubtractImageType::Pointer subtractImageFilter = SubtractImageType::New();

subtractImageFilter->SetInput1(ProbModel1);
subtractImageFilter->SetInput2(ProbModel2);
subtractImageFilter->Update();


ccipd::SaveImage( subtractImageFilter->GetOutput(), fileDescriptor+"DifferenceProbImage.mhd", true );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
typedef itk::ThresholdLabelerImageFilter< 
  ccipd::VolumeImageType,
  ccipd::VolumeMaskImageType
> ThresholdFilterType;

ThresholdFilterType::Pointer thresholdFilter = ThresholdFilterType::New();

std::vector< double >  thresholdVector;
thresholdVector.push_back((double) 0);

thresholdFilter->SetRealThresholds( thresholdVector   );
thresholdFilter->SetInput         ( subtractImageFilter->GetOutput() );

thresholdFilter->Update();
ccipd::SaveImage( thresholdFilter->GetOutput(), fileDescriptor+"ThresholdImage.mhd", true );

ccipd::EvaluationValueType diceBiasCorrectImage(
  ccipd::CalculateDice( 
    thresholdFilter->GetOutput(), fixedTRUS->GetMask("Prostate"), 0, 0 
  ) );

std::cout << "Bias Correct DICE: " << diceBiasCorrectImage << std::endl;  
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
return EXIT_SUCCESS;
//////////////////////////////////////////////////////////////////////////////////////////
}