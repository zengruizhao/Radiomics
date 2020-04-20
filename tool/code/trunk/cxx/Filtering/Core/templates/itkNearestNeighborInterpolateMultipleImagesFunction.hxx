#ifndef __itkNearestNeighborInterpolateMultipleImagesFunction_hxx
#define __itkNearestNeighborInterpolateMultipleImagesFunction_hxx

#include "itkNearestNeighborInterpolateMultipleImagesFunction.h"

#include "vnl/vnl_math.h"

namespace itk
{
/**
 * Evaluate at image index position
 */
template< typename TInputImage, typename TCoordRep >
typename NearestNeighborInterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::OutputType
NearestNeighborInterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::EvaluateUnoptimized(const std::vector<ContinuousIndexType> & indexes) const
{
  // When RealType is VariableLengthVector, 'value' will be resized properly
  // below when it's assigned again.
  typedef typename NumericTraits< RealType >::ScalarRealType RealTypeScalarRealType;
  RealType value;
  value = NumericTraits< RealTypeScalarRealType >::Zero;

  int countAdd = 0;

  for (unsigned int idx=0; idx < indexes.size(); idx++)
  {
    IndexType nindex;
    this->ConvertContinuousIndexToNearestIndex(indexes[idx], nindex);

    bool add = true;
    for (unsigned dim = 0; dim < this->ImageDimension; dim++ )
      {
        if ( nindex[dim] > this->m_EndIndex[idx][dim] )
        {
          nindex[dim] = this->m_EndIndex[idx][dim];
          add = false;
        }
        if ( nindex[dim] < this->m_StartIndex[idx][dim] )
        {
          nindex[dim] = this->m_StartIndex[idx][dim];
          add = false;
        }
      }
      if (add)
      {
        OutputType pixval = static_cast< OutputType >( this->GetInputImage(idx)->GetPixel(nindex) );    
        value += pixval;
        countAdd++;
      }
  }

  return ( static_cast< OutputType >( (RealType)value/(RealType)countAdd ) );
}
} // end namespace itk

#endif
