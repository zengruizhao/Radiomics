#ifndef __ccipdExtractFeaturesFromPatients_h
#define __ccipdExtractFeaturesFromPatients_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPatients.h"
#include "ccipdFeatureTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Extract the features from the studies of all patients 
 * 
 * \author  Mirabela Rusu
 * \ingroup Data
 */
void ExtractFeaturesFromPatients(
  Patients                        & patients,
  const FeatureIDConstCollection  whichFeatures);
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Extract the features from the studies of a patient
 * 
 * \author  Mirabela Rusu
 * \ingroup Data
 */
void ExtractFeaturesFromPatient(
  Patient                         & patient,
  const FeatureIDConstCollection  whichFeatures);
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Extract the features from the studies of all patients 
 * 
 * \author  Mirabela Rusu
 * \ingroup Data
 */
void ExtractFeaturesFromStudy(
  Study                          & study,
  const FeatureIDConstCollection whichFeatures);
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace
#endif // __ccipdExtractFeaturesFromPatients
