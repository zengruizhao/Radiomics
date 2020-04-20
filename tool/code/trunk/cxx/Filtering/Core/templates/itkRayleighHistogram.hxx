#ifndef __itkRayleighHistogram_hxx
#define __itkRayleighHistogram_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkRayleighHistogram.h"

// itk includes
#include <itkNumericTraits.h>
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
template< class TInputPixel >
RayleighHistogram< TInputPixel >
	::RayleighHistogram(): m_squareSum(0), m_count(0), m_nanCount(0)
{ 
}

template< class TInputPixel >
void
RayleighHistogram< TInputPixel >
	::AddPixel(const TInputPixel &p)
{   
  if (p!=p)
    m_nanCount++; 
  else
	m_squareSum += (p*p) / 2; //this wont change the overall formula but I trying to make sure I dont hit an inf

  m_count++;
}

template< class TInputPixel >
void
RayleighHistogram< TInputPixel >
	::RemovePixel(const TInputPixel &p)
{   
  if (p!=p)
    m_nanCount--; 
  else
	m_squareSum -= (p*p)/2;
  m_count--;
}

template< class TInputPixel >
TInputPixel
RayleighHistogram< TInputPixel >
	::GetValue(const TInputPixel &)
{ 

double m_value;

if( m_nanCount > 0 )
  m_value = std::numeric_limits< TInputPixel >::quiet_NaN();
else
  {
  if (m_squareSum > 0 )
    m_value = std::sqrt( m_squareSum/( static_cast< double > (m_count) ) );
  else 
    m_value = 0;
  }
return static_cast< TInputPixel > (m_value);
}

} //end namespace itk

#endif
