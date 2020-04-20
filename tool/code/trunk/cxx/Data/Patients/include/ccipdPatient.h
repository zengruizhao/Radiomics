#ifndef __ccipdPatient_h
#define __ccipdPatient_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPatientTypes.h" // for typedef's
#include "ccipdMacros.h"       // for easy coding
#include "ccipdHashable.h"     // we want to be hashable
#include "ccipdDataPatientsOptions.h"  // do we have the date/time library?
#include "ccipdXMLBaseFactory.h"
#include "ccipdEnableSharedFromThis.hxx"
#include "ccipdCloneableFactory.h"
#include "ccipdFeatureCollection.h" // for features
#include "ccipdFeatureID.h" // for features
#include "ccipdFeatureTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////
// itk includes

#include "ccipdDisableWarningsMacro.h"
  #include "itkImage.h"
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



/**
 * \brief Store a patient with multiple studies.
 * 
 * \see
 *   - ccipd::Study
 *   - ccipd::Patients
 * 
 * \date    2012-02-19
 * \author  Rob Toth
 * \ingroup PatientData
 */
class Patient :
  public Hashable,
  public XMLBase,
  public XMLBaseFactory< Patient >,
  public Copyable< Patient >,
  public CloneableFactory< Patient >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( Patient )
public:
  typedef XMLBaseFactory< Patient > SuperclassFactory;
  ccipdFactoryTypedefsMacro

  typedef CloneableFactory< Self >         SuperclassCloneableFactory;
  using SuperclassCloneableFactory::New;                         
  using SuperclassCloneableFactory::ConstNew;
//////////////////////////////////////////////////////////////////////////////////////////


public:
//////////////////////////////////////////////////////////////////////////////////////////
  // for iterating through studies
  typedef StudyCollectionType::iterator       iterator;
  typedef StudyCollectionType::const_iterator const_iterator;

  // point to the start of the study collection
  iterator        begin();
  const_iterator  begin() const;
  const_iterator cbegin() const;

  // point to the end of the study collection
  iterator        end  ();
  const_iterator  end  () const;
  const_iterator cend  () const;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * For loading a patient from an XML structure.
   * 
   * <patient_id>Patient ID</patient_id>
   * 
   * <study>
   *   ... (see ccipdStudy.h)
   * </study>
   * 
   * <study>
   *   ... (another study)
   * </study>
   * 
   * Note that this will not delete any existing
   * studies, but rather append studies.
   */
  virtual bool LoadFromXML( const XMLNode & patientRoot );
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * This will set/get the current patient ID.
   */
  ccipdSetByrefDeclareMacro( PatientID, PatientIDType )
  ccipdGetConstDeclareMacro( PatientID, PatientIDType )

  /**
   * This will add a new study to the current collection.
   */
  void AddStudy  ( const StudyPointer          study   );
  void AddStudies( const StudyCollectionType & studies );
 

  /**
   * This will extract the studies for this patient.
   */
  ccipdGetConstDeclareMacro( AllStudies     , StudyCollectionType )
  ccipdGetConstDeclareMacro( NumberOfStudies, std::size_t         )

  /**
   * This will remove existant study from the current collection.
   */
  void RemoveStudy  ( const StudyPointer          study   );
  void RemoveStudies( const StudyCollectionType & studies );

  /**
   * This will extract all mask objects that at least one study has.
   */
  ccipdGetConstDeclareMacro( AllMaskObjects, MaskObjectCollectionType )

  /**
   * This will extract only those studies which have a given mask.
   */
  StudyCollectionType GetStudiesWithMaskObject(
    const MaskObjectType & maskObject ) const;
  bool HasStudiesWithMaskObject( const MaskObjectType & maskObject ) const;

  /**
   * This will extract only those studies with all the given masks.
   */
  StudyCollectionType GetStudiesWithMaskObjects(
    const MaskObjectCollectionType & maskObjects ) const;
  bool HasStudiesWithMaskObjects( const MaskObjectCollectionType & maskObjects ) const;

  /**
   * This will extract a single (semi-random) study
   * for the patient. This is useful if you know the
   * patient only has one study.
   */
  ccipdGetConstDeclareMacro( OneStudy, StudyPointer )

  /**
   * These functions will extract a set of studies
   * which have the specified parameters.
   * 
   * TODO: After a certain date?
   * 
   */
  StudyCollectionType GetStudiesWithDescription( const DescriptionType & description ) const;
  StudyCollectionType GetStudiesWithModality( const ModalityType & modality ) const;
  StudyCollectionType GetStudiesWithAnnotator( const AnnotatorType & annotator ) const;

  ccipdGetConstDeclareMacro( StudiesWithVolume,           StudyCollectionType )
  ccipdGetConstDeclareMacro( StudiesWithWarpField,        StudyCollectionType )
  ccipdGetConstDeclareMacro( StudiesWithAnyTransform,     StudyCollectionType )   
  ccipdGetConstDeclareMacro( StudiesWithAffineTransform,  StudyCollectionType ) 
  ccipdGetConstDeclareMacro( StudiesWithBSplineTransform, StudyCollectionType )
  
#ifdef Boost_DATE_TIME_FOUND
  /**
   * Select studies which all appeared on the same date.
   */
  StudyCollectionType GetStudiesWithDate( const DateType & date ) const;
  
  /**
   * Get the study with the earliest/latest date.
   * 
   * If a study doesn't have its date set,
   * it is ignored.
   * 
   * If no studies have a date set, one study
   * will be returned, but which study that is.
   * is undefined.
   */
  ccipdGetConstDeclareMacro( EarliestStudy, StudyPointer )
  ccipdGetConstDeclareMacro( LatestStudy  , StudyPointer )
#endif // Boost_DATE_TIME_FOUND
  

  /**
   * Are there any studies which have a given parameter?
   */
  bool HasStudiesWithModality( const ModalityType & modality ) const;
  bool HasStudiesWithAnnotator( const AnnotatorType & annotator ) const;
#ifdef Boost_DATE_TIME_FOUND
  bool HasStudiesWithDate    ( const DateType     & date     ) const;
#endif // Boost_DATE_TIME_FOUND
  bool HasStudiesWithVolume   ()                                const;
  bool HasStudiesWithWarpField()                                const;

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
   * GetBoundingVolume
   * Goes through all studies within the patient to identify the volume that contains all of them
   */
  VolumeImageConstPointer GetBoundingVolume(const ccipd::VolumeImageType::SpacingType spacing);

  /**
   * GetBoundingBox
   * Goes through all studies within the patient to identify the volume that contains all them given masks
   */
  VolumeImageConstPointer GetMaskBoundingVolume( 
    const MaskObjectType                      & objectName, 
    const ccipd::VolumeImageType::SpacingType spacing,
    const StudyPointer                        studyAxial,
    const unsigned int                        padding = 0);

  /**
   * Comparison with another Patient.
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
#endif // __ccipdPatient_h
