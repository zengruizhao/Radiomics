#ifndef __itkCohenBiasCorrectionFilter_h
#define __itkCohenBiasCorrectionFilter_h



/*
 * 
 * The basic idea of this filter is:
 * output(i) = mean(input) * input(i) / smoothed(i)
 * where smoothed is a Gaussian-smoothed version of input.
 * 
 * REFERENCE
 * ---------
 * M. S. Cohen, R. M. Dubois, & M. M. Zeineh, ?Rapid and effective 
 * correction of RF inhomogeneity for high field magnetic resonance 
 * imaging, Human Brain Mapping, vol. 10, no. 4, pp. 204-211, 2000.
 * 
 */



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"

#include <itkImageToImageFilter.h> // base class

#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



template< class TInputImage, class TOutputImage = TInputImage > // template types
class ITK_EXPORT CohenBiasCorrectionFilter :                    // class declaration
public ImageToImageFilter < TInputImage, TOutputImage >         // the type of filter we are
{



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Standard class typedefs. */
  typedef CohenBiasCorrectionFilter                       Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(CohenBiasCorrectionFilter, ImageToImageFilter);

  /** The dimension of the image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Input and output image related type definitions. */
  typedef TOutputImage                       OutputImageType;
  typedef TInputImage                        InputImageType;
  typedef typename InputImageType::PixelType InputImagePixelType;
////////////////////////////////////////////////////////////////////////////////////////////////////




public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  typedef typename NumericTraits< InputImagePixelType >::ScalarRealType ScalarRealType;
  typedef typename itk::FixedArray<
    ScalarRealType, 
	TInputImage::ImageDimension>                                        SigmaArrayType;

  /** Sigma will control the Gaussian smoothing. **/
  void SetSigma(const ScalarRealType sigma); 
  itkGetConstMacro(Sigma, ScalarRealType);
  
  /**Sigmas will control the Gaussian smoothing 
    * separately in each Dimension**/
  void SetSigmaArray(const SigmaArrayType & sigmaArray);
  itkGetConstMacro(SigmaArray, SigmaArrayType);
////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////
  CohenBiasCorrectionFilter();
  void GenerateData();
////////////////////////////////////////////////////////////////////////////////////////////////////



private:
////////////////////////////////////////////////////////////////////////////////////////////////////
  CohenBiasCorrectionFilter(const Self &); //purposely not implemented
  void operator=(const Self &);            //purposely not implemented
////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////
  // Variables
  ScalarRealType m_Sigma;
  SigmaArrayType m_SigmaArray;
////////////////////////////////////////////////////////////////////////////////////////////////////



}; // class 
} // namespace itk



////////////////////////////////////////////////////////////////////////////////////////////////////
// this is where the implementation is actually included in the header
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCohenBiasCorrectionFilter.hxx"
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////



#endif // __itkCohenBiasCorrectionFilter_h
