


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPatient.h"
#include "ccipdPatientTypes.h"       // for typedef's
#include "ccipdStudy.h"              // our collection of studies
#include "ccipdDataPatientsOptions.h"        // do we have the date/time library?
#include "ccipdHashDataCollection.h" // for hashing our studies
#include "ccipdCloneableFactory.hxx" // for cloneing 
#include "ccipdBoundingBox.h"


#include "ccipdDisableWarningsMacro.h"

// boost includes
#include <boost/foreach.hpp>             // for loops
#include <boost/property_tree/ptree.hpp> // for xml trees
#include <boost/functional/hash.hpp>     // for hashing

#ifdef Boost_DATE_TIME_FOUND
  #include <boost/date_time/gregorian/gregorian.hpp> // for dates
  #include <boost/date_time/special_defs.hpp>        // for invalid dates
#endif
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


//////////////////////////////////////////////////////////////////////////////////////////
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< Patient >;

template
class Copyable< Patient >;

template
class Cloneable< Patient >;

template
class CloneableFactory< Patient >;
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class Patient::SecretData
{
public: // public to the Patient class

  /* Constructor */
  SecretData() { }

  /* Variables */
  PatientIDType       m_PatientID;
  StudyCollectionType m_AllStudies;

}; // Patient::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

Patient::Patient() :
ccipdNewPIMPLMacro
{
}

Patient::~Patient()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patient::operator==( const Hashable & other ) const
{

  // cast it to our type
  const Patient & otherPatient =
    dynamic_cast< const Patient & >( other );

  // we've compared to ourself!
  if ( &otherPatient == this )
    return true;
  
  // check the variables

  if ( this->GetPatientID() != otherPatient.GetPatientID() )
    return false;

  const StudyCollectionType
    ourStudies   = this->       GetAllStudies(),
    otherStudies = otherPatient.GetAllStudies();

  if ( ourStudies.size() != otherStudies.size() )
    return false;

  BOOST_FOREACH( const auto ourStudy, ourStudies )
    {
    
    // does the current study exist in other?
    if ( otherStudies.find( ourStudy ) == otherStudies.cend() )
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
HashValueType Patient::ReHash() const
{
  
  // shortcuts
  using boost::hash_combine;
  
  // initialize our hasher
  const HashValueType seed = 0;
  HashValueType hashValue = seed;

  // hash all the variables of the object
  hash_combine( hashValue, this->GetPatientID() );
  
  // extract our studies
  const StudyCollectionType studies = this->GetAllStudies();
  hash_combine( hashValue, studies.size() );
  
  // hash our studies
  hash_combine( hashValue, GetHashValue( studies ) );

  // output
  return hashValue;

} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patient::GetStudiesWithMaskObject(
  const MaskObjectType & maskObject ) const
{

  // prepare our output
  StudyCollectionType outputStudies;

  // go through each study
  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {

    // does the study have the desired mask?
    const bool hasMask = study->HasMask( maskObject );

    // add it to our list
    if ( hasMask )
      outputStudies.insert( study );

    } // for each study

  return outputStudies;

} // GetStudiesWithMaskObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patient::GetStudiesWithMaskObjects(
  const MaskObjectCollectionType & maskObjects ) const
{

  // prepare our output
  StudyCollectionType outputStudies;

  // go through each study
  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {

    // does the study have the desired mask?
    const bool hasMasks = study->HasMasks( maskObjects );

    // add it to our list
    if ( hasMasks )
      outputStudies.insert( study );

    } // for each study

  return outputStudies;

} // GetStudiesWithMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectCollectionType Patient::GetAllMaskObjects() const
{

  // initialize
  MaskObjectCollectionType allMaskObjects;

  // go through each study
  const StudyCollectionType studies = this->GetAllStudies();
  BOOST_FOREACH ( const StudyPointer study, studies )
    {

    // extract our current mask objects
    const auto currentMaskObjects = study->GetMaskObjects();

    // add it to our output
    allMaskObjects.insert( currentMaskObjects.cbegin(), currentMaskObjects.cend() );

    } // for each study

  return allMaskObjects;

} // GetAllMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::AddStudy( const StudyPointer study )
{

  // append the new study
  this->m_SecretData->m_AllStudies.insert( study );

  // our hash value has changed
  this->ClearHash();

} // AddStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::AddStudies( const StudyCollectionType & studies )
{

  BOOST_FOREACH ( const StudyPointer study, studies )
    {
    this->AddStudy( study );
    }

} // AddStudies
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::RemoveStudy( const StudyPointer studyToRemove )
{
  // go through each patient
  StudyCollectionType & studies = this->m_SecretData->m_AllStudies;

  // search for the given patient
  const auto foundIter = studies.find( studyToRemove );

  // if we found one, remove it
  if ( foundIter != studies.cend() )
    studies.erase( foundIter );

} // RemoveStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::RemoveStudies( const StudyCollectionType & studies )
{

  BOOST_FOREACH ( const StudyPointer study, studies )
    {
    this->RemoveStudy( study );
    }

} // RemoveStudies
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// XML functions
bool Patient::LoadFromXML( const XMLNode & patientRoot )
{

  bool anythingLoaded = false;
  
  // patient id
  string patientIDString;
  const bool loadedPatientID = GetXMLString( patientRoot, "patient_id", patientIDString );
  if ( loadedPatientID )
    {
    anythingLoaded = true;
    this->SetPatientID( patientIDString );
    }

  // go through each child with the <study> tag
  const auto studyNodes = GetXMLChildNodes( patientRoot, "study" );
  BOOST_FOREACH( const XMLPointer & studyNode, studyNodes )
    {

    // add in a new study using the study's xml node recursively
    const bool returnNULLOnError = true;
    const StudyPointer newStudy =
      Study::New( *studyNode, returnNULLOnError );
    
    if ( newStudy )
      {
      anythingLoaded = true;
      this->AddStudy( newStudy );
      } // newStudy
    
    } // for each study

  return anythingLoaded;
  
} // LoadPatientFromXML
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::OutputToStream( std::ostream & stream ) const
{
  
  std::string indent = "  "; 
  
  // display variables
  stream  << indent << "<patient_id>" << this->GetPatientID() << "</patient_id>" << endl;
    
  indent = "    ";

  // display the studies
  const StudyCollectionType studies = this->GetAllStudies();
  BOOST_FOREACH ( const StudyPointer study, studies )
    {
    stream << indent << "<study>" << endl;
    stream << *study;
    stream << indent << "</study>" << endl;

    } // for each study

} // operator<<
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyPointer Patient::GetOneStudy() const
{

  // extract our studies
  const StudyCollectionType studies = this->GetAllStudies();

  if ( studies.empty() )
    return StudyPointer();
  else
    // just return the first study in the collection
    return *( studies.cbegin() );

} // GetOneStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patient::GetStudiesWithModality(
  const ModalityType & modality ) const
{

  // prepare our output
  StudyCollectionType studies;

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {

      // if we have the modality, include the study
      if ( study->GetModality().compare( modality ) == 0 )
        studies.insert( study );

    } // for each study

  return studies;

} // GetStudiesWithModality
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patient::GetStudiesWithAnnotator(
  const AnnotatorType & annotator ) const
{

  // prepare our output
  StudyCollectionType studies;

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {

      // if we have the modality, include the study
      if ( study->GetAnnotator().compare( annotator ) == 0 )
        studies.insert( study );

    } // for each study

  return studies;

} // GetStudiesWithModality
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patient::GetStudiesWithDescription(
	const DescriptionType & description ) const
{

  // prepare our output
  StudyCollectionType studies;

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {

      // if we have the modality, include the study
      if ( study->GetDescription().compare( description ) == 0 )
        studies.insert( study );

    } // for each study

  return studies;

} // GetStudiesWithDescription
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_DATE_TIME_FOUND
StudyCollectionType Patient::GetStudiesWithDate(
  const DateType & date ) const
{

  // prepare our output
  StudyCollectionType studies;

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {

    // get the current study's date
    const DateType currentDate( *( study->GetDate() ) );

    // if we have the date, include the study
    if ( currentDate == date )
      studies.insert( study );

    } // for each study

  return studies;

} // GetStudiesWithDate
#endif // Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_DATE_TIME_FOUND
StudyPointer Patient::GetEarliestStudy() const
{

  // just start out with an invalid date
  using boost::date_time::not_a_date_time;
  DateType earliestDate( not_a_date_time );
  
  // initialize with a "random" study
  StudyPointer earliestStudy = this->GetOneStudy();
  
  // go through each study
  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {
    
    // extract our date
    const bool studyHasDate = study->HasDate();
    const DateType currentDate = studyHasDate ?
      *( study->GetDate() ) :
      DateType( not_a_date_time );
    
    // first, if we haven't set any date yet, set it
    // then, if we have a valid currentDate and 
    // it's less than the current earliest date, set it
    if ( earliestDate.is_not_a_date() ||
      ( studyHasDate  && currentDate < earliestDate ) )
	{
	earliestDate = currentDate;
	earliestStudy = study;
	}

    } // for each study

  return earliestStudy;

} // GetStudiesWithDate
#endif // Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_DATE_TIME_FOUND
StudyPointer Patient::GetLatestStudy() const
{

  // just start out with an invalid date
  using boost::date_time::not_a_date_time;
  DateType latestDate( not_a_date_time );
  
  // initialize with a "random" study
  StudyPointer latestStudy = this->GetOneStudy();
  
  // go through each study
  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {
    
    // extract our date
    const bool studyHasDate = study->HasDate();
    const DateType currentDate = studyHasDate ?
      *( study->GetDate() ) :
      DateType( not_a_date_time );
    
    // first, if we haven't set any date yet, set it
    // then, if we have a valid currentDate and 
    // it's less than the current latest date, set it
    if ( latestDate.is_not_a_date() ||
      ( studyHasDate  && currentDate > latestDate ) )
	{
	latestDate = currentDate;
	latestStudy = study;
	}

    } // for each study

  return latestStudy;

} // GetStudiesWithDate
#endif // Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#define ccipdGetStudiesWithVariableMacro( VariableName )            \
StudyCollectionType                                                 \
Patient::GetStudiesWith##VariableName() const                       \
{                                                                   \
                                                                    \
  StudyCollectionType studies;                                      \
                                                                    \
  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )  \
    {                                                               \
                                                                    \
      /* if we have a non-null object, include it */                \
      if ( study->Has##VariableName() )                             \
        studies.insert( study );                                    \
                                                                    \
    } /* for each study*/                                           \
                                                                    \
  return studies;                                                   \
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// all the implementations of ccipdGetStudiesWithVariableMacro
ccipdGetStudiesWithVariableMacro( Volume           )
ccipdGetStudiesWithVariableMacro( AffineTransform  )
ccipdGetStudiesWithVariableMacro( BSplineTransform )
ccipdGetStudiesWithVariableMacro( WarpField        )
///////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyCollectionType Patient::GetStudiesWithAnyTransform() const
{

  StudyCollectionType studies;

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {

      // if we have any transform types return the study
      if ( study->HasWarpField()|| study->HasAffineTransform() || study->HasBSplineTransform() )
        studies.insert( study );

    } // for each study

  return studies;

} // GetStudiesWithTransform
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patient::HasStudiesWithMaskObject( const MaskObjectType & maskObject ) const
{
  return !( this->GetStudiesWithMaskObject( maskObject ).empty() );
} // HasStudiesWithMaskObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patient::HasStudiesWithMaskObjects(
  const MaskObjectCollectionType & maskObjects ) const
{
  return !( this->GetStudiesWithMaskObjects( maskObjects ).empty() );
} // HasStudiesWithMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patient::HasStudiesWithModality( const ModalityType & modality ) const
{
  return !( this->GetStudiesWithModality( modality ).empty() );
} // HasStudiesWithModality
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patient::HasStudiesWithAnnotator( const AnnotatorType & annotator ) const
{
  return !( this->GetStudiesWithAnnotator( annotator ).empty() );
} // HasStudiesWithModality
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_DATE_TIME_FOUND
bool Patient::HasStudiesWithDate( const DateType & date ) const
{
  return !( this->GetStudiesWithDate( date ).empty() );
} // HasStudiesWithDate
#endif // Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patient::HasStudiesWithVolume() const
{
  return !( this->GetStudiesWithVolume().empty() );
} // HasStudiesWithVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Patient::HasStudiesWithWarpField() const
{
  return !( this->GetStudiesWithWarpField().empty() );
} // HasStudiesWithVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::size_t Patient::GetNumberOfStudies() const
{

  return this->GetAllStudies().size();

} // HasStudiesWithVolume
//////////////////////////////////////////////////////////////////////////////////////////
 


