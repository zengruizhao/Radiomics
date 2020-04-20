#ifndef __ccipdPatientsStatistics_h
#define __ccipdPatientsStatistics_h



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdPatient.h"
#include "ccipdPatients.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{

/**
 * computes the max volume of all studies and masks
 * \param inImage the input patient
 * \param threahold used 
 * 
 * \author  Mirabela Rusu
 */
EvaluationValueType GetMaxVolume(
	const ccipd::Patient  & inPatient,
  const MaskObjectType  maskObject,
	const float           threshold = 1.0,
  const bool            latest = true );

/**
 * computes the max volume of all the patients, studies and masks
 * \param inImage the input patient list
 * \param threahold used 
 * 
 * \author  Mirabela Rusu
 */
EvaluationValueType GetMaxVolume(
	const ccipd::Patients & inPatients,
	const MaskObjectType  maskObject,
  const float           threshold = 1.0,
  const bool            latest = true );

/**
 * computes the min volume of all studies and masks
 * \param inImage the input patient
 * \param threahold used 
 * 
 * \author  Mirabela Rusu
 */
EvaluationValueType GetMinVolume(
	const ccipd::Patient  & inPatient,
  const MaskObjectType  maskObject,
	const float      threshold = 1.0,
  const bool            latest = true );

/**
 * computes the min volume of all the patients, studies and masks
 * \param inImage the input patient list
 * \param threahold used 
 * 
 * \author  Mirabela Rusu
 */
EvaluationValueType GetMinVolume(
	const ccipd::Patients & inPatients,
	const MaskObjectType  maskObject,
  const float      threshold = 1.0,
  const bool            latest = true );

/**
 * computes the median volume of all the patients, studies and masks
 * \param inImage the input patient list
 * \param threahold used 
 * 
 * \author  Mirabela Rusu
 */
EvaluationValueType GetMedianVolume(
	const ccipd::Patients & inPatients,
	const MaskObjectType  maskObject,
  const float           threshold = 1.0,
  const bool            latest = true );

/**
 * Get min spacing 
 */
VolumeImageType::SpacingType GetMinSpacing(
	const ccipd::Patients & inPatients,
  const bool            latest = true );


/**
 * Get max spacing 
 */
VolumeImageType::SpacingType GetMaxSpacing(
	const ccipd::Patients & inPatients,
  const bool            latest = true );

} //namespace ccipd

#endif //__ccipdStatistics_h
