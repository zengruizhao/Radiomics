#ifndef __ccipdFeatureID_h
#define __ccipdFeatureID_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdHashable.h"
#include "ccipdMacros.h"
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdFilteringFeatureCollectionOptions.h"
#include "ccipdCloneableFactory.h"
#include "ccipdSerializable.h"
#include "ccipdDisableWarningsMacro.h"
// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/split_member.hpp>
  #include <boost/variant.hpp>
  #include <boost/serialization/tracking.hpp>
#endif // Boost_SERIALIZATION_FOUND

// std includes
#include <memory> // for smart pointers
#include "ccipdEnableWarningsMacro.h"
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Define a unique feature identifier.
 * 
 * \author  Rob Toth
 * \ingroup FeatureCollection
 */
class FeatureID :
  public Hashable,
  public CloneableFactory< FeatureID >,
  public Copyable< FeatureID >, 
  public Serializable< FeatureID >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( FeatureID )

public:
  typedef CloneableFactory< Self > SuperclassFactory;
  ccipdFactoryTypedefsMacro

protected:
  virtual void CopyParametersInternal( const Self & other );
//////////////////////////////////////////////////////////////////////////////////////////
  


public:
//////////////////////////////////////////////////////////////////////////////////////////
  /// create constructors from a few different input types
  static Pointer New(  const TextureFeatureEnum            textureFeature );
  static Pointer New(  const TextureFeatureEnumPointer     textureFeature );
  static Pointer New(  const FeatureDescriptionType      & description    );
  static Pointer New(  const AnyTextureFeatureParameters & parameters     );

  static Pointer New(  const TextureFeatureEnum            textureFeature
	                   , const FeatureDescriptionType      & description 
                    );

  static Pointer New(  const TextureFeatureEnumPointer textureFeature
                     , const FeatureDescriptionType  & description 
					          );

  static Pointer New(  const TextureFeatureEnum            textureFeature
	                   , const AnyTextureFeatureParameters & parameters 
					          );

  static Pointer New(  const TextureFeatureEnumPointer     textureFeature
	                   , const AnyTextureFeatureParameters & parameters 
					          ); 

  static Pointer New(  const TextureFeatureEnum            textureFeature
	                 , const FeatureDescriptionType      & description
					 , const AnyTextureFeatureParameters & parameters 
					);

  static Pointer New(  const TextureFeatureEnumPointer     textureFeature
	                   , const FeatureDescriptionType      & description
					           , const AnyTextureFeatureParameters & parameters 
					          ); 
//////////////////////////////////////////////////////////////////////////////////////////

public:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Make sure we can compare it to another one.
  virtual bool operator==( const Hashable & other ) const;
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Make sure we are hashable
  virtual HashValueType ReHash() const;
//////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Internal function to get feature name
  virtual std::string GetFeatureName   (                                 ) const;
  virtual void        SetTextureFeature( const std::string & featureName )      ;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Set/Get functions*/

  /// Are we associated with a texture feature?
  ccipdSetByvalDeclareMacro( TextureFeature, TextureFeatureEnum        )
  ccipdSetByvalDeclareMacro( TextureFeature, TextureFeatureEnumPointer )
  ccipdGetConstDeclareMacro( TextureFeature, TextureFeatureEnumPointer )
  bool HasTextureFeature() const;

  /// Are we associated with set of parameters? 
  /// ideally we want each parameters type for the correct enum; 
  /// however since the enum can change we need to allow the parameters type to change.
  ccipdSetByrefDeclareMacro( Parameters, AnyTextureFeatureParameters )
  ccipdGetConstDeclareMacro( Parameters, AnyTextureFeatureParameters & )
  bool HasParameters() const;

  /// What's our description?
  ccipdSetByrefDeclareMacro( Description, FeatureDescriptionType )
  ccipdGetConstDeclareMacro( Description, FeatureDescriptionType )

//////////////////////////////////////////////////////////////////////////////////////////


private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// We are serializable.
#ifdef Boost_SERIALIZATION_FOUND

  friend class boost::serialization::access;      

  // Save/Load Serialization set differently
  template< class Archive >                             
  void save( Archive & archive, const unsigned int version ) const;
  
  template< class Archive >                             
  void load( Archive & archive, const unsigned int version );

  BOOST_SERIALIZATION_SPLIT_MEMBER()

// Private helper to call old serialization method.
// It must be const so that save may call it.
  template< class Archive >   
  void SerializationDepreciated(Archive & archive, const unsigned int version ) const;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace

#ifdef Boost_SERIALIZATION_FOUND
  BOOST_CLASS_VERSION( ccipd::FeatureID, 2 )
  BOOST_CLASS_TRACKING(ccipd::FeatureID, boost::serialization::track_always)
#endif // Boost_SERIALIZATION_FOUND

#endif // __ccipdFeatureID_h
