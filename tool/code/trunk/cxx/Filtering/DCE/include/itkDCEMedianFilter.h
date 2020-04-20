#ifndef __itkDCEMedianFilter_h
#define __itkDCEMedianFilter_h



////////////////////////////////////////////////////////////////////////////////////////////////////

  // itk include
  #include <itkBoxImageFilter.h>
  #include <itkImage.h>
  #include <itkVectorImage.h>

////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
/** \class DCEFeaturesFilter
 * \brief Applies a DCE Feature Extraction filter to a given vector image
 *  The vector image is a 4D image or a Time series where each point is a 3D Volume image
 * \author  Ahmad Algohary
 * \ingroup Filtering
 *
 */
template< class TInputImage, class TOutputImage >
class DCEMedianFilter: public BoxImageFilter< TInputImage, TOutputImage >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension , unsigned int, TInputImage::ImageDimension );
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Standard class typedefs. */
  typedef DCEMedianFilter                                          Self        ;
  typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass  ;
  typedef SmartPointer< Self >                                  Pointer     ;
  typedef SmartPointer< const Self >                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DCEMedianFilter, BoxImageFilter);

  /** Image typedef support. */
  typedef typename InputImageType::PixelType                 InputVectorType;
  typedef typename InputVectorType::ComponentType            InputPixelType ;
  typedef typename OutputImageType::PixelType                OutputPixelType;
  typedef typename NumericTraits< InputPixelType >::RealType InputRealType  ;

  typedef typename InputImageType::RegionType  InputImageRegionType ;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  
  typedef typename InputImageType::SizeType InputSizeType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( SameDimensionCheck, ( Concept::SameDimension< InputImageDimension, OutputImageDimension > ) );
  itkConceptMacro( InputConvertibleToOutputCheck, ( Concept::Convertible< InputPixelType, OutputPixelType > ) );
  itkConceptMacro( InputLessThanComparableCheck, ( Concept::LessThanComparable< InputPixelType > ) );
  /** End concept checking */
#endif


protected:
  DCEMedianFilter();
  virtual ~DCEMedianFilter() {}
  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId);


  typename TOutputImage::PixelType GetMax(const std::vector<OutputPixelType> & vec);
  typename TOutputImage::PixelType GetMean(const std::vector<OutputPixelType> & vec);
  typename TOutputImage::PixelType GetMin(const std::vector<OutputPixelType> & vec);
  typename TOutputImage::PixelType GetMedian(/*const*/ std::vector<OutputPixelType> /*&*/ vec);

  typename TOutputImage::PixelType GetRange(const std::vector<OutputPixelType> & vec);
  typename TOutputImage::PixelType GetSum(const std::vector<OutputPixelType> & vec);
  typename TOutputImage::PixelType GetStdDeviation(const std::vector<OutputPixelType> & vec);
  typename TOutputImage::PixelType GetVariance(const std::vector<OutputPixelType> & vec);



private:
  DCEMedianFilter(const Self &); //purposely not implemented
  void operator=(const Self &);    //purposely not implemented

   // member variables
  const static unsigned int   m_NumberOfSupportedDCEFeatures = 1; // has to be modified when adding new DCE Features

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
	#include "itkDCEMedianFilter.hxx"
#endif

#endif
