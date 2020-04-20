#ifndef __ccipdPatients_h
#define __ccipdPatients_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMacros.h"       // for easy coding
#include "ccipdHashable.h"     // we want to be hashable
#include "ccipdPatientTypes.h" // our types
#include "ccipdXMLBaseFactory.h"
#include "ccipdDataPatientsOptions.h"  // do we have the date/time library?
#include "ccipdEnableSharedFromThis.hxx"
#include "ccipdCloneableFactory.h"
#include "ccipdFeatureCollection.h" // for features
#include "ccipdFeatureID.h" // for features
#include "ccipdFeatureTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Store multiple patients
 * 
 * \see
 *   - ccipd::Study
 *   - ccipd::Patient
 * 
 * \date    2012-02-19
 * \author  Rob Toth
 * \ingroup PatientData
 */
class Patients :
  public Hashable,
  public XMLBase,
  public XMLBaseFactory< Patients >,
  public Copyable< Patients >,
  public CloneableFactory< Patients >

{
  
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( Patients )
public:
  typedef XMLBaseFactory< Patients > SuperclassFactory;
  ccipdFactoryTypedefsMacro

  typedef CloneableFactory< Self >         SuperclassCloneableFactory;
  using SuperclassCloneableFactory::New;                         
  using SuperclassCloneableFactory::ConstNew;

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  // for iterating through patients
  typedef PatientCollectionType::iterator       iterator;
  typedef PatientCollectionType::const_iterator const_iterator;

  // point to the start of the patient collection
  iterator        begin();
  const_iterator  begin() const;
  const_iterator cbegin() const;

  // point to the end of the patient collection
  iterator        end  ();
  const_iterator  end  () const;
  const_iterator cend  () const;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * For loading a set of patients from an XML structure.
   * 
   * <patient>
   *   ... (see ccipdPatient.h)
   * </patient>
   * 
   * <patient>
   *   ... (another patient)
   * </patient>
   * 
   * Note that this will not delete any existing
   * patients, but rather append patients.
   */
  using XMLBase::LoadFromXML;
  virtual bool LoadFromXML( const XMLNode & patientsRoot );
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * Include another patient in our collection.
   */
  void AddPatient ( const PatientPointer          patient  );
  void AddPatients( const PatientCollectionType & patients );

  /**
   * Remove a patient from our collection
   */
  void RemovePatient ( const PatientPointer          patient  );
  void RemovePatients( const PatientCollectionType & patients );

  /**
   * How many patients/studies do we have?
   */
  ccipdGetConstDeclareMacro( NumberOfPatients, std::size_t )
  ccipdGetConstDeclareMacro( NumberOfStudies , std::size_t )

  /**
   * Extract the patients or studies.
   */
  ccipdGetConstDeclareMacro( AllPatients, PatientCollectionType )
  ccipdGetConstDeclareMacro( OnePatient , PatientPointer        )
  ccipdGetConstDeclareMacro( AllStudies , StudyCollectionType   )
  ccipdGetConstDeclareMacro( OneStudy   , StudyPointer          )

  /**
   * Extract a set of patients, or just one patient, with a given ID.
   * 
   * You can extract multiple patients since the ID's aren't required
   * to be unique, or one could change it from a PatientPointer.
   * 
   * However, you can specify to only extract one patient from an ID.
   * 
   * If none exist, this will return a NULL pointer.
   */
  PatientCollectionType GetPatientsWithID( const PatientIDType & patientID ) const;
  PatientPointer        GetPatientWithID ( const PatientIDType & patientID ) const;
  bool                  HasPatientsWithID( const PatientIDType & patientID ) const;

   /*
   * Return all patients with the given list of IDS 
   */
  PatientCollectionType GetPatientsWithIDs( 
    const PatientIDCollectionType & patientIDs ) const;

  /**
   * Extract a set of patients in which at least one study has the given mask(s).
   */
  PatientCollectionType GetPatientsWithMaskObject(
    const MaskObjectType & maskObject ) const;

  PatientCollectionType GetPatientsWithMaskObjects(
    const MaskObjectCollectionType & maskObjects ) const;

  bool HasPatientsWithMaskObject ( const MaskObjectType           & maskObject ) const;
  bool HasPatientsWithMaskObjects( const MaskObjectCollectionType & maskObjects ) const;

  /** 
   * Extract a set of studies with the given mask(s).
   */
  StudyCollectionType GetStudiesWithMaskObject(
    const MaskObjectType & maskObject ) const;

  StudyCollectionType GetStudiesWithMaskObjects(
    const MaskObjectCollectionType & maskObjects ) const;

  bool HasStudiesWithMaskObject ( const MaskObjectType           & maskObject ) const;
  bool HasStudiesWithMaskObjects( const MaskObjectCollectionType & maskObjects ) const;

  /**
   * These functions will extract a set of patients
   * which have the specified parameters.
   * 
   * TODO: After a certain date?
   * TODO: Description search?
   * 
   */
  PatientCollectionType GetPatientsWithModality( const ModalityType & modality   ) const;
#ifdef Boost_DATE_TIME_FOUND
  PatientCollectionType GetPatientsWithDate    ( const DateType     & date     ) const;
#endif // Boost_DATE_TIME_FOUND
  ccipdGetConstDeclareMacro( PatientsWithVolume,    PatientCollectionType )
  ccipdGetConstDeclareMacro( PatientsWithWarpField, PatientCollectionType )
  ccipdGetConstDeclareMacro( StudiesWithWarpField,  StudyCollectionType   )

  /**
   * This will extract all mask objects that at least one study has.
   */
  ccipdGetConstDeclareMacro( AllMaskObjects, MaskObjectCollectionType )

  /**
   * Are there any patients which have a given parameter?
   */
  bool HasPatientsWithModality( const ModalityType & modality )  const;
  bool HasPatientsWithDate    ( const DateType     & date     )  const;
  bool HasPatientsWithVolume   ()                                const;
  bool HasPatientsWithWarpField()                                const;
  
  /**
   * This will remove a mask for the given object name,
   * also will remove the mask file name.
   */
  void EraseMask( const MaskObjectType      & objectName = "" );

  /**
   * This will remove a mask for the given object name,
   * also will remove the mask file name.
   */
  void EraseMasks( const MaskObjectCollectionType & maskObjects );

  /**
   * This will remove a volumes for the given object name,
   * also will remove the file name.
   */
  void EraseVolume( );

  /**
   * This will remove a feature from the feature collection
   */
  void EraseFeature( const FeatureIDConstPointer featureID );

  /**
   * This will remove a feature from the feature collection
   */
  void EraseFeatures( const FeatureIDConstCollection featureIDs );


  /**
   * This function will merge any patients that
   * have the same PatientID.
   */
  void MergeDuplicates();

  /**
   * Comparison with another Patients.
   */
  virtual bool operator==( const Hashable & other ) const;

  /**
   * For outputting this class.
   */
  virtual void OutputToStream( std::ostream & stream ) const;
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * We want to be hashable.
   */
  virtual HashValueType ReHash() const;
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
virtual void CopyParametersInternal( const Self & other );
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class


} // namespace
#endif // __ccipdPatients_h
