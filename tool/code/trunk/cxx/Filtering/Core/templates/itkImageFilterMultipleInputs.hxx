#ifndef __itkImageFilterMultipleInputs_hxx
#define __itkImageFilterMultipleInputs_hxx
 
#include "itkImageFilterMultipleInputs.h"
 
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
 
namespace itk
{
 
template< class TInputImage, class TOutputImage >
ImageFilterMultipleInputs<TInputImage, TOutputImage>::ImageFilterMultipleInputs()
{
  this->SetNumberOfRequiredInputs(2);
}
 
template< class TInputImage, class TOutputImage >
void ImageFilterMultipleInputs<TInputImage, TOutputImage>::SetInput(const int index, const TInputImage* image)
{
  this->SetNthInput(index, const_cast<TInputImage*>(image));
} 
}// end namespace
 
 
#endif
