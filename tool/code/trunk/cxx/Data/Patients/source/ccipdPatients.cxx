


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdPatients.h"
#include "ccipdPatient.h" // our collection
#include "ccipdStudy.h"   // for iterator access
#include "ccipdHashDataCollection.h" // for hashing collections
#include "ccipdCloneableFactory.hxx"
#include "ccipdFeatureCollection.h" // for features
#include "ccipdFeatureTypes.h"

// boost includes
#include <boost/functional/hash.hpp>     // for hashing
#include <boost/foreach.hpp>             // for loops
#include <boost/property_tree/ptree.hpp> // for xml trees

// std includes
#include <unordered_set> // for a collection of unique patient id's
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  
//////////////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< Patients >;

template
class Copyable< Patients >;

template
class Cloneable< Patients >;

template
class CloneableFactory< Patients >;
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
class Patients::SecretData
{
public: // public to the Patient class

  /* Constructor */
  SecretData() {}

  /* Variables */
  PatientCollectionType m_AllPatients;

}; // Patient::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

Patients::Patients() :
ccipdNewPIMPLMacro
{
}

Patients::~Patients()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// XML functions
bool Patients::LoadFromXML( const XMLNode & patientsRoot )
{
  
  bool anythingLoaded = false;
  
  // go through each child with the <patient> tag
  const auto patientNodes = GetXMLChildNodes( patientsRoot, "patient" );
  BOOST_FOREACH( const XMLPointer & patientNode, patientNodes )
    {

    // add in a new study using the study's xml node recursively
    const bool returnNULLOnError = true;
    const PatientPointer newPatient =
      Patient::New( *patientNode, returnNULLOnError );
    
    if ( newPatient )
      {
      std::cout << "Done reading patient " << newPatient->GetPatientID() << std::endl;
      anythingLoaded = true;
      this->AddPatient( newPatient );
      } // newPatient
    
    } // for each patient
    
  return anythingLoaded;    

} // LoadPatientFromXML
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patients::OutputToStream( std::ostream & stream ) const
{
  
  // display the patients
  const auto childPatients = this->GetAllPatients();
  BOOST_FOREACH ( const auto patient, childPatients )
    {

    stream << endl << "<patient>" << endl;
    stream << *patient;
    stream << "</patient>" << endl;

    } // for each study

} // OutputToStream
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::operator==( const Hashable & other ) const
{

  // cast it to our type
  const Patients & otherPatients =
    dynamic_cast< const Patients & >( other );

  // we've compared to ourself!
  if ( &otherPatients == this )
    return true;
  
  // check the variables

  const PatientCollectionType
    ourPatientsCollection   = this->        GetAllPatients(),
    otherPatientsCollection = otherPatients.GetAllPatients();

  if ( ourPatientsCollection.size() != otherPatientsCollection.size() )
    return false;

  BOOST_FOREACH( const auto ourPatient, otherPatientsCollection )
    {
    
    // does the current patient exist in other?
    if ( otherPatientsCollection.find( ourPatient ) == otherPatientsCollection.cend() )
      {
      // nope
      return false;
      }

    } // for each study

  // if we've gotten here, we've checked all the variables
  return true;

} // operator==
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType Patients::ReHash() const
{
  
  // shortcuts
  using boost::hash_combine;
  
  // initialize our hasher
  const HashValueType seed = 0;
  HashValueType hashValue = seed;

  // extract our patients
  const PatientCollectionType patients = this->GetAllPatients();
  hash_combine( hashValue, patients.size() );
  
  // hash our patients
  hash_combine( hashValue, GetHashValue( patients ) );
  
  // output
  return hashValue;

} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patients::RemovePatient ( const PatientPointer patientToRemove )
{

  // go through each patient
  PatientCollectionType & patients = this->m_SecretData->m_AllPatients;

  // search for the given patient
  const auto foundIter = patients.find( patientToRemove );

  // if we found one, remove it
  if ( foundIter != patients.cend() )
    patients.erase( foundIter );

} // RemovePatient
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patients::RemovePatients( const PatientCollectionType & patients )
{

  // go through each patient
  BOOST_FOREACH ( const PatientPointer patient, patients )
    {

    this->RemovePatient( patient );

    } // for each patient

} // RemovePatients
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patients::MergeDuplicates()
{

  // to store the unique patient ID's in this dataset
  PatientIDCollectionType patientIDs;

  // first, we extract all the patient ID's
  const PatientCollectionType patients = this->GetAllPatients();
  BOOST_FOREACH ( const PatientPointer patient, patients )
    {
    patientIDs.insert( patient->GetPatientID() );
    }

  // then, we go through each unique ID
  BOOST_FOREACH ( const PatientIDType & patientID, patientIDs )
    {

    // extract all the patients with the given ID
    const PatientCollectionType patientsWithCurrentID =
      this->GetPatientsWithID( patientID );

    // prepare a patient to contain the merged result
    const PatientPointer mergedPatient = Patient::New();
    mergedPatient->SetPatientID( patientID );

    // go through each patient with the ID
    BOOST_FOREACH ( const PatientPointer patient, patientsWithCurrentID )
      {

      // add in the studies of the current patient to the merged patient
      mergedPatient->AddStudies( patient->GetAllStudies() );

      } // for each patient with current ID

    // now, we will replace the set of studies with the merged study
    this->RemovePatients( patientsWithCurrentID );
    this->AddPatient    ( mergedPatient         );

    } // for each patient id

} // MergeDuplicates
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectCollectionType Patients::GetAllMaskObjects() const
{

  // initialize
  MaskObjectCollectionType allMaskObjects;

  // go through each patient
  const PatientCollectionType patients = this->GetAllPatients();
  BOOST_FOREACH ( const PatientPointer patient, patients )
    {

    // extract our current mask objects
    const MaskObjectCollectionType
      currentMaskObjects = patient->GetAllMaskObjects();

    // add it to our output
    allMaskObjects.insert( currentMaskObjects.cbegin(), currentMaskObjects.cend() );

    } // for each study

  return allMaskObjects;

} // GetAllMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PatientPointer Patients::GetOnePatient() const
{

  // extract our patients
  const PatientCollectionType patients = this->GetAllPatients();

  if ( patients.empty() )
    return PatientPointer();
  else
    // just return the first patient in the collection
    return *( patients.cbegin() );

} // GetOnePatient
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyPointer Patients::GetOneStudy() const
{

  // extract our studies
  const auto studies = this->GetAllStudies();

  if ( studies.empty() )
    return StudyPointer();
  else
    // just return the first study in the collection
    return *( studies.cbegin() );

} // GetOnePatient
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patients::AddPatient( const PatientPointer patient )
{

  // append the new study
  this->m_SecretData->m_AllPatients.insert( patient );

  // our hash value has changed
  this->ClearHash();

} // AddStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patients::AddPatients( const PatientCollectionType & patients )
{

  BOOST_FOREACH ( const PatientPointer patient, patients )
    {

    this->AddPatient( patient );

    } // for each patient

} // AddStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patients::GetAllStudies() const
{

  // prepare our output
  StudyCollectionType allStudies;

  // extract our patients
  const auto allPatients = this->GetAllPatients();

  // go through each patient
  BOOST_FOREACH( const auto patient, allPatients )
    {

    allStudies.insert( patient->cbegin(), patient->cend() );

    } // for each patient

  return allStudies;

} // GetAllStudies
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PatientCollectionType Patients::GetPatientsWithID(
  const PatientIDType & patientID ) const
{
  
  // prepare our output
  PatientCollectionType patientsWithID;

  // extract our patients
  const PatientCollectionType allPatients = this->GetAllPatients();

  // go through each patient
  BOOST_FOREACH( const PatientPointer patient, allPatients )
    {

    if ( patient->GetPatientID().compare( patientID ) == 0 )
      patientsWithID.insert( patient );

    } // for each patient

  return patientsWithID;

} // GetPatientsWithID
//////////////////////////////////////////////////////////////////////////////////////////

 /*
 * Return all patients with the given list of IDS 
 */
