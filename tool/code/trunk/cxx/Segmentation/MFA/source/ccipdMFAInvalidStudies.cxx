


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAInvalidStudies.h"
#include "ccipdPatients.h"
#include "ccipdStudy.h"

// boost includes
#include <boost/foreach.hpp>

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void MFADisplayInvalidStudies(
  const Patients            & allPatients,
  const StudyCollectionType & studiesUsed )
{

  const std::size_t
    totalStudies = allPatients.GetNumberOfStudies(),
    numberOfStudiesUsed  = studiesUsed.size();

  cout << "# of studies = " <<
    numberOfStudiesUsed << " / " << totalStudies << endl;

  if ( numberOfStudiesUsed != totalStudies )
    {
    const StudyCollectionType 
      allStudies = allPatients.GetAllStudies();

    const StudyCollectionType::const_iterator
      studiesUsedEnd = studiesUsed.cend();

    BOOST_FOREACH( const StudyPointer & currentStudy, allStudies )
      {

      if ( studiesUsed.find( currentStudy ) == studiesUsedEnd )
        {
        cout << "Excluding study " << currentStudy->GetDescription() << endl;
        } // not found

      } // for each study

    } // unequal # of studies

} // MFADisplayInvalidStudies
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
