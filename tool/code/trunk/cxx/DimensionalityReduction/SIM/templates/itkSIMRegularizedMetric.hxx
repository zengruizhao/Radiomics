#ifndef __itkSIMRegularizedMetric_hxx
#define __itkSIMRegularizedMetric_hxx




////////////////////////////////////////////////////////////////////////////////////////////////////
// itk includes
#include "itkSIMRegularizedMetric.h"
#include "ccipdSIMStorage.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
class SIMRegularizedMetric< TPixelType, TDimension >::SecretData
{
public: // public to the SIMRegularizedMetric class

  SecretData();

  typedef SIMRegularizedMetric<
    TPixelType,
    TDimension
  > EncapsulatingClass;

  typedef typename EncapsulatingClass::SIMStorageType  SIMStorageType;

  typename SIMStorageType::Pointer m_SIM;

  SampleLocationsContainer  m_Samples;
}; // class SecretData
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
SIMRegularizedMetric< TPixelType, TDimension >
::SIMRegularizedMetric() 
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
SIMRegularizedMetric< TPixelType, TDimension >
::SecretDataWorkaround::SecretDataWorkaround() :
std::unique_ptr< SecretData >( new SecretData )
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
SIMRegularizedMetric< TPixelType, TDimension >
::SecretData::SecretData() :
m_SIM( SIMStorageType::New() )
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
SIMRegularizedMetric< TPixelType, TDimension >
::SecretDataWorkaround::~SecretDataWorkaround()
{
} // Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
SIMRegularizedMetric< TPixelType, TDimension >
::~SIMRegularizedMetric()
{
} // Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
void
SIMRegularizedMetric< TPixelType, TDimension >
::Initialize() throw ( ExceptionObject )
{
  // first initialize the super class
  this->Superclass::Initialize();

  // then initialize the locations of where we are sampling the SIM from.
  this->SampleSIM();
} // Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
void
SIMRegularizedMetric< TPixelType, TDimension >
::SampleSIM()
{
  // the MultiThreadingInitialize function actually
  // pre-samples our fixed image, which is exactly what
  // we want to do.  The result is in m_FixedImageSamples.
  this->Superclass::MultiThreadingInitialize();
  
  // prepare the output
  SampleLocationsContainer samples;
  samples.resize( this->Superclass::m_FixedImageSamples.size() );

  // prepare some iterators to actually populate a list of only points (not values)
  typename FixedImageSampleContainer::const_iterator
    fixedIter = this->Superclass::m_FixedImageSamples.begin();
  const typename FixedImageSampleContainer::const_iterator
    fixedEnd = this->Superclass::m_FixedImageSamples.end();
  typename SampleLocationsContainer::iterator
    samplesIter = samples.begin();

  for ( ; fixedIter != fixedEnd; ++fixedIter, ++samplesIter )
    {
    
    *samplesIter = fixedIter->point;
    }

  this->m_SecretData->m_Samples = samples;
} // SampleSIM
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace itk
#endif // __itkSIMRegularizedMetric_hxx
