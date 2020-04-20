#ifndef __itkSelectiveInterpolateMultipleImagesFunction_hxx
#define __itkSelectiveInterpolateMultipleImagesFunction_hxx

#include "itkSelectiveInterpolateMultipleImagesFunction.h"

#include "vnl/vnl_math.h"

namespace itk
{

/**
 * Evaluate at image index position
 * FIXME: ONLY works for the 3D case
 */
template< typename TInputImage, typename TCoordRep >
typename SelectiveInterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::OutputType
SelectiveInterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::EvaluateUnoptimized(const std::vector<ContinuousIndexType> & indexes) const
{
  unsigned int dim;  // index over dimension

  std::vector<IndexType>      
    baseIndexes(indexes.size());

  std::vector <IndexType> nindexes;
  IndexType nindex;
  nindex[0] = 0; nindex[1] = 0; nindex[2] = 0; nindexes.push_back(nindex);
  nindex[0] = 1; nindex[1] = 0; nindex[2] = 0; nindexes.push_back(nindex);
  nindex[0] = 0; nindex[1] = 1; nindex[2] = 0; nindexes.push_back(nindex);
  nindex[0] = 1; nindex[1] = 1; nindex[2] = 0; nindexes.push_back(nindex);

  nindex[0] = 0; nindex[1] = 0; nindex[2] = 1; nindexes.push_back(nindex);
  nindex[0] = 1; nindex[1] = 0; nindex[2] = 1; nindexes.push_back(nindex);
  nindex[0] = 0; nindex[1] = 1; nindex[2] = 1; nindexes.push_back(nindex);
  nindex[0] = 1; nindex[1] = 1; nindex[2] = 1; nindexes.push_back(nindex);

  std::vector<RealType> values;
  std::vector<InternalComputationType> distance;
  for (dim = 0; dim < this->ImageDimension; dim++ )
    distance.push_back( NumericTraits< InternalComputationType >::Zero);

  typedef typename NumericTraits< RealType >::ScalarRealType RealTypeScalarRealType;
  RealType outValue = NumericTraits< RealTypeScalarRealType >::Zero;

  IndexType neighIndex;
  RealType countAdds = 0.0;
  for (unsigned int idx=0; idx < indexes.size(); idx++ )
    {
    for (dim = 0; dim < this->ImageDimension; dim++ )
      {
      /**
       * Compute base index = closet index below point
       * Compute distance from point to base index
       */
      baseIndexes[idx][dim]     = Math::Floor< IndexValueType >(indexes[idx][dim]);
      distance[dim]             = indexes[idx][dim] - baseIndexes[idx][dim];
      }

    values.clear();
    int countOutside = 0;
    for (unsigned int nidx=0; nidx < nindexes.size(); ++nidx)
    {
      bool outsideBorder = false;
      for (dim = 0; dim < this->ImageDimension; dim++)
      {
        neighIndex[dim] = baseIndexes[idx][dim]+nindexes[nidx][dim];

        //make sure we are not out of bounce
        if ( neighIndex[dim] < this->m_StartIndex[idx][dim]) {
          neighIndex[dim] = this->m_StartIndex[idx][dim];
          outsideBorder = true;
        }
        
        if (neighIndex[dim] > this->m_EndIndex[idx][dim]) {
          neighIndex[dim] =  this->m_EndIndex[idx][dim];
          outsideBorder = true;
        }
      }

      if (!outsideBorder)
        values.push_back( this->GetInputImage(idx)->GetPixel(neighIndex) );
      else {
        values.push_back( NumericTraits< RealTypeScalarRealType >::Zero );
        countOutside++;
      }
    }

    if (countOutside<nindexes.size()) {
      RealType v00    = distance[0]*(values[1]-values[0])+values[0];
      RealType v10    = distance[0]*(values[3]-values[2])+values[2];
      RealType v0     = distance[1]*(v10-v00) + v00;

      RealType v01    = distance[0]*(values[5]-values[4])+values[4];
      RealType v11    = distance[0]*(values[7]-values[6])+values[6];
      RealType v1     = distance[1]*(v11-v01) + v01;
      RealType value  = distance[2]*(v1-v0) + v0;

      RealType factor = distance[0]>(1-distance[0])?distance[0]:(1-distance[0]);
      RealType product3Max = factor; //the product of the 3 max distances
      factor = distance[1]>(1-distance[1])?distance[1]:(1-distance[1]);
      product3Max *= factor;
      factor = distance[2]>(1-distance[2])?distance[2]:(1-distance[2]);
      product3Max *= factor;

      outValue += value*product3Max;
      countAdds += product3Max; 
    }
  }
  outValue /= (RealType) (countAdds);
  return ( static_cast< OutputType >( outValue ) );
}
} // end namespace itk

#endif
