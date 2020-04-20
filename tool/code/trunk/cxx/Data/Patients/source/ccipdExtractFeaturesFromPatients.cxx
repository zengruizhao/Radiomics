#include "ccipdExtractFeaturesFromPatients.h" 
#include "ccipdFeatureCollection.h" // for features
#include "ccipdStudy.h"
#include "ccipdPatient.h"

// boost includes
#include <boost/foreach.hpp>             // for loops


#include "ccipdDisableWarningsMacro.h"
  #include <itkComposeImageFilter.h> // this itk function does what we need
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{

//////////////////////////////////////////////////////////////////////////////////////////
void ExtractFeaturesFromPatients(
  Patients                       & toExtractFromPatients,
  const FeatureIDConstCollection whichFeatures)
{
  std::cout << "Extracting features in the cohort" << std::endl; 
  const PatientCollectionType patients = toExtractFromPatients.GetAllPatients();

  BOOST_FOREACH( const auto patient, patients)
    ExtractFeaturesFromPatient(*patient, whichFeatures);

  std::cout << " Done extracting features in the cohort" << std::endl; 

}; // ExtractFeaturesFromPatients
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Extract the features from the studies of a patient
 * 
 * \author  Mirabela Rusu
 * \ingroup Data
 */
void ExtractFeaturesFromPatient(
  Patient                       & patient,
  const FeatureIDConstCollection whichFeatures)
{
  const StudyCollectionType studies = patient.GetAllStudies();

  BOOST_FOREACH( const auto study, studies )
      ExtractFeaturesFromStudy(*study, whichFeatures );
};
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
  const FeatureIDConstCollection whichFeatures)
{
  FeatureCollectionPointer featureCollection = study.GetFeatureCollection();  
  featureCollection->SetVolume(study.GetVolume()); //rehash it

  //get new features, if already computed, it will not recompute them
  featureCollection->GetFeatures( whichFeatures );
  study.SetFeatureCollection( featureCollection );
  std::cout << " Done extracting from study: " << study.GetDescription() <<
    std::endl;
};
//////////////////////////////////////////////////////////////////////////////////////////



}

