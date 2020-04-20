

//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"
#include "ccipdBoundingBoxCreation.h"


// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImageBase.h>
#include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
itk::ImageRegion< TDimension >
CreateBoundingBox(
  const itk::PointSet<
    LandmarkType,
    TDimension,
    itk::DefaultStaticMeshTraits<
      MeshPixelType,
      TDimension,
      TDimension,
      MeshPixelType,
      MeshPixelType,
      MeshPixelType
  > > & points,
  const itk::ImageBase< TDimension > & image )
{

  // typedefs
  typedef itk::Point< double, TDimension > PointType;
  typedef itk::ImageBase<     TDimension > ImageType;
  typedef itk::ImageRegion<   TDimension > BoundingBoxType;
  typedef typename ImageType::IndexType    IndexType;

  const auto numberOfPoints = points.GetNumberOfPoints();
  if ( numberOfPoints < 2 )
    {
    std::cerr << "At least two points should be provided in order to create a bounding box" << std::endl;
    return BoundingBoxType();
    }

  // get the min and max corners
  // because the user may have
  // switched + and - or something

  PointType minCorner, maxCorner;
  
  for ( unsigned int dimension = 0; dimension < TDimension; ++dimension )
    {
      
    minCorner[ dimension ] = points.GetPoint( 0 )[ dimension ];
    maxCorner[ dimension ] = points.GetPoint( 0 )[ dimension ];

    for ( unsigned int pointIndex = 1; pointIndex < numberOfPoints; ++pointIndex )
      {
      const auto currentPointValue = points.GetPoint( pointIndex )[ dimension ];
      minCorner[ dimension ] = minCorner[ dimension ] > currentPointValue ? currentPointValue : minCorner[ dimension ];
      maxCorner[ dimension ] = maxCorner[ dimension ] < currentPointValue ? currentPointValue : maxCorner[ dimension ];
      }

    } // for dimension

  // we will now extract the from the global coordinates bounding box

  IndexType calculatedMinIndex, calculatedMaxIndex;

  const bool
    foundMinIndex = image.TransformPhysicalPointToIndex(
      minCorner, calculatedMinIndex ),
    foundMaxIndex = image.TransformPhysicalPointToIndex(
      maxCorner, calculatedMaxIndex );

  // if we couldn't find it, just use the largest image region
  const BoundingBoxType bufferedBoundingBox = image.GetBufferedRegion();

  // update the min and max indices
  const IndexType
    finalMinIndex = foundMinIndex ?
      calculatedMinIndex : bufferedBoundingBox.GetIndex(),
    finalMaxIndex = foundMaxIndex ?
      calculatedMaxIndex : bufferedBoundingBox.GetUpperIndex();

  // TODO:
  // they may have only gone
  // "over" or "under" in one
  // dimension, so potentially
  // keep some of the dimensions'
  // index values

  // create our bounding box
  BoundingBoxType finalBoundingBox;
  finalBoundingBox.SetIndex     ( finalMinIndex );
  finalBoundingBox.SetUpperIndex( finalMaxIndex );

  // output
  return finalBoundingBox;

} // CreateBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
itk::ImageRegion< TDimension >
CreateBoundingBox(
  const itk::Point< double, TDimension > & corner1,
  const itk::Point< double, TDimension > & corner2,
  const itk::ImageBase<     TDimension > & image )
{

  typedef itk::PointSet<
    LandmarkType,
    TDimension,
    itk::DefaultStaticMeshTraits<
      MeshPixelType,
      TDimension,
      TDimension,
      MeshPixelType,
      MeshPixelType,
      MeshPixelType
  > > PointSetType;

  const typename PointSetType::Pointer points = PointSetType::New();
  points->Initialize();

  points->SetPoint( 0, corner1 );
  points->SetPoint( 1, corner2 );

  return CreateBoundingBox< TDimension >( *points, image );
 
} // CreateBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////

 template<class TPointSetType>
 typename TPointSetType::Pointer
 GetCornerPoints(typename TPointSetType::Pointer pInputSet)
 {
   // typedefs
   typedef typename TPointSetType::PointType PointType;
   typedef typename TPointSetType::Pointer PointSetPointerType;

   const auto numberOfPoints = pInputSet->GetNumberOfPoints();
   if ( numberOfPoints < 2 )
   {
     std::cerr << "At least two points should be provided in order to create a bounding box" << std::endl;
     return NULL;
   }
 
   PointType minCorner, maxCorner;
   
    for ( unsigned int dimension = 0; dimension < pInputSet->PointDimension; ++dimension )
    {
  
      minCorner[ dimension ] = pInputSet->GetPoint(0)[ dimension ];
      maxCorner[ dimension ] = pInputSet->GetPoint(0)[ dimension ];
  
      for ( unsigned int pointIndex = 1; pointIndex < numberOfPoints; ++pointIndex )
      {
        const auto currentPointValue = pInputSet->GetPoint( pointIndex )[ dimension ];
        minCorner[ dimension ] = minCorner[ dimension ] > currentPointValue ? currentPointValue : minCorner[ dimension ];
        maxCorner[ dimension ] = maxCorner[ dimension ] < currentPointValue ? currentPointValue : maxCorner[ dimension ];
      }
  
    } // for dimension
 
  
     //typedef itk::PointSet<LandmarkType, TDimension,
      // itk::DefaultStaticMeshTraits<MeshPixelType, TDimension, TDimension,
     //  MeshPixelType, MeshPixelType, MeshPixelType> 
      //                    > PointSetType;
 
     typename TPointSetType::Pointer pCornerPoints = TPointSetType::New();
    //typename PointSetType::Pointer pCornerPoints = PointSetType::New();
    
	
	pCornerPoints->Initialize();
  
    pCornerPoints->SetPoint( 0, minCorner );
    pCornerPoints->SetPoint( 1, maxCorner );
  
    return pCornerPoints;
 
 } // 

 template< unsigned int TDimension >
 itk::ImageRegion< TDimension >
   CreateBoundingBox2(
   const itk::PointSet<
   LandmarkType,
   TDimension,
   itk::DefaultStaticMeshTraits<
   MeshPixelType,
   TDimension,
   TDimension,
   MeshPixelType,
   MeshPixelType,
   MeshPixelType
   > > & points,
   const itk::ImageBase< TDimension > & image )
 {

   // typedefs
   typedef itk::Point< double, TDimension > PointType;
   typedef itk::ImageBase<     TDimension > ImageType;
   typedef itk::ImageRegion<   TDimension > BoundingBoxType;
   typedef typename ImageType::IndexType    IndexType;

   const auto numberOfPoints = points.GetNumberOfPoints();
   if ( numberOfPoints < 2 )
   {
     std::cerr << "At least two points should be provided in order to create a bounding box" << std::endl;
     return BoundingBoxType();
   }

   PointType minCorner, maxCorner;

   for ( unsigned int dimension = 0; dimension < TDimension; ++dimension )
   {

     minCorner[ dimension ] = points.GetPoint( 0 )[ dimension ];
     maxCorner[ dimension ] = points.GetPoint( 0 )[ dimension ];

     for ( unsigned int pointIndex = 1; pointIndex < numberOfPoints; ++pointIndex )
     {
       const auto currentPointValue = points.GetPoint( pointIndex )[ dimension ];
       minCorner[ dimension ] = minCorner[ dimension ] > currentPointValue ? currentPointValue : minCorner[ dimension ];
       maxCorner[ dimension ] = maxCorner[ dimension ] < currentPointValue ? currentPointValue : maxCorner[ dimension ];
     }

   } // for dimension


   // we will now extract the from the global coordinates bounding box

   IndexType calculatedMinIndex, calculatedMaxIndex;

   const bool foundMinIndex = image.TransformPhysicalPointToIndex(minCorner, calculatedMinIndex);
   const bool foundMaxIndex = image.TransformPhysicalPointToIndex(maxCorner, calculatedMaxIndex);

   if (!foundMinIndex || !foundMaxIndex)
   {
     std::cerr << "Failed to get indexes of picked points in order to create a bounding box" << std::endl;
     return BoundingBoxType();
   }



   // if we couldn't find it, just use the largest image region
   //const BoundingBoxType bufferedBoundingBox = image.GetBufferedRegion();

   // update the min and max indices
//    IndexType
//      finalMinIndex = foundMinIndex ? calculatedMinIndex : bufferedBoundingBox.GetIndex(),
//      finalMaxIndex = foundMaxIndex ? calculatedMaxIndex : bufferedBoundingBox.GetUpperIndex();

   for (unsigned int i = 2; i < image.ImageDimension; i++)
   {
     // Check if the depth of the bounding box == 1, then extend it to the number of slices
     if (calculatedMinIndex[i] == calculatedMaxIndex[i])
     {
       calculatedMinIndex[i] = image.GetLargestPossibleRegion().GetIndex(i);
       calculatedMaxIndex[i] = image.GetLargestPossibleRegion().GetSize(i) + calculatedMinIndex[i] - 1;
     }
   }

   // create our bounding box
   BoundingBoxType finalBoundingBox;
   finalBoundingBox.SetIndex     ( calculatedMinIndex );
   finalBoundingBox.SetUpperIndex( calculatedMaxIndex );

   // output
   return finalBoundingBox;

 } // CreateBoundingBox2





} // namespace ccipd
