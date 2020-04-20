#ifndef __itkMaskStandardDeviationImageFilter_h
#define __itkMaskStandardDeviationImageFilter_h




//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  #include "itkImageToImageFilter.h"
  #include "itkMacro.h"
  #include "itkArray.h"
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



/**
 * \brief Mean Absolute Distance segmentation accuracy measure.
 * 
 * Modeled after itkSimilarityIndexImageFilter
 * 
 * \author  Rob Toth
 * \date    2012-02-02
 * \ingroup Evaluation
 */
template< class TInputImage1, class TInputImage2 = TInputImage1 >
class MaskStandardDeviationImageFilter:
  public ImageToImageFilter< TInputImage1, TInputImage1 >
{



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Standard Self typedef */
  typedef MaskStandardDeviationImageFilter                 Self;
  typedef ImageToImageFilter< TInputImage1, TInputImage1 > Superclass;
  typedef SmartPointer< Self >                             Pointer;
  typedef SmartPointer< const Self >                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Runtime information support. */
  itkTypeMacro( MaskStandardDeviationImageFilter, ImageToImageFilter );

  /** Image related typedefs. */
  typedef TInputImage1                        DisplacementFieldType;
  typedef TInputImage2                        MaskImageType;
  typedef typename TInputImage1::Pointer      DisplacementFieldPointer;
  typedef typename TInputImage2::Pointer      MaskImagePointer;
  typedef typename TInputImage1::ConstPointer DisplacementFieldConstPointer;
  typedef typename TInputImage2::ConstPointer MaskImageConstPointer;

  typedef typename TInputImage1::RegionType RegionType;
  typedef typename TInputImage1::SizeType   SizeType;
  typedef typename TInputImage1::IndexType  IndexType;

  typedef typename TInputImage1::PixelType DisplacementFieldPixelType;
  typedef typename TInputImage2::PixelType MaskImagePixelType;

  /** Image related typedefs. */
  itkStaticConstMacro( ImageDimension, unsigned int,
                       TInputImage1::ImageDimension );
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Type to use form computations. */
  typedef typename NumericTraits< MaskImageType >::RealType RealType;

  /** Set the first input. */
  void SetDisplacementField( const DisplacementFieldType *image)
  {
  this->SetInput(image);
  }
  
  /** Set the second input. */
  void SetMask( const MaskImageType *image);

  /** Get the first input. */
  const DisplacementFieldType * GetDisplacementField()
  { return this->GetInput(); }

  /** Get the second input. */
  const MaskImageType * GetMask();

  itkSetMacro( Radius, SizeValueType );
  itkGetConstMacro( Radius, SizeValueType );

  itkSetMacro( Output, MaskImagePointer );
  itkGetConstMacro( Output, MaskImagePointer );  

  itkGetConstMacro( MaskStandardDeviation, RealType );

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( Input1HasNumericTraitsCheck,
                   ( Concept::HasNumericTraits< DisplacementFieldPixelType > ) );
  itkConceptMacro( Input2HasNumericTraitsCheck,
                   ( Concept::HasNumericTraits< MaskImagePixelType > ) );
  /** End concept checking */
#endif
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  MaskStandardDeviationImageFilter();
  ~MaskStandardDeviationImageFilter(){}
  void PrintSelf(std::ostream & os, Indent indent) const;

  /** Pass the input through unmodified. Do this by Grafting in the
   * AllocateOutputs method. */
  //void AllocateOutputs();

  /** Initialize some accumulators before the threads run. */
  void BeforeThreadedGenerateData();

  /** Do final mean and variance computation from data accumulated in threads.
    */
  void AfterThreadedGenerateData();

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData( const RegionType &
                              outputRegionForThread,
                              ThreadIdType threadId);

  // Override since the filter needs all the data for the algorithm
  void GenerateInputRequestedRegion();

  // Override since the filter produces all of its output
  void EnlargeOutputRequestedRegion(DataObject *data);

  //////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  MaskStandardDeviationImageFilter(const Self &); //purposely not implemented
  void operator=                 (const Self &); //purposely not implemented

  RealType m_MaskStandardDeviation;

  //DisplacementFieldPointer m_DisplacementField;

  //MaskImagePointer m_Mask;

  MaskImagePointer m_Output;

  RegionType m_MaskBoundingBox;

  SizeValueType m_Radius;

  SizeValueType m_CountMaskPixels;

//////////////////////////////////////////////////////////////////////////////////////////



}; // end of class
} // end namespace itk



#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMaskStandardDeviationImageFilter.hxx"
#endif



#endif // __itkMaskStandardDeviationImageFilter_h
