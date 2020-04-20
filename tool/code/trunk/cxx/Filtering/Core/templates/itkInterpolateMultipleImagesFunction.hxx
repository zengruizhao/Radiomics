#ifndef __itkInterpolateMultipleImagesFunction_hxx
#define __itkInterpolateMultipleImagesFunction_hxx

#include "itkInterpolateMultipleImagesFunction.h"

#include "vnl/vnl_math.h"

namespace itk
{
/**
 * Define the number of neighbors
 */
template< typename TInputImage, typename TCoordRep >
const unsigned long
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::m_Neighbors = 1 << TInputImage::ImageDimension;

/**
 * Constructor
 */
template< typename TInputImage, typename TCoordRep >
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::InterpolateMultipleImagesFunction()
{
}

template< typename TInputImage, typename TCoordRep >
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::~InterpolateMultipleImagesFunction()
{}

/**
 * PrintSelf
 */
template< typename TInputImage, typename TCoordRep >
void
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::PrintSelf(std::ostream & os, Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/** AddInputImage */ 
template< typename TInputImage, typename TCoordRep >
void
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::AddInputImage(const TInputImage * imgPtr )
{
  m_Images.push_back(imgPtr);

  IndexType 
    StartIndex, 
    EndIndex;
  
  ContinuousIndexType
    StartContinuousIndex,
    EndContinuousIndex;

  if ( imgPtr )
    {
    typename InputImageType::SizeType size = imgPtr->GetBufferedRegion().GetSize();
    StartIndex = imgPtr->GetBufferedRegion().GetIndex();

    for ( unsigned int j = 0; j < ImageDimension; j++ )
      {
      EndIndex[j] = StartIndex[j] + static_cast< IndexValueType >( size[j] ) - 1;
      StartContinuousIndex[j] = static_cast< CoordRepType >( StartIndex[j] - 0.5 );
      EndContinuousIndex[j]   = static_cast< CoordRepType >( EndIndex[j] + 0.5 );
      }
    }
  else
    {
    StartIndex.Fill(0);
    EndIndex.Fill(0);
  
   StartContinuousIndex.Fill(0.0f);
   EndContinuousIndex.Fill(0.0f);
   }

  m_StartIndex.push_back(StartIndex);
  m_EndIndex.push_back(EndIndex);

  m_StartContinuousIndex.push_back(StartContinuousIndex);
  m_EndContinuousIndex.push_back(EndContinuousIndex);
};

/** CrearInput */ 
template< typename TInputImage, typename TCoordRep >
void
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::ClearInputImages()
{
  //FIXME: not sure if I really need to make them first NULL; but did just in case
  for (unsigned int idx=0; idx < m_Images.size(); ++idx)
    m_Images[idx] =  NULL;

  m_Images.clear();
};

/** SetInputImage */ 
template< typename TInputImage, typename TCoordRep >
void
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::SetInputImage(const int index, const TInputImage * imgPtr)
{
  std::cout << "Setting images " << index << std::endl;
  
  //fill the images as some of the indexes have not been assigned
  while (m_Images.size()<=index)
    AddInputImage(NULL);

  if (!m_Images[index])
    m_Images[index] = imgPtr;


  IndexType 
    StartIndex, 
    EndIndex;
  
  ContinuousIndexType
    StartContinuousIndex,
    EndContinuousIndex;

  if ( imgPtr )
    {
    typename InputImageType::SizeType size = imgPtr->GetBufferedRegion().GetSize();
    StartIndex = imgPtr->GetBufferedRegion().GetIndex();

    for ( unsigned int j = 0; j < ImageDimension; j++ )
      {
      EndIndex[j] = StartIndex[j] + static_cast< IndexValueType >( size[j] ) - 1;
      StartContinuousIndex[j] = static_cast< CoordRepType >( StartIndex[j] - 0.5 );
      EndContinuousIndex[j]   = static_cast< CoordRepType >( EndIndex[j] + 0.5 );
      }
    }
  else
    {
    StartIndex.Fill(0);
    EndIndex.Fill(0);
  
   StartContinuousIndex.Fill(0.0f);
   EndContinuousIndex.Fill(0.0f);
   }

  m_StartIndex.push_back(StartIndex);
  m_EndIndex.push_back(EndIndex);

  m_StartContinuousIndex.push_back(StartContinuousIndex);
  m_EndContinuousIndex.push_back(EndContinuousIndex);

  std::cout << "Done setting images " << index << std::endl;

};

/** 
 * getInputImage 
 */ 
template< typename TInputImage, typename TCoordRep >
const TInputImage * 
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::GetInputImage(const int index) const
{
  return m_Images[index].GetPointer();
};

/** 
 * getInputImage 
 */ 
template< typename TInputImage, typename TCoordRep >
const TInputImage * 
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::GetInputImage() const
{
  return m_Images[0].GetPointer();
};


/**
 * Evaluate at image index position
 */
template< typename TInputImage, typename TCoordRep >
typename InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::OutputType
InterpolateMultipleImagesFunction< TInputImage, TCoordRep >
::EvaluateUnoptimized(const std::vector<ContinuousIndexType> & indexes) const
{
  unsigned int dim;  // index over dimension

  /**
   * Compute base index = closet index below point
   * Compute distance from point to base index
   */
  //IndexType                   baseIndex;
  std::vector<IndexType>      baseIndexes;
  ContinuousIndexType         index = indexes[0]; 
  //InternalComputationType     distance[ImageDimension];
  std::vector<std::vector<InternalComputationType > > 
    distances,
    distancesFloor,
    distancesCeil;

  for (unsigned int idx=0; idx < indexes.size(); idx++ )
    {
    for ( dim = 0; dim < ImageDimension; dim++ )
      {
      baseIndexes[idx][dim]     = Math::Floor< IndexValueType >(indexes[idx][dim]);
      distances[idx][dim]       = (indexes[idx][dim] - static_cast< InternalComputationType >( baseIndexes[idx][dim] ));

      auto voxelSpacing = m_Images[idx]->GetSpacing()[idx];

      distancesFloor[idx][dim]  = distances[idx][dim]       * voxelSpacing;
      distancesCeil[idx][dim]   = (1.0-distances[idx][dim]) * voxelSpacing;
      }
    }

  /**
   * Interpolated value is the weighted sum of each of the surrounding
   * neighbors. The weight for each neighbor is the fraction overlap
   * of the neighbor pixel with respect to a pixel centered on point.
   */
  // When RealType is VariableLengthVector, 'value' will be resized properly
  // below when it's assigned again.
  typedef typename NumericTraits< RealType >::ScalarRealType RealTypeScalarRealType;
  RealType value;
  value = NumericTraits< RealTypeScalarRealType >::Zero;

  typedef typename NumericTraits< InputPixelType >::ScalarRealType InputPixelScalarRealType;
  InputPixelScalarRealType totalOverlap = NumericTraits< InputPixelScalarRealType >::Zero;
  bool firstOverlap = true;

  for ( unsigned int counter = 0; counter < m_Neighbors; ++counter )
    {
    InternalComputationType       overlap = 1.0;    // fraction overlap
    unsigned int upper = counter;  // each bit indicates upper/lower neighbour
    IndexType    neighIndex;

    // get neighbor index and overlap fraction
    for ( dim = 0; dim < ImageDimension; dim++ )
      {
      if ( upper & 1 )
        {
        neighIndex[dim] = baseIndexes[0][dim] + 1;
        // Take care of the case where the pixel is just
        // in the outer upper boundary of the image grid.
        if ( neighIndex[dim] > this->m_EndIndex[0][dim] )
          {
          neighIndex[dim] = this->m_EndIndex[0][dim];
          }
        overlap *= distances[0][dim];
        }
      else
        {
        neighIndex[dim] = baseIndexes[0][dim];
        // Take care of the case where the pixel is just
        // in the outer lower boundary of the image grid.
        if ( neighIndex[dim] < this->m_StartIndex[0][dim] )
          {
          neighIndex[dim] = this->m_StartIndex[0][dim];
          }
        overlap *= 1.0 - distances[0][dim];
        }

      upper >>= 1;
      }

    // Update output value only if overlap is not zero.
    // Overlap can be 0 when one or more index dims is an integer.
    // There will always be at least one iteration of 'counter' loop
    // that has overlap > 0, even if index is out of bounds.
    if ( overlap )
      {
      if( firstOverlap )
        {
        // Performing the first assignment of value like this allows
        // VariableLengthVector type to be resized properly.
        value = static_cast< RealType >( this->GetInputImage(0)->GetPixel(neighIndex) ) * overlap;
        firstOverlap = false;
        }
      else
        {
        value += static_cast< RealType >( this->GetInputImage(0)->GetPixel(neighIndex) ) * overlap;
        }
      totalOverlap += overlap;
      }

    if ( totalOverlap == 1.0 )
      {
      // finished
      break;
      }
    }

  return ( static_cast< OutputType >( value ) );



}
} // end namespace itk

#endif
