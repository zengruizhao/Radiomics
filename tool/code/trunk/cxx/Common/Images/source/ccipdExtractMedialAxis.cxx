

//CCIPD//////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdExtractMedialAxis.h"

#include "ccipdDisableWarningsMacro.h"
  //ITK////////////////////////////////////////////////////////////////////////////////////////////////
  #include <itkRecursiveGaussianImageFilter.h>
  #include <itkGradientImageFilter.h>
  #include "itkImageSliceIteratorWithIndex.h"
  #include <itkStatisticsImageFilter.h>
  #include <itkPointSet.h>

  //ITKThirdParty//////////////////////////////////////////////////////////////////////////////////////
  #include <itkAverageOutwardFluxImageFilter.h>
  #include <itkMedialCurveImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

VolumeMaskImageConstPointer ExtractMedialAxis(
  const VolumeImageType * distanceVolume,
  float sigma,
	float threshold	)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1.	Compute the associated average outward flux
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// To have a good quality gradient of the distance map, perform a light smooth over it. Define  
	// convolution kernels in each direction and use them recursively. 

	std::cout << "Extracting Medial Axis..." << std::endl;
	std::cout << "  Applying Gaussian of sigma " << sigma << "... " <<  std::endl;
	typedef itk::RecursiveGaussianImageFilter< VolumeImageType, VolumeImageType > RecursiveGaussianFilterType;
	RecursiveGaussianFilterType::Pointer gaussianFilterX = RecursiveGaussianFilterType::New();
	RecursiveGaussianFilterType::Pointer gaussianFilterY = RecursiveGaussianFilterType::New();
	RecursiveGaussianFilterType::Pointer gaussianFilterZ = RecursiveGaussianFilterType::New();

	gaussianFilterX->SetDirection( 0 );
	gaussianFilterY->SetDirection( 1 );
	gaussianFilterZ->SetDirection( 2 );

	gaussianFilterX->SetOrder( RecursiveGaussianFilterType::ZeroOrder );
	gaussianFilterY->SetOrder( RecursiveGaussianFilterType::ZeroOrder );
	gaussianFilterZ->SetOrder( RecursiveGaussianFilterType::ZeroOrder );

	gaussianFilterX->SetNormalizeAcrossScale( false );
	gaussianFilterY->SetNormalizeAcrossScale( false );
	gaussianFilterZ->SetNormalizeAcrossScale( false );

	gaussianFilterX->SetInput( distanceVolume );
	gaussianFilterY->SetInput( gaussianFilterX->GetOutput() );
	gaussianFilterZ->SetInput( gaussianFilterY->GetOutput() );

	gaussianFilterX->SetSigma( sigma );
	gaussianFilterY->SetSigma( sigma );
	gaussianFilterZ->SetSigma( sigma );

	gaussianFilterX->Update();
	gaussianFilterY->Update();
	gaussianFilterZ->Update();

	std::cout << "  Computing Gradient..." <<  std::endl;
	typedef itk::GradientImageFilter< VolumeImageType, InternalPixelType, InternalPixelType > GradientFilterType;

	// Compute the gradient.
	GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
	gradientFilter->SetInput( gaussianFilterZ->GetOutput() );
	gradientFilter->SetInput( gaussianFilterY->GetOutput() );
	gradientFilter->Update();

	// Compute the average outward flux.
	std::cout << "  Computing Average Outward Flux..." <<  std::endl;
	typedef itk::AverageOutwardFluxImageFilter< VolumeImageType, InternalPixelType, GradientFilterType::OutputImageType::PixelType > AOFFilterType;
	AOFFilterType::Pointer aofFilter = AOFFilterType::New();
	aofFilter->SetInput( distanceVolume );
	aofFilter->SetGradientImage( gradientFilter->GetOutput() );
	aofFilter->Update();

	/*
	typedef itk::StatisticsImageFilter<VolumeImageType> StatisticsImageFilterType;
  StatisticsImageFilterType::Pointer statisticsImageFilter
            = StatisticsImageFilterType::New ();
	statisticsImageFilter->SetInput(aofFilter->GetOutput());
	statisticsImageFilter->Update();
	std::cout << "[" << statisticsImageFilter->GetMinimum() <<  "," << statisticsImageFilter->GetMaximum() <<  "] " << std::endl;
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. Compute the skeleton
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << "  Computing skeleton using threhold " << threshold << "... " <<  std::endl;
	typedef itk::MedialCurveImageFilter< VolumeImageType > MedialCurveFilter;
	MedialCurveFilter::Pointer medialFilter = MedialCurveFilter::New();
	medialFilter->SetInput( distanceVolume );
	medialFilter->SetAverageOutwardFluxImage( aofFilter->GetOutput() );
	medialFilter->SetThreshold( threshold );
	medialFilter->Update();

	const VolumeMaskImageConstPointer 
		output = medialFilter->GetOutput();
	
	std::cout << "Extracting Medial Axis... Done!" <<  std::endl;

	return output;
};

LandmarkSetPointer ExtractMedialAxisIntoLandmark(
  const VolumeImageType * distanceVolume,
  float sigma,
	float threshold)
{
	auto medialCurveVolume = ExtractMedialAxis(distanceVolume, sigma, threshold);

	//prepare PointsContainer
  LandmarkSetPointer            pointSet       = LandmarkSetType     ::New();
  LandmarkSetContainer::Pointer Landmarks      = LandmarkSetContainer::New();
  LandmarkPointType             MedialPoint;

	//slice interator
  typedef itk::ImageSliceIteratorWithIndex< const ccipd::VolumeMaskImageType > IteratorType;
  IteratorType it( medialCurveVolume, medialCurveVolume->GetLargestPossibleRegion() );

  it.GoToBegin();
  it.SetFirstDirection( 0 );  // 0=x, 1=y, 2=z
  it.SetSecondDirection( 1 ); // 0=x, 1=y, 2=z

  VolumeImageType::IndexType index;
	int pointId = 0;
  while( !it.IsAtEnd() ) // going through slice on z
  {
    while( !it.IsAtEndOfSlice() )
    {
      while( !it.IsAtEndOfLine() )
      {
        if (it.Get() == 1) // is in the medial axis
        {
          index = it.GetIndex();
          MedialPoint[0] = static_cast< LandmarkPointType::ValueType >(
            index[0]*medialCurveVolume->GetSpacing()[0] + medialCurveVolume->GetOrigin()[0] );
          MedialPoint[1] = static_cast< LandmarkPointType::ValueType >(
					  index[1]*medialCurveVolume->GetSpacing()[1] + medialCurveVolume->GetOrigin()[1] );
          MedialPoint[2] = static_cast< LandmarkPointType::ValueType >(
					  index[2]*medialCurveVolume->GetSpacing()[2] + medialCurveVolume->GetOrigin()[2] );
					Landmarks->InsertElement (pointId++, MedialPoint );
        }
        ++it;
      }
      it.NextLine();
    }
		it.NextSlice();
  }

  pointSet->SetPoints(Landmarks);

  return pointSet;
};


} // namespace
