#ifndef __itkNakagamiHistogram_hxx
#define __itkNakagamiHistogram_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkNakagamiHistogram.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkNumericTraits.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
template< class TInputPixel >
NakagamiHistogram< TInputPixel >
  ::NakagamiHistogram(): 
    m_squareSumPower(0),
    m_squareMultPower(1),
    m_count(0),
    m_nanCount(0)
{
} // Constructor

template< class TInputPixel >
void
NakagamiHistogram< TInputPixel >
::AddPixel(const TInputPixel &p)
{ 

  if (p!=p)
    m_nanCount++; 
  else
    {
    m_squareSumPower += p*p;
    m_squareMultPower*= p*p;
    }
  m_count++;
} // Add Pixel

template< class TInputPixel >
void
NakagamiHistogram< TInputPixel >
  ::RemovePixel(const TInputPixel &p)
{  
  if (p!=p)
    m_nanCount--; 
  else
   {
    m_squareSumPower-= p*p;
    m_squareMultPower/=p*p;
    }
  m_count--;
} // Remove Pixel

template< class TInputPixel >
TInputPixel
NakagamiHistogram< TInputPixel >
::GetValue(const TInputPixel &)
{ 
  if (m_nanCount > 0 )
    return std::numeric_limits< TInputPixel >::quiet_NaN();

  long double y = std::log( ( m_squareSumPower/m_count ) / ( std::pow( m_squareMultPower, 1/m_count) ) );
  long double m;

	if(y <= 0.1 || y>17 ) // if negative or zeros set to zero this should not happen
	{
		m = 0;
	}
	else
	{
		if ( y <0.5772 )
		{
          m = (0.50008 + 0.16488*y - 0.05442*(y*y) )/(y);
		}
		else
		{
		 m =  (8.898919 + 9.05995*y +0.9775373*(y*y) )/(y* (17.79728 + 11.96847*y+ y*y) );
		}
	}

	return static_cast<TInputPixel>(m);

} // Get Value

} //end namespace itk
 
#endif
