


///////////////////////////////////////////////////////////////////////////
#include "ccipdHashImageIndex.hxx"
///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
// Compile for known dimensions

template
ccipd::HashValueType ccipd::ImageIndexHashFunctor< 1 >::operator()(
  const ccipd::ImageIndexHashFunctor< 1 >::IndexType & ) const;

template
ccipd::HashValueType ccipd::ImageIndexHashFunctor< 2 >::operator()(
  const ccipd::ImageIndexHashFunctor< 2 >::IndexType & ) const;

template
ccipd::HashValueType ccipd::ImageIndexHashFunctor< 3 >::operator()(
  const ccipd::ImageIndexHashFunctor< 3 >::IndexType & ) const;

///////////////////////////////////////////////////////////////////////////

