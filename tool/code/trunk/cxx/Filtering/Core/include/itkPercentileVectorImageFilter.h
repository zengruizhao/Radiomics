#ifndef __itkPercentileVectorImageFilter_h
#define __itkPercentileVectorImageFilter_h


#include "ccipdCore.h" 

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
/** \class PercentileVectorImageFilter
 * \brief Applies a Percentile filter to an vector image
 *
 * Computes an image where a given pixel is the value representing the
 * median of the pixels in a neighborhood (including all components of the vector image) about the corresponding input pixel.
 *
 * \author  Mirabela Rusu
 * \ingroup Filtering
 *
 */
template< class TInputImage, class TOutputImage >
class PercentileVectorImageFilter:
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
  typedef PercentileVectorImageFilter                             Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef SmartPointer< Self >                                  Pointer;
  typedef SmartPointer< const Self >                            ConstPointer;
  typedef float                                                 RealType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PercentileVectorImageFilter, BoxImageFilter);

  /** Image typedef support. */
  typedef typename InputImageType::PixelType  InputVectorType;
  typedef typename InputVectorType::ComponentType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename NumericTraits< InputPixelType >::RealType InputRealType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  
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
 
 /*
  * does error check to see if percentile is between 0 and 1
  */
 void SetPercentile( RealType percentile);

 /**/
 RealType GetPercentile() { return m_percentile;};

protected:
  PercentileVectorImageFilter();
  virtual ~PercentileVectorImageFilter() {}

  /** PercentileImageFilter can be implemented as a multithreaded filter.
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
  PercentileVectorImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);    //purposely not implemented

  RealType m_percentile; // number between 0 and 1
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkPercentileVectorImageFilter.hxx"
#endif

#endif
