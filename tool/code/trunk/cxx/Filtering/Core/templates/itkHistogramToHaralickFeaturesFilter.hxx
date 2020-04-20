
 #ifndef __itkHistogramToHaralickFeaturesFilterFilter_hxx
 #define __itkHistogramToHaralickFeaturesFilterFilter_hxx
 #include "itkHistogramToHaralickFeaturesFilter.h"
 #include "itkNumericTraits.h"
 #include "vnl/vnl_math.h"
 #include <algorithm>

 namespace itk 
{
namespace Statistics
{
//constructor
 template<class THistogram>
HistogramToHaralickFeaturesFilter< THistogram >::HistogramToHaralickFeaturesFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs(1);

  // allocate the data objects for the outputs which are
  // just decorators real types
  for ( int i = 0; i < 9; ++i )
    {
    this->ProcessObject::SetNthOutput( i, this->MakeOutput(i) );
    }
}

template< class THistogram >
void
HistogramToHaralickFeaturesFilter< THistogram >
::SetInput(const HistogramType *histogram)
{
  this->ProcessObject::SetNthInput( 0, const_cast< HistogramType * >( histogram ) );
}
template< class THistogram >
const typename
HistogramToHaralickFeaturesFilter< THistogram >::HistogramType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetInput() const
{
  return itkDynamicCastInDebugMode< const HistogramType * >( this->GetPrimaryInput() );
}

template< class THistogram >
typename
HistogramToHaralickFeaturesFilter< THistogram >::DataObjectPointer
HistogramToHaralickFeaturesFilter< THistogram >
::MakeOutput( DataObjectPointerArraySizeType itkNotUsed(idx) )
{
  return MeasurementObjectType::New().GetPointer();
}

template< class THistogram >
void
HistogramToHaralickFeaturesFilter<THistogram>::
GenerateData()
{	 
   const HistogramType* inputHistogram = this->GetInput();
   unsigned long histogramSizeX = inputHistogram->GetSize()[0];
   unsigned long histogramSizeY = inputHistogram->GetSize()[1];

   TotalRelativeFrequencyType totalFrequency = static_cast< TotalRelativeFrequencyType >( inputHistogram->GetTotalFrequency() );

   // Now get the pixel mean.
   MeasurementType m_Mean     = NumericTraits< MeasurementType >::Zero;   


   for (HistogramIterator hit = inputHistogram->Begin(); hit != inputHistogram->End(); ++hit)
   {
     m_Mean += inputHistogram->GetIndex(hit.GetInstanceIdentifier())[0] * (RelativeFrequencyType) hit.GetFrequency() / totalFrequency;
   }

   MeasurementType m_SumAverage      = NumericTraits< MeasurementType >::Zero; 
   MeasurementType m_SumEntropy      = NumericTraits< MeasurementType >::Zero; 
   MeasurementType m_SumVariance     = NumericTraits< MeasurementType >::Zero; 


   double PSSquareCumul = 0;
   double log2 = vcl_log(2.);
 
  // Calculate SumAverage, SumVariance
  unsigned long totalBins = histogramSizeX + histogramSizeY;
  for (unsigned long i = 0; i < totalBins; ++i)
  {

///////////////////////////////
    unsigned long start = std::max<unsigned long>(static_cast <unsigned long> (0), i - histogramSizeY);
    unsigned long end   = std::min<unsigned long>(i, histogramSizeX);
  
    double psTmp = 0;
    IndexType index;
    index.SetSize(2);
    
    for (unsigned long j = start; j < end; ++j)
    {
      index[0] = j;
      index[1] = i - j;
      psTmp += (RelativeFrequencyType) inputHistogram->GetFrequency(index) / totalFrequency ;
    }

//////////////////////////////


    m_SumAverage += (MeasurementType) i * psTmp;
    m_SumEntropy -= (psTmp > 0.0001) ? psTmp * vcl_log(psTmp) / log2 : 0;
    PSSquareCumul += i * i * psTmp;
  }

  m_SumVariance = PSSquareCumul - (m_SumAverage * m_SumAverage);
 
  unsigned long minSizeHist = std::min<unsigned long>(histogramSizeX, histogramSizeY);


 MeasurementType m_DifferenceEntropy      = NumericTraits< MeasurementType >::Zero; 
 MeasurementType m_DifferenceVariance      = NumericTraits< MeasurementType >::Zero; 
 
  double PDSquareCumul = 0;
  double PDCumul = 0;
 
  // Calculate DifferenceVariance and DifferenceEntropy
  for (long unsigned int i = 0; i < minSizeHist; ++i)
  {
    //double pdTmp = ComputePD(i);
    //////////////////////////////////////////////////
    double pdTmp = 0;
    IndexType index;
    index.SetSize(2);
    unsigned long end = std::min<unsigned long>(histogramSizeX - i, histogramSizeY);
    for (unsigned long j = 0; j < end; ++j)
    {
      index[0] = j + i;
      index[1] = j;
      pdTmp += (RelativeFrequencyType) inputHistogram->GetFrequency(index) / totalFrequency;
    }

    //////////////////////////////////////////////////
    PDCumul += i * pdTmp;
    m_DifferenceEntropy -= (pdTmp > 0.0001) ? pdTmp * vcl_log(pdTmp) / log2 : 0;
    PDSquareCumul += i * i * pdTmp;
  }

  m_DifferenceVariance = PDSquareCumul - PDCumul * PDCumul;
 
  
 
  double hx = 0;
  double hy = 0;
 
  // Compute hx and hy need to compute f12 and f13 texture coefficients
  for (unsigned long i = 0; i < histogramSizeX; ++i)
  {
    RelativeFrequencyType marginalfreq = (RelativeFrequencyType) inputHistogram->GetFrequency(i, 0) / totalFrequency;
    hx += (marginalfreq > 0.0001) ? vcl_log (marginalfreq) * marginalfreq : 0;
  }
 
  for (unsigned long j = 0; j < histogramSizeY; ++j)
  {
    RelativeFrequencyType marginalfreq = (RelativeFrequencyType) inputHistogram->GetFrequency(j, 1) / totalFrequency;
    hy += (marginalfreq > 0.0001) ? vcl_log (marginalfreq) * marginalfreq : 0;
  }
 
  double hxy1 = 0.0;
  double hxy2 = 0.0;
 
 
  double Entropy = 0.0;
  MeasurementType m_Variance      = NumericTraits< MeasurementType >::Zero;


  // Calculate Sum of squares (variance), entropy (needed for f12)
  for (HistogramIterator hit = inputHistogram->Begin(); hit != inputHistogram->End(); ++hit)
  {
    MeasurementType relFrequency = (RelativeFrequencyType) hit.GetFrequency() / totalFrequency;
 
    IndexType index = inputHistogram->GetIndex(hit.GetInstanceIdentifier());
 
    m_Variance += ((index[0] - m_Mean) * (index[0] - m_Mean)) * relFrequency;
    Entropy -= (relFrequency > 0.0001) ? relFrequency * vcl_log(relFrequency) / log2 : 0;
 
    double pipj = (RelativeFrequencyType)inputHistogram->GetFrequency(index[0], 0) * (RelativeFrequencyType)inputHistogram->GetFrequency(index[1], 1) / (totalFrequency * totalFrequency);
 
    hxy1 -= (pipj > 0.0001) ? relFrequency * vcl_log(pipj) : 0;
    hxy2 -= (pipj > 0.0001) ? pipj * vcl_log(pipj) : 0;
  }
 
  //Calculate f12 and f13 

  MeasurementType  m_IC1 = (vcl_abs(std::max (hx, hy)) > 0.0001) ? (Entropy - hxy1) / (std::max (hx, hy)) :NumericTraits< MeasurementType >::Zero;
 
  MeasurementType m_IC2 = 1 - vcl_exp (-2. * vcl_abs (hxy2 - Entropy));
  m_IC2 = (m_IC2 >= 0) ? vcl_sqrt (m_IC2) :NumericTraits< MeasurementType >::Zero; 






  MeasurementObjectType *meanOutputObject        = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(0) );
  meanOutputObject->Set(m_Mean);

