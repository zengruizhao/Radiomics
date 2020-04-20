

////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPatientsStatistics.h"
#include "ccipdStatistics.h"
#include "ccipdPatient.h"
#include "ccipdPatients.h"
#include "ccipdStudy.h"
////////////////////////////////////////////////////////////////////////////////////////////////////

// boost includes
#include <boost/foreach.hpp>             // for loops

using std::cout;
using std::endl;


namespace ccipd
{

EvaluationValueType GetMaxVolume(
	const Patient       & inPatient,
  const MaskObjectType  maskObject,
	const float           threshold,
  const bool            latest)
{

  EvaluationValueType 
    vol = 0,
    maxVol = -1;

  Study::Pointer study;  
  if (latest) 
    study = inPatient.GetLatestStudy();
  else
    study = inPatient.GetEarliestStudy();
  
  try
    {
    vol = GetVolume( study->GetMask(maskObject), threshold );
      
    if (maxVol < vol)
      maxVol = vol;
     }
     catch(...)
     {
       std::cout << "couldn't compute volume for study " << study->GetDescription() << " " << maskObject << std::endl;
     }

  return maxVol;
};

EvaluationValueType GetMaxVolume(
	const Patients      & inPatients,
  const MaskObjectType  maskObject,
	const float           threshold,
  const bool            latest   )
{
   EvaluationValueType 
    vol = 0,
    maxVol = -1;
  ccipd::PatientCollectionType patients = inPatients.GetAllPatients();
  BOOST_FOREACH (const auto patient, patients )
    {
    try
      {
      vol = GetMaxVolume( *patient, maskObject, threshold, latest );

      if (maxVol < vol)
        maxVol = vol;
      
      }
      catch(...)
      {
        std::cout << "couldn't compute volume for patient \"" << patient->GetPatientID() << "\"" << std::endl;
      }
    }
  return maxVol;
};

EvaluationValueType GetMinVolume(
	const Patient       & inPatient,
  const MaskObjectType  maskObject,
	const float           threshold,
  const bool            latest)
{

  EvaluationValueType 
    vol = 0,
    minVol = 1e23;

  Study::Pointer study;
  if (latest) 
    study = inPatient.GetLatestStudy();
  else
    study = inPatient.GetEarliestStudy();

  try
    {
    vol = GetVolume( study->GetMask(maskObject), threshold );
      
    if (minVol > vol)
      minVol = vol;
     }
     catch(...)
     {
       std::cout << "couldn't compute volume for study " << study->GetDescription() << " " << maskObject << std::endl;
     }

  return minVol;
};

EvaluationValueType GetMinVolume(
	const Patients      & inPatients,
  const MaskObjectType  maskObject,
	const float           threshold,
  const bool            latest   )
{
  EvaluationValueType 
    vol = 0,
    minVol = 1e23;
    
  ccipd::PatientCollectionType patients = inPatients.GetAllPatients();
  BOOST_FOREACH (const auto patient, patients )
    {
    try
      {
      vol = GetMinVolume( *patient, maskObject, threshold, latest );

      if (minVol > vol)
        minVol = vol;
      
      }
      catch(...)
      {
        std::cout << "couldn't compute volume for patient \"" << patient->GetPatientID() << "\"" << std::endl;
      }
    }

  return minVol;
};

EvaluationValueType GetMedianVolume(
	const ccipd::Patients & inPatients,
	const MaskObjectType    maskObject,
	const float             threshold,
	const bool              latest )
{
  EvaluationValueType 
    vol = 0;

  std::vector<EvaluationValueType> vols;
  ccipd::PatientCollectionType patients = inPatients.GetAllPatients();
  BOOST_FOREACH (const auto patient, patients )
    {
    Study::Pointer study;
    if (latest)
      study = patient->GetLatestStudy();
    else
      study = patient->GetEarliestStudy();

    if (study->GetMask(maskObject)) // masks exists
      {
      try
        { 
        vol = GetVolume( study->GetMask(maskObject), threshold );
        
        vols.push_back(vol);
        }
      catch( ... )
        {
        std::cout << "Couldn't compute volume for patient \"" << patient->GetPatientID() << "\"" << std::endl;
        }
      }
      else
      {
        std::cout << "Couldn't find mask \"" << maskObject << "\"" << " for " << patient->GetPatientID() << std::endl;
      }
    }

  const size_t medianPosition = vols.size() / 2;

  const std::vector< EvaluationValueType >::iterator medianIterator = vols.begin() + medianPosition;
  std::nth_element( vols.begin(), medianIterator, vols.end() );

  return static_cast< EvaluationValueType >( *medianIterator ) ;
};

/**
 * Get min spacing 
 */
VolumeImageType::SpacingType GetMinSpacing(
	const ccipd::Patients & inPatients,
  const bool              latest  )
{
  VolumeImageType::SpacingType 
    minSpacing,
    volSpacing;

  for (unsigned int i = 0; i < VolumeDimension; i++)
    minSpacing[i] = 1e10;   

  ccipd::PatientCollectionType patients = inPatients.GetAllPatients();
  BOOST_FOREACH (const auto patient, patients )
    {
    Study::Pointer study;
    if (latest)
      study = patient->GetLatestStudy();
    else
      study = patient->GetEarliestStudy();

    volSpacing = study->GetVolume()->GetSpacing();

    for (unsigned int i = 0; i < VolumeDimension; i++)
      if (minSpacing[i] > volSpacing[i])
        minSpacing[i] = volSpacing[i]; 
    }


  return minSpacing;
  
};

/**
 * Get min spacing 
 */
VolumeImageType::SpacingType GetMaxSpacing(
	const ccipd::Patients & inPatients,
  const bool              latest  )
{
  VolumeImageType::SpacingType 
    maxSpacing,
    volSpacing;

  for (unsigned int i = 0; i < VolumeDimension; i++)
    maxSpacing[i] = 0;   

  ccipd::PatientCollectionType patients = inPatients.GetAllPatients();
  BOOST_FOREACH (const auto patient, patients )
    {
    Study::Pointer study;
    if (latest)
      study = patient->GetLatestStudy();
    else
      study = patient->GetEarliestStudy();

    volSpacing = study->GetVolume()->GetSpacing();

    for (unsigned int i = 0; i < VolumeDimension; i++)
      if (maxSpacing[i] < volSpacing[i])
        maxSpacing[i] = volSpacing[i]; 
    }


  return maxSpacing;
  
};


} //namespace ccipd} //namespace ccipd
