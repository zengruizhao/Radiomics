#ifndef __ccipdRegionShape_h
#define __ccipdRegionShape_h



//////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdEnableSharedFromThis.h"
#include "ccipdMacros.h"
#include "ccipdITKForwardDeclarations.h"
#include "ccipdCore.h"

// boost includes
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Define an image region with an arbitrary shape.
 * 
 * \date    2013-02-19
 * \author  Rob Toth
 * \ingroup Common
 */
template< unsigned int TDimension >
class RegionShape :
  public EnableSharedFromThis< RegionShape< TDimension > >,
  virtual public boost::noncopyable
{


  
//////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( RegionShape )
  typedef RegionShape< TDimension > Self;

public: // typedefs
  typedef EnableSharedFromThis< Self >             SuperclassPointer;
  typedef typename SuperclassPointer::Pointer      Pointer;
  typedef typename SuperclassPointer::ConstPointer ConstPointer;

  typedef itk::Point< TransformValueType, TDimension > PointType;
  typedef itk::ImageRegion< TDimension >               ImageRegionType;
  typedef itk::ImageBase  < TDimension >               ImageType;
  typedef itk::SmartPointer<       ImageType >         ImagePointer;
  typedef itk::SmartPointer< const ImageType >         ImageConstPointer;
  
//////////////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Provide the maximum
   * image region over which
   * this shape is defined.
   */
  virtual ImageRegionType GetMaximumRegion() const = 0;

  /**
   * Provide whether or not
   * a point is included
   * in this shape.
   */
  virtual bool IncludePointInShape( const PointType & point ) const = 0;

  /**
   * Store the image on which this region is defined.
   */
  ccipdSetByvalDeclareMacro( Image, ImageConstPointer )

  ccipdGetConstDeclareMacro( Image, ImageConstPointer )

//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////////////



}; // class RegionShape
} // namespace ccipd
#endif // __ccipdRegionShape_h
