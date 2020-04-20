/*
 * This file just contains some simple math
 * functions which may be useful to other projects.
 */
#ifndef __ccipdMath_h
#define __ccipdMath_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  #include <vector>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Will return a numberOfElements-length vector containing
 * a random permutation of values 0 to numberOfElements - 1.
 * 
 * \date    2011-11-04
 * \author  Rob Toth
 * \ingroup Common
 */
std::vector< unsigned int > randperm( const unsigned int numberOfElements );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdMath_h