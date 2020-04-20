

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureFeatureExtractor.h"
#include "ccipdTextureFeatureMapParametersType.h" // for mapping from enum to parameter type
#include "ccipdTextureFeatureFirstOrderParameters.h" // for parameter types
#include "ccipdTextureFeatureSobelParameters.h" // for parameter types
#include "ccipdTextureFeatureGaborParameters.h"
#include "ccipdTextureFeatureHaralickParameters.h"
#include "ccipdTextureFeatureParser.h" // for string values
#include "ccipdDuplicateImage.h" // for intensities
#include "ccipdVolumeContainerToVolumeFeatures.h" // for concatenating features
#include "ccipdHashImage.h" // for hashing an image
#include "ccipdCreateGaborKernel.hxx"
#include "ccipdTextureFeatureGetParametersByVariant.h"

//#include "ccipdSaveImage.h"
//#include "ccipdLoadImage.h"

// our itk includes
#include "itkSkewImageFilter.h"
#include "itkStandardDeviationImageFilter.h"
#include "itkRangeImageFilter.h"
#include "itkRayleighHistogram.h"
#include "itkNakagamiHistogram.h"
#include "itkMeanHistogram.h"


#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkGradientMagnitudeImageFilter.h>
  #include <itkMedianImageFilter.h>
  #include <itkImageDuplicator.h>
  #include <itkFlatStructuringElement.h>
  #include <itkMovingHistogramImageFilter.h>
  #include <itkSobelOperator.h>
  #include <itkNeighborhoodOperatorImageFilter.h>
  #include <itkConvolutionImageFilter.h>
  #include "itkGaborFilter.h"
  #include "itkHaralickFilter.h"

  #include "itkHaralickEnergyFilter.h"
  #include "itkHaralickVarianceFilter.h"
  #include "itkHaralickClusterShadeFilter.h"
  #include "itkHaralickInverseDifferenceMomentFilter.h"
  #include "itkHaralickSumEntropyFilter.h"
  #include "itkHaralickDifferenceEntropyFilter.h"
  #include "itkHaralickInertiaFilter.h"
  #include "itkHaralickIMC1Filter.h"
  #include "itkHaralickIMC2Filter.h"

  #include "itkCorrelationFilter.h"

#include <itkFFTConvolutionImageFilter.h>




  // boost includes
  #include <boost/functional/hash.hpp>  // for hashing
  #include <boost/assign/list_of.hpp>   // for outputting what features we are extracting
  #include <boost/variant/apply_visitor.hpp>




// std includes
#include <unordered_map>
#include <ctime>
#include <iostream>
#include <memory>

#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////
class TextureFeatureExtractor::SecretData
{
public: // public to the TextureFeatureExtractor class

  /* Constructor */
  SecretData();

  /** Typedefs */

  // HashValueType is used to store the hash for the volume
  // Caches each Volume according to Parameter hash value
  typedef std::unordered_map<  HashValueType,  VolumeImageConstPointer > ParametersFeatureCacheType; 

  // Caches each ParameterFeatureCacheType according to Enum
  // this order is important: different enums have different
  // parameter types (which may hash in unique ways).
  typedef std::unordered_map< TextureFeatureEnum, ParametersFeatureCacheType, std::hash< int > > EnumFeaturesCacheType;

  // Caches each EnumFeatureCacheType according to image hash value.
  typedef std::unordered_map< HashValueType, EnumFeaturesCacheType> FeaturesCacheType;

  // the base type of all feature extractors
  typedef itk::ImageToImageFilter< VolumeImageType,  VolumeImageType > FilterBase;
  typedef FilterBase::Pointer FilterPointer;

  // we only need to create this once
  // and then we can use this for any feature
  typedef std::unordered_map< TextureFeatureEnum, // the key is what type of feature
                              FilterPointer,      // the value is an actual itk filter base class
                              std::hash< int >
                            > FilterMap;

  /* Variables */
  TextureFeatureParser                    m_Parser;
  VolumeImageConstPointer                 m_Volume;
  FeaturesCacheType                       m_FeaturesCache;
  bool                                    m_Verbose;
  static const FilterMap                  m_FilterMap;

