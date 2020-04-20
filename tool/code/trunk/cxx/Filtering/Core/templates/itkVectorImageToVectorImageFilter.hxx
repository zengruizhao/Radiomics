#ifndef __itkVectorImageToVectorImageFilter_hxx
#define __itkVectorImageToVectorImageFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkVectorImageToVectorImageFilter.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkComposeImageFilter.h>
  #include <itkVectorIndexSelectionCastImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{

//Constructor
template< class TInputImage, class TOutputImage, class TFilter >
VectorImageToVectorImageFilter< TInputImage, TOutputImage, TFilter >::
VectorImageToVectorImageFilter()
{

m_ComponentFilter = FilterType::New();

} //VectorImageToVectorImageFilter()


//Destructor
template< class TInputImage, class TOutputImage, class TFilter >
VectorImageToVectorImageFilter< TInputImage, TOutputImage, TFilter >::
~VectorImageToVectorImageFilter()
{
}// ~VectorImageToVectorImageFilter()



//Function to print information
template< class TInputImage, class TOutputImage, class TFilter >
void
VectorImageToVectorImageFilter< TInputImage, TOutputImage, TFilter >::
PrintSelf(std::ostream & os, Indent indent ) const
{

Superclass::PrintSelf(os, indent);
this->m_ComponentFilter->Print(os, indent);

} //PrintSelf(std::ostream & os, Indent indent ) const



template< class TInputImage, class TOutputImage, class TFilter >
void
VectorImageToVectorImageFilter< TInputImage, TOutputImage, TFilter >::
GenerateData()
{

//set up the compose image filter
typedef ComposeImageFilter<
  ComponentImageType,
  OutputImageType
> ComposeFilterType;

typename ComposeFilterType::Pointer composeFilter = ComposeFilterType::New();
unsigned int numComponents = Superclass::GetInput()->GetNumberOfComponentsPerPixel();

//for each component in the input image
for(unsigned int i = 0; i< numComponents; i++)
  {
  composeFilter->SetInput( i, this->GetComponent(i) );
  } // for(unsigned int i = 0; i< numComponents; i++)

//graft the output
composeFilter->Update();
this->GraftOutput(composeFilter->GetOutput() );

} //GenerateData()



template< class TInputImage, class TOutputImage, class TFilter >
typename VectorImageToVectorImageFilter< TInputImage, TOutputImage, TFilter>::ComponentImageConstPointer
VectorImageToVectorImageFilter< TInputImage, TOutputImage, TFilter >::
GetComponent( unsigned int index) const
{

//set up the filter to get each component
typedef VectorIndexSelectionCastImageFilter< 
  InputImageType,
  ComponentImageType
> ExtractComponentType;

typename ExtractComponentType::Pointer 
  extractor = ExtractComponentType::New();

extractor->SetInput( Superclass::GetInput() );
extractor->SetIndex( index);
extractor->Update();

//get a copy of the filter and update
const FilterPointer filter = this->m_ComponentFilter->Clone();
filter->SetInput(extractor->GetOutput());
filter->Update();

return filter->GetOutput();

}//GetComponent( unsigned int index) const



} //end namespace itk
#endif //__itkVectorImageToVectorImageFilter_hxx