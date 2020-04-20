


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCalculateWarpFieldSSD.h"
#include "ccipdCalculateWarpFieldDifference.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegionConstIterator.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////

EvaluationValueType CalculateWarpFieldSSD(
  const VolumeWarpFieldType * const field1,
  const VolumeWarpFieldType * const field2
  )
{
  EvaluationValueType ssd= 0;

  VolumeWarpFieldPointer diffField = CalculateWarpFieldDifference( field1, field2 );

  itk::ImageRegionConstIterator< 
    VolumeWarpFieldType
  > fieldIter ( diffField, diffField->GetBufferedRegion() );
  
  for ( fieldIter.GoToBegin();!fieldIter.IsAtEnd(); ++fieldIter )
    {
    VolumeWarpFieldType::PixelType diffVector = fieldIter.Get();
    EvaluationValueType diffSum = 0;

    for (unsigned int i=0; i< diffVector.Dimension;++i)
      {
        diffSum+= ( diffVector[i]*diffVector[i] );
      } //for unsigned int i

    ssd+= std::sqrt( diffSum );

    } // for fieldIter

  return ssd;
};

//////////////////////////////////////////////////////////////////////////////////////////

} //namespace ccipd