#include "ccipdConcatenateFromPatients.h" 
#include "ccipdFeatureCollection.h" 
#include "ccipdMaskToLevelset.h"
#include "ccipdPatient.h"
#include "ccipdStudy.h"

// boost includes
#include <boost/foreach.hpp>             // for loops

#include "ccipdDisableWarningsMacro.h"
  #include <itkCastImageFilter.h> // this itk function does what we need  
  #include <itkAbsImageFilter.h>
#include "ccipdEnableWarningsMacro.h"



namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer ConcatenateStudyVolumeFromPatients(
  const Patients & toConcatenatePatients )
{
  ComposeImageFilter::Pointer composer = ComposeImageFilter::New();
 
  int index = 0;

  const PatientCollectionType patients = toConcatenatePatients.GetAllPatients();
  BOOST_FOREACH( const auto patient, patients)
    {
    const StudyCollectionType studies = patient->GetAllStudies();

    BOOST_FOREACH( const auto study, studies )
      {
      composer->SetInput( index, study->GetVolume() );
      ++index;
      }
    }
 
  composer->Update();
  return composer->GetOutput();

}; // ConcatenateStudyVolumeFromPatients
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer ConcatenateStudyLevelsetFromPatients(
  const Patients        & toConcatenatePatients, 
  const MaskObjectType  maskObject,
  const bool            normalize,
  const bool            sigmoid,
  const float           sigmoidWidth,
  const float           smoothingVariance,
  const bool            returnAbsoluteVal)
{
   typedef itk::AbsImageFilter< 
    ccipd::VolumeImageType, 
    ccipd::VolumeImageType > AbsFilterType;

  ComposeImageFilter::Pointer composer = ComposeImageFilter::New();
  int index = 0;

  const PatientCollectionType patients = toConcatenatePatients.GetAllPatients();
  BOOST_FOREACH( const auto patient, patients)
    {
    const StudyCollectionType studies = patient->GetAllStudies();

    BOOST_FOREACH( const auto study, studies )
      {
      if (study->HasMask( maskObject ))
        {
        ccipd::VolumeImagePointer img = ccipd::MaskToLevelset< InternalPixelType >
            (study->GetMask(maskObject).GetPointer(),normalize, sigmoid, sigmoidWidth,  smoothingVariance, false);

        if (returnAbsoluteVal)
        {
          AbsFilterType::Pointer absFilter = AbsFilterType::New();
          absFilter->SetInput(img);
          absFilter->Update();

          img = absFilter->GetOutput();
        }
             
        composer->SetInput( index, static_cast<const ccipd::VolumeImageType *> (img));
        ++index;
        }
      }
    }

  composer->Update();
  return composer->GetOutput();
}; //ConcatenateStudyMaskFromPatients
//////////////////////////////////////////////////////////////////////////////////////////

VolumeVectorPointer ConcatenateStudyMaskFromPatients(
  const Patients        & toConcatenatePatients, 
  const MaskObjectType  maskObject
 )
{
  ComposeImageFilter::Pointer composer = ComposeImageFilter::New();
  typedef itk::CastImageFilter<VolumeMaskImageType, VolumeImageType>
  CasterType;


  int index = 0;

  const PatientCollectionType patients = toConcatenatePatients.GetAllPatients();
  BOOST_FOREACH( const auto patient, patients)
    {
    const StudyCollectionType studies = patient->GetAllStudies();

    BOOST_FOREACH( const auto study, studies )
      {
      if (study->HasMask( maskObject ))
        {
        CasterType::Pointer caster = CasterType::New();
        caster->SetInput(study->GetMask(maskObject).GetPointer());
        caster->Update();

        composer->SetInput( index, static_cast<const ccipd::VolumeImageType *> (caster->GetOutput() ));
        ++index;
        }
      }
    }

  composer->Update();
  return composer->GetOutput();

};
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer ConcatenateStudyFeatureFromPatients(
  const Patients                & toConcatenatePatients,
  const FeatureIDConstPointer   featureID)
{
  ComposeImageFilter::Pointer composer = ComposeImageFilter::New();
  int index = 0;

  const PatientCollectionType patients = toConcatenatePatients.GetAllPatients();
  BOOST_FOREACH( const auto patient, patients)
    {
    const StudyCollectionType studies = patient->GetAllStudies();

    BOOST_FOREACH( const auto study, studies )
      {
      composer->SetInput( index, study->GetFeatureCollection()->GetFeature( featureID ).at(0));
      ++index;
      }
    }

  composer->Update();
  return composer->GetOutput();
};
//////////////////////////////////////////////////////////////////////////////////////////

}

