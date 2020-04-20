


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCalculateMedialAxesDistance.h"
#include "ccipdLevelsets.h"
#include "ccipdCalculateMedialAxesRMSE.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkPointSet.h>
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
 * \brief computes medial distance by searching the closest point in the other point counde
 */
EvaluationValueType CalculateMedialAxesDistance(
  const VolumeImageType * levelSet1,
  const VolumeImageType * levelSet2)
{
  const MedialAxisSetPointer set1 = CalculateSimpleMedialAxisFromLevelset(levelSet1);
  const MedialAxisSetPointer set2 = CalculateSimpleMedialAxisFromLevelset(levelSet2);
 

  return CalculateMedialAxesRMSE( set1, set2 );
} // CalculateDistanceCentralAxes
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief computes medial distance by searching the closest point in the other point cound
 *
 * \ingroup Segmentation
 */
EvaluationValueType CalculateMedialAxesDistance(
  const LandmarkSetPointer landmarkSet1,
  const LandmarkSetPointer landmarkSet2)
{
  const EvaluationValueType 
    dist1 = CalculateMedialAxesRMSE(landmarkSet1, landmarkSet2),
    dist2 = CalculateMedialAxesRMSE(landmarkSet2, landmarkSet1);

  return (dist1+dist2)/2;
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief compute distance
 * \param landmarks a vector of landmark points
 */
EvaluationValueType CalculateMedialAxesDistance(
  std::vector<LandmarkSetPointer> & landmarks, EvaluationValueType *sd)
{
  EvaluationValueType 
    avg=0.0,
	  avgSq=0.0,
    value;

  int 
	  iSize  = static_cast< int >( landmarks.size() ),
	  iCount = 0;
  
	for (int i=0; i< iSize-1; i++)
    for (int j=i+1; j< iSize; j++)
    {
      value = CalculateMedialAxesDistance(landmarks[i], landmarks[j]);
      if (value>=0) //nan - distance couldn't be computed most likely because of missing landmarks
      {
        iCount++;
        avg += value;
        std::cout << value << std::endl;
        avgSq += value*value;
      }
      else
      {
        std::cout << "Cound't compute distances between " << i << " and " << j << endl;
      }
    }
  
  if (iCount != 0)
  {
    avg /= (float)iCount;
    avgSq /=  (float)iCount;
  }
  
  if (sd)
    *sd = float( sqrt(avgSq - avg*avg) );
  
	return avg;
};


} // namespace
