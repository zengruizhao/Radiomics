#ifndef __itkMeanVectorImageFilter_h
#define __itkMeanVectorImageFilter_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  // itk include
  #include <itkBoxImageFilter.h>
  #include <itkImage.h>
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
/** \class MeanVectorImageFilter
 * \brief Applies a Median filter to an vector image
 *
 * Computes an image where a given pixel is the value representing the
 * median of the pixels in a neighborhood (including all components of the vector image) about the corresponding input pixel.
 *
 * \author  Mirabela Rusu
 * \ingroup Filtering
 *
 */
template< class TInputImage, class TOutputImage >
class MeanVectorImageFilter:
	public BoxImageFilter< TInputImage, TOutputImage >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Standard class typedefs. */
  typedef MeanVectorImageFilter                                 Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef SmartPointer< Self >                                  Pointer;
  typedef SmartPointer< const Self >                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MeanVectorImageFilter, BoxImageFilter);

  /** Image typedef support. */
  typedef typename InputImageType::PixelType  InputVectorType;
  typedef typename InputVectorType::ComponentType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename NumericTraits< InputPixelType >::RealType InputRealType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::Pointer OutputImagePointer;
  
  typedef typename InputImageType::SizeType InputSizeType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( SameDimensionCheck,
                   ( Concept::SameDimension< InputImageDimension, OutputImageDimension > ) );
  itkConceptMacro( InputConvertibleToOutputCheck,
                   ( Concept::Convertible< InputPixelType, OutputPixelType > ) );
  itkConceptMacro( InputLessThanComparableCheck,
                   ( Concept::LessThanComparable< InputPixelType > ) );
  /** End concept checking */
#endif

  /* user defined factor allows to use the filter as frequency */
  void SetMultiplicationFactor (float factor); 

  /* user defined factor allows to use the filter as frequency */
  void SetMultiplicationFactorImage (OutputImageType * image); 

  void CreateMultiplicationFactorImage();

protected:
  MeanVectorImageFilter();
  virtual ~MeanVectorImageFilter() {}

  /** MeanImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            ThreadIdType threadId);

  /** If an imaging filter needs to perform processing after the buffer
   * has been allocated but before threads are spawned, the filter can
   * can provide an implementation for BeforeThreadedGenerateData(). The
   * execution flow in the default GenerateData() method will be:
   *      1) Allocate the output buffer
   *      2) Call BeforeThreadedGenerateData()
   *      3) Spawn threads, calling ThreadedGenerateData() in each thread.
   *      4) Call AfterThreadedGenerateData()
   * Note that this flow of control is only available if a filter provides
   * a ThreadedGenerateData() method and NOT a GenerateData() method. */
  virtual void BeforeThreadedGenerateData();

private:
  MeanVectorImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);    //purposely not implemented

  OutputImagePointer m_MultiplicationFactorImage;
  float              m_MultiplicationFactor;

  bool m_WasFactorImageAssigned;

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMeanVectorImageFilter.hxx"
#endif

#endif
