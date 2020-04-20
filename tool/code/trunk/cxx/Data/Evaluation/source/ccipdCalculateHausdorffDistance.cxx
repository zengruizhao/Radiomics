


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCalculateHausdorffDistance.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkHausdorffDistanceImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateHausdorffDistance(
  const VolumeImageType * const fixed,
  const VolumeImageType * const moving
  )
{
  EvaluationValueType value;
  typedef itk::HausdorffDistanceImageFilter 
          <VolumeImageType,VolumeImageType>  HDFilter;

  HDFilter::Pointer filter = HDFilter::New();

  filter->SetInput1( fixed );
  filter->SetInput2( moving );
  filter->Update();

  value = filter->GetHausdorffDistance();
  
  return value;
}
//////////////////////////////////////////////////////////////////////////////////////////

} //namespace