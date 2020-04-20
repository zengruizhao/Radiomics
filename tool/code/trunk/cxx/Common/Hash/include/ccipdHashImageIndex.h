#ifndef __ccipdHashImageIndex_h
#define __ccipdHashImageIndex_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Hash an ITK index.
 * 
 * To use an itk::Index
 * as the key in an std::unordered_map,
 * you can use this functor
 * to get a hash value from the index.
 * 
 * For example, 
 *
 * 
 * \date    2012-04-23
 * \author  Rob Toth
 * \ingroup Common
 */
template< unsigned int IndexDimension >
struct ImageIndexHashFunctor
{

  typedef itk::Index< IndexDimension > IndexType;

  HashValueType operator()( const IndexType & index ) const;

};
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdHashImageIndex_h
