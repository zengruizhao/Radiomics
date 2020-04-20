#ifndef __ccipdSIMStorage_hxx
#define __ccipdSIMStorage_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdSIMStorage.h"
#include "ccipdSaveImage.hxx"         // for saving
#include "ccipdFeatureID.h"           // for defining our features
#include "ccipdMatrixConcatenation.h" // for concatenating submatrices
#include "ccipdFactory.hxx"

// serialization includes
#include "ccipdDimensionalityReductionOptions.h" // whether or not to serialize
#ifdef Boost_SERIALIZATION_FOUND
  #include "ccipdMatrixSerialization.h"          // for serializing the linked matrix
  #include "ccipdSharedPointerSerialization.hxx" // for serializing pointers
  #include "ccipdUnorderedMapSerialization.h"    // for serializing unordered maps
  #include <boost/serialization/nvp.hpp>        // for xml tags
  #include <boost/serialization/vector.hpp>     // for serializing std::vector
#endif // Boost_SERIALIZATION_FOUND

// eigen includes
#include <Eigen/Core> // for matrix types

// std includes
#include <unordered_map> // for storing our means and eigenvectors

// boost includes
#include <boost/foreach.hpp>      // for loops
#include <boost/lexical_cast.hpp> // for converting integers to strings

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageFileReader.h> // since we are templated we use this
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{




//////////////////////////////////////////////////////////////////////////////////////////
/** \brief PIMPL interface to ccipd::SIMStorage
 * 
 * \author  Rachel Sparks
 * \ingroup SIM
 */
template < class TPixelType, unsigned int Dimension >
class SIMStorage<   TPixelType,              Dimension >::SecretData
{
public: // public to the MFABase class

  /** Constructor */
  SecretData();

////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Typedef's */
  typedef SIMStorage<
    TPixelType,
    Dimension
  > SIMStorageType;

  typedef typename SIMStorageType::VectorImageType  VectorImageType;
  typedef typename VectorImageType::ConstPointer VectorImageConstPointer;

  typedef typename SIMStorageType::WhichRowsType WhichRowsType;

  typedef std::unordered_map<
    FeatureIDConstPointer,
    VectorImageConstPointer,
    ccipd::HashableFunctor< FeatureIDConstPointer >,
    ccipd::HashableFunctor< FeatureIDConstPointer >
  > VectorImageMap;
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  // where to save the files?
  std::string m_OutputFolder;

  // a 2D matrix with the eigenvectors' linking
  LinkedMatrixPointer m_LinkedMatrix;

  // defining which rows each feature belongs to, in the linked matrix
  WhichRowsType
    m_StartingRows,
    m_NumberOfRows;

  // the first image is the mean, the rest are the eigenvectors
  VectorImageMap m_MeanAndEigenvectorImages;

  // we need to explicitly say what version of SecretData we are on
  unsigned int m_Version;
////////////////////////////////////////////////////////////////////////////////////////////////////



private:
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

  ccipdSerializationDeclarationMacro

  // define special saving code
  template< class Archive >
  void save( Archive & archive, const unsigned int version) const;

  // define special loading code
  template< class Archive >
  void load( Archive & load, const unsigned int version );

#endif // Boost_SERIALIZATION_FOUND
////////////////////////////////////////////////////////////////////////////////////////////////////



}; // SIMStorage::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TPixelType, unsigned int Dimensions >
SIMStorage  <       TPixelType,              Dimensions >
::SIMStorage() :
ccipdNewPIMPLMacro
{
} // constructor

template < class TPixelType, unsigned int Dimensions >
SIMStorage  <       TPixelType,              Dimensions >
::SecretData::SecretData() :
m_OutputFolder( "./" )
{
}

// destructor
template < class TPixelType, unsigned int Dimensions >
SIMStorage  <       TPixelType,              Dimensions >
::~SIMStorage()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TPixelType, unsigned int Dimensions >
void SIMStorage <   TPixelType,              Dimensions >
::AddFeature(
  const FeatureIDConstPointer & whichFeature,
  const VectorImageType       * meanAndEigenvectors )
{

  if ( meanAndEigenvectors->GetNumberOfComponentsPerPixel() < 1 )
    throw itk::ExceptionObject( __FILE__, __LINE__,
      "You need at least a mean image to input into SIMStorage!" );

  // update our map
  this->m_SecretData->m_MeanAndEigenvectorImages[ whichFeature ] = meanAndEigenvectors;

} // AddFeature
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TPixelType, unsigned int Dimensions >
void SIMStorage <   TPixelType,              Dimensions >
::RemoveFeature( const FeatureIDConstPointer & whichFeature )
{

  // erase it from the map
  this->m_SecretData->m_MeanAndEigenvectorImages.erase( whichFeature );

} // RemoveFeature
////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TPixelType, unsigned int Dimensions >
void SIMStorage <   TPixelType,              Dimensions >
::ClearFeatures()
{

  // clear the map
  this->m_SecretData->m_MeanAndEigenvectorImages.clear();

} // ClearFeatures
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template <      class TPixelType, unsigned int Dimensions >
bool SIMStorage <        TPixelType,              Dimensions >
::HasFeatures( const FeatureIDConstCollection & whichFeatures ) const
{

  // check each feature
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    if ( !this->HasFeature( whichFeature ) )
      return false;

  return true;

} // HasFeatures
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template <      class TPixelType, unsigned int Dimensions >
bool SIMStorage <        TPixelType,              Dimensions >
::HasFeature( const FeatureIDConstPointer & whichFeature ) const
{
  // check if it exists in our map
  return
    this->m_SecretData->m_MeanAndEigenvectorImages.find( whichFeature ) !=
    this->m_SecretData->m_MeanAndEigenvectorImages.cend();

} // HasFeature
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template <                   class TPixelType, unsigned int Dimensions >
FeatureIDConstCollection SIMStorage < TPixelType,              Dimensions >
::GetFeatureIDs() const
{

  // prepare something to store the output
  FeatureIDConstCollection featureIDs;

  // extract our feature cache
  typedef typename SecretData::VectorImageMap VectorImageMap;
  const VectorImageMap & featuresMap = this->m_SecretData->m_MeanAndEigenvectorImages;

  // loop through all the mean and eigenvectors
  BOOST_FOREACH ( const typename VectorImageMap::value_type & keyAndValue, featuresMap )
    {

    // extract just the id
    const FeatureIDConstPointer & featureID = keyAndValue.first;

    // add it to our collection
    featureIDs.push_back( featureID );

    } // for each mean and eigenvector image

  return featureIDs;

} // GetFeatures
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TPixelType, unsigned int Dimensions > typename
SIMStorage <        TPixelType,              Dimensions >::VectorImageConstPointer
SIMStorage <        TPixelType,              Dimensions >
::GetMeanAndEigenvectors( const FeatureIDConstPointer & whichFeature ) const
{

  if ( !HasFeature( whichFeature ) )
    {
    std::cerr << "Warning: Trying to get a non-existant " <<
      "feature. Returning NULL." << std::endl;
    return VectorImageConstPointer();
    }

  // return the feature
  return this->m_SecretData->m_MeanAndEigenvectorImages[ whichFeature ];

} // GetFeature
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

template< class TPixelType, unsigned int Dimensions > template< class Archive >
void SIMStorage<   TPixelType,              Dimensions >
::serialize( Archive & archive, const unsigned int version )
{

  // I couldn't get boost::serialization::version to work with SecretData
  this->m_SecretData->m_Version = version;
  
  boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) );

  ccipdSerializationImplementationBodyMacro;

} // serialize