  /* Functions */
  static FilterPointer CreateGaborFilter(  const MapParametersType< TextureFeatureGabor >::AllParametersType & parameters
	                                       , const ccipd::VolumeImageConstPointer & volume  
										                    );
  static FilterPointer CreateHaralickFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
	                                          , const ccipd::VolumeImageConstPointer & volume  
	                                         );
  static FilterPointer CreateHaralickEnergyFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
	                                                , const ccipd::VolumeImageConstPointer & volume  
	                                               );
  static FilterPointer CreateHaralickVarianceFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
                                                    , const ccipd::VolumeImageConstPointer & volume  
                                                   );
  static FilterPointer CreateHaralickClusterShadeFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
                                                        , const ccipd::VolumeImageConstPointer & volume  
                                                       );
  static FilterPointer CreateHaralickInverseDifferenceMomentFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
                                                                   , const ccipd::VolumeImageConstPointer & volume  
                                                                  );
  static FilterPointer CreateHaralickInertiaFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
                                                   , const ccipd::VolumeImageConstPointer & volume  
                                                  );
  static FilterPointer CreateHaralickDifferenceEntropyFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
	                                        , const ccipd::VolumeImageConstPointer & volume  
	                                       );
  static FilterPointer CreateHaralickSumEntropyFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
	                                        , const ccipd::VolumeImageConstPointer & volume  
	                                       );
  static FilterPointer CreateHaralickIMC1Filter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
	                                        , const ccipd::VolumeImageConstPointer & volume  
	                                       );
  static FilterPointer CreateHaralickIMC2Filter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
	                                        , const ccipd::VolumeImageConstPointer & volume  
	                                       );

  static FilterPointer CreateCorrelationFilter(  const MapParametersType< TextureFeatureHaralick >::AllParametersType & parameters
	                                           , const ccipd::VolumeImageConstPointer & volume  
	                                          );

  static FilterPointer CreateGradientMagnitudeFilter(/*  const MapParametersType< TextureFeatureGradientMagnitude >::AllParametersType & parameters */);
  static FilterPointer CreateMeanFilter             (  const MapParametersType< TextureFeatureMean>::AllParametersType & parameters
                                                     , const ccipd::VolumeImageConstPointer & volume  );
  static FilterPointer CreateMedianFilter           (  const MapParametersType< TextureFeatureMedian   >::AllParametersType & parameters );
  static FilterPointer CreateNakagamiFilter         (  const MapParametersType< TextureFeatureNakagami >::AllParametersType & parameters );
  static FilterPointer CreateRangeFilter            (  const MapParametersType< TextureFeatureRange    >::AllParametersType & parameters );
  static FilterPointer CreateRayleighFilter         (  const MapParametersType< TextureFeatureRayleigh >::AllParametersType & parameters );
  static FilterPointer CreateSkewFilter             (  const MapParametersType< TextureFeatureSkew     >::AllParametersType & parameters );
  static FilterPointer CreateSobelFilter            (  const MapParametersType< TextureFeatureSobel    >::AllParametersType & parameters );
  static FilterPointer CreateStandardDeviationFilter(  const MapParametersType< TextureFeatureStandardDeviation >::AllParametersType & parameters );
}; // TextureFeatureExtractor::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::TextureFeatureExtractor() : ccipdNewPIMPLMacro
{
} // constructor

TextureFeatureExtractor::SecretData::SecretData() : m_Verbose( false )
{
}

TextureFeatureExtractor::~TextureFeatureExtractor()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool TextureFeatureExtractor::operator==( const Hashable & other ) const
{
  // we trust the hash to be unique for images
  return this->GetHash() == other.GetHash();

} // operator==
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType TextureFeatureExtractor::ReHash() const
{
  // just hash our volume image
  return HashImage( this->GetVolume().GetPointer() );
} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateGaborFilter(  const MapParametersType<TextureFeatureGabor>::AllParametersType & parameters
                                                       , const ccipd::VolumeImageConstPointer & volume  
													                            )
{
  typedef itk::GaborFilter<ccipd::VolumeImageType> GaborFilterType;
  GaborFilterType::Pointer gaborFilter = GaborFilterType::New();

  gaborFilter->SetInput(volume);
  

  gaborFilter->SetFrequency(parameters.GetFrequency());
  gaborFilter->SetRotationZ(parameters.GetDirection());
  gaborFilter->NormalizeOn(); // This will normalize the Gabor kernel created inside


  gaborFilter->Update();

  return dynamic_cast< FilterBase* >( gaborFilter.GetPointer() );

} // CreateGaborFilter
//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                          , const ccipd::VolumeImageConstPointer & volume  
                                                         )
{
	typedef itk::HaralickFilter<VolumeImageType, VolumeImageType>  HaralickFilterType;
	HaralickFilterType::Pointer haralickFilter = HaralickFilterType::New();

	haralickFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	haralickFilter->SetRadius(parameters.GetRadius());

	//haralickFilter->SetInputImageMinimum(0.0f);
	//haralickFilter->SetInputImageMaximum(1237.0f); // Min and max will be defaulted to those of the input image of not set by user

	haralickFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	haralickFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user
	//haralickFilter->SetNumberOfThreads(32); // Severely affects computational time

	//// Display current date/time based on current system
	//time_t t_before = time(0);
	haralickFilter->Update();


	return dynamic_cast<FilterBase*>(haralickFilter.GetPointer());

} // CreateHaralickFilter

TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickEnergyFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                                , const ccipd::VolumeImageConstPointer & volume  
                                                               )
{
	typedef itk::HaralickEnergyFilter <VolumeImageType, VolumeImageType>  HaralickEnergyFilterType;
	HaralickEnergyFilterType::Pointer HaralickEnergyFilter = HaralickEnergyFilterType::New();

	HaralickEnergyFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickEnergyFilter->SetRadius(parameters.GetRadius());

	HaralickEnergyFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickEnergyFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickEnergyFilter->Update();

	return dynamic_cast<FilterBase*>(HaralickEnergyFilter.GetPointer());

} // CreateHaralickEnergyFilter

TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickVarianceFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                                  , const ccipd::VolumeImageConstPointer & volume  
                                                                 )
{
	typedef itk::HaralickVarianceFilter <VolumeImageType, VolumeImageType>  HaralickVarianceFilterType;
	HaralickVarianceFilterType::Pointer HaralickVarianceFilter = HaralickVarianceFilterType::New();

	HaralickVarianceFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickVarianceFilter->SetRadius(parameters.GetRadius());

	HaralickVarianceFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickVarianceFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickVarianceFilter->Update();

	return dynamic_cast<FilterBase*>(HaralickVarianceFilter.GetPointer());

} // CreateHaralickVarianceFilter

TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickClusterShadeFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                                , const ccipd::VolumeImageConstPointer & volume  
                                                               )
{
	typedef itk::HaralickClusterShadeFilter <VolumeImageType, VolumeImageType>  HaralickClusterShadeFilterType;
	HaralickClusterShadeFilterType::Pointer HaralickClusterShadeFilter = HaralickClusterShadeFilterType::New();

	HaralickClusterShadeFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickClusterShadeFilter->SetRadius(parameters.GetRadius());

	HaralickClusterShadeFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickClusterShadeFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickClusterShadeFilter->Update();

	return dynamic_cast<FilterBase*>(HaralickClusterShadeFilter.GetPointer());

} // CreateHaralickClusterShadeFilter

TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickInverseDifferenceMomentFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                                , const ccipd::VolumeImageConstPointer & volume  
                                                               )
{
	typedef itk::HaralickInverseDifferenceMomentFilter <VolumeImageType, VolumeImageType>  HaralickInverseDifferenceMomentFilterType;
	HaralickInverseDifferenceMomentFilterType::Pointer HaralickInverseDifferenceMomentFilter = HaralickInverseDifferenceMomentFilterType::New();

	HaralickInverseDifferenceMomentFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickInverseDifferenceMomentFilter->SetRadius(parameters.GetRadius());

	HaralickInverseDifferenceMomentFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickInverseDifferenceMomentFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickInverseDifferenceMomentFilter->Update();

	return dynamic_cast<FilterBase*>(HaralickInverseDifferenceMomentFilter.GetPointer());

} // CreateHaralickInverseDifferenceMomentFilter

TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickInertiaFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                                 , const ccipd::VolumeImageConstPointer & volume  
                                                                )
{
	typedef itk::HaralickInertiaFilter<VolumeImageType, VolumeImageType>  HaralickInertiaFilterType;
	HaralickInertiaFilterType::Pointer HaralickInertiaFilter = HaralickInertiaFilterType::New();

	HaralickInertiaFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickInertiaFilter->SetRadius(parameters.GetRadius());

	HaralickInertiaFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickInertiaFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickInertiaFilter->Update();

	return dynamic_cast<FilterBase*>(HaralickInertiaFilter.GetPointer());

} // CreateHaralickInertiaFilter

TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickSumEntropyFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                          , const ccipd::VolumeImageConstPointer & volume  
                                                         )
{
	typedef itk::HaralickSumEntropyFilter<VolumeImageType, VolumeImageType>  HaralickSumEntropyFilterType;
	HaralickSumEntropyFilterType::Pointer HaralickSumEntropyFilter = HaralickSumEntropyFilterType::New();

	HaralickSumEntropyFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickSumEntropyFilter->SetRadius(parameters.GetRadius());

	HaralickSumEntropyFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickSumEntropyFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickSumEntropyFilter->Update();

	return dynamic_cast<FilterBase*>(HaralickSumEntropyFilter.GetPointer());

} // CreateHaralickSumEntropyFilter

TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickDifferenceEntropyFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                          , const ccipd::VolumeImageConstPointer & volume  
                                                         )
{
	typedef itk::HaralickDifferenceEntropyFilter<VolumeImageType, VolumeImageType>  HaralickDifferenceEntropyFilterType;
	HaralickDifferenceEntropyFilterType::Pointer HaralickDifferenceEntropyFilter = HaralickDifferenceEntropyFilterType::New();

	HaralickDifferenceEntropyFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickDifferenceEntropyFilter->SetRadius(parameters.GetRadius());

	HaralickDifferenceEntropyFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickDifferenceEntropyFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickDifferenceEntropyFilter->Update();

	return dynamic_cast<FilterBase*>(HaralickDifferenceEntropyFilter.GetPointer());

} // CreateHaralickDifferenceEntropyFilter


TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickIMC1Filter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                          , const ccipd::VolumeImageConstPointer & volume  
                                                         )
{
	typedef itk::HaralickIMC1Filter<VolumeImageType, VolumeImageType>  HaralickIMC1FilterType;
	HaralickIMC1FilterType::Pointer HaralickIMC1Filter = HaralickIMC1FilterType::New();

	HaralickIMC1Filter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickIMC1Filter->SetRadius(parameters.GetRadius());

	HaralickIMC1Filter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickIMC1Filter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickIMC1Filter->Update();

	return dynamic_cast<FilterBase*>(HaralickIMC1Filter.GetPointer());

} // CreateHaralickIMC1Filter


TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateHaralickIMC2Filter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                          , const ccipd::VolumeImageConstPointer & volume  
                                                         )
{
	typedef itk::HaralickIMC2Filter<VolumeImageType, VolumeImageType>  HaralickIMC2FilterType;
	HaralickIMC2FilterType::Pointer HaralickIMC2Filter = HaralickIMC2FilterType::New();

	HaralickIMC2Filter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	HaralickIMC2Filter->SetRadius(parameters.GetRadius());

	HaralickIMC2Filter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	HaralickIMC2Filter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	HaralickIMC2Filter->Update();

	return dynamic_cast<FilterBase*>(HaralickIMC2Filter.GetPointer());

} // CreateHaralickIMC2Filter

TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateCorrelationFilter(  const MapParametersType<TextureFeatureHaralick>::AllParametersType & parameters
                                                          , const ccipd::VolumeImageConstPointer & volume  
                                                         )
{
	typedef itk::CorrelationFilter<VolumeImageType, VolumeImageType>  CorrelationFilterType;
	CorrelationFilterType::Pointer CorrelationFilter = CorrelationFilterType::New();

	CorrelationFilter->SetInput(const_cast<VolumeImageType*>(volume.GetPointer())); // AA Convert ConstPointer to Pointer
	CorrelationFilter->SetRadius(parameters.GetRadius());

	CorrelationFilter->SetNumberOfBinsPerAxis(parameters.GetNumberOfHistogramBins()); // will be defaulted to 16 if not set by user
	CorrelationFilter->SetOffset(parameters.GetOffset()); // Offset will be defaulted to 0,1,0 if not set by user

	CorrelationFilter->Update();

	return dynamic_cast<FilterBase*>(CorrelationFilter.GetPointer());

} // CreateCorrelationFilter



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateGradientMagnitudeFilter(/*const MapParametersType< TextureFeatureGradientMagnitude >::AllParametersType & parameters*/  )
{
  //(void) parameters; // because this filter takes no parameters
  const bool useImageSpacing = true;

  // create our filter
  typedef itk::GradientMagnitudeImageFilter< VolumeImageType, VolumeImageType > GradientFilterType;
  GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
  gradientFilter->SetUseImageSpacing( useImageSpacing );

  // calculate
  // cast and return
  const FilterPointer filter = dynamic_cast< FilterBase * >( gradientFilter.GetPointer() );
  return filter;

} // CreateGradientMagnitudeFilter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateMeanFilter( 
const MapParametersType< TextureFeatureMean >::AllParametersType & parameters, 
const ccipd::VolumeImageConstPointer & volume)
{
  // Create Kernel
  VolumeImageType::Pointer pKernel = VolumeImageType::New();
  VolumeImageType::IndexType pKernelIndex = {{0, 0, 0}};
  // Kernel Size 
  VolumeImageType::SizeType pKernelSize; 
  pKernelSize.Fill(parameters.GetWindowSize());
  VolumeImageType::RegionType pKernelRegion;
  pKernelRegion.SetIndex(pKernelIndex);
  pKernelRegion.SetSize(pKernelSize);
  pKernel->SetRegions(pKernelRegion);
  pKernel->Allocate();
  pKernel->FillBuffer(1.0f); // Will be normalized when passed to the 


  // Filter
  typedef itk::FFTConvolutionImageFilter<VolumeImageType> FFTFilterType;
  FFTFilterType::Pointer meanFilter = FFTFilterType::New();
  meanFilter->SetInput(volume);
  meanFilter->SetKernelImage(pKernel);
  meanFilter->NormalizeOn(); // This will normalize the kernel
  itk::ConstantBoundaryCondition<VolumeImageType> nbc;
  meanFilter->SetBoundaryCondition(&nbc); // zero padding

  //// Display current date/time based on current system
  //time_t t_before = time(0);
  meanFilter->Update();
  //time_t t_after = time(0); 
  //cout << "Feature extracted in: " << difftime(t_after, t_before) << " seconds." << endl;

  VolumeImageType::Pointer pOut = meanFilter->GetOutput();
  pOut->SetSpacing(volume->GetSpacing());   
  pOut->SetOrigin(volume->GetOrigin());
  pOut->SetDirection(volume->GetDirection());
  
//  SaveImage(meanFilter->GetOutput(), "Fm01.mha", true);

  return dynamic_cast<FilterBase*>(meanFilter.GetPointer());

} // CreateMeanFilter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateMedianFilter( 
  const MapParametersType< TextureFeatureMedian >::AllParametersType & parameters )
{

  // use a median filter
  typedef itk::MedianImageFilter<VolumeImageType, VolumeImageType > MedianFilterType;

  MedianFilterType::Pointer medianFilter = MedianFilterType::New();
  medianFilter->SetRadius( parameters.GetWindowSize() );

  const FilterPointer filter = dynamic_cast< FilterBase * >( medianFilter.GetPointer() );

  return filter;

} // CreateMedianFilter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateNakagamiFilter(
  const MapParametersType< TextureFeatureNakagami >::AllParametersType & parameters )
{

  // our typedefs for the RayleighFilter
  typedef itk::NakagamiHistogram< VolumeImageType::PixelType > NakagamiHistogramType;
  typedef itk::FlatStructuringElement< VolumeDimension > StructuringElementType;

  typedef itk::MovingHistogramImageFilter<VolumeImageType, VolumeImageType, StructuringElementType, NakagamiHistogramType > NakagamiFilterType;

  //create the appropriate structuring element
  StructuringElementType::RadiusType elementRadius;
  elementRadius.Fill( parameters.GetWindowSize() );
  StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);

  //create the moving histogram
  NakagamiFilterType::Pointer nakagamiFilter = NakagamiFilterType::New();
  nakagamiFilter->SetKernel( structuringElement );

  // cast and return
  const FilterPointer filter = dynamic_cast< FilterBase * >( nakagamiFilter.GetPointer() );
  return filter;

} // CreateNakagamiFilter
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateRangeFilter(
  const MapParametersType< TextureFeatureRange >::AllParametersType & parameters )
{
  // create our filter
  typedef itk::RangeImageFilter< VolumeImageType, VolumeImageType > RangeFilterType;
  RangeFilterType::Pointer rangeFilter = RangeFilterType::New();
  rangeFilter->SetRadius(parameters.GetWindowSize());

  // cast and return
  const FilterPointer filter = dynamic_cast< FilterBase * >(rangeFilter.GetPointer());
  return filter;

} // CreateRangeFilter
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateRayleighFilter(
const MapParametersType< TextureFeatureRayleigh >::AllParametersType & parameters )
{

  // our typedefs for the RayleighFilter
  typedef itk::RayleighHistogram<VolumeImageType::PixelType> RayleighHistogramType;

  typedef itk::FlatStructuringElement< VolumeDimension > StructuringElementType;

  typedef itk::MovingHistogramImageFilter< VolumeImageType, VolumeImageType, StructuringElementType, RayleighHistogramType> RayleighFilterType;

  //create the appropriate structuring element
  StructuringElementType::RadiusType elementRadius;
  elementRadius.Fill( parameters.GetWindowSize() );
  StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);

  //create the moving histogram
  RayleighFilterType::Pointer rayleighFilter = RayleighFilterType::New();
  rayleighFilter->SetKernel( structuringElement );


  // cast and return
  const FilterPointer filter = dynamic_cast< FilterBase * >( rayleighFilter.GetPointer() );
  return filter;


} // CreateRayleighFilter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateSkewFilter(
const MapParametersType< TextureFeatureSkew >::AllParametersType & parameters )
{
  // create the filter
  typedef itk::SkewImageFilter< VolumeImageType, VolumeImageType > SkewFilterType;
  SkewFilterType::Pointer skewFilter = SkewFilterType::New();
  skewFilter->SetRadius( parameters.GetWindowSize() );

  // cast and return
  const FilterPointer filter = dynamic_cast< FilterBase * >( skewFilter.GetPointer() );
  return filter;
}// CreateSkewFilter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateSobelFilter(
const MapParametersType< TextureFeatureSobel >::AllParametersType & parameters )
{

  typedef itk::NeighborhoodOperatorImageFilter< VolumeImageType, VolumeImageType  > OperatorFilterType;

  itk::SobelOperator< InternalPixelType, VolumeDimension > sobelKernel;
  
  sobelKernel.SetDirection( parameters.GetDirection() );
  itk::Size<3> radiusElement;
  
  radiusElement.Fill( parameters.GetWindowSize() );
  sobelKernel.CreateToRadius(radiusElement);

  OperatorFilterType::Pointer operatorFilter = OperatorFilterType::New();
  operatorFilter->SetOperator(sobelKernel);

  
  // cast and return
  const FilterPointer filter = dynamic_cast< FilterBase * >(operatorFilter.GetPointer() );
  return filter;
}// CreateSobelFilter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureExtractor::SecretData::FilterPointer
TextureFeatureExtractor::SecretData::CreateStandardDeviationFilter(
const MapParametersType< TextureFeatureStandardDeviation >::AllParametersType & parameters )
{

  // create the filter
  typedef itk::StandardDeviationImageFilter< VolumeImageType, VolumeImageType > StdFilterType;
  StdFilterType::Pointer stdFilter = StdFilterType::New();
  stdFilter->SetRadius( parameters.GetWindowSize() );

  // cast and return
  const FilterPointer filter = dynamic_cast< FilterBase * >( stdFilter.GetPointer() );
  return filter;

} // CreateStandardDeviationFilter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool TextureFeatureExtractor::HasVolume() const
{
  //TODO: this should be stored in m_bHasVolume
  return this->GetVolume().IsNotNull();

} // HasVolume
//////////////////////////////////////////////////////////////////////////////////////////

VolumeConstImagesContainer TextureFeatureExtractor::GetMultiBandFeature(  const TextureFeatureEnum            whichFeature
                                                                        , const AnyTextureFeatureParameters & parameters
                                                                       )
{
  VolumeConstImagesContainer extractedFeature;
  
  const bool verbose = this->GetVerbose();
  
  // get our FeatureImage cache
  //SecretData::FeaturesCacheType & cache = this->m_SecretData->m_FeaturesCache;
  
  // first, check the cache-- this is simply the hash value from an image
  //const HashValueType thisHashValue = this->GetHash();
  ParameterPointer whichParameters = boost::apply_visitor(GetParametersByVariant(), parameters);
  //const HashValueType parametersHashValue = whichParameters->GetHash();

  // return a pointer if we have the same image, same features
  //VolumeConstImagesContainer extractedFeature = cache[ thisHashValue ][ whichFeature ][ parametersHashValue ]; 

  // if we found one in the cache, use it
  if ( 0 != extractedFeature.size() ) return extractedFeature;

  // extract the given feature
  if ( this->HasVolume() )
  {
    const VolumeImageConstPointer volume = this->GetVolume();

    if ( verbose )
    {
        cout << "Extracting feature " << this->m_SecretData->m_Parser.GetTextureFeatureString( whichFeature ) << ":" << endl;
    }  

    if ( whichFeature == TextureFeatureIntensity )
    {
      // this is a special case which just uses an external function
      extractedFeature.push_back(VolumeImageConstPointer(DuplicateImage(volume.GetPointer()))); 
    }
    else
    {
      // extract the desired filter for this feature & parameter-- For each new enum you must add an appropriate switch 
      SecretData::FilterPointer filter;

	    switch( whichFeature )
	    {
			case TextureFeatureGabor:
				filter = this->m_SecretData->CreateGaborFilter( *boost::get< std::shared_ptr< MapParametersType< TextureFeatureGabor >::AllParametersType>>( parameters )
                                                         , volume 
                                                        );
			break;

			
			  
  case TextureFeatureHaralickEntropy :
  break;
  


  case TextureFeatureHaralickClusterProminence:
  break;
  case TextureFeatureHaralickCorrelation:
  break;
  
  case TextureFeatureHaralickMean  :
  break;  
  
  case TextureFeatureHaralickSumAverage    :
break;  
  case TextureFeatureHaralickSumVariance    :     
break;
  
 
  case TextureFeatureHaralickDifferenceVariance  :    
  break;

			
			
			
			
		    case TextureFeatureHaralick:
		      filter = this->m_SecretData->CreateHaralickFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                            , volume
                                                           );
			  break;
			  
			  
			  
		    case TextureFeatureHaralickEnergy:
		      filter = this->m_SecretData->CreateHaralickEnergyFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                            , volume
                                                           );
        break;

        case TextureFeatureHaralickVariance:
          filter = this->m_SecretData->CreateHaralickVarianceFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                          , volume
                                                         );
			  break;
        case TextureFeatureHaralickClusterShade:
          filter = this->m_SecretData->CreateHaralickClusterShadeFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                          , volume
                                                         );
        break;

  case TextureFeatureHaralickInverseDifferenceMoment:
  filter = this->m_SecretData->CreateHaralickInverseDifferenceMomentFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                          , volume
                                                         );
  break;

				 		 case TextureFeatureHaralickInertia:
		  filter = this->m_SecretData->CreateHaralickInertiaFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                            , volume
                                                           );
			     break;
				 		 case TextureFeatureHaralickSumEntropy:
		  filter = this->m_SecretData->CreateHaralickSumEntropyFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                            , volume
                                                           );
			     break;
				 		 case TextureFeatureHaralickDifferenceEntropy:
		  filter = this->m_SecretData->CreateHaralickDifferenceEntropyFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                            , volume
                                                           );
			     break;
				 		 case TextureFeatureHaralickIMC1:
		  filter = this->m_SecretData->CreateHaralickIMC1Filter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                            , volume
                                                           );
			     break;
				 		 case TextureFeatureHaralickIMC2:
		  filter = this->m_SecretData->CreateHaralickIMC2Filter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                            , volume
                                                           );
			     break;
				 		 case TextureFeatureCorrelation:
		  filter = this->m_SecretData->CreateCorrelationFilter( *boost::get<std::shared_ptr< MapParametersType< TextureFeatureHaralick >::AllParametersType>>(parameters)
                                                            , volume
                                                           );
			     break;








        case TextureFeatureGradientMagnitude:
		  filter = this->m_SecretData->CreateGradientMagnitudeFilter(/* 
            *boost::get< std::shared_ptr< MapParametersType< TextureFeatureGradientMagnitude >::AllParametersType > >( parameters )*/ );
          break;
        case TextureFeatureIntensity:
          filter = NULL;  // this is just because gcc is being stupid; this line will never be reached.
          break;
	      case TextureFeatureMean:
		      filter = this->m_SecretData->CreateMeanFilter( 
			      *boost::get< std::shared_ptr< MapParametersType< TextureFeatureMean >::AllParametersType > >( parameters ), volume  );
          break;
		    case TextureFeatureMedian:
		      filter = this->m_SecretData->CreateMedianFilter(
			      *boost::get< std::shared_ptr< MapParametersType< TextureFeatureMedian >::AllParametersType > >( parameters ) );
          break;
		    case TextureFeatureNakagami:
		      filter = this->m_SecretData->CreateNakagamiFilter(
			      *boost::get< std::shared_ptr< MapParametersType< TextureFeatureNakagami >::AllParametersType > >( parameters ) );
          break;
		    case TextureFeatureRange:
		      filter = this->m_SecretData->CreateRangeFilter (
			      *boost::get< std::shared_ptr< MapParametersType< TextureFeatureRange >::AllParametersType > >( parameters ) );
          break;
		    case TextureFeatureRayleigh:
		      filter = this->m_SecretData->CreateRayleighFilter (
			      *boost::get< std::shared_ptr<  MapParametersType< TextureFeatureRayleigh >::AllParametersType > >( parameters ) );
          break;
        case TextureFeatureSkew:
          filter = this->m_SecretData->CreateSkewFilter( 
            *boost::get< std::shared_ptr< MapParametersType< TextureFeatureSkew >::AllParametersType > >( parameters ) );
          break;
		    case TextureFeatureSobel:
		      filter = this->m_SecretData->CreateSobelFilter (
			      *boost::get< std::shared_ptr< MapParametersType< TextureFeatureSobel >::AllParametersType > >( parameters ) );
          break;
		    case TextureFeatureStandardDeviation:
		      filter = this->m_SecretData->CreateStandardDeviationFilter (
			      *boost::get< std::shared_ptr< MapParametersType< TextureFeatureStandardDeviation >::AllParametersType > >( parameters ) );
          break;
	      } // switch( whichFeature )

      // use it
      filter->SetInput( volume );
      filter->UpdateLargestPossibleRegion(); // This is used instead of filter->Update();

      for(unsigned int i = 0; i < filter->GetNumberOfOutputs(); i++)
      {
        extractedFeature.push_back(filter->GetOutput(i));
      }

      } // else

    if ( verbose ) cout << "Done ............ \n";

    } // we have a volume

    // update our cache // we need to cash the volume too
    //cache[ thisHashValue ][ whichFeature ][ parametersHashValue ] = extractedFeature;

    return extractedFeature;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer TextureFeatureExtractor::GetFeatures(  const TextureFeatureEnumCollection & whichFeatures
                                                           , const TextureFeatureParametersCollection & whichParameters )  // need to pass in a collection for parameters as well
{
  // how many features do we have?
  const size_t numberOfFeatures = whichFeatures.size();

  // make sure it's at least one and we have a volume
  if ( numberOfFeatures < 1 || !( this->HasVolume() ) )
    return VolumeFeaturesPointer();

  VolumeConstImagesContainer features;

  // go through each feature
  for ( size_t featureIndex = 0; featureIndex < numberOfFeatures; ++featureIndex )
    {

    // extract which feature and parameter it is
    const TextureFeatureEnum           whichFeature = whichFeatures[ featureIndex ];
	  const AnyTextureFeatureParameters  whichParameter = whichParameters[ featureIndex ];

    // calculate that feature
    
    VolumeConstImagesContainer featureImage =  this->GetMultiBandFeature( whichFeature, whichParameter ) ;
    for(unsigned int componentIndex = 0; componentIndex < featureImage.size(); componentIndex++)
    {
      features.push_back(featureImage.at(componentIndex));
    }

    } // for featureIndex

  // concatenate and return
  return VolumeContainerToVolumeFeatures(features);

} // GetFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer TextureFeatureExtractor::GetAllFeatures()
{

  // collect all the features we're aware of.
  TextureFeatureEnumCollection whichFeatures;
  whichFeatures.push_back( TextureFeatureGabor             );
  whichFeatures.push_back( TextureFeatureHaralick          );

  whichFeatures.push_back( TextureFeatureHaralickEnergy            );
  whichFeatures.push_back( TextureFeatureHaralickInertia           );
  whichFeatures.push_back( TextureFeatureHaralickSumEntropy        );
  whichFeatures.push_back( TextureFeatureHaralickDifferenceEntropy );
  whichFeatures.push_back( TextureFeatureHaralickIMC1              );
  whichFeatures.push_back( TextureFeatureHaralickIMC2              );
  
  whichFeatures.push_back( TextureFeatureCorrelation       );

  whichFeatures.push_back( TextureFeatureGradientMagnitude );
  whichFeatures.push_back( TextureFeatureIntensity         );
  whichFeatures.push_back( TextureFeatureMean              );
  whichFeatures.push_back( TextureFeatureMedian            );
  whichFeatures.push_back( TextureFeatureNakagami          );
  whichFeatures.push_back( TextureFeatureRange             );
  whichFeatures.push_back( TextureFeatureRayleigh          );
  whichFeatures.push_back( TextureFeatureSkew              );
  whichFeatures.push_back( TextureFeatureSobel             );
  whichFeatures.push_back( TextureFeatureStandardDeviation );

  // initialize default parameters
  TextureFeatureParametersCollection whichParameters;

  whichParameters.push_back(  MapParametersType< TextureFeatureGabor             >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureHaralick          >::AllParametersType::New() );

//whichParameters.push_back(  MapParametersType< TextureFeatureHaralickEnergy            >::AllParametersType::New() );
//whichParameters.push_back(  MapParametersType< TextureFeatureHaralickInertia           >::AllParametersType::New() );
//whichParameters.push_back(  MapParametersType< TextureFeatureHaralickSumEntropy        >::AllParametersType::New() );
//whichParameters.push_back(  MapParametersType< TextureFeatureHaralickDifferenceEntropy >::AllParametersType::New() );
//whichParameters.push_back(  MapParametersType< TextureFeatureHaralickIMC1              >::AllParametersType::New() );
//whichParameters.push_back(  MapParametersType< TextureFeatureHaralickIMC2              >::AllParametersType::New() );
//
//whichParameters.push_back(  MapParametersType< TextureFeatureCorrelation          >::AllParametersType::New() );

  whichParameters.push_back(  MapParametersType< TextureFeatureGradientMagnitude >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureIntensity         >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureMean              >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureMedian            >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureNakagami          >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureRange             >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureRayleigh          >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureSkew              >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureSobel             >::AllParametersType::New() );
  whichParameters.push_back(  MapParametersType< TextureFeatureStandardDeviation >::AllParametersType::New() );

  // calculate and return
  return this->GetFeatures( whichFeatures, whichParameters );
  
} // GetAllFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// set/get functions
ccipdGetConstImplementMacro    ( TextureFeatureExtractor, Volume , VolumeImageConstPointer )
ccipdSetByvalImplementHashMacro( TextureFeatureExtractor, Volume , VolumeImageType *       )
ccipdGetConstImplementMacro    ( TextureFeatureExtractor, Verbose, bool                    )
ccipdSetByvalImplementMacro    ( TextureFeatureExtractor, Verbose, bool                    ) // doesn't change the hash value 
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
