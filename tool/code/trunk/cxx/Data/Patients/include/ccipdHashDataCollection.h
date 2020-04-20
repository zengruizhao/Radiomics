#ifndef __ccipdHashDataCollection_h
#define __ccipdHashDataCollection_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdPatientTypes.h"
#include "ccipdHashCollection.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
// specializing for our data types

/**
 * Sometimes our collection of patient data can be
 * unsorted. But when we hash an entire collection,
 * we really want to know what the hash would be
 * of a sorted collection.
 *
 * That functionality is what these functions provide.
 *
 * You don't need to include the .hxx file
 * (which will slow down compilation)
 *
 * \date    2012-02-18
 * \author  Rob Toth
 * \ingroup PatientData
 */
HashValueType GetHashValue(
  const StudyCollectionType & studies );

/**
 * \see GetHashValue()
 * 
 * \date    2012-02-18
 * \author  Rob Toth
 * \ingroup PatientData
 */
HashValueType GetHashValue(
  const PatientCollectionType & patients );

//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdHashDataCollection_h
