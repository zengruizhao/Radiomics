


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMultivariateGaussianDistribution.hxx"
#include "ccipdFactory.hxx"

#ifdef Boost_SERIALIZATION_FOUND
#include <boost/serialization/export.hpp>
#include <boost/serialization/version.hpp>

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND









typedef ccipd::MultivariateGaussianDistribution< double, 12 > ccipdMultivariateGaussianDistributionDoubleTwelve;
BOOST_CLASS_EXPORT( ccipdMultivariateGaussianDistributionDoubleTwelve )

template
void ccipd::MultivariateGaussianDistribution< double, 12 >::
serialize< boost::archive::xml_iarchive >(
  boost::archive::xml_iarchive & archive,
  const unsigned int version );

template
void ccipd::MultivariateGaussianDistribution< double, 12 >::
serialize< boost::archive::xml_oarchive >(
  boost::archive::xml_oarchive & archive,
  const unsigned int version );

template
struct boost::serialization::version<
  ccipd::MultivariateGaussianDistribution< double, 12 > >;

template
struct boost::serialization::tracking_level<
  ccipd::MultivariateGaussianDistribution< double, 12 > >;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class MultivariateGaussianDistribution< float, 3 >;

template
class Factory< MultivariateGaussianDistribution< float, 3 > >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class MultivariateGaussianDistribution< float, 12 >;

template
class Factory< MultivariateGaussianDistribution< float, 12 > >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class MultivariateGaussianDistribution< double, 3 >;

template
class Factory< MultivariateGaussianDistribution< double, 3 > >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class MultivariateGaussianDistribution< double, 12 >;

template
class Factory< MultivariateGaussianDistribution< double, 12 > >;
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd

