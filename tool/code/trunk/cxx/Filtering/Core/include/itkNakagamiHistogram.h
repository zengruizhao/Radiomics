#ifndef __itkNakagamiHistogram_h
#define __itkNakagamiHistogram_h



namespace itk
{
/**\class NakagamiHistogram
 * \brief Calculated Nakagami m parameter over a moving window, adding and removing only 
 * select pixels.
 *
 * Computes the Nakagami m parametere for a window centered at a given pixel
 * utilizing itkMovingWindowUImageFilter to manage which pixels are added and removed 
 * from the histogram.
 *
 * NakagamiHistogram is one of a family of statistical histograms.
 *
 * \author  Rachel Sparks
 * \ingroup Filtering
 */
template< class TInputPixel >
class NakagamiHistogram
{



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
	 /** Standard class typedefs. */
	typedef NakagamiHistogram              Self;
	typedef SmartPointer< Self>            Pointer;
	typedef SmartPointer< const Self>      ConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Constructor */
  NakagamiHistogram();

  /** Destructor */
  virtual ~NakagamiHistogram(){};
		
  /** What to do when adding boundary pixels */
  void AddBoundary(){}; //Do nothing for pixel outside the region

  /** What to do when removing boundary pixels */
  void RemoveBoundary(){}; //Do nothing for pixel outside the region

  /** What to do when adding pixels */
  void AddPixel(const TInputPixel &p);

  /** What to do when removing pixels */
  void RemovePixel(const TInputPixel &p);

  /** Calculate the value for a given window. */
  TInputPixel GetValue(const TInputPixel &); 
////////////////////////////////////////////////////////////////////////////////////////////////////



private:	
////////////////////////////////////////////////////////////////////////////////////////////////////
	long double m_squareSumPower;
	long double m_squareMultPower;
	signed int m_count;
    signed int m_nanCount; // adding this to keep track of nan pixels
////////////////////////////////////////////////////////////////////////////////////////////////////



};

} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNakagamiHistogram.hxx"
#endif

#endif