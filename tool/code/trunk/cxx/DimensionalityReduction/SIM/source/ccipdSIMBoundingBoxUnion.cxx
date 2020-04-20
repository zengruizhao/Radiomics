


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMBoundingBoxUnion.h"
#include "ccipdSIMBoundingBoxExtractor.h"
#include "ccipdStudyAligner.h"
#include "ccipdBoundingBoxCoordinates.h"
#include "ccipdBoundingBoxCreation.h"
#include "ccipdDisableWarningsMacro.h"

  // itk includes
  #include <itkImage.h>
  #include <itkTransform.h>


// boost includes
#include <boost/foreach.hpp>

#ifndef NDEBUG
#include "ccipdBoundingBox.h"
#include "ccipdApplyTransformation.h"
#include "ccipdStudy.h"
#include "ccipdSaveImage.h"
#include <boost/lexical_cast.hpp>
#endif

//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
SIMBoundingBoxType GetBoundingBox(
  const VolumeImageBaseConstPointer & templateImage,
  const StudyConstCollectionType    & studies,
  const SIMBoundingBoxExtractor     & extractor,
  const StudyAligner                & aligner,
  const float                         padding )
{
  
  if ( !templateImage )
    return SIMBoundingBoxType();

  // we will store the min and max coordinates
  VolumePointType globalMinCorner, globalMaxCorner;
  
  // initialize with clearly incorrect values
  globalMinCorner.Fill( std::numeric_limits< VolumePointType::ValueType >::max() );
  globalMaxCorner.Fill( std::numeric_limits< VolumePointType::ValueType >::lowest() );

#ifndef NDEBUG
  SaveImage(
    dynamic_cast< const VolumeMaskImageType * >(
      templateImage.GetPointer() ), "template_image.mha", true );
#endif

  // loop through each study
  BOOST_FOREACH( const StudyConstPointer & study, studies )
    {

    // make sure we have a study
    if ( !study ) continue;

    // extract the bounding box
    const SIMBoundingBoxType boundingBox =
      extractor.GetBoundingBox( study );

    // extract the image
    const auto boundingBoxImage =
      extractor.GetBoundingBoxImage( study );

#ifndef NDEBUG
    using std::cout; using std::endl; using std::string;
    cout << "Study = " << study->GetHash() << endl;
    cout << "Bounding box = " << boundingBox << endl;
    
    SaveImage(
      dynamic_cast< const VolumeMaskImageType * >(
        boundingBoxImage.GetPointer() ),
      string( "bounding_box_image_" ) + 
      boost::lexical_cast< string >( study->GetHash() ) +
      string( ".mha" ), true );
#endif

    if ( !boundingBoxImage ) continue;

    // extract the bounding box coordinates
    const VolumePointType
      localMinCorner = GetMinBoundingBoxCoordinates< double, VolumeDimension >( boundingBox, *boundingBoxImage ),
      localMaxCorner = GetMaxBoundingBoxCoordinates< double, VolumeDimension >( boundingBox, *boundingBoxImage );

#ifndef NDEBUG
    // try something out
    const auto testTransformed = 
      ApplyTransform( aligner.GetTransform( study ),
        dynamic_cast< const VolumeMaskImageType * >(
          boundingBoxImage.GetPointer() ), templateImage );
    const auto testBoundingBox = ccipd::GetBoundingBox( testTransformed, 0 );

    //const auto testPoint1 = GetMinBoundingBoxCoordinates( testBoundingBox, *boundingBoxImage );
    //const auto testPoint2 = GetMaxBoundingBoxCoordinates( testBoundingBox, *boundingBoxImage );
#endif // NDEBUG

    // extract the transform
    // TODO: check if we should take the inverse
    const auto transform = aligner.GetTransform( study )->GetInverseTransform();

    if ( !transform ) continue;

    // we now need to transform the coordinates
    const VolumePointType
      transformedMinCorner = transform->TransformPoint( localMinCorner ),
      transformedMaxCorner = transform->TransformPoint( localMaxCorner );

    // loop through each dimension
    for ( unsigned int dimension = 0; dimension < VolumeDimension; ++dimension )
      {

      if ( transformedMinCorner[ dimension ] < globalMinCorner[ dimension ] )
        globalMinCorner[ dimension ] = transformedMinCorner[ dimension ];

      if ( transformedMaxCorner[ dimension ] > globalMaxCorner[ dimension ] )
        globalMaxCorner[ dimension ] = transformedMaxCorner[ dimension ];

      } // for each dimension

#ifndef NDEBUG
    std::cout << "Local min corner = " << transformedMinCorner << std::endl;
    std::cout << "Local max corner = " << transformedMaxCorner << std::endl;
#endif

    } // for each study

  // expand it by the padding
  globalMinCorner -= padding;
  globalMaxCorner += padding;

#ifndef NDEBUG
  std::cout << "Global min corner = " << globalMinCorner << std::endl;
  std::cout << "Global max corner = " << globalMaxCorner << std::endl;
#endif

  // we will now extract the from the global coordinates bounding box
  const SIMBoundingBoxType finalBoundingBox =
    CreateBoundingBox( globalMinCorner, globalMaxCorner, *templateImage );

  // output
  return finalBoundingBox;

} // GetBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
