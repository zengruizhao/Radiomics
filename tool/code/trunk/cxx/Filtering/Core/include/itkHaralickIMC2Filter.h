/*=========================================================================
 *
 *  Copyright CCIPD
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the ITK Toolkit Version 4 copyright.
 *
 *  Copyright (c) Ahmad Algohary
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef __itkHaralickIMC2Filter_h
#define __itkHaralickIMC2Filter_h

#include <itkBoxImageFilter.h>
#include <itkVectorContainer.h>

namespace itk
{

  /** \class HaralickIMC2Filter
 * \brief class for HaralickIMC2Filtering Texture Features Extraction,
 *        it takes an N-D image as input and produces Feature images as output
 *
 * BoxImageFilter is the base class for all process objects that output
 * image data and require image data as input. Specifically, this class
 * defines the GetInputRequestedRegion() method for defining the input to a filter.
 *
 * This class supports multithreaded processing of images yet.
 * It provides an implementation of GenerateThreadedData() where the image 
 * processing will run in multiple thread and the superclass is 
 * responsible for allocating its output data images.  
 *
 * BoxImageFilter provides an implementation of
 * GenerateInputRequestedRegion().  The base assumption to this point in the
 * hierarchy for the implementation of GenerateInputRequestedRegion() in this 
 * class is to request an input that does not match the size of the requested 
 * output.  This filter requires more input (it uses neighborhood
 * information). By convention, this implementations calls the Superclass' 
 * method first.
 *
 * Access methods (Setters/Getters) are provided for this class. The default 
 * radius for window size is 1 (i.e. {1, 1, 1} for 3D images). Default value for Offset is x+1 (i.e. {1, 0} for 2D Images 
 * and {1, 0, 0} for 3D Images) and is 256 for Number of Histogram Bins. For the histogram maximum and minimum values,
 * default values are the maximum and minimum pixel/voxel values of the input image.
 *
 * \ingroup ImageFilters
 * \ingroup FetureExtraction
 *
 */
template< class TInputImage, class TOutputImage >
class ITK_EXPORT HaralickIMC2Filter: public BoxImageFilter< TInputImage, TOutputImage >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef          TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer             InputImagePointerType;
  typedef typename InputImageType::ConstPointer        InputImageConstPointerType;
  typedef typename InputImageType::PixelType           InputImagePixelType;
  typedef typename InputImageType::RegionType          InputImageRegionType;
  typedef typename InputImageRegionType::SizeType      InputImageRegionSizeType;
  typedef typename InputImageType::OffsetType          InputImageOffsetType;

  typedef VectorContainer<unsigned int, InputImageOffsetType> InputImageOffsetVector            ;
  typedef typename InputImageOffsetVector::Pointer            InputImageOffsetVectorPointer     ;
  typedef typename InputImageOffsetVector::ConstPointer       InputImageOffsetVectorConstPointer;




  typedef typename InputImageType::IndexType           InputImageIndexType;
  typedef typename InputImageRegionType::IndexType     InputImageRegionIndexType;
  typedef          ImageRegionIterator<InputImageType> InputImageIteratorType;

  typedef          TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer             OutputImagePointerType;
  typedef typename OutputImageType::PixelType           OutputImagePixelType;
  typedef typename OutputImageType::RegionType          OutputImageRegionType;
  typedef typename OutputImageRegionType::SizeType      OutputImageRegionSizeType;
  typedef typename OutputImageType::IndexType           OutputImageIndexType;
  typedef typename OutputImageRegionType::IndexType     OutputImageRegionIndexType;
  typedef          ImageRegionIterator<OutputImageType> OutputImageIteratorType;


//   itkSetMacro(Radius, InputImageRegionSizeType);
//   itkGetMacro(Radius, InputImageRegionSizeType);
  itkSetMacro(Offset, InputImageOffsetType);
  itkGetMacro(Offset, InputImageOffsetType);
  //  itkSetMacro(OffsetVector, InputImageOffsetVectorType);
  //  itkGetMacro(OffsetVector, InputImageOffsetVectorType);
  itkSetMacro(InputImageMinimum, InputImagePixelType);
  itkGetMacro(InputImageMinimum, InputImagePixelType);
  itkSetMacro(InputImageMaximum, InputImagePixelType);
  itkGetMacro(InputImageMaximum, InputImagePixelType);
  itkSetMacro(NumberOfBinsPerAxis, unsigned int);
  itkGetMacro(NumberOfBinsPerAxis, unsigned int);

  void SetInput(InputImagePointerType);

  /** Standard class typedefs. */
  typedef HaralickIMC2Filter                                 Self;
  typedef BoxImageFilter< InputImageType, OutputImageType > Superclass;
  typedef SmartPointer< Self >                              Pointer;
  typedef SmartPointer< const Self >                        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(HaralickIMC2Filter, BoxImageFilter);


//#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
//  itkConceptMacro( InputHasNumericTraitsCheck,  ( Concept::HasNumericTraits< InputPixelType > ) );
  /** End concept checking */
//#endif

protected:
  HaralickIMC2Filter();
  virtual ~HaralickIMC2Filter() {}

  /** HaralickIMC2Filter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa HaralickIMC2Filter::ThreadedGenerateData()
   */
  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId);
  void PrintSelf(std::ostream& os, Indent indent) const;


private:
  HaralickIMC2Filter(const Self &); //purposely not implemented
  void operator=(const Self &);   //purposely not implemented

   // member variables
   // InputImageRegionSizeType  m_Radius; // already in the superclass
   InputImageOffsetType       m_Offset;
   //  InputImageOffsetVectorType m_OffsetVector;
   InputImagePixelType        m_InputImageMinimum;
   InputImagePixelType        m_InputImageMaximum;
   unsigned int               m_NumberOfBinsPerAxis;

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHaralickIMC2Filter.hxx"
#endif

#endif // __itkHaralickIMC2Filter_h