template< class TPixelType, unsigned int Dimensions > template< class Archive >
void SIMStorage<   TPixelType,              Dimensions >::SecretData
::serialize( Archive & archive, const unsigned int version )
{

  // serialize the linked matrix (ccipdMatrixSerialization.h)
  using boost::serialization::make_nvp;
  LinkedMatrixPointer & linkedMatrixPointer = this->m_LinkedMatrix;
  if ( !linkedMatrixPointer ) linkedMatrixPointer =
    LinkedMatrixPointer( new LinkedMatrixType );
  LinkedMatrixType & linkedMatrix = *linkedMatrixPointer;
  archive & make_nvp( "LinkedMatrix", linkedMatrix );

  // I couldn't get the boost::serialization::version to
  // work with this nested templated class
  const unsigned int actualVersion = this->m_Version;
  (void)version; // don't warn about not using the input "version"

  if ( actualVersion >= 2 )
    {
    // serialize the output folder to save / load the actual feature images
    archive & make_nvp( "OutputFolder", this->m_OutputFolder );
    }

  // serialize separate saving and loading
  boost::serialization::split_member( archive, *this, actualVersion );

} // serialize

template< class TPixelType, unsigned int Dimensions > template< class Archive >
void SIMStorage<   TPixelType,              Dimensions >::SecretData
::save( Archive & archive, const unsigned int version ) const
{

  const std::string folder = m_OutputFolder + "/";

  // to temporarily store them to save into the archive
  FeatureIDConstCollection featureIDs;

  // go through each feature
  BOOST_FOREACH(
    const typename VectorImageMap::value_type & meanAndEigenvectorPair,
    this->m_MeanAndEigenvectorImages )
    {

    // extract the data
    const FeatureIDConstPointer & featureID = meanAndEigenvectorPair.first;
    const VectorImageConstPointer
      meanAndEigenvectorImage = meanAndEigenvectorPair.second;

    // get the hash
    const HashValueType featureHash = featureID->GetHash();

    // create a file name based on the hash value
    const std::string fileName = folder +
      boost::lexical_cast< std::string >( featureHash ) + ".mha";

    // save the actual file
    SaveImage< VectorImageType >( meanAndEigenvectorImage, fileName, true );

    if ( version >= 1 )
      featureIDs.push_back( featureID );

    } // for each feature

  using boost::serialization::make_nvp;

  if ( version >= 1 )
    {
    // save the featureID to the archive
    archive & make_nvp( "FeatureID", featureIDs );
    }

  if ( version >= 3 )
    {
    // save the rows
    archive & make_nvp( "StartingRows", this->m_StartingRows );
    archive & make_nvp( "NumberOfRows", this->m_NumberOfRows );
    }

} // save

template< class TPixelType, unsigned int Dimensions > template< class Archive >
void SIMStorage<   TPixelType,              Dimensions >::SecretData
::load( Archive & archive, const unsigned int version )
{

  // this code was added in version 2
  if ( version < 2 )
    return;

  // note if you want to clear the existing
  // features you must do that yourself

  const std::string folder = m_OutputFolder + "/";

  // serialize the feature id's
  FeatureIDCollection featureIDs;
  using boost::serialization::make_nvp;
  archive & make_nvp( "FeatureID", featureIDs );

  // go through each feature id
  BOOST_FOREACH( FeatureIDConstPointer featureID, featureIDs )
    {

    // get the hash
    const HashValueType featureHash = featureID->GetHash();

    // create a file name based on the hash value
    const std::string
      fileNamePrefix = folder + boost::lexical_cast< std::string >( featureHash );
    std::vector< std::string > fileNamesToTry;
    fileNamesToTry.push_back( fileNamePrefix + std::string( ".mha" ) );
    fileNamesToTry.push_back( fileNamePrefix + std::string( ".mhd" ) );
    fileNamesToTry.push_back( fileNamePrefix + std::string( ".vtk" ) );

    // try to load the file
    VectorImageConstPointer loadedImage;

    BOOST_FOREACH( const std::string & fileName, fileNamesToTry )
      {

      // if we've already loaded, just skip
      if ( loadedImage ) continue;

      // now, just make a generic reader and try to load it
      typedef itk::ImageFileReader< VectorImageType > ReaderType;
      const typename ReaderType::Pointer reader = ReaderType::New();

      try
        {

        // read the image
        reader->SetFileName( fileName );
	      reader->Update();

        loadedImage = reader->GetOutput();

        } // try
      catch ( ... )
        {
        }
      } // for each file name

    // assign the image (or null if none was loaded)
    m_MeanAndEigenvectorImages[ featureID ] = loadedImage;

    } // for each featureID

  if ( version >= 3 )
    {
    
    // we can't load into const pointers 
    typedef std::unordered_map<
      FeatureIDPointer,
      typename SIMStorageType::LinkedMatrixIndexType,
      HashableFunctor< FeatureIDPointer >,
      HashableFunctor< FeatureIDPointer >
    > WhichRowsUnconstType;
    WhichRowsUnconstType
      startingRows,
      numberOfRows;

    archive & make_nvp( "StartingRows", startingRows ); 
    archive & make_nvp( "NumberOfRows", numberOfRows );
   
    // now that we've loaded an unconst version, we will simply
    // populate our actual data member
    this->m_StartingRows.clear();
    this->m_StartingRows.insert( startingRows.cbegin(), startingRows.cend() );

    this->m_NumberOfRows.clear();
    this->m_NumberOfRows.insert( numberOfRows.cbegin(), numberOfRows.cend() );

    } // version >= 3

} // load

#endif // Boost_SERIALIZATION_FOUND
////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension > typename
SIMStorage<        TPixelType,              Dimension >::LinkedMatrixPointer
SIMStorage<        TPixelType,              Dimension >
::GetLinkedMatrix( const FeatureIDConstCollection & whichFeatures ) const
{

  // make sure we have a valid input
  if ( !this->HasFeatures( whichFeatures ) )
    return LinkedMatrixPointer(); // return an empty pointer

  // prepare something to store the matrices
  typedef std::vector< LinkedMatrixConstPointer > SubMatrixCollection;
  SubMatrixCollection subMatrices;

  // go through each feature
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    {

    // extract the submatrix for this feature
    subMatrices.push_back( this->GetLinkedMatrix( whichFeature ) );
    
    } // for each feature

  // concatenate the matrices
  return ConcatenateMatrixRows< PixelType >( subMatrices );

} // GetLinkedMatrix
////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension > typename
SIMStorage<        TPixelType,              Dimension >::LinkedMatrixPointer
SIMStorage<        TPixelType,              Dimension >
::GetLinkedMatrix( const FeatureIDConstPointer & whichFeature ) const
{
  // extract our variables
  const LinkedMatrixPointer & linkedMatrix = this->m_SecretData->m_LinkedMatrix;
  const WhichRowsType
    & startingRowsAll = this->m_SecretData->m_StartingRows,
    & numberOfRowsAll = this->m_SecretData->m_NumberOfRows;

  // make sure we have a valid input
  if ( !this->HasFeature( whichFeature ) || !linkedMatrix )
    {
    std::cerr << "Error: no feature or linked matrix." << std::endl;
    return LinkedMatrixPointer(); // return an empty pointer
    }

  // how big is our matrix?
  const LinkedMatrixIndexType
    startingCol  = 0,
    numberOfCols = linkedMatrix->cols();

  // extract the rows for this feature
  const LinkedMatrixIndexType
    startingRow  = startingRowsAll.at( whichFeature ),
    numberOfRows = numberOfRowsAll.at( whichFeature );

  // extract store our new matrix
  return LinkedMatrixPointer(
    new LinkedMatrixType(
        linkedMatrix->block(
        startingRow , startingCol,
        numberOfRows, numberOfCols ) ) );

} // GetLinkedMatrix( one feature )
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Set/Get linked matrix

template< class TPixelType, unsigned int Dimension > typename
SIMStorage<        TPixelType,              Dimension >::LinkedMatrixPointer
SIMStorage<        TPixelType,              Dimension >
::GetLinkedMatrix() const
{
  return this->m_SecretData->m_LinkedMatrix;
}

template< class TPixelType, unsigned int Dimension >
void SIMStorage<   TPixelType,              Dimension >
::SetLinkedMatrix(
    const LinkedMatrixPointer & linkedMatrix,
    const WhichRowsType       & startingRows,
    const WhichRowsType       & numberOfRows )
{

  this->m_SecretData->m_LinkedMatrix = linkedMatrix;
  this->m_SecretData->m_StartingRows = startingRows;
  this->m_SecretData->m_NumberOfRows = numberOfRows;

} // SetLinkedMatrix
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Set/Get output folder

template<      class TPixelType, unsigned int Dimension >
std::string SIMStorage< TPixelType,              Dimension >
::GetOutputFolder() const
{
  return this->m_SecretData->m_OutputFolder;
}

template< class TPixelType, unsigned int Dimension >
void SIMStorage<   TPixelType,              Dimension >
::SetOutputFolder( const std::string & outputFolder )
{
  this->m_SecretData->m_OutputFolder = outputFolder;
}
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd



#endif // __ccipdSIMStorage_hxx
