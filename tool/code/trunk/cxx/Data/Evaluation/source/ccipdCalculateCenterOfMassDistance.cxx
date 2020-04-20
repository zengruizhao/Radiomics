


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCalculateCenterOfMassDistance.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkImageMomentsCalculator.h> // for center of mass
#include "ccipdEnableWarningsMacro.h"

#include <limits>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
//////////////////////////////////////////////////////////////////////////////////////////
Data_EXPORT
EvaluationValueType CalculateCenterOfMassDistance(
  const VolumeMaskImageType * const mask1,
  const VolumeMaskImageType * const mask2 )
{

  typedef VolumeMaskImageType ImageType;

  // create a calculator
  typedef itk::ImageMomentsCalculator< ImageType >
    MomentsCalculatorType;

  const MomentsCalculatorType::Pointer
    mask1MomentCalculator = MomentsCalculatorType::New(),
    mask2MomentCalculator = MomentsCalculatorType::New();

  // input the masks
  mask1MomentCalculator->SetImage( mask1 );
  mask2MomentCalculator->SetImage( mask2 );

  itk::Vector< double, VolumeDimension > 
    mask1CenterOfMass,
    mask2CenterOfMass;
 
  try 
    {
    // compute
    mask1MomentCalculator->Compute();
    mask2MomentCalculator->Compute();

    // calculate the center of mass of each image
    mask1CenterOfMass = mask1MomentCalculator->GetCenterOfGravity(),
    mask2CenterOfMass = mask2MomentCalculator->GetCenterOfGravity();
    }
  catch( ... )
  {
    std::cout << "Unable to calculate center of mass. Returning NULL" << std::endl;
    return std::numeric_limits< ccipd::EvaluationValueType >::quiet_NaN();
  }

  // calculate the distance
  EvaluationValueType distance = 0;
  for ( unsigned int dimension = 0; dimension < VolumeDimension; ++dimension )
    {

    const EvaluationValueType
      displacement = mask1CenterOfMass[ dimension ] - mask2CenterOfMass[ dimension ];

    distance += displacement * displacement;

    } // for dimension

  // sqrt( (x1 - x2)^2 + ...
  distance = std::sqrt( distance );

  // output
  return distance;

} // CalcualteCenterOfMassDistance
//////////////////////////////////////////////////////////////////////////////////////////

} //namespace ccipd
