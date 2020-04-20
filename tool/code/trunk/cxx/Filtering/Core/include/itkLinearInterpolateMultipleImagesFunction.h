#ifndef __itkLinearInterpolateMultipleImagesFunction_h
#define __itkLinearInterpolateMultipleImagesFunction_h

#include "itkInterpolateMultipleImagesFunction.h"

namespace itk
{
/** \class LinearInterpolateMultipleImagesFunction
 * \brief Linearly interpolate an image at specified positions.
 *
 * LinearInterpolateMultipleImagesFunction linearly interpolates image intensity at
 * a non-integer pixel position. This class is templated
 * over the input image type and the coordinate representation type
 * (e.g. float or double).
 *
 * This function works for N-dimensional images.
 *
 * This function works for images with scalar and vector pixel
 * types, and for images of type VectorImage.
 *
 */
template< typename TInputImage, typename TCoordRep = double >
class LinearInterpolateMultipleImagesFunction:
  public InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef LinearInterpolateMultipleImagesFunction            Self;
  typedef InterpolateImageFunction< TInputImage, TCoordRep > Superclass;
  typedef SmartPointer< Self >                               Pointer;
  typedef SmartPointer< const Self >                         ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LinearInterpolateMultipleImagesFunction, InterpolateImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** InputImageType typedef support. */
  typedef typename Superclass::InputImageType InputImageType;  

  /** CoordRepType typedef support. */
  typedef TCoordRep CoordRepType;

  /** InputImagePointer typedef support */
  typedef typename InputImageType::ConstPointer InputImageConstPointer;

  /** InputPixelType typedef support. */
  typedef typename Superclass::InputPixelType InputPixelType;

  /** RealType typedef support. */
  typedef typename Superclass::RealType RealType;

  /** Dimension underlying input image. */
  //itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Index typedef support. */
  typedef typename Superclass::IndexType      IndexType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename ContinuousIndexType::ValueType  InternalComputationType;

protected:
  LinearInterpolateMultipleImagesFunction(){};
  ~LinearInterpolateMultipleImagesFunction(){};

private:
  LinearInterpolateMultipleImagesFunction(const Self &);//purposely not implemented
  void operator=(const Self &);                         //purposely not implemented

  virtual inline OutputType EvaluateUnoptimized(
    const std::vector<ContinuousIndexType> & indexes) const;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLinearInterpolateMultipleImagesFunction.hxx"
#endif

#endif