  MeasurementObjectType *varianceOutputObject    = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(1) );
  varianceOutputObject->Set(m_Variance);

  MeasurementObjectType *sumAverageOutputObject  = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(2) );
  sumAverageOutputObject->Set(m_SumAverage);

  MeasurementObjectType *sumVarianceOutputObject = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(3) );
  sumVarianceOutputObject->Set(m_SumVariance);

  MeasurementObjectType *sumEntropyOutputObject  = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(4) );
  sumEntropyOutputObject->Set(m_SumEntropy);

  MeasurementObjectType *differenceEntropyOutputObject = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(5) );
  differenceEntropyOutputObject->Set(m_DifferenceEntropy);

  MeasurementObjectType *differenceVarianceOutputObject = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(6) );
  differenceVarianceOutputObject->Set(m_DifferenceVariance);

  MeasurementObjectType *iMC1OutputObject = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(7) );
  iMC1OutputObject->Set(m_IC1);

  MeasurementObjectType *iMC2OutputObject = static_cast< MeasurementObjectType * >( this->ProcessObject::GetOutput(8) );
 iMC2OutputObject->Set(m_IC2);

 }

 
template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetMeanOutput() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(0) );
}	

template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetVarianceOutput() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(1) );
}	

template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetSumAverageOutput() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(2) );
}	 

