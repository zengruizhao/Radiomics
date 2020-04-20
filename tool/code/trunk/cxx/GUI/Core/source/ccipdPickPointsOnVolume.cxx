


////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdPickPointsOnVolume.h"
#include "ccipdPickPointsDialog.h"
#include "ccipdApplyTransformation.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImage.h>
#include <itkPointSet.h>
#include <itkMatrixOffsetTransformBase.h>
#include "ccipdEnableWarningsMacro.h"

////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
////////////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer PickPointsOnImage(
  const VolumeImageConstPointer & image )
{
  return PickPointsOnImage( image, LandmarkSetConstPointer() );
}
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer PickPointsOnImage(
  const VolumeImageConstPointer & image,
  const LandmarkSetConstPointer & initialLandmarks )
{

  // display the dialog
  PickPointsDialog dialog( image, initialLandmarks );

  // run the dialog
  const auto returnValue = dialog.exec();

  // return if the user pressed cancel
  if ( returnValue != QDialog::Accepted )
    return LandmarkSetPointer();

  // output the landmarks the user picked
  const LandmarkSetPointer landmarks = dialog.GetLandmarks();

  if ( !landmarks ) return landmarks;
  /*
  // create a new transform
  typedef itk::MatrixOffsetTransformBase<
    TransformValueType, VolumeDimension
  > TransformType;

  // get the matrix
  const TransformType::MatrixType
    imageMatrix = image->GetDirection(),
    matrixInverse( imageMatrix );

  const TransformType::Pointer transform = TransformType::New();
  transform->SetIdentity();
  transform->SetMatrix( matrixInverse );

  using std::cout;
  using std::endl;

  cout << "Transform = " << endl << transform << endl;
  // loop through
  for ( LandmarkSetType::PointIdentifier pointIndex = 0;
    pointIndex < landmarks->GetNumberOfPoints(); ++pointIndex )
    {

    // get the point
    const LandmarkPointType
      originalPoint = landmarks->GetPoint( pointIndex ),
      transformedPoint = transform->TransformPoint( originalPoint );

    // get the indices
    typedef VolumeImageBase::IndexType IndexType;
    IndexType originalIndex, transformedIndex, nullIndex;
    nullIndex.Fill( -1 );
    
    const bool
      hasOriginalIndex = image->TransformPhysicalPointToIndex(
        originalPoint, originalIndex ),
      hasTransformedIndex = image->TransformPhysicalPointToIndex(
        transformedPoint, transformedIndex );

    cout << "Which point = " << pointIndex << endl;
    cout << "Point = " << originalPoint << endl;
    cout << "T( point ) = " << transformedPoint << endl;
    cout << "Index = " <<
      ( hasOriginalIndex ? originalIndex : nullIndex ) << endl;
    cout << "T( Index ) = " <<
      ( hasTransformedIndex ? transformedIndex : nullIndex ) << endl;

    } // for pointIndex

  return ApplyTransform( transform, landmarks );
  */

  return landmarks;

} // PickPointsOnImage
////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
