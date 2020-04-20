#ifndef __itkInterpolateMultipleImagesFunction_h
#define __itkInterpolateMultipleImagesFunction_h

#include "itkInterpolateImageFunction.h"

namespace itk
{
/** \class InterpolateMultipleImagesFunction
 * \brief Linearly interpolate an image at specified positions.
 *
 * InterpolateMultipleImagesFunction linearly interpolates image intensity at
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
class InterpolateMultipleImagesFunction:
  public InterpolateImageFunction< TInputImage, TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef InterpolateMultipleImagesFunction                  Self;
  typedef InterpolateImageFunction< TInputImage, TCoordRep > Superclass;
  typedef SmartPointer< Self >                               Pointer;
  typedef SmartPointer< const Self >                         ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(InterpolateMultipleImagesFunction, InterpolateImageFunction);

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
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Index typedef support. */
  typedef typename Superclass::IndexType      IndexType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename ContinuousIndexType::ValueType  InternalComputationType;

  /** Evaluate the function at a ContinuousIndex position
   *
   * Returns the linearly interpolated image intensity at a
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual inline OutputType EvaluateAtContinuousIndexes(const
                                                      std::vector<ContinuousIndexType> &
                                                      indexes) const
  {
    return this->EvaluateUnoptimized(indexes);
  }

  /** Evaluate the function at a ContinuousIndex position
   *
   * Returns the linearly interpolated image intensity at a
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual inline OutputType EvaluateAtContinuousIndex(const
                                                      ContinuousIndexType &
                                                      itkNotUsed(index)) const
  {
    return static_cast<OutputType>(0); 
  }
  /** AddInputImage */ 
  void AddInputImage(const TInputImage * imgPtr );

  /** CrearInput */ 
  void ClearInputImages();

  /** SetInputImage */ 
  void SetInputImage(const int index, const TInputImage * imgPtr );

  /** GetInputImage */ 
  const TInputImage * GetInputImage(const int index ) const;

  /** GetInputImage */ 
  const TInputImage * GetInputImage() const;

protected:
  InterpolateMultipleImagesFunction();
  ~InterpolateMultipleImagesFunction();
  void PrintSelf(std::ostream & os, Indent indent) const;

  std::vector<InputImageConstPointer> m_Images;

  /** Cache some values for testing if indices are inside buffered region. */
  std::vector<IndexType> m_StartIndex;
  std::vector<IndexType> m_EndIndex;

  std::vector<ContinuousIndexType> m_StartContinuousIndex;
  std::vector<ContinuousIndexType> m_EndContinuousIndex;

  /** Number of neighbors used in the interpolation */
  static const unsigned long m_Neighbors;

private:
  InterpolateMultipleImagesFunction(const Self &); //purposely not implemented
  void operator=(const Self &);                    //purposely not implemented

  struct DispatchBase {};
  template< unsigned int >
  struct Dispatch: public DispatchBase {};

  inline OutputType EvaluateOptimized(const DispatchBase &,
                                      const ContinuousIndexType & index) const
  {
    return this->EvaluateUnoptimized(index);
  }

  virtual inline OutputType EvaluateUnoptimized(
    const std::vector<ContinuousIndexType> & indexes) const;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInterpolateMultipleImagesFunction.hxx"
#endif

#endif
