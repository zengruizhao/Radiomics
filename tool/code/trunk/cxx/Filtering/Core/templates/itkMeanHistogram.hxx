#ifndef __itkMeanHistogram_hxx
#define __itkMeanHistogram_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkMeanHistogram.h>
  #include <itkNumericTraits.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
template< class TInputPixel >
MeanHistogram< TInputPixel >
  ::MeanHistogram():
    m_sum(0), 
    m_count(0), 
    m_nanCount(0)
{ 
};

template< class TInputPixel >
void
MeanHistogram< TInputPixel >
	::AddPixel(const TInputPixel &p)
{   
  if (p!=p)
    m_nanCount++; 
  else
	m_sum+= p;
  m_count++;
}

template< class TInputPixel >
void
MeanHistogram< TInputPixel >
	::RemovePixel(const TInputPixel &p)
{
  if (p!=p)
    m_nanCount--; 
  else
	m_sum-= p;
  m_count--;
}

template< class TInputPixel >
TInputPixel
MeanHistogram< TInputPixel >
	::GetValue(const TInputPixel &)
{ 

if( m_nanCount > 0 )
  return std::numeric_limits< TInputPixel >::quiet_NaN();
else
  {
  float m_value = ( m_sum/ (m_count) );
  return static_cast< TInputPixel > (m_value);
  }

}

} //end namespace itk

#endif