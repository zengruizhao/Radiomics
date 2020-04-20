


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdConcatenateLandmarks.h"
#include "ccipdDuplicateLandmarks.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

/**
 * \brief   Simple code to concatenate two landmark lists 
 */
LandmarkSetPointer ConcatenateLandmarkSets( const LandmarkSetType * const landmarkSet1, const LandmarkSetType * const landmarkSet2 )
{
  LandmarkSetPointer landmarkSet = LandmarkSetType::New();

  if (!landmarkSet1 && !landmarkSet2 )
    return LandmarkSetPointer();

  if (!landmarkSet1)
    return DuplicateLandmarkSet( landmarkSet2 );

  if (!landmarkSet2)
    return DuplicateLandmarkSet( landmarkSet1 );


  const LandmarkSetType::PointIdentifier
    sizeSet1 = landmarkSet1->GetNumberOfPoints(),
    sizeSet2 = landmarkSet2->GetNumberOfPoints();
 
  for ( LandmarkSetType::PointIdentifier i = 0; i< sizeSet1; ++i)
    landmarkSet->SetPoint(i, landmarkSet1->GetPoint(i));  
 
  for ( LandmarkSetType::PointIdentifier i = 0; i< sizeSet2; ++i)
    landmarkSet->SetPoint(sizeSet1+i, landmarkSet2->GetPoint(i));  
 
  return landmarkSet;
};


/**
 * \brief   Simple code to concatenate two landmark lists 
 */
LandmarkSetPointer ConcatenateLandmarks( const LandmarkPointType landmark1, const LandmarkPointType landmark2 )
{ 
  
  LandmarkSetPointer landmarkSet = LandmarkSetType::New();

  landmarkSet->SetPoint(landmarkSet->GetNumberOfPoints(), landmark1);
  landmarkSet->SetPoint(landmarkSet->GetNumberOfPoints(), landmark2);

  return landmarkSet;
};


} // namespace
