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
#ifndef __itkGaborFilter_h
#define __itkGaborFilter_h

#include <itkFFTConvolutionImageFilter.h>


namespace itk
{

  /** \class GaborFilter
 * \brief class for Gabor Texture Features Extraction,
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

template< class TInputImage, class TKernelImage = TInputImage, class TOutputImage = TInputImage, class TInternalPrecision = double>
class ITK_EXPORT GaborFilter: public FFTConvolutionImageFilter< TInputImage, TKernelImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef GaborFilter                                                        Self;
  typedef FFTConvolutionImageFilter<TInputImage, TKernelImage, TOutputImage> Superclass;
  typedef SmartPointer< Self >                                               Pointer;
  typedef SmartPointer< const Self >                                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GaborFilter, FFTConvolutionImageFilter);

  /** Dimensionality of input and output data is assumed to be the same. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension,  unsigned int, TInputImage ::ImageDimension);
  itkStaticConstMacro(KernelImageDimension, unsigned int, TKernelImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  ///** Convenient typedefs for simplifying declarations. */
  //typedef          TInputImage                         InputImageType;
  //typedef typename InputImageType::Pointer             InputImagePointerType;
  //typedef typename InputImageType::ConstPointer        InputImageConstPointerType;
  //typedef typename InputImageType::PixelType           InputImagePixelType;
  //typedef typename InputImageType::RegionType          InputImageRegionType;
  //typedef typename InputImageRegionType::SizeType      InputImageRegionSizeType;
  //typedef typename InputImageType::OffsetType          InputImageOffsetType;
  //typedef typename InputImageType::IndexType           InputImageIndexType;
  //typedef typename InputImageRegionType::IndexType     InputImageRegionIndexType;
  //typedef          ImageRegionIterator<InputImageType> InputImageIteratorType;

  //typedef          TOutputImage                         OutputImageType;
  //typedef typename OutputImageType::Pointer             OutputImagePointerType;
  //typedef typename OutputImageType::PixelType           OutputImagePixelType;
  //typedef typename OutputImageType::RegionType          OutputImageRegionType;
  //typedef typename OutputImageRegionType::SizeType      OutputImageRegionSizeType;
  //typedef typename OutputImageType::IndexType           OutputImageIndexType;
  //typedef typename OutputImageRegionType::IndexType     OutputImageRegionIndexType;
  //typedef          ImageRegionIterator<OutputImageType> OutputImageIteratorType;

  // Setters/Getters
  itkSetMacro(Frequency, double);
  itkGetMacro(Frequency, double);
  itkSetMacro(RotationZ, double);
  itkGetMacro(RotationZ, double);

  
protected:
  GaborFilter();
  virtual ~GaborFilter() {}
  void GenerateData();
  void PrintSelf(std::ostream& os, Indent indent) const;
  typename TKernelImage::Pointer CreateGaborKernel();
  //KernelImageType

  // member variables
  double m_Frequency;   
  double m_RotationZ; // only around z-axis // TODO:Support Rotation in other directions

private:
  GaborFilter(const Self &); //purposely not implemented
  void operator=(const Self &);   //purposely not implemented
  itkSetInputMacro(KernelImage, TKernelImage); //purposely located here to prevent user from setting kernel manually
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGaborFilter.hxx"
#endif

#endif // __itkGaborFilter_h

