#ifndef __ccipdNormalDirectionCalculator_h
#define __ccipdNormalDirectionCalculator_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdITKForwardDeclarations.h"
#include "ccipdCloneableFactory.h"
#include "ccipdMacros.h"
#include <itkMacro.h>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Calculate normal directions at given points.
 * 
 * Given a mask, this class will calculate
 * several things for a given point in space:
 *   - The closest point on the mask surface
 *   - The direction to the mask surface
 *   - The distance to the mask
 * 
 * This is called the "normal" direction
 * because if you input points
 * on the surface of the mask,
 * then the outputs would be
 * the normal to the surface.
 * 
 * \author  Rob Toth
 * \date    2013-03-24
 * \ingroup Common
 */
template< unsigned int TDimension >
class NormalDirectionCalculator :
  public CloneableFactory< NormalDirectionCalculator< TDimension > >,
  public Copyable        < NormalDirectionCalculator< TDimension > >
{



//////////////////////////////////////////////////////////////////////////////////////////

  ccipdConstructorDeclarationNoSelfMacro( NormalDirectionCalculator )

public: // typedefs
  typedef NormalDirectionCalculator< TDimension >  Self;
  typedef CloneableFactory< Self >                 SuperclassFactory;

  ccipdFactoryTypedefsTemplatedMacro

  typedef itk::Image< MaskPixelType, TDimension > MaskType;
  typedef itk::SmartPointer<       MaskType >     MaskPointer;
  typedef itk::SmartPointer< const MaskType >     MaskConstPointer;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
public:

  /// You can input the mask at creation
  static Pointer New(
    const MaskConstPointer & mask,
    const bool verbose = false );

  static ConstPointer ConstNew(
    const MaskConstPointer & mask,
    const bool verbose = false );

//////////////////////////////////////////////////////////////////////////////////////////

  

protected:
//////////////////////////////////////////////////////////////////////////////////////////
  virtual void CopyParametersInternal( const Self & other );
//////////////////////////////////////////////////////////////////////////////////////////



public: // set/get
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * \brief Set the mask.
   * 
   * This will set the mask
   * to use for calculations of the surfaces.
   */
  ccipdSetByrefDeclareMacro( Mask, MaskConstPointer )
  ccipdGetConstDeclareMacro( Mask, MaskConstPointer )

  virtual void ClearMask();

  ccipdSetByvalDeclareMacro( Verbose, bool )
  ccipdGetConstDeclareMacro( Verbose, bool )
  itkBooleanMacro          ( Verbose )

  /// Get the signed minimum distance to the surface from a point (negative if in mask)
  template< typename TPrecision >
  TPrecision GetSignedDistanceToSurface(
    const itk::Point< TPrecision, TDimension > & point );

  template< typename TPrecision >
  TPrecision GetSignedDistanceToSurface(
    const itk::Point< TPrecision, TDimension > & point ) const;

  /**
   * \brief Get the normalized direction to the closest surface point
   * 
   * The output vector will have a norm of 1.
   * 
   * However, if there are any issues
   * (such as the point being outside
   * the image, or the mask not being input),
   * then the output will have a norm of 0.
   * 
   * The output will always be
   * pointing away from the
   * closest surface if the
   * point is outside, or
   * towards the closest surface
   * if the point is inside
   * the surface.
   * Essentially, think about it
   * like the direction is a ray
   * of light from the centroid
   * of the object pointing
   * towards, and then through,
   * the surface.
   * So while inside, that
   * ray points towards the object
   * surface, while outside
   * that ray is pointing towards
   * the edge of the image.
   * 
   */
  template< typename TPrecision >
  itk::CovariantVector< TPrecision, TDimension >
  GetNormalDirection(
    const itk::Point< TPrecision, TDimension > & point );

  template< typename TPrecision >
  itk::CovariantVector< TPrecision, TDimension >
  GetNormalDirection(
    const itk::Point< TPrecision, TDimension > & point ) const;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class NormalDirectionCalculator
} // namespace ccipd
#endif // __ccipdNormalDirectionCalculator_h
