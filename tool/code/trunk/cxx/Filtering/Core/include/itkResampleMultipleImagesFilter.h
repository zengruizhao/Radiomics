#ifndef __itkResampleMultipleImagesFilter_h
#define __itkResampleMultipleImagesFilter_h

#include "itkTransform.h"
#include "itkImageRegionIterator.h"
#include "itkImageFilterMultipleInputs.h"
#include "itkInterpolateMultipleImagesFunction.h"
//#include "itkInterpolateImageFunction.h"
#include "itkSize.h"

namespace itk
{
/** \class ResampleMultipleImagesFilter
 * \brief Resample several images via their coordinate transform
 *
 * Since this filter produces an image which is a different size than
 * its input, it needs to override several of the methods defined
 * in ProcessObject in order to properly manage the pipeline execution model.
 * In particular, this filter overrides
 * ProcessObject::GenerateInputRequestedRegion() and
 * ProcessObject::GenerateOutputInformation().
 *
 * \author Mirabela Rusu
 * \lastupdate 02/23/2014
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType = double >
class ResampleMultipleImagesFilter:
  public ImageFilterMultipleInputs< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef ResampleMultipleImagesFilter                       Self;
  typedef ImageFilterMultipleInputs< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  typedef TInputImage                           InputImageType;
  typedef TOutputImage                          OutputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ResampleMultipleImagesFilter, ImageFilterMultipleInputs);

  /** Number of dimensions. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Transform typedef.
   *
   * \todo Check that input and output images have the same number of
   * dimensions; this is required by the current implementation of
   * AffineTransform. */
  typedef Transform< TInterpolatorPrecisionType, itkGetStaticConstMacro(ImageDimension),
                     itkGetStaticConstMacro(ImageDimension) > TransformType;
  typedef typename TransformType::ConstPointer
  TransformPointerType;

  /** Interpolator typedef. */
  typedef InterpolateMultipleImagesFunction< InputImageType, TInterpolatorPrecisionType > InterpolatorType;
  typedef typename InterpolatorType::Pointer                                     InterpolatorPointerType;

  /** Image size typedef. */
  typedef Size< itkGetStaticConstMacro(ImageDimension) > SizeType;

  /** Image index typedef. */
  typedef typename TOutputImage::IndexType IndexType;

  /** Image point typedef. */
  typedef typename InterpolatorType::PointType PointType;

  /** Image pixel value typedef. */
  typedef typename TOutputImage::PixelType PixelType;

  /** Typedef to describe the output image region type. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** Image spacing,origin and direction typedef */
  typedef typename TOutputImage::SpacingType   SpacingType;
  typedef typename TOutputImage::PointType     OriginPointType;
  typedef typename TOutputImage::DirectionType DirectionType;

  /** Set the interpolator function.  The default is
   * itk::LinearInterpolateImageFunction<InputImageType, TInterpolatorPrecisionType>.  */
  itkSetObjectMacro(Interpolator, InterpolatorType);

  /** Get a pointer to the interpolator function. */
  itkGetModifiableObjectMacro(Interpolator, InterpolatorType);

  /** Set the size of the output image. */
  itkSetMacro(Size, SizeType);

  /** Get the size of the output image. */
  itkGetConstReferenceMacro(Size, SizeType);

  /** Set the pixel value when a transformed pixel is outside of the
   * image.  The default default pixel value is 0. */
  itkSetMacro(DefaultPixelValue, PixelType);

  /** Get the pixel value when a transformed pixel is outside of the image */
  itkGetConstMacro(DefaultPixelValue, PixelType);

  /** Set the output image spacing. */
  itkSetMacro(OutputSpacing, SpacingType);
  virtual void SetOutputSpacing(const double *values);

  /** Get the output image spacing. */
  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  /** Set the output image origin. */
  itkSetMacro(OutputOrigin, OriginPointType);
  virtual void SetOutputOrigin(const double *values);

  /** Get the output image origin. */
  itkGetConstReferenceMacro(OutputOrigin, OriginPointType);

  /** Set the output direciton cosine matrix. */
  itkSetMacro(OutputDirection, DirectionType);
  itkGetConstReferenceMacro(OutputDirection, DirectionType);

  /** Set the start index of the output largest possible region.
   * The default is an index of all zeros. */
  itkSetMacro(OutputStartIndex, IndexType);

  /** Get the start index of the output largest possible region. */
  itkGetConstReferenceMacro(OutputStartIndex, IndexType);

  /** ResampleMultipleImagesFilter produces an image which is a different size
   * than its input.  As such, it needs to provide an implementation
   * for GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below. \sa ProcessObject::GenerateOutputInformaton() */
  virtual void GenerateOutputInformation();

  /** ResampleMultipleImagesFilter needs a different input requested region than
   * the output requested region.  As such, ResampleMultipleImagesFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   * \sa ProcessObject::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion();

  /** This method is used to set the state of the filter before
   * multi-threading. */
  virtual void BeforeThreadedGenerateData();

  /** This method is used to set the state of the filter after
   * multi-threading. */
  virtual void AfterThreadedGenerateData();

  /** Method Compute the Modified Time based on changed to the components. */
  ModifiedTimeType GetMTime(void) const;

  /** set the input */
  virtual void SetInput(const int index, const TInputImage* image);

  /** Set Nth Transform */
  void SetTransform( const int index, const TransformType * transf );

protected:
  ResampleMultipleImagesFilter();
  ~ResampleMultipleImagesFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

  /** ResampleMultipleImagesFilter can be implemented as a multithreaded filter.  Therefore,
   * this implementation provides a ThreadedGenerateData() routine which
   * is called for each processing thread. The output image data is allocated
   * automatically by the superclass prior to calling ThreadedGenerateData().
   * ThreadedGenerateData can only write to the portion of the output image
   * specified by the parameter "outputRegionForThread"
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            ThreadIdType threadId);

private:
  ResampleMultipleImagesFilter(const Self &); //purposely not implemented
  void operator=(const Self &);            //purposely not implemented

  SizeType                m_Size;       // Size of the output image
  std::vector<TransformPointerType>    m_Transforms;  // Coordinate transform to use
  InterpolatorPointerType m_Interpolator;
  // Image function for interpolation
  PixelType m_DefaultPixelValue;
  // default pixel value if the point
  // is outside the image
  SpacingType       m_OutputSpacing;           // output image spacing
  OriginPointType   m_OutputOrigin;            // output image origin
  DirectionType     m_OutputDirection;         // output image direction cosines
  IndexType         m_OutputStartIndex;        // output start index
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkResampleMultipleImagesFilter.hxx"
#endif

#endif
