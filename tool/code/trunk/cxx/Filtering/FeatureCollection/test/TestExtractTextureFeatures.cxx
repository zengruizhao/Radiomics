/* 
*  This program test out image filters 
*
*/



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"                // for our image types
#include "ccipdLoadVolume.h"
#include "ccipdSaveImage.h"
#include "ccipdTextureFeatureParser.h"
#include "ccipdTextureFeatureFirstOrderParameters.h"
#include "ccipdTextureFeatureGaborParameters.h"
#include "ccipdFeatureID.h"
#include "ccipdFeatureTypes.h"
#include "ccipdFeatureCollection.h"
#include "ccipdTextureFeatureMapParametersType.h"

#include <boost/math/constants/constants.hpp>

#include "ccipdDisableWarningsMacro.h"
  //ITK includes
  #include <itkVectorImage.h>
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////


int main( int argc, char *argv[] )
{


//////////////////////////////////////////////////////////////////////////////////////////
  if ( argc == 1 )
    {
    std::cout << std::endl << "Usage:" << std::endl <<
      argv[ 0 ] << " inputFileOrDicomFolder" <<
      "[outputFile = TextureFeatureImage.mhd] " <<
      "[outputXML = TextureFeatureParameters.xml]" <<
      std::endl << std::endl;
    return EXIT_SUCCESS;
    } // 1 argument
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  const std::string
    inputFile  = argc > 1 ? argv[1]: "./", 
    outputFile = argc > 2 ? argv[2]: "GaborFeatureImage.mha",
    outputXML  = argc > 3 ? argv[3]: "TextureFeatureParameters.xml";

 //////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  
  std::cout << "Begin generating texture feature parameters." << std::endl;
  ccipd::FeatureIDCollection   ids;
 

  ccipd::FeatureIDPointer enumID1( ccipd::FeatureID::New( ccipd::TextureFeatureGabor ) );
  ids.push_back( enumID1 ); // test gabor
  enumID1->Save(outputXML);
  
 ccipd::FeatureIDPointer enumID2 = ccipd::FeatureID::New();
   enumID2->Load(outputXML);
  //ccipd::FeatureIDPointer enum1( ccipd::FeatureID::New( ccipd::TextureFeatureMean));
  //ccipd::FeatureIDPointer enum2( ccipd::FeatureID::New( ccipd::TextureFeatureMedian));

  ids.push_back( enumID1 );
  ids.push_back( enumID2 );

  float pi = boost::math::constants::pi<float>();
  ccipd::TextureFeatureGaborParameters::Pointer myParameters = ccipd::TextureFeatureGaborParameters::New(2, pi/2 );

  ccipd::FeatureIDPointer enumID3( ccipd::FeatureID::New( ccipd::TextureFeatureGabor, myParameters  ) );
  ids.push_back( enumID3 ); // test gabor

  //ccipd::TextureFeatureFirstOrderParameters::Pointer skewParameters = ccipd::TextureFeatureFirstOrderParameters::New(2);
  //ccipd::FeatureIDPointer enumID2( ccipd::FeatureID::New( ccipd::TextureFeatureSkew, skewParameters ) );
 // ids.push_back( enumID2 ); // test skew


  // for the texture features read in each 
  /*for (int i = 3; i < argc; i++) 
    {
      ccipd::TextureFeatureEnum featureEnum;
      try
        {
        featureEnum = ccipd::TextureFeatureParser::GetTextureFeatureEnum( argv[i] );
        std::cout << argv[i] << " added to texture features." << std::endl;
        }
      catch(...)
        {
        std::cerr << argv[i] << " not recognized as a valid texture feature." << std::endl;
        continue;
        }
      try
        {
        ccipd::FeatureIDPointer enumID( ccipd::FeatureID::New( featureEnum ) );
        ids.push_back( enumID );
        }
      catch(...)
        {
        std::cerr << "Unable to place " << argv[i] << " in the feature collection" << std::endl; 
        }
    } // int i

  if(ids.empty() )
    {
		std::cerr << "No texture features loaded" << std::endl;
		return EXIT_FAILURE;
    }
  std::cout << "Done loading texture features." << std::endl; */
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  // load image
  std::cout << "Begin loading image." << std::endl;

  const ccipd::VolumeImageConstPointer inputImage(
    ccipd::LoadVolume ( inputFile, false ) );

  if ( !inputImage )
    {
	  std::cout << "Could not load image from: " << inputFile << "." << std::endl;
    return EXIT_FAILURE;
    }
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  //calculate feature image
  std::cout << "Begin initializing image texture features" << std::endl;

  ccipd::FeatureCollectionPointer
    imageCollection( ccipd::FeatureCollection::New() );

  imageCollection->SetVolume( inputImage );
  imageCollection->SetVerbose(true);
  
  std::cout << "Done initializing image texture features" << std::endl;
//////////////////////////////////////////////////////////////////////////////////////////

  
  
//////////////////////////////////////////////////////////////////////////////////////////
  // calculate texture features
  std::cout << "Begin calculating image texture features." << std::endl;

  const ccipd::VolumeFeaturesConstPointer 
    featureImage( imageCollection->GetFeatures(ids) );
  
  std::cout << "Done calculating image texture features." << std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  //save featureImage
  std::cout << "Saving " << outputFile << std::endl;

  const bool saved = ccipd::SaveImage(featureImage.GetPointer(), outputFile, true);

  if( !saved)
    {
    std::cerr << "Unable to save " << outputFile << "." <<std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Saving done." << std::endl;
//////////////////////////////////////////////////////////////////////////////////////////

  
  return EXIT_SUCCESS;

} // main