PatientCollectionType Patients::GetPatientsWithIDs( 
  const PatientIDCollectionType & patientIDs ) const
{
  // prepare our output
  PatientCollectionType patientsWithID;

  // go through each patient
  BOOST_FOREACH( const PatientIDType patientID, patientIDs )
    {
      PatientPointer patient = this->GetPatientWithID(patientID);
      if (patient)
        patientsWithID.insert( patient );
    } // for each patient

  return patientsWithID;

};


//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasPatientsWithID(
  const PatientIDType & patientID ) const
{
  return !( this->GetPatientsWithID( patientID ).empty() );
} // HasPatientsWithID
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasPatientsWithMaskObject(
  const MaskObjectType & maskObject ) const
{
  return !( this->GetPatientsWithMaskObject( maskObject ).empty() );
} // HasPatientsWithMaskObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasPatientsWithMaskObjects(
  const MaskObjectCollectionType & maskObjects ) const
{
  return !( this->GetPatientsWithMaskObjects( maskObjects ).empty() );
} // HasPatientsWithMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasStudiesWithMaskObject(
  const MaskObjectType & maskObject ) const
{
  return this->HasPatientsWithMaskObject( maskObject );
} // HasStudiesWithMaskObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasStudiesWithMaskObjects(
  const MaskObjectCollectionType & maskObjects ) const
{
  return this->HasPatientsWithMaskObjects( maskObjects );
} // HasStudiesWithMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PatientPointer Patients::GetPatientWithID (
  const PatientIDType & patientID ) const
{

  // extract our patients
  const PatientCollectionType allPatients = this->GetAllPatients();

  // go through each patient
  BOOST_FOREACH( const PatientPointer patient, allPatients )
    {

    if ( patient->GetPatientID().compare( patientID ) == 0 )
      return patient;

    } // for each patient

  // just return an empty pointer
  return PatientPointer();

} // GetPatientWithID
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::size_t Patients::GetNumberOfPatients() const
{

  return this->GetAllPatients().size();

} // GetNumberOfPatients
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::size_t Patients::GetNumberOfStudies() const
{

  return this->GetAllStudies().size();

} // GetNumberOfStudies
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PatientCollectionType Patients::GetPatientsWithMaskObject(
  const MaskObjectType & maskObject ) const
{

  PatientCollectionType patients;

  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

    if ( patient->HasStudiesWithMaskObject( maskObject ) )
      patients.insert( patient );

    } // for each patient

  return patients;

} // GetPatientsWithMaskObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PatientCollectionType Patients::GetPatientsWithMaskObjects(
  const MaskObjectCollectionType & maskObjects ) const
{

  PatientCollectionType patients;

  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

    if ( patient->HasStudiesWithMaskObjects( maskObjects ) )
      patients.insert( patient );

    } // for each patient

  return patients;

} // GetPatientsWithMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patients::GetStudiesWithMaskObject(
  const MaskObjectType & maskObject ) const
{

  // prepare our output
  StudyCollectionType studiesWithMaskObject;

  // extract the patients with the given mask
  PatientCollectionType patients = this->GetPatientsWithMaskObject( maskObject );

  // go through each extract patient
  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

    // extract the studies with the given mask
    const StudyCollectionType currentStudiesWithMaskObject =
      patient->GetStudiesWithMaskObject( maskObject );

    // insert it into our output
    studiesWithMaskObject.insert(
      currentStudiesWithMaskObject.cbegin(),
      currentStudiesWithMaskObject.cend() );

    } // for each study

  return studiesWithMaskObject;

} // GetStudiesWithMaskObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patients::GetStudiesWithMaskObjects(
  const MaskObjectCollectionType & maskObjects ) const
{

  // prepare our output
  StudyCollectionType studiesWithMaskObject;

  // extract the patients with the given mask
  PatientCollectionType patients = this->GetPatientsWithMaskObjects( maskObjects );

  // go through each extract patient
  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

    // extract the studies with the given mask
    const StudyCollectionType currentStudiesWithMaskObject =
      patient->GetStudiesWithMaskObjects( maskObjects );

    // insert it into our output
    studiesWithMaskObject.insert(
      currentStudiesWithMaskObject.cbegin(),
      currentStudiesWithMaskObject.cend() );

    } // for each study

  return studiesWithMaskObject;

} // GetStudiesWithMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PatientCollectionType Patients::GetPatientsWithModality(
  const ModalityType & modality ) const
{

  PatientCollectionType patients;

  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      if ( patient->HasStudiesWithModality( modality ) )
        patients.insert( patient );

    } // for each study

  return patients;

} // GetPatientsWithModality
//////////////////////////////////////////////////////////////////////////////////////////



