#ifndef __ccipdRegionCube_h
#define __ccipdRegionCube_h



//////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegionShape.h"
#include "ccipdFactory.h"

//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Define a cuboid image region.
 * 
 * \date    2013-02-19
 * \author  Rob Toth
 * \ingroup Common
 */
template< unsigned int TDimension >
class RegionCube :
  public RegionShape< TDimension >,
  public Factory< RegionCube< TDimension > >,
  virtual public boost::noncopyable
{



//////////////////////////////////////////////////////////////////////////////////////////////////
public:
  typedef RegionCube< TDimension > Self;
  ccipdConstructorDeclarationNoSelfMacro( RegionCube )
  ccipdFactorySuperclassTypedefsTemplatedMacro

public: // typedefs
  typedef RegionShape< TDimension >              Superclass;
  typedef typename Superclass::PointType         PointType;
  typedef typename Superclass::ImageRegionType   ImageRegionType;
  typedef typename Superclass::ImageType         ImageType;
  typedef typename Superclass::ImagePointer      ImagePointer;
  typedef typename Superclass::ImageConstPointer ImageConstPointer;
  typedef typename PointType::ValueType          LengthType;

//////////////////////////////////////////////////////////////////////////////////////////////////




public:
//////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Provide the maximum
   * image region over which
   * this cube is defined.
   * 
   * \note This will not go outside the image.
   * 
   */
  ccipdGetConstDeclareMacro( MaximumRegion, ImageRegionType )

  /**
   * Provide whether or not
   * a point is included
   * in this cube.
   */
  virtual bool IncludePointInShape( const PointType & point ) const;

  /**
   * Define half the distance to the centroid
   * for a given side.
   * 
   * The cube is defined as the centroid
   * +/- this distance.
   * 
   * By default it is 0, so
   * an empty region is returned.
   */
  ccipdSetByvalDeclareMacro( HalfCubeLength, LengthType )

  /**
   * Define the centroid of the cube.
   * 
   * default = { 0 }
   * 
   */
  ccipdSetByrefDeclareMacro( Centroid, PointType )

  /**
   * The region is recomputed when the image is input.
   */
  ccipdSetByvalDeclareMacro( Image, ImageConstPointer )

//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////////////



}; // class RegionCube
} // namespace ccipd
#endif // __ccipdRegionCube_h
