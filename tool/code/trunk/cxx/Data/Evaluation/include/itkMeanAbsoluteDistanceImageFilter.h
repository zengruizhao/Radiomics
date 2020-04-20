#ifndef __itkMeanAbsoluteDistanceImageFilter_h
#define __itkMeanAbsoluteDistanceImageFilter_h




//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  #include <itkImageToImageFilter.h>
  #include <itkMacro.h>
  #include <itkArray.h>
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
class MeanAbsoluteDistanceImageFilter:
  public ImageToImageFilter< TInputImage1, TInputImage1 >
{



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Standard Self typedef */
  typedef MeanAbsoluteDistanceImageFilter                  Self;
  typedef ImageToImageFilter< TInputImage1, TInputImage1 > Superclass;
  typedef SmartPointer< Self >                             Pointer;
  typedef SmartPointer< const Self >                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Runtime information support. */
  itkTypeMacro( MeanAbsoluteDistanceImageFilter, ImageToImageFilter );

  /** Image related typedefs. */
  typedef TInputImage1                        SegmentationImageType;
  typedef TInputImage2                        GroundTruthImageType;
  typedef typename TInputImage1::Pointer      SegmentationImagePointer;
  typedef typename TInputImage2::Pointer      GroundTruthImagePointer;
  typedef typename TInputImage1::ConstPointer SegmentationImageConstPointer;
  typedef typename TInputImage2::ConstPointer GroundTruthImageConstPointer;

  typedef typename TInputImage1::RegionType RegionType;
  typedef typename TInputImage1::SizeType   SizeType;
  typedef typename TInputImage1::IndexType  IndexType;

  typedef typename TInputImage1::PixelType SegmentationImagePixelType;
  typedef typename TInputImage2::PixelType GroundTruthImagePixelType;

  /** Image related typedefs. */
  itkStaticConstMacro( ImageDimension, unsigned int,
                       TInputImage1::ImageDimension );
//////////////////////////////////////////////////////////////////////////////////////////
// typedefs
  typedef Image< double, ImageDimension >     LevelsetImageType;
  typedef typename LevelsetImageType::ConstPointer LevelsetImageConstPointer;



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Type to use form computations. */
  typedef typename NumericTraits< SegmentationImageType >::RealType RealType;

  /** Set the first input. */
  void SetSegmentation( const SegmentationImageType *image)
  { this->SetInput(image);}

  /** set the already computed levelset */
  void SetSegmentationLevelset(LevelsetImageType  *levelset);
 
  /** set the already computed levelset */
  void SetGroundTruthLevelset(LevelsetImageType  *levelset);
  
  /** Set the second input. */
  void SetGroundTruth( const GroundTruthImageType *image);

  /** Get the first input. */
  const SegmentationImageType * GetSegmentation()
  { return this->GetInput(); }

  /** Get the secong input. */
  const GroundTruthImageType * GetGroundTruth();

  /** compute level sets?? */
  void SetComputeLevelsets( const bool compute = true )
  {m_ComputeLevelsets = compute;};

  /** Return the computed similarity index. */
  itkGetConstMacro( MeanAbsoluteDistance, RealType );

  // Set the thickness of the border for which
  // to compute the mean absolute data
  itkSetMacro     ( SegmentationBorderThickness, RealType );
  itkGetConstMacro( SegmentationBorderThickness, RealType );
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( Input1HasNumericTraitsCheck,
                   ( Concept::HasNumericTraits< SegmentationImagePixelType > ) );
  itkConceptMacro( Input2HasNumericTraitsCheck,
                   ( Concept::HasNumericTraits< GroundTruthImagePixelType > ) );
  /** End concept checking */
#endif
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  MeanAbsoluteDistanceImageFilter();
  ~MeanAbsoluteDistanceImageFilter(){}
  void PrintSelf(std::ostream & os, Indent indent) const;

  /** Pass the input through unmodified. Do this by Grafting in the
   * AllocateOutputs method. */
  void AllocateOutputs();

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
  MeanAbsoluteDistanceImageFilter(const Self &); //purposely not implemented
  void operator=                 (const Self &); //purposely not implemented

  LevelsetImageConstPointer
    m_SegmentationLevelset,
    m_GroundTruthLevelset;

  RealType
    m_MeanAbsoluteDistance,
    m_SegmentationBorderThickness;

  //should the levelsets be computed ?
  bool m_ComputeLevelsets;

  // should return the squared Distances ()
  bool m_SquaredDistances;

  Array< RealType      > m_AbsoluteDistanceSums;
  Array< SizeValueType > m_CountBorderPixels;

//////////////////////////////////////////////////////////////////////////////////////////



}; // end of class
} // end namespace itk



#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMeanAbsoluteDistanceImageFilter.hxx"
#endif



#endif // __itkMeanAbsoluteDistanceImageFilter_h
