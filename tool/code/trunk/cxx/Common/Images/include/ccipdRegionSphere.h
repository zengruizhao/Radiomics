#ifndef __ccipdRegionSphere_h
#define __ccipdRegionSphere_h



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
class RegionSphere :
  public RegionShape< TDimension >,
  public Factory< RegionSphere< TDimension > >,
  virtual public boost::noncopyable
{


  
//////////////////////////////////////////////////////////////////////////////////////////////////


  ccipdConstructorDeclarationNoSelfMacro( RegionSphere )
public: // typedefs
  typedef RegionSphere< TDimension >               Self;
  typedef RegionShape < TDimension >               Superclass;
  ccipdFactorySuperclassTypedefsTemplatedMacro

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
   * this Sphere is defined.
   * 
   * \note This will not go outside the image.
   * 
   */
  ccipdGetConstDeclareMacro( MaximumRegion, ImageRegionType )

  /**
   * Provide whether or not
   * a point is included
   * in this Sphere.
   */
  virtual bool IncludePointInShape( const PointType & point ) const;

  /**
   * Define the radius of the sphere, in mm.
   * 
   * By default it is 0, so
   * an empty region is returned.
   */
  ccipdSetByvalDeclareMacro( Radius, LengthType )

  /**
   * Define the centroid of the Sphere.
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



}; // class RegionSphere
} // namespace ccipd
#endif // __ccipdRegionSphere_h
