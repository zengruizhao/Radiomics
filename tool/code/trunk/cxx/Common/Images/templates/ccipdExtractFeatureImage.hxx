#ifndef __ccipdExtractFeatureImage_hxx
#define __ccipdExtractFeatureImage_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdExtractFeatureImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVectorImage.h>
  #include <itkVectorImageToImageAdaptor.h>
  #include <itkVectorIndexSelectionCastImageFilter.h>
  #include <itkComposeImageFilter.h>
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
typename itk::Image< TPixelType, Dimension >::Pointer ExtractFeatureImage(
  const itk::VectorImage< TPixelType, Dimension > * featureImage,
  const unsigned int                                whichComponent )
{
	typedef itk::Image<TPixelType, Dimension> OutputImageType;
	typedef typename OutputImageType     ::Pointer OutputImagePointer;

  // safety checks - return empty
  if ( whichComponent >= featureImage->GetNumberOfComponentsPerPixel() )
    {
    using std::cerr;
    using std::endl;
    cerr << endl <<  "Error: Invalid component index on line " << __LINE__ <<
     " of file " << __FILE__ <<  ". Returning empty volume." << endl;
    return OutputImagePointer();
    }

  // typedefs
  typedef itk::VectorImage<TPixelType, Dimension> InputVectorImageType;

  

  typedef itk::VectorIndexSelectionCastImageFilter< InputVectorImageType, OutputImageType > FeatureExtractorType;

  
  typedef typename FeatureExtractorType::Pointer FeatureExtractorPointer;
  // objects
  FeatureExtractorPointer featureExtractor = FeatureExtractorType::New();

  // inputs
  featureExtractor->SetInput( featureImage   );
  featureExtractor->SetIndex( whichComponent );

  // update and output
  featureExtractor->Update();
  return featureExtractor->GetOutput();

} // ExtractFeatureImage
////////////////////////////////////////////////////////////////////////////////////////////////////

template< class TPixelType, unsigned int Dimension >
typename itk::VectorImage< TPixelType, Dimension >::Pointer 
RemoveFeatureImage(  const itk::VectorImage< TPixelType, Dimension > * featureImage
                   , const unsigned int                                componentIndex 
                  )
{
    
    typedef itk::VectorImage<TPixelType, Dimension> VectorImageType;
    
    typedef itk::Image<TPixelType, Dimension> ScalarComponentImageType;

    if(!featureImage || componentIndex >= featureImage->GetNumberOfComponentsPerPixel())
    {
       std::cerr << "\nError(): Invalid image pointer or component index ... \n";
       return NULL;//typename VectorImageType::Pointer();
    }

    typedef itk::ComposeImageFilter<ScalarComponentImageType>   ImageToVectorImageFilterType;
    typename ImageToVectorImageFilterType::Pointer imageToVectorImageFilter = ImageToVectorImageFilterType::New();
    
    for(unsigned int i = 0, n = 0; i < featureImage->GetNumberOfComponentsPerPixel(); i++)
    {
      if(i != componentIndex) 
      {
        imageToVectorImageFilter->SetInput(n, ExtractFeatureImage(featureImage, i));
        n++;
      }
    }

    imageToVectorImageFilter->Update();
 
    return imageToVectorImageFilter->GetOutput();
}













} // namespace



#endif // __ccipdExtractFeatureImage_hxx