//////////////////////////////////////////////////////////////////////////////////////////
void Patient::EraseMask(const MaskObjectType      & objectName)
{

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {
    study->EraseMask(objectName);
    } // for each study

};//EraseMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::EraseMasks( const MaskObjectCollectionType & maskObjects )
{

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {
    study->EraseMasks(maskObjects);
    } // for each study

};//EraseMasks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::EraseVolume( )
{

 BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {
    study->EraseVolume();
    } // for each study

}; //EraseVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::EraseFeature( const FeatureIDConstPointer featureID)
{

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {
    study->EraseFeature(featureID);
    } // for each study

};//EraseFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Patient::EraseFeatures( const FeatureIDConstCollection featureIDs)
{

  BOOST_FOREACH( const StudyPointer study, this->GetAllStudies() )
    {
    study->EraseFeatures(featureIDs);
    } // for each study

};//EraseFeatures
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * GetBoundingVolume
 * Goes through all studies within the patient to identify the volume that contains all of them
 */
VolumeImageConstPointer Patient::GetBoundingVolume(ccipd::VolumeImageType::SpacingType spacing)
{
  VolumeImageType::Pointer newVol = VolumeImageType::New();

  ccipd::VolumePointType 
      allMinPoint, allMaxPoint;

  bool allPlaneInitialized = false;
  ccipd::VolumeImageType::IndexType index;


  auto allStudies = this->GetAllStudies();
  BOOST_FOREACH( const auto study, allStudies)
    {
    std::cout << study->GetPlane() << std::endl;

    bool initialized = false;
    ccipd::VolumePointType 
      point, minPoint, maxPoint;

    for (unsigned int x=0; x<=1; x++)
      {

      int iDir = 0;
      if (x==0)
        index[iDir] = 0;  
      else
        index[iDir] = study->GetVolume()->GetLargestPossibleRegion().GetSize(iDir);  
      
      for (unsigned int y=0; y<=1; y++)
        {
        iDir = 1;
        if (y==0)
          index[iDir] = 0;  
        else
          index[iDir] = study->GetVolume()->GetLargestPossibleRegion().GetSize(iDir);  
        
        for (unsigned int z=0; z<=1; z++)
          {
          iDir = 2;
          if (z==0)
            index[iDir] = 0;  
          else
            index[iDir] = study->GetVolume()->GetLargestPossibleRegion().GetSize(iDir);  
        
          study->GetVolume()->TransformIndexToPhysicalPoint(index,point);
          //std::cout<< index << ":" << point << std::endl;
  
          if (!initialized)
          {
            initialized = true; 
            minPoint = point;
            maxPoint = point;
          }
          else
          {
            for (unsigned int i=0; i<VolumeDimension; i++)
            {
              if (minPoint[i] > point[i])
                minPoint[i] = point[i];
              if (maxPoint[i] < point[i])
                maxPoint[i] = point[i];                  
            } 
          }//max

          } //z
        } //y
      } //x
      std::cout<< "min :" << minPoint << std::endl;
      std::cout<< "max :" << maxPoint << std::endl;

      if (!allPlaneInitialized)
      {
        allPlaneInitialized = true;
        allMinPoint = minPoint;
        allMaxPoint = maxPoint;
      }
      else
      {
        for (unsigned int i=0; i<VolumeDimension; i++)
        {
          if (allMinPoint[i] > minPoint[i])
            allMinPoint[i] = minPoint[i];
          if (allMaxPoint[i] < maxPoint[i])
            allMaxPoint[i] = maxPoint[i];
        } 
        } // if (!allPlaneInitialized)
    
    } // allstudies
  std::cout << "minAll :" << allMinPoint << std::endl;
  std::cout << "maxAll :" << allMaxPoint << std::endl;

  ccipd::VolumeImageType::SizeType newVolSize;

  for (unsigned int i=0; i<VolumeDimension; i++)
    {
      newVolSize[i] = (allMaxPoint[i] - allMinPoint[i])/spacing[i];
      index[i] = 0;
    }

  std::cout << "Creating volume with size " << newVolSize << std::endl;;

  ccipd::VolumeImageType::RegionType newVolRegion;
  newVolRegion.SetIndex( index );
  newVolRegion.SetSize( newVolSize );

  newVol->SetRegions( newVolRegion );
  newVol->SetOrigin( allMinPoint );
  //newVol->SetDirection( this->GetVolume()->GetDirection() );// should be identity
  newVol->SetSpacing( spacing );
  newVol->Allocate();
  newVol->FillBuffer(0);

  return newVol.GetPointer();
}; // GetBoundingVolume

