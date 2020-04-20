



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdHashDataCollection.h"
#include "ccipdHashCollection.hxx" // we actually need the code from here
#include "ccipdStudy.h"            // we will hash you
#include "ccipdPatient.h"          // we will hash you
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
HashValueType GetHashValue(
  const StudyCollectionType & studies )
{

  return GetHashValue(
    GetSortedHashValues< StudyCollectionType >( studies ) );

} // GetHashValue

HashValueType GetHashValue(
  const PatientCollectionType & patients )
{

  return GetHashValue(
    GetSortedHashValues< PatientCollectionType >( patients ) );

} // GetHashValue
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
