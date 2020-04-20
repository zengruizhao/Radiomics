


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAAlgorithmTableWidgetSet.h"
#include "ccipdMFAAlgorithmTableWidgetAdd.h"
#include "ccipdMFAAlgorithmTableWidget.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
//////////////////////////////////////////////////////////////////////////////////////////
void SetAlgorithms(
        MFAAlgorithmTableWidget        & table,
  const RegisterMFAAlgorithmCollection & algorithms )
{

  // first, clear the table
  table.clear();

  // now add in all the algorithms
  AddAlgorithms( table, algorithms );  
  
} // SetAlgorithms
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
