#ifndef __ccipdPatientTypes_h
#define __ccipdPatientTypes_h



/**
 * This file contains some common
 * types which several "segmentation"
 * classes use.
 * 
 * For example, a collection of
 * object names is something that
 * several classes will use.
 * 
 * This file is somewhat lightweight,
 * but contains some stl classes
 * for containers.
 */



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdHashable.h"                          // for unordered collections

#include <string>                                  // for names
#include <memory>                                  // for smart pointers
#include <set>                                     // for collections
#include <unordered_set>                           // for collections
/////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// forward declarations so you don't have to include the boost headers
namespace boost { namespace gregorian {
  class date;
} }
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class Study;
class Patient;
class Patients;
//////////////////////////////////////////////////////////////////////////////////////////



/**
 * \addtogroup PatientData
 * @{
 */



//////////////////////////////////////////////////////////////////////////////////////////
// Just some public typedef's
typedef std::string            PatientIDType;
typedef std::string            ModalityType;
typedef std::string            PlaneType;
typedef std::string            AnnotatorType;
typedef std::string            DescriptionType;
typedef std::string            MaskObjectType;
typedef boost::gregorian::date DateType;

typedef std::shared_ptr<       Patients > PatientsPointer;
typedef std::shared_ptr<       Patient  > PatientPointer;
typedef std::shared_ptr<       Study    > StudyPointer;
typedef std::unique_ptr<       DateType > DatePointer;
typedef std::shared_ptr< const Patients > PatientsConstPointer;
typedef std::shared_ptr< const Patient  > PatientConstPointer;
typedef std::shared_ptr< const Study    > StudyConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// our collection typedef's
// these will allow you to iterator over things
typedef std::set<
  MaskObjectType
> MaskObjectCollectionType;

typedef std::set<
  ModalityType
> ModalityCollectionType;

typedef std::unordered_set<
  StudyPointer,
  HashableFunctor< StudyPointer >,
  HashableFunctor< StudyPointer >
> StudyCollectionType;

typedef std::unordered_set<
  StudyConstPointer,
  HashableFunctor< StudyConstPointer >,
  HashableFunctor< StudyConstPointer >
> StudyConstCollectionType;

typedef std::unordered_set<
  PatientPointer,
  HashableFunctor< PatientPointer >,
  HashableFunctor< PatientPointer >
> PatientCollectionType;

typedef std::unordered_set< 
  PatientIDType 
> PatientIDCollectionType;
//////////////////////////////////////////////////////////////////////////////////////////




/** @} */
} // namespace
#endif // __ccipdPatientTypes_h
