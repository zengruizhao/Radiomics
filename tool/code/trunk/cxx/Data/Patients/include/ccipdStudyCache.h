#ifndef __ccipdStudyCache_h
#define __ccipdStudyCache_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMacros.h"
#include "ccipdPatientTypes.h"
#include "ccipdEnableSharedFromThis.hxx"

// serialization includes
#include "ccipdDataPatientsOptions.h"
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/tracking.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * This class will be used
 * as a cache of objects
 * associated with each study.
 * 
 * You give it, as a template parameter,
 * what object to store
 * for each study.
 * 
 * \author  Rob Toth
 * \date    2013-02-05
 * \ingroup Patients
 */
template< typename TCachedObject >
class StudyCache :
  public EnableSharedFromThis< StudyCache< TCachedObject > >
{

  

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( StudyCache )
public: // typedefs
  typedef StudyCache< TCachedObject > Self;
  typedef TCachedObject CachedObjectType;
//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * \brief Output the object for a given study.
   * 
   * \note
   *   This version is not const and therefore
   *   will be able to cache the object
   *   so that the next time you call it,
   *   the cached value can be used.
   */
  virtual CachedObjectType GetCachedObject(
    const StudyConstPointer & study );

  /**
   * \see GetObject
   * 
   * \note
   *   This version is const and therefore
   *   will not be able to cache the transform.
   *   Therefore, the if the transform is already
   *   cached, it will be used. However, if it
   *   is not cached, it is calculated. Yet
   *   the resulting, calculated transform
   *   will not be cached for future uses (since
   *   we are const).
   */
  virtual CachedObjectType GetCachedObject(
    const StudyConstPointer & study ) const;

//////////////////////////////////////////////////////////////////////////////////////////


  
protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Child classes must provide
   * the ability to calculate
   * an object for a given study.
   */
  virtual CachedObjectType CalculateObject( const Study & study ) const = 0;

  /// Remove any cached objects.
  virtual void ClearCache();

//////////////////////////////////////////////////////////////////////////////////////////


  
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class StudyCache
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
// http://bit.ly/11HPC2J
namespace boost {
namespace serialization {
 
template< typename TCachedObject >
struct version< ccipd::StudyCache< TCachedObject > >
{
  // This is version 1
  typedef mpl::int_< 1 > type;
  typedef mpl::integral_c_tag tag;
  BOOST_STATIC_CONSTANT( unsigned int, value = version::type::value );
};

template< typename TCachedObject >
struct tracking_level< ccipd::StudyCache< TCachedObject > >
{
  typedef mpl::integral_c_tag tag;
  typedef mpl::int_< track_always > type;
  BOOST_STATIC_CONSTANT( int, value = tracking_level::type::value );
};

/*
 * Note you must do the following:

    template
    struct boost::serialization::version<
      ccipd::StudyCache< MyType > >;

    template
    struct boost::serialization::tracking_level<
      ccipd::StudyCache< MyType > >;

 */

} } // namespaces
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdStudyCache_h
