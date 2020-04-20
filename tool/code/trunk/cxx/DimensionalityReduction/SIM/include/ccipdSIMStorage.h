#ifndef __ccipdSIMStorage_h
#define __ccipdSIMStorage_h



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdFeatureCollectionTypes.h" // for our feature typedefs
#include "ccipdITKForwardDeclarations.h" // for itk classes
#include "ccipdMatrixTypes.h"            // for Eigen types
#include "ccipdMacros.h"                 // for easy coding
#include "ccipdHashable.h"
#include "ccipdFactory.h"

// std includes
#include <vector>        // for storing projections
#include <string>        // for folder names
#include <memory>        // for smart pointers
#include <unordered_map> // for defining which parts of the linked matrix each feature belongs to

// serialization includes
#include "ccipdDimensionalityReductionOptions.h" // whether or not we can serialize
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/serialization/version.hpp>
  ccipdSerializationForwardDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief A trained SIM.
 * 
 * This class contains the base methods for storing
 * the mean and basis images for each texture feature
 * and for each levelset.
 *
 * Also, it contains code for loading and saving the MFA to files.
 *
 * Classes extending this class should also extend another ITK
 * class.  This class provides no smart pointer management, and
 * is meant to be used in conjunction with another class,
 * such as ImageToImageMetric.
 * 
 * \author  Rachel Sparks
 * \ingroup SIM
 */
template< class TPixelType, unsigned int Dimension > // template types
class SIMStorage :
  public Factory< SIMStorage< TPixelType, Dimension > >
{



////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( SIMStorage )
public:
  typedef SIMStorage< TPixelType, Dimension > Self;
  ccipdFactorySuperclassTypedefsTemplatedMacro
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** The dimensionality of the images. */
  static const unsigned int ImageDimension = Dimension;

  /** some public types */
  typedef TPixelType PixelType;

  typedef Eigen::Matrix<
    PixelType,
    -1, -1, Eigen::RowMajor, -1, -1
  > LinkedMatrixType;

  typedef std::size_t LinkedMatrixIndexType;

  typedef std::shared_ptr<
    const LinkedMatrixType
  > LinkedMatrixConstPointer;

  typedef std::shared_ptr<
    LinkedMatrixType
  > LinkedMatrixPointer;

  typedef std::unordered_map<
    FeatureIDConstPointer,      // which feature is the key
    LinkedMatrixIndexType, // the index number is the value
    HashableFunctor< FeatureIDConstPointer >,
    HashableFunctor< FeatureIDConstPointer >
  > WhichRowsType;

  /** This stores the mean and eigenvector images images */
  typedef itk::VectorImage< TPixelType, Dimension >  VectorImageType;
  typedef itk::SmartPointer< const VectorImageType > VectorImageConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** \brief Get which features we have */
  FeatureIDConstCollection GetFeatureIDs() const;

  /** \brief Do we have the given feature? */
  bool HasFeature( const FeatureIDConstPointer & whichFeature ) const;

  /** \brief Do we have all the given features? */
  bool HasFeatures( const FeatureIDConstCollection & whichFeatures ) const;

  /**
   * Extract the mean (component 0)
   * and eigenvectors (components >= 1,
   * sorted in descending order)
   * of a given feature
   */
  VectorImageConstPointer GetMeanAndEigenvectors(
    const FeatureIDConstPointer & whichFeature ) const;

  /** The first component should be the mean, and the rest are eigenvectors */
  virtual void AddFeature(
    const FeatureIDConstPointer & whichFeature,
    const VectorImageType       * meanAndEigenvectors );

  /** \note This does nothing if the feature doesn't exist */
  virtual void RemoveFeature( const FeatureIDConstPointer & whichFeature );

  /** \brief Remove all features */
  virtual void ClearFeatures();
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /**
   * Set the linked matrix.
   * You must also say which row each feature
   * corresponds to (where 0 is the first row).
   */
  void SetLinkedMatrix(
    const LinkedMatrixPointer & linkedMatrix,
    const WhichRowsType       & startingRows,
    const WhichRowsType       & numberOfRows );

  /** \brief Get the full linked matrix */
  ccipdGetConstDeclareMacro( LinkedMatrix, LinkedMatrixPointer )

  /// Get the linked matrix for a single feature.
  LinkedMatrixPointer GetLinkedMatrix(
    const FeatureIDConstPointer & whichFeature ) const;

  /** \brief Get the linked matrix, but only with some rows */
  LinkedMatrixPointer GetLinkedMatrix(
    const FeatureIDConstCollection & whichFeatures ) const;

  /** \brief Set/Get the location to store the output images */
  ccipdSetByrefDeclareMacro( OutputFolder, std::string )
  ccipdGetConstDeclareMacro( OutputFolder, std::string )
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
////////////////////////////////////////////////////////////////////////////////////////////////////



private:
////////////////////////////////////////////////////////////////////////////////////////////////////
  class SecretData;
  const std::unique_ptr< SecretData > m_SecretData;
////////////////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace ccipd


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef Boost_SERIALIZATION_FOUND


namespace boost
{
namespace serialization
{

  /** Tell boost which version of SIMStorage we are on */
  template< class TPixelType, unsigned int Dimension >
  struct version< ccipd::SIMStorage< TPixelType, Dimension > >
  {
    static const int value = 3;
  }; // struct version

}} // namespaces
////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // Boost_SERIALIZATION_FOUND


#endif // __ccipdSIMStorage_h
