#ifndef __ccipdMacros_h
#define __ccipdMacros_h



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This is a very lightweight file containing
 * some macros for commonly used segmentation
 * C++ code.
 */
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * To disallow copying a class.
 * Required for the way I've implemented PIMPL.
 */
#define ccipdNoCopyMacro( ClassName )                  \
  static_assert( false,                                \
    "Deprecated; Please extend boost::noncopyable!" );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Constructor/destructor declarations
 */
#define ccipdConstructorDeclarationNoSelfMacro( ClassName ) \
protected:                                                  \
  ClassName();                                              \
public:                                                     \
  virtual ~ClassName();
  
/**
 * \brief Constructor/destructor empty definitions and declarations.
 */
#define ccipdConstructorEmptyDefinitionNoSelfMacro( ClassName ) \
protected:                                                  \
  ClassName() { }                                           \
public:                                                     \
  virtual ~ClassName() { }

/**
 * \brief Constructor/destructor declarations and a Self typedef
 */
#define ccipdConstructorDeclarationMacro( ClassName ) \
  ccipdConstructorDeclarationNoSelfMacro( ClassName ) \
public:                                               \
  typedef ClassName Self;

/**
 * \brief Constructor/destructor empty definitions and a Self typedef
 */
#define ccipdConstructorEmptyDefinitionsMacro( ClassName ) \
  ccipdConstructorEmptyDefinitionNoSelfMacro ( ClassName ) \
public:                                                    \
  typedef ClassName Self;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief For classes extending Factory
 * \note This assumes you have "Self" typedef'd!
 * \note This assumes you extend Factory< Self >!
 */
#define ccipdFactorySuperclassMacro \
                                    \
public:                             \
  typedef Factory< Self > SuperclassFactory;

#define ccipdFactoryUsingMacro                          \
                                                        \
public:                                                 \
  using SuperclassFactory::New;                         \
  using SuperclassFactory::ConstNew;

/**
 * This assumes you already have a typedef for SuperclassFactory
 */
#define ccipdFactoryTypedefsMacro                       \
                                                        \
public:                                                 \
  typedef SuperclassFactory::Pointer      Pointer;      \
  typedef SuperclassFactory::ConstPointer ConstPointer; \
                                                        \
  ccipdFactoryUsingMacro

#define ccipdFactoryTypedefsTemplatedMacro                       \
                                                                 \
public:                                                          \
  typedef typename SuperclassFactory::Pointer      Pointer;      \
  typedef typename SuperclassFactory::ConstPointer ConstPointer; \
                                                                 \
  ccipdFactoryUsingMacro


/**
 * This is for classes which extend Factory directly
 * and only requires "Self" typedef'd
 */
#define ccipdFactorySuperclassTypedefsMacro \
  ccipdFactorySuperclassMacro ccipdFactoryTypedefsMacro

#define ccipdFactorySuperclassTypedefsTemplatedMacro \
  ccipdFactorySuperclassMacro ccipdFactoryTypedefsTemplatedMacro

/**
 * Implement the Factory and EnableSharedFromThis hxx. 
 * You must include Factory.hxx in the function this is called from
 */
#define ccipdFactoryImplementationMacro( ClassName ) \
  template class Factory < ClassName >; \
  template class EnableSharedFromThis < ClassName >; \
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * PIMPL:
 * http://en.wikipedia.org/wiki/Opaque_pointer
 * http://herbsutter.com/gotw/_100/
 * So others including this class don't see what type
 * of data the class uses, and also don't
 * need to include any custom itk .hxx files
 * This goes into a header file.
*/
#define ccipdPIMPLMacro \
  private:                   \
  class SecretData;          \
  const std::unique_ptr< SecretData > m_SecretData;

/** Workaround for using PIMPl in ITK classes */
#define ccipdPIMPLMacroITK( ClassName ) \
  class SecretData;                          \
  struct SecretDataWorkaround :              \
    public std::unique_ptr< SecretData >     \
  {                                          \
    /* work around for using PIMPL in */     \
    /* itk classes http://bit.ly/JKm49d */   \
     SecretDataWorkaround();                 \
    ~SecretDataWorkaround();                 \
  } const m_SecretData;

/** Clearing the data */
#define ccipdClearDataMacro( ClassName )    \
  void ClassName::ClearData()               \
  {                                         \
    m_SecretData.reset( new SecretData() ); \
  }

#define ccipdNewPIMPLMacro m_SecretData( new SecretData() )

#ifndef THIS
#define THIS this->m_SecretData //  [6/3/2013 axa667]
#endif
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * For implementing a "Set" function using the PIMPL idiom.
 * The "Implement" macros go in a C++ file.
 * The "Declare" macros go in a header file.
 * Modeled after http://itk.org/Doxygen/html/itkMacro_8h_source.html
 */

// just internal helper macros
#define ccipdSetBodyMacro( VariableName )    \
  m_SecretData->m_##VariableName = variable; \

#define ccipdSetBodyHashMacro( VariableName ) \
  ccipdSetBodyMacro( VariableName )           \
  this->ClearHash();                          \

/* By Value */
#define ccipdSetByvalDeclareMacro( VariableName, VariableType ) \
  virtual void Set##VariableName( const VariableType value );   \

#define ccipdSetByvalImplementMacro(                               \
  ClassName, VariableName, VariableType )                          \
  void ClassName::Set##VariableName( const VariableType variable ) \
  {                                                                \
    ccipdSetBodyMacro( VariableName )                              \
  }

#define ccipdSetByvalImplementHashMacro(                           \
  ClassName, VariableName, VariableType )                          \
  void ClassName::Set##VariableName( const VariableType variable ) \
  {                                                                \
    ccipdSetBodyHashMacro( VariableName )                          \
  }

/* By Reference */
#define ccipdSetByrefDeclareMacro( VariableName, VariableType )     \
  virtual void Set##VariableName( const VariableType & reference );

#define ccipdSetByrefImplementMacro(                                 \
  ClassName, VariableName, VariableType )                            \
  void ClassName::Set##VariableName( const VariableType & variable ) \
  {                                                                  \
    ccipdSetBodyMacro( VariableName )                                \
  }

#define ccipdSetByrefImplementHashMacro(                             \
  ClassName, VariableName, VariableType )                            \
  void ClassName::Set##VariableName( const VariableType & variable ) \
  {                                                                  \
    ccipdSetBodyHashMacro( VariableName )                            \
  }                                                                  
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * For implementing a "Get" function using the PIMPL idiom.
 * The "Implement" macros go in a C++ file.
 * The "Declare" macros go in a header file.
 * Modeled after http://itk.org/Doxygen/html/itkMacro_8h_source.html
 */

// just an internal helper macro
#define ccipdGetBodyMacro( VariableName ) \
  {                                       \
  return m_SecretData->m_##VariableName;  \
  }                                      

/* Non-constant version */
#define ccipdGetDeclareMacro( VariableName, VariableType ) \
  virtual VariableType Get##VariableName();               

#define ccipdGetImplementMacro(               \
  ClassName, VariableName, VariableType )     \
  VariableType ClassName::Get##VariableName() \
  ccipdGetBodyMacro( VariableName )            

/* Constant version */
#define ccipdGetConstDeclareMacro( VariableName, VariableType ) \
  virtual VariableType Get##VariableName() const;              

#define ccipdGetConstImplementMacro(                 \
  ClassName, VariableName, VariableType )            \
  VariableType ClassName::Get##VariableName() const  \
  ccipdGetBodyMacro( VariableName )                  
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Forward declaration for serialization (saving and loading)
 */
#define ccipdSerializationForwardDeclarationMacro \
  /* forward declare serialization */             \
  namespace boost { namespace serialization       \
  {                                               \
  class access;                                   \
  } }

/**
 * Declare a serialization method.
 */
#define ccipdSerializationDeclarationMacro              \
private:                                                \
  /* allow serialization access to private variables */ \
  friend class boost::serialization::access;            \
                                                        \
  /* define the serialization code */                   \
  template< class Archive >                             \
  void serialize(                                       \
    Archive          & archive,                         \
    const unsigned int version );

/**
 * Implement the body of a serialization method with SecretData
 */
#define ccipdSerializationImplementationBodyMacro \
  using boost::serialization::make_nvp;           \
  archive & boost::serialization::make_nvp(       \
    "Data", *this->m_SecretData );

#define ccipdSerializationImplementationHeadingMacro( ClassName ) \
  template< class Archive >                                       \
  void ClassName::serialize(                                      \
    Archive & archive,                                            \
    const unsigned int version )

#define ccipdSerializationImplementationMacro( ClassName )  \
  ccipdSerializationImplementationHeadingMacro( ClassName ) \
  {                                                         \
    (void)version; /* don't warn */                         \
    ccipdSerializationImplementationBodyMacro               \
  } // serialize

/**
 * This compiles the code for loading / saving 
 * XML archives.
 */
#define ccipdSerializationXMLMacro( ClassName )\
                                               \
  /* xml loading */                            \
  template void ClassName::serialize(          \
    boost::archive::xml_iarchive & archive,    \
    const unsigned int version );              \
                                               \
  /* xml saving */                             \
  template void ClassName::serialize(          \
    boost::archive::xml_oarchive & archive,    \
    const unsigned int version );

#ifndef Boost_SERIALIZATION_FOUND
  // these should be empty
  #define ccipdLoadImplementationBodyMacro
  #define ccipdSaveImplementationBodyMacro
#else // we have the serialization library
  #define ccipdLoadImplementationBodyMacro     \
    try                                        \
      {                                        \
      /* Create a stream using the fileName */ \
      std::ifstream inputStream( fileName );   \
      boost::archive::xml_iarchive             \
        inputArchive( inputStream );           \
                                               \
      inputArchive >>                          \
        boost::serialization::make_nvp(        \
        "this", *this );                       \
                                               \
      loaded = true;                           \
      }                                        \
    catch ( ... )                              \
      {                                        \
      std::cerr << "Warning: Unable to load "  \
        << "serialized archive " <<            \
        fileName << std::endl;                 \
      loaded = false;                          \
      }

  #define ccipdSaveImplementationBodyMacro       \
    try                                          \
      {                                          \
      /* Create a stream using the fileName */   \
      std::ofstream outputStream( fileName );    \
                                                 \
      /* Create an archive using the steram */   \
      boost::archive::xml_oarchive               \
        outputArchive( outputStream );           \
                                                 \
      /* Save ourself to the archive */          \
      outputArchive <<                           \
        boost::serialization::make_nvp(          \
        "this", *this );                         \
                                                 \
      /* Success! */                             \
      saved = true;                              \
      }                                          \
    catch ( ... )                                \
      {                                          \
      /* Catch an error and display a warning */ \
      std::cerr << "Warning: Unable to save " << \
        "serialized archive " <<                 \
        fileName << std::endl;                   \
      saved = false;                             \
      }

#endif // Boost_SERIALIZATION_FOUND

#define ccipdLoadSaveDeclareMacro   \
                                    \
  /** Load from an XML file */      \
  virtual bool Load(                \
    const std::string & fileName ); \
                                    \
  /** Save to an XML file */        \
  virtual bool Save(                \
    const std::string & fileName ) const;

#define ccipdLoadSaveImplementationMacro( ClassName )  \
  bool ClassName::Load( const std::string & fileName ) \
  {                                                    \
    bool loaded = false;                               \
    ccipdLoadImplementationBodyMacro                   \
    return loaded;                                     \
  }                                                    \
                                                       \
  bool ClassName::Save( const std::string & fileName ) \
  const                                                \
  {                                                    \
    bool saved = false;                                \
    ccipdSaveImplementationBodyMacro                   \
    return saved;                                      \
  }                                                    

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Workaround for this bug: http://bit.ly/U1CDE5
 * \note  This should go outside of any namespaces.
 * \note  Please include <boost/type_traits/is_virtual_base_of.hpp>
 */
#define ccipdBoostVirtualBaseTrue( Base, Derived ) \
                                                   \
  namespace boost                                  \
  {                                                \
  template<>                                       \
  struct is_virtual_base_of< Base, Derived > :     \
    public true_type{};                            \
  } /* namespace boost */

/**
 * \brief Workaround for this bug: http://bit.ly/U1CDE5
 * \note  This should go outside of any namespaces.
 * \note  Please include <boost/type_traits/is_virtual_base_of.hpp>
 */
#define ccipdBoostVirtualBaseFalse( Base, Derived ) \
                                                    \
  namespace boost                                   \
  {                                                 \
  template<>                                        \
  struct is_virtual_base_of< Base, Derived > :      \
    public false_type{};                            \
  } /* namespace boost */

//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdMacros_h
