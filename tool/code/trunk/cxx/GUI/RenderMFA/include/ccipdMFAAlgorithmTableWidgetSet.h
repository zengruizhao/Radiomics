#ifndef __ccipdMFAAlgorithmTableWidgetSet_h
#define __ccipdMFAAlgorithmTableWidgetSet_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFATypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
class MFAAlgorithmTableWidget;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Update the algorithm table
 * to perfectly mirror a set of
 * existing algorithms.
 * 
 * This will remove any existing
 * table rows.
 */
void SetAlgorithms(
        MFAAlgorithmTableWidget        & table,
  const RegisterMFAAlgorithmCollection & algorithms );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFAAlgorithmTableWidgetSet_h