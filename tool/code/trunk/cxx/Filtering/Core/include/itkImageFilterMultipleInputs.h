#ifndef __itkImageFilterMultipleInputs_h
#define __itkImageFilterMultipleInputs_h
 
#include "itkImageToImageFilter.h"
 
namespace itk
{
template< class TInputImage, class TOutputImage>
class ImageFilterMultipleInputs : public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef ImageFilterMultipleInputs             Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage >  Superclass;
  typedef SmartPointer< Self >                  Pointer;
 
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFilterMultipleInputs, ImageToImageFilter);
 
  /** SetNthImage.*/
  void SetInput(const int index, const TInputImage* image);

  /** VerifyInputInformation: overload such that the verify image doesn't check for input overlap*/
  void VerifyInputInformation(){};
 
protected:
  ImageFilterMultipleInputs();
  ~ImageFilterMultipleInputs(){}
 
  /** Does the real work. */
  //virtual void GenerateData();
 
private:
  ImageFilterMultipleInputs(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented
 
};
} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageFilterMultipleInputs.hxx"
#endif
 
 
#endif // __itkImageFilterMultipleInputs_h
