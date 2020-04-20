#ifndef __ccipdSliceVolumeConversion_hxx
#define __ccipdSliceVolumeConversion_hxx



//////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSliceVolumeConversion.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>
  #include <itkExtractImageFilter.h>
  #include <itkJoinSeriesImageFilter.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType >
itk::SmartPointer< itk::Image< TPixelType, ccipd::SliceDimension > >
GetSliceFromVolume(
  const itk::Image< TPixelType, ccipd::VolumeDimension > * const volume,
  const unsigned int whichSlice )
{
  // set up the input volume and output slice image types
  typedef itk::Image< TPixelType, ccipd::VolumeDimension > InputVolumeImageType;
  typedef itk::Image< TPixelType, ccipd::SliceDimension > OutputSliceImageType;

  // in case user inputs empty volume
  if ( !volume )
  {
    std::cerr << "Error: no volume detected." << std::endl;
    typename OutputSliceImageType::Pointer emptySlice = NULL;
    return emptySlice;
  }

  // extract volume information
  typename InputVolumeImageType::RegionType volumeRegion = volume->GetLargestPossibleRegion();
  //std::cout << "Volume region: " << volumeRegion << std::endl;
  //std::cout << "Slice to extract: " << whichSlice << std::endl;
  
  // in case user inputs empty volume
  if ( whichSlice > volumeRegion.GetSize()[2] )
  {
    std::cerr << "Error: requested slice number " << whichSlice << " not in volume of max " << volumeRegion.GetSize()[2] << " slices." << std::endl;
    typename OutputSliceImageType::Pointer emptySlice = NULL;
    return emptySlice;
  }
  
  // set slice region: modify origin by picking slice
  typename InputVolumeImageType::IndexType desiredStart = volumeRegion.GetIndex();
  desiredStart.Fill(0);
  desiredStart[2] = whichSlice;

  // set slice region: remove last dimension of volume to make a slice
  typename InputVolumeImageType::SizeType desiredSize = volumeRegion.GetSize();
  desiredSize[2] = 0;

  // create the slice region
  typename InputVolumeImageType::RegionType desiredRegion(desiredStart, desiredSize);

  // use ITK filter to extract
  typedef itk::ExtractImageFilter< InputVolumeImageType, OutputSliceImageType > FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetExtractionRegion(desiredRegion);
  filter->SetInput(volume);
  filter->SetDirectionCollapseToIdentity();
  filter->Update();
 
  typename OutputSliceImageType::Pointer output = filter->GetOutput();
  output->DisconnectPipeline();

  //std::cout << "New slice: " << output->GetLargestPossibleRegion() << std::endl;

  return output;

} // GetSliceFromVolume
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType >
itk::SmartPointer< itk::Image< TPixelType, ccipd::VolumeDimension > >
GetVolumeFromSlice(
  const itk::Image< TPixelType, ccipd::SliceDimension > * const slice )
{
  // set up the input volume and output slice image types
  typedef itk::Image< TPixelType, ccipd::SliceDimension > InputSliceImageType;
  typedef itk::Image< TPixelType, ccipd::VolumeDimension > OutputVolumeImageType;

  // in case user inputs empty slice
  if ( !slice )
  {
    std::cerr << "Error: no slice detected." << std::endl;
    typename OutputVolumeImageType::Pointer emptyVolume = NULL;
    return emptyVolume;
  }
  
  // extract slice information
  typename InputSliceImageType::RegionType sliceRegion = slice->GetLargestPossibleRegion();
  //std::cout << "Slice region: " << sliceRegion << std::endl;

  // use ITK filter to add a new dimension to slice
  typedef itk::JoinSeriesImageFilter< InputSliceImageType, OutputVolumeImageType > FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput(slice);
  filter->Update();
 
  typename OutputVolumeImageType::Pointer output = filter->GetOutput();
  output->DisconnectPipeline();

  //std::cout << "New volume: " << output->GetLargestPossibleRegion() << std::endl;

  return output;

} // GetVolumeFromSlice
//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdSliceVolumeConversion_hxx