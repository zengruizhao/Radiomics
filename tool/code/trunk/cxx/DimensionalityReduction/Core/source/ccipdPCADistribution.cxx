


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPCADistribution.hxx"

#ifdef Boost_SERIALIZATION_FOUND
#include <boost/serialization/export.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class ccipd::PCADistribution< float >;

//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

typedef ccipd::PCADistribution< float > ccipdPCADistributionFloat;
BOOST_CLASS_EXPORT( ccipdPCADistributionFloat )

template
void ccipd::PCADistribution< float >::
serialize< boost::archive::xml_iarchive >(
  boost::archive::xml_iarchive & archive,
  const unsigned int version );

template
void ccipd::PCADistribution< float >::
serialize< boost::archive::xml_oarchive >(
  boost::archive::xml_oarchive & archive,
  const unsigned int version );

template
struct boost::serialization::version<
  ccipd::PCADistribution< float > >;

template
struct boost::serialization::tracking_level<
  ccipd::PCADistribution< float > >;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////
