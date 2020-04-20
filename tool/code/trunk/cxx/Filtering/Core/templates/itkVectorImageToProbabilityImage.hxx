#ifndef __itkVectorImageToProbabilityImage_hxx
#define __itkVectorImageToProbabilityImage_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkVectorImageToProbabilityImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegionIterator.h>
  #include <itkImageRegionConstIterator.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{

template < class TInputImage, class TOutputImage,
           class TMembershipType >
void 
VectorImageToProbabilityImage< TInputImage, TOutputImage,
                               TMembershipType >
::PrintSelf( std::ostream & os, Indent indent ) const
{

  Superclass::PrintSelf( os, indent );
  os << indent <<"Membership Function:" << std::endl;
  m_MembershipFunction ->Print( os, indent.GetNextIndent() );

} //Print Self



template < class TInputImage, class TOutputImage,
           class TMembershipType >
void 
VectorImageToProbabilityImage< TInputImage, TOutputImage,
                               TMembershipType >
::ThreadedGenerateData( const OutputImageRegionType & outputRegionForThread,
                        ThreadIdType threadId )
{

  // threadID isn't used
  (void)threadId;

  // Prepare our inputs and outputs
  typename OutputImageType::Pointer      output = this->GetOutput();
  typename InputImageType ::ConstPointer input  = this->GetInput ();

  // prepare our iterators
  ImageRegionIterator< OutputImageType > outputIter( output, outputRegionForThread );
  ImageRegionConstIterator< InputImageType >  inputIter( input,  outputRegionForThread );

  while (!inputIter.IsAtEnd() )
    {
    InputPixelType inputPixel=inputIter.Get();

    OutputPixelType probVal (
	  static_cast< OutputPixelType > ( m_MembershipFunction->Evaluate(inputPixel) ) );
	outputIter.Set(probVal);

	++outputIter;
	++inputIter;

    } //while inputIter.isAtEnd

} //ThreadedGenerateData

} //namespace
#endif //__itkVectorImageToProbabilityImage_hxx
