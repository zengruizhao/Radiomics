


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdGaussianDistribution.hxx"

#ifdef Boost_SERIALIZATION_FOUND
#include <boost/serialization/export.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class ccipd::GaussianDistribution< float, 1 >;

template
class ccipd::GaussianDistribution< float, 3 >;

template
class ccipd::GaussianDistribution< float, 12 >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class ccipd::GaussianDistribution< double, 1 >;

template
class ccipd::GaussianDistribution< double, 3 >;

template
class ccipd::GaussianDistribution< double, 12 >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

typedef ccipd::GaussianDistribution< double, 12 > ccipdGaussianDistributionDoubleTwelve;
BOOST_CLASS_EXPORT( ccipdGaussianDistributionDoubleTwelve )

template
void ccipd::GaussianDistribution< double, 12 >::
serialize< boost::archive::xml_iarchive >(
  boost::archive::xml_iarchive & archive,
  const unsigned int version );

template
void ccipd::GaussianDistribution< double, 12 >::
serialize< boost::archive::xml_oarchive >(
  boost::archive::xml_oarchive & archive,
  const unsigned int version );

template
struct boost::serialization::version<
  ccipd::GaussianDistribution< double, 12 > >;

template
struct boost::serialization::tracking_level<
  ccipd::GaussianDistribution< double, 12 > >;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////
