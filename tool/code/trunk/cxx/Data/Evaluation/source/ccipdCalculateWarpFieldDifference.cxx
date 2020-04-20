


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd incldues
#include "ccipdCalculateWarpFieldDifference.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegionConstIterator.h>
  #include <itkImageRegionIterator.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////

VolumeWarpFieldPointer CalculateWarpFieldDifference(
  const VolumeWarpFieldType * const field1,
  const VolumeWarpFieldType * const field2
  )
{
/* Since this function takes in two VolumeWarpFieldType which are inverses
 * of each other, the function (1) transforms the inverse field, (2) adds the fields together 
 * The first warpfield is used as a template. 
 */

  VolumeWarpFieldPointer newField = VolumeWarpFieldType::New();
  
  newField->SetDirection( field1->GetDirection() );
  newField->SetSpacing (field1->GetSpacing() );
  newField->SetOrigin (field1->GetOrigin() );
  
  newField->SetRegions( field1->GetBufferedRegion() );
  newField->Allocate();

  itk::ImageRegionConstIterator< 
    VolumeWarpFieldType
  > fieldIter ( field1, field1->GetBufferedRegion() );

  itk::ImageRegionIterator< 
    VolumeWarpFieldType
  > newFieldIter ( newField, newField->GetBufferedRegion() );

  newFieldIter.GoToBegin();
  fieldIter.GoToBegin();

 
  for (; !fieldIter.IsAtEnd() ;++fieldIter, ++newFieldIter)
   {
   VolumeWarpFieldType::PixelType forward    = fieldIter.Get();

   //get the location for the forward displacement field
   VolumeWarpFieldType::PointType forwardLoc;
   field1->TransformIndexToPhysicalPoint( fieldIter.GetIndex(), forwardLoc);


   //transform the location to the backward displacement field
   VolumeWarpFieldType::PointType backwardLoc;
   for (unsigned int i = 0; i < VolumeWarpFieldType::PixelType::Dimension; ++i)
     {
     backwardLoc[i] = forwardLoc[i] + forward[i];
     }//for unsigned int i

   VolumeWarpFieldType::IndexType backwardIndex;
   field2->TransformPhysicalPointToIndex(backwardLoc, backwardIndex);

   VolumeWarpFieldType::PixelType backward = field2->GetPixel(backwardIndex);

   VolumeWarpFieldType::PixelType difference;
   for(unsigned int i = 0; i < VolumeWarpFieldType::PixelType::Dimension; ++i)
     {
     difference[i] = (forward[i] + backward[i]);
     } //for unsigned int i

   newFieldIter.Set(difference);
   }// for fieldIter

 return newField;
}

} // namespace ccipd