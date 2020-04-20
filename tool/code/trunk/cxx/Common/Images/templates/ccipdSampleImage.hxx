#ifndef __ccipdSampleImage_hxx
#define __ccipdSampleImage_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdSampleImage.h"

// std includes
#include <vector>
#include <omp.h>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkNearestNeighborExtrapolateImageFunction.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TImage, class TInterpolator, class TExtrapolator, class TOutputIter >
void SampleImage(
  const std::vector< typename TImage::PointType > locations,
  const TImage                                  * inputImage,
  TOutputIter                                     outputIter,
  TInterpolator                                 * interpolator,
  const bool                                      extrapolatePixelsOutsideImage,
  const typename TImage::PixelType                valueForPixelsOutsideImage )
{

  // check our inputs
  const auto numberOfSamples = locations.size();
  if ( !numberOfSamples || !inputImage || !interpolator )
    return;

  // we will use our interpolate to sample points on each image
  const typename TImage::ConstPointer oldInput = interpolator->GetInputImage();

  // we might use an extrapolator for points outside the image
  typedef TExtrapolator ExtrapolateType;
  typename ExtrapolateType::Pointer extrapolator = ExtrapolateType::New();

  // assign our extrapolator and interpolator to the correct feature
  extrapolator->SetInputImage( inputImage );
  interpolator->SetInputImage( inputImage );

  // how many threads will the sampler be using?
  int numberOfThreads;
  #pragma omp parallel
    {
    numberOfThreads = omp_get_num_threads();
    }

  // Be even more granular in case a thread ends early.
  // While the above value is actually how many threads
  // the program will use, the below value is actually
  // how many splits the data will have.
  const int numberOfChunks = numberOfThreads * 2;

  #pragma omp parallel for // parallel for loops
  for ( int chunk = 0; chunk < numberOfChunks; ++chunk )
    {

    // figure out how many samples to process in this thread
    int chunkSize = static_cast< int >( numberOfSamples / numberOfChunks ); // floor
    const unsigned int firstSampleIndex = chunk * chunkSize;
    if ( chunk == numberOfChunks - 1 ) // last chunk
      {
      // the last chunk has a different chunk size
      chunkSize = static_cast< int >(
        numberOfSamples - ( numberOfChunks - 1 ) * chunkSize );
      }

    // prepare our iterators
    typedef typename TImage::PointType LocationType;
    typedef std::vector< LocationType > LocationsContainer;
    typename LocationsContainer::const_iterator locationsIter =
      locations.begin() + firstSampleIndex;
    const typename LocationsContainer::const_iterator locationsEnd =
      locationsIter + chunkSize;
    TOutputIter currentOutputIter = outputIter + firstSampleIndex;

    // go through each sample
    for ( ; locationsIter != locationsEnd; ++locationsIter, ++currentOutputIter )
      {

      // get the current location
      const LocationType currentLocation = *locationsIter;

      // check if it's inside the image
      const bool isInside = interpolator->IsInsideBuffer( currentLocation );

      // interpolate (or extrapolate) at the point

      const typename TImage::PixelType sampledValue = static_cast< typename TImage::PixelType >(
        isInside ? // either interpolate or extrapolate our mapped moving point
        interpolator->Evaluate( currentLocation ) :

        // nope, not inside.  either extrapolate or just set it
        extrapolatePixelsOutsideImage ?
          extrapolator->Evaluate( currentLocation ) :
          valueForPixelsOutsideImage );

      // output it
      *currentOutputIter = sampledValue;

      } // locationsIter

    } // chunk

  // restore our interpolator
  interpolator->SetInputImage( oldInput );

}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TImage, class TOutputIter >
void SampleImage(

  const std::vector< typename TImage::PointType > locations,
  const TImage *                                  inputImage,
  TOutputIter                                     outputIter,
  const bool extrapolatePixelsOutsideImage,
  const typename TImage::PixelType valueForPixelsOutsideImage )
{
  typedef itk::LinearInterpolateImageFunction < 
    TImage
  > InterpolatorType;

  typename InterpolatorType::Pointer interpolator = InterpolatorType::New();

  typedef itk::NearestNeighborExtrapolateImageFunction<
    TImage
  > ExtrapolatorType;

  return SampleImage< TImage, InterpolatorType, ExtrapolatorType, TOutputIter >(
	  locations, inputImage, outputIter, interpolator,
	  extrapolatePixelsOutsideImage, valueForPixelsOutsideImage );

} // SampleImage ( default interpolator )
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TImage, class TOutputIter >
void SampleVectorImageRowWise(

  const std::vector< typename TImage::PointType >  locations,

  const TImage                                   * inputImage,

  TOutputIter                                      outputIter,

  itk::InterpolateImageFunction < 
    itk::VectorImageToImageAdaptor <
      typename TImage::InternalPixelType,
      TImage::ImageDimension
	> >                                       *  interpolator,

  const bool extrapolatePixelsOutsideImage,

  const typename TImage::InternalPixelType valueForPixelsOutsideImage )
{

  // create a vector to store temporary sampled pixels
  typedef std::vector<
    InternalPixelType
  > InternalVectorType;
  typedef std::vector<
    InternalVectorType
  > ExternalVectorType;
  typedef std::vector< InternalVectorType::iterator > ExternalVectorDataType;

  ExternalVectorType     externalVector; // this stores the vectors
  ExternalVectorDataType externalData;   // this stores pointers to the vector data

  // prepare the temporary storage
  const unsigned int numberOfComponents = inputImage->GetNumberOfComponentsPerPixel();
  const size_t       numberOfPixels     = locations.size();

 

  for ( unsigned int componentIndex = 0; componentIndex < numberOfComponents; ++componentIndex )
    {
    InternalVectorType internalVector( numberOfPixels );
    externalVector.push_back( internalVector );
    externalData  .push_back( externalVector[componentIndex].begin() );

    } // componentIndex
  
  // store pointers to every internal vector we will sample

  // sample all the components
  SampleVectorImageColWise( locations, inputImage,
    externalData.begin(), interpolator,
    extrapolatePixelsOutsideImage,
    valueForPixelsOutsideImage );

  // now copy it to rowwise in our output

  // remember where we started
  const TOutputIter initialOutputIter = outputIter;

  for ( unsigned int componentIndex = 0; componentIndex < numberOfComponents; ++componentIndex )
    {
    int count=0;
    // this stores a pointer to the current row
    TOutputIter currentOutputIter = initialOutputIter;

    // this stores the actual sampled column
    const InternalVectorType & internalVector = externalVector[ componentIndex ];

    // this stores the first element of the data for the given column
    auto internalIter = internalVector.begin();

    // this stores the last element of the data for the given column
    const auto internalEnd = internalVector.cend();

    // loop through each of our sampled elements
    for (; internalIter != internalEnd; ++internalIter, ++currentOutputIter, ++count)
      {

      *( (*currentOutputIter) + componentIndex ) = *internalIter;

      } // for internalIter, currentOutputIter

    } // componentIndex
  
}// SampleVectorImageRowWise
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TImage, class TOutputIter >
void SampleVectorImageColWise(

  const std::vector< typename TImage::PointType > locations,
  const TImage                                   * inputImage,
  TOutputIter                                      outputIter,

  itk::InterpolateImageFunction < 
    itk::VectorImageToImageAdaptor <
      typename TImage::InternalPixelType,
      TImage::ImageDimension
	> > *  interpolator,

  const bool extrapolatePixelsOutsideImage,

  const typename TImage::InternalPixelType valueForPixelsOutsideImage )
{

  //typedef TPixelType InternalPixelType;
  typedef typename TImage::InternalPixelType InternalPixelType;

  const unsigned int Dimension = TImage::ImageDimension;

  // create an object to extract each feature component
  typedef itk::VectorImageToImageAdaptor<
    InternalPixelType,
    Dimension
  > ExtractorType;
  
  typename ExtractorType::Pointer extractor = ExtractorType::New ();

  // create interpolator and extrapolator types
  // which correpond to our extractor
  typedef itk::InterpolateImageFunction<
    ExtractorType
  > InterpolatorType;

  typedef itk::NearestNeighborExtrapolateImageFunction<
    ExtractorType
  > ExtrapolatorType;


  // we promise we won't modify inputImage!
  TImage * unconstInputImage = const_cast< TImage * >( inputImage );

  extractor->SetImage(unconstInputImage);

  // this is an iterator to the given column
  TOutputIter currentOutputIter = outputIter;

  const unsigned int numberOfComponents = inputImage->GetNumberOfComponentsPerPixel();

  for ( unsigned int componentIndex = 0;
    componentIndex < numberOfComponents;
    ++componentIndex, ++currentOutputIter )
    {

    // get a pointer to the beginning of the
    // column for this component
    auto internalBegin = currentOutputIter.operator*();
    //auto internalBegin = *currentOutputIter; // this doesn't compile in gcc 4.4.5


    
    // actually sample our image from the given component
    extractor->SetExtractComponentIndex( componentIndex );

    ccipd::SampleImage< 
      ExtractorType,
      InterpolatorType, 
      ExtrapolatorType
    >(locations, extractor, internalBegin, interpolator,
      extrapolatePixelsOutsideImage, valueForPixelsOutsideImage );

  	} // componentIndex

} // SampleVectorImageColWise
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/** This version uses a default linear interpolator */
template < class TImage, class TOutputIter >
void SampleVectorImageRowWise(

  const std::vector< typename TImage::PointType >  locations,
  const TImage                                   * inputImage,
  TOutputIter                                      outputIter,
  const bool                                       extrapolatePixelsOutsideImage,
  const typename TImage::InternalPixelType         valueForPixelsOutsideImage )
{

  // create a default linear interpolator
  typedef itk::LinearInterpolateImageFunction < 
    itk::VectorImageToImageAdaptor <
      typename TImage::InternalPixelType, TImage::ImageDimension
    > > InterpolateType;
  typename InterpolateType::Pointer interpolator = InterpolateType::New();

  // call SampleVectorImage using our default interpolator
  return SampleVectorImageRowWise< TImage, TOutputIter >(
    locations, inputImage, outputIter, interpolator,
    extrapolatePixelsOutsideImage, valueForPixelsOutsideImage );

} // SampleVectorImageRowWise
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/** This version uses a default linear interpolator */
template < class TImage, class TOutputIter >
void SampleVectorImageColWise(

  const std::vector< typename TImage::PointType >  locations,
  const TImage                                   * inputImage,
  TOutputIter                                      outputIter,
  const bool                                       extrapolatePixelsOutsideImage,
  const typename TImage::InternalPixelType         valueForPixelsOutsideImage )
{

  // create a default linear interpolator
  typedef itk::LinearInterpolateImageFunction < 
    itk::VectorImageToImageAdaptor <
      typename TImage::InternalPixelType, TImage::ImageDimension
    > > InterpolateType;
  typename InterpolateType::Pointer interpolator = InterpolateType::New();

  // call SampleVectorImage using our default interpolator
  return SampleVectorImageColWise< TImage, TOutputIter >(
    locations, inputImage, outputIter, interpolator,
    extrapolatePixelsOutsideImage, valueForPixelsOutsideImage );

} // SampleVectorImageColWise
////////////////////////////////////////////////////////////////////////////////////////////////////


} // namespace
#endif // __ccipdSampleImage_hxx
