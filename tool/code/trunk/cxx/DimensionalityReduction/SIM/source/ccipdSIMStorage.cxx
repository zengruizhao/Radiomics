


////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"
#include "ccipdSIMTypes.h"
#include "ccipdSIMStorage.hxx"
#include "ccipdFactory.hxx"

// serialization includes
#include "ccipdDimensionalityReductionOptions.h"
#ifdef Boost_SERIALIZATION_FOUND
  #include "ccipdMacros.h"
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/export.hpp>
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/** This file just generates the MFABase for our existing data types. */
template class SIMStorage< InternalPixelType, VolumeDimension >;

template class Factory< SIMStorage< InternalPixelType, VolumeDimension > >;

template class EnableSharedFromThis< SIMStorage< InternalPixelType, VolumeDimension > >;
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

  /** Compile code for xml serialization of MFABase. */
  ccipdSerializationXMLMacro( SIMVolumeStorage )

#endif // #Boost_SERIALIZATION_FOUND
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd



////////////////////////////////////////////////////////////////////////////////////////////////////'
#ifdef Boost_SERIALIZATION_FOUND

typedef ccipd::ConcreteFactorySelf< ccipd::SIMVolumeStorage > SIMVolumeStorageConcrete;
BOOST_CLASS_EXPORT_GUID( ccipd::SIMVolumeStorage, "SIMVolumeStorage" )
BOOST_CLASS_EXPORT( SIMVolumeStorageConcrete )

// we must specialize our MFA "version"
namespace boost { namespace serialization
{

  template struct version< ccipd::SIMVolumeStorage >;

}} // namespaces

BOOST_CLASS_VERSION(
  SIMVolumeStorageConcrete,
  boost::serialization::version< ccipd::SIMVolumeStorage >::value )

#endif // #Boost_SERIALIZATION_FOUND
////////////////////////////////////////////////////////////////////////////////////////////////////

