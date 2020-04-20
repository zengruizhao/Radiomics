#ifndef __ccipdConcatenateFromPatients_h
#define __ccipdConcatenateFromPatients_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPatients.h"
#include "ccipdVolumeTypes.h"
#include "ccipdFeatureTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVectorImage.h>
  #include <itkComposeImageFilter.h> // this itk function does what we need
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  
  // create our filter
  typedef itk::ComposeImageFilter<
    VolumeImageType,
    VolumeVectorType
  > ComposeImageFilter;

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Concatenates the volume of studies accross all the patients
 * if patient has more than one study all studies will be concatenated
 * 
 * \Note They must all be in the same coordinate frame.
 * \Note Not using VolumeContainerToVolumeFeatures due to memory usage: for
 * isotropic data memory is way to high, making it unfeasible to hold a image
 * container and vector image in memory. 
 * 
 * \author  Mirabela Rusu
 * \ingroup Data
 */
VolumeFeaturesPointer ConcatenateStudyVolumeFromPatients(
  const Patients & patients );
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Concatenates the Masks given by the MaskOjecte accross all the patients
 * if patient has more than one study all masks will be concatenated
 * 
 * \Note They must all be in the same coordinate frame.
 * 
 * \author  Mirabela Rusu
 * \ingroup Data
 */
VolumeFeaturesPointer ConcatenateStudyLevelsetFromPatients(
  const Patients        & patients, 
  const MaskObjectType  maskObject,
  const bool            normalize         = false,
  const bool            sigmoid           = false,
  const float           sigmoidWidth      = 4.0,
  const float           smoothingVariance = 0.0,
  const bool            returnAbsoluteVal = false 
 );
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Concatenates the Masks given by the MaskOjecte accross all the patients
 * if patient has more than one study all masks will be concatenated
 * 
 * \Note They must all be in the same coordinate frame.
 * 
 * \author  Mirabela Rusu
 * \ingroup Data
 */
VolumeVectorPointer ConcatenateStudyMaskFromPatients(
  const Patients        & patients, 
  const MaskObjectType  maskObject
 );
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Concatenates the given feature for the studies accross all the patients
 * if patient has more than one study all studies will be concatenated
 * 
 * \Note They must all be in the same coordinate frame.
 * 
 * \author  Mirabela Rusu
 * \ingroup Data
 */
VolumeFeaturesPointer ConcatenateStudyFeatureFromPatients(
  const Patients                & patients,
  const FeatureIDConstPointer   featureID);
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace
#endif // __ccipdConcatenateFromPatients
