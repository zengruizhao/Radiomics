#ifndef __ccipdHistogramTypes_h
#define __ccipdHistogramTypes_h



/**
 * This file contains common types for the histogram type 
 */
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkHistogram.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \addtogroup Common
 * @{
 */
 typedef itk::Statistics::Histogram<float >    HistogramTypeF;
 typedef itk::Statistics::Histogram<double>    HistogramTypeD;

 typedef HistogramTypeF::Pointer HistogramFPointer;
 typedef HistogramTypeD::Pointer HistogramDPointer;

/** @} */
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdHistogramTypes_h