/**
 * GetBoundingBox
 * Goes through all studies within the patient to identify the volume that contains all them given masks
 */
VolumeImageConstPointer Patient::GetMaskBoundingVolume( 
  const MaskObjectType                      & objectName, 
  const ccipd::VolumeImageType::SpacingType spacing,
  const StudyPointer                        studyAxial,
  const unsigned int                        padding)
{ 
  ccipd::VolumeImageType::RegionType 
    regionAxial = ccipd::GetBoundingBox(studyAxial->GetMask(objectName), 0);
 
  ccipd::VolumeImageType::SizeType    newVolSize; 
  ccipd::VolumeImageType::IndexType   index = regionAxial.GetIndex();
  ccipd::VolumeImageType::SpacingType axialSpacing = studyAxial->GetVolume()->GetSpacing();

  for (unsigned int i=0; i<VolumeDimension; i++)
    {
    newVolSize[i] = (regionAxial.GetSize()[i]*studyAxial->GetVolume()->GetSpacing()[i]+2*padding) /spacing[i];
    index[i] = index[i] - int(padding/axialSpacing[i]); // index might be negative but that is ok for gettting physical location
    }


  std::cout << "Creating volume with size " <<  std::endl;;

  VolumeImageType::Pointer newVol = VolumeImageType::New();

  //define region of interest  
  ccipd::VolumeImageType::RegionType newVolRegion;
  newVolRegion.SetIndex( index );
  newVolRegion.SetSize( newVolSize );
  newVol->SetRegions( newVolRegion );

  // get origin 
  ccipd::VolumePointType point;
  studyAxial->GetVolume()->TransformIndexToPhysicalPoint(index,point);
  newVol->SetOrigin( point ); 

  //direction & spacing
  newVol->SetDirection( studyAxial->GetVolume()->GetDirection() );// should be identity
  newVol->SetSpacing( spacing );

  //now allocate
  newVol->Allocate();
  newVol->FillBuffer(0);

  return newVol.GetPointer();

}; //GetMaskBoundingVolume


//////////////////////////////////////////////////////////////////////////////////////////
// iterating functions
Patient::iterator Patient::begin()
{
  return this->m_SecretData->m_AllStudies.begin();
}
Patient::const_iterator Patient::begin() const
{
  return this->m_SecretData->m_AllStudies.cbegin();
}
Patient::const_iterator Patient::cbegin() const
{
  return this->m_SecretData->m_AllStudies.cbegin();
}
Patient::iterator Patient::end()
{
  return this->m_SecretData->m_AllStudies.end();
}
Patient::const_iterator Patient::end() const
{
  return this->m_SecretData->m_AllStudies.cend();
}
Patient::const_iterator Patient::cend() const
{
  return this->m_SecretData->m_AllStudies.cend();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
void Patient::CopyParametersInternal( const Self & other )
{

  SetPatientID( other.GetPatientID() );

   // go through each study
  const StudyCollectionType studies = other.GetAllStudies();
  BOOST_FOREACH ( const StudyPointer study, studies )
    {
      this->AddStudy( Study::New(*study));   
    } // for each study

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// set/get functions
ccipdGetConstImplementMacro    ( Patient, PatientID , PatientIDType       )
ccipdSetByrefImplementHashMacro( Patient, PatientID , PatientIDType       )
ccipdGetConstImplementMacro    ( Patient, AllStudies, StudyCollectionType ) 
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