#ifdef Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////
PatientCollectionType Patients::GetPatientsWithDate(
  const DateType & date ) const
{
  
  PatientCollectionType patients;

  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      if ( patient->HasStudiesWithDate( date ) )
        patients.insert( patient );

    } // for each study

  return patients;

} // GetPatientsWithDate
//////////////////////////////////////////////////////////////////////////////////////////
#endif // Boost_DATE_TIME_FOUND


//////////////////////////////////////////////////////////////////////////////////////////
PatientCollectionType Patients::GetPatientsWithVolume() const
{
  
  PatientCollectionType patients;

  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      if ( patient->HasStudiesWithVolume() )
        patients.insert( patient );

    } // for each study

  return patients;

} // GetPatientsWithVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PatientCollectionType Patients::GetPatientsWithWarpField() const
{
  
  PatientCollectionType patients;

  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      if ( patient->HasStudiesWithWarpField() )
        patients.insert( patient );

    } // for each study

  return patients;

} // GetPatientsWithVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patients::GetStudiesWithWarpField() const
{

  // prepare our output
  StudyCollectionType studiesWithWarpField;

  // extract the patients with the given mask
  PatientCollectionType patients = this->GetPatientsWithWarpField( );

  // go through each extract patient
  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

    // extract the studies with the given mask
    const StudyCollectionType currentStudiesWithWarpField =
      patient->GetStudiesWithWarpField( );

    // insert it into our output
    studiesWithWarpField.insert(
      currentStudiesWithWarpField.cbegin(),
      currentStudiesWithWarpField.cend() );

    } // for each study

  return studiesWithWarpField;

} // GetStudiesWithWarpField
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasPatientsWithModality( const ModalityType & modality ) const
{
  return !( this->GetPatientsWithModality( modality ).empty() );
} // HasPatientsWithModality
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasPatientsWithDate( const DateType & date ) const
{
  return !( this->GetPatientsWithDate( date ).empty() );
} // HasPatientsWithDate
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasPatientsWithVolume() const
{
  return !( this->GetPatientsWithVolume().empty() );
} // HasPatientsWithVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patients::HasPatientsWithWarpField() const
{
  return !( this->GetPatientsWithWarpField().empty() );
} // HasPatientsWithVolume
//////////////////////////////////////////////////////////////////////////////////////////

 
//////////////////////////////////////////////////////////////////////////////////////////
void Patients::EraseMask(const MaskObjectType      & objectName)
{

  // go through each extract patient
  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      patient->EraseMask(objectName);
     
    } // for each study

};//EraseMask
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
void Patients::EraseMasks( const MaskObjectCollectionType & maskObjects )
{

  // go through each extract patient
  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      patient->EraseMasks(maskObjects);
     
    } // for each study


};//EraseMasks
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
void Patients::EraseVolume( )
{

 // go through each extract patient
  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      patient->EraseVolume();
     
    } // for each study

}; //EraseVolume
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
void Patients::EraseFeature( const FeatureIDConstPointer featureID)
{

  // go through each extract patient
  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      patient->EraseFeature(featureID);
     
    } // for each study


};//EraseFeature
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
void Patients::EraseFeatures( const FeatureIDConstCollection featureIDs)
{

  // go through each extract patient
  BOOST_FOREACH( const PatientPointer patient, this->GetAllPatients() )
    {

      patient->EraseFeatures(featureIDs);
     
    } // for each study

};//EraseFeatures
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
// iterating functions
Patients::iterator Patients::begin()
{
  return this->m_SecretData->m_AllPatients.begin();
}
Patients::const_iterator Patients::begin() const
{
  return this->m_SecretData->m_AllPatients.cbegin();
}
Patients::const_iterator Patients::cbegin() const
{
  return this->m_SecretData->m_AllPatients.cbegin();
}
Patients::iterator Patients::end()
{
  return this->m_SecretData->m_AllPatients.end();
}
Patients::const_iterator Patients::end() const
{
  return this->m_SecretData->m_AllPatients.cend();
}
Patients::const_iterator Patients::cend() const
{
  return this->m_SecretData->m_AllPatients.cend();
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
void Patients::CopyParametersInternal( const Self & other )
{
 
  // go through each patient
  const PatientCollectionType patients = other.GetAllPatients();
  BOOST_FOREACH ( const PatientPointer patient, patients  )
    {
      this->AddPatient(Patient::New(*patient));
    } // for each study

}

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// set/get functions
ccipdGetConstImplementMacro( Patients, AllPatients, PatientCollectionType )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
