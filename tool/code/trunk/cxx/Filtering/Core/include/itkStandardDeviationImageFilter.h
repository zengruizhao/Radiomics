#ifndef __itkStandardDeviationImageFilter_h
#define __itkStandardDeviationImageFilter_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  // itk include
  #include <itkBoxImageFilter.h>
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
/** \class StandardDeviationImageFilter
 * \brief Applies a standard deviation filter to an image
 *
 * Computes an image where a given pixel is the value representing the
 * standard deviation of the pixels in a neighborhood about the 
 * corresponding input pixel.
 *
 * A standard filter is one of the family of nonlinear filters.  
 * 
 * \author  Rachel Sparks
 * \ingroup Filtering
 *
 */
template< class TInputImage, class TOutputImage >
class StandardDeviationImageFilter:
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
  typedef StandardDeviationImageFilter                          Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef SmartPointer< Self >                                  Pointer;
  typedef SmartPointer< const Self >                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StandardDeviationImageFilter, BoxImageFilter);

  /** Image typedef support. */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename NumericTraits< InputPixelType >::RealType InputRealType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  
  typedef typename InputImageType::SizeType InputSizeType;

protected:
  StandardDeviationImageFilter();
  virtual ~StandardDeviationImageFilter() {}

  /** StandardDeviation can be implemented as a multithreaded filter.
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

private:
  StandardDeviationImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);    //purposely not implemented
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkStandardDeviationImageFilter.hxx"
#endif

#endif