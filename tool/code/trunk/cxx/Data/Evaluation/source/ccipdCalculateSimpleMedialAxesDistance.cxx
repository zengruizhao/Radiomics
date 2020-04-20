


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCalculateSimpleMedialAxesDistance.h"
#include "ccipdLevelsets.h"
#include "ccipdCalculateSimpleMedialAxesRMSE.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkPointSet.h>
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief computes medial distance on a slice by slice basis
 */
EvaluationValueType CalculateSimpleMedialAxesDistance(
  const VolumeImageType * levelSet1,
  const VolumeImageType * levelSet2)
{
  const MedialAxisSetPointer set1 = CalculateSimpleMedialAxisFromLevelset(levelSet1);
  const MedialAxisSetPointer set2 = CalculateSimpleMedialAxisFromLevelset(levelSet2);
 

  return CalculateSimpleMedialAxesRMSE( set1, set2 );
} // CalculateSimpleMedialAxesDistance
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateSimpleMedialAxesDistance(
  VolumeImagesContainer levelSets)
{
  int iSize = static_cast< int >( levelSets.size() );
  EvaluationValueType
    avg=0.0,
    dist;
  int iCount = 0;
  for (int i=0; i< iSize-1; i++)
    for (int j=i+1; j< iSize; j++)
    {
      dist = CalculateSimpleMedialAxesDistance(levelSets[i], levelSets[j]);
      
      if (dist != std::numeric_limits< EvaluationValueType >::max() )
      {
        iCount++;
        avg += dist;
      }
    }
  if (iCount != 0)
    avg /= (float)iCount;

   return avg;
}; // CalculateSimpleMedialAxesDistance
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