template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetSumVarianceOutput() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(3) );
}  

template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetSumEntropyOutput() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(4) );
}	  

template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetDifferenceEntropyOutput() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(5) );
}	 

template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetDifferenceVarianceOutput() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(6) );
}	

template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetIMC1Output() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(7) );
}
template< class THistogram >
const
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementObjectType *
HistogramToHaralickFeaturesFilter< THistogram >
::GetIMC2Output() const
{
  return static_cast< const MeasurementObjectType * >( this->ProcessObject::GetOutput(8) );
}

template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetMean() const
{
  return this->GetMeanOutput()->Get();
}

template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetVariance() const
{
  return this->GetVarianceOutput()->Get();
}	

template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetSumAverage() const
{
  return this->GetSumAverageOutput()->Get();
}
 
template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetSumVariance() const
{
  return this->GetSumVarianceOutput()->Get();
}  
		 
template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetSumEntropy() const
{
  return this->GetSumEntropyOutput()->Get();
}  


template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetDifferenceEntropy() const
{
  return this->GetDifferenceEntropyOutput()->Get();
} 	  

template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetDifferenceVariance() const
{
  return this->GetDifferenceVarianceOutput()->Get();
} 	 

template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetIMC1() const
{
  return this->GetIMC1Output()->Get();
}  

template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetIMC2() const
{
  return this->GetIMC2Output()->Get();
} 



template< class THistogram >
typename HistogramToHaralickFeaturesFilter< THistogram >::MeasurementType
HistogramToHaralickFeaturesFilter< THistogram >
::GetFeature(ExtendedTextureFeatureName feature)
{
  switch ( feature )
    {
    case Mean :
      return this->GetMean();
    case Variance:
      return this->GetVariance();
    case SumAverage:
      return this->GetSumAverage();
    case SumVariance:
      return this->GetSumVariance();
    case SumEntropy:
      return this->GetSumEntropy();
    case DifferenceEntropy:
      return this->GetDifferenceEntropy();
    case DifferenceVariance:
      return this->GetDifferenceVariance();
    case IMC1:
      return this->GetIMC1();		  
    case IMC2:
      return this->GetIMC2();
    default:
      return 0;
    }
}


 template<class THistogram>
 void
 HistogramToHaralickFeaturesFilter<THistogram>::
 PrintSelf(std::ostream& os, Indent indent) const

 {
   Superclass::PrintSelf(os, indent);
 }
} // end of namespace Statistics
} // end of namespace itk
 
 #endif

