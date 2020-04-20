#ifndef __itkRayleighHistogram_h
#define __itkRayleighHistogram_h



////////////////////////////////////////////////////////////////////////////////////////////////////
namespace itk
{
/**\class RayleighHistogram
 * \brief Calculated Rayleigh deviation over a moving window, adding and removing only 
 * select pixels.
 *
 * Computes the Rayleigh standard deviation for a window centered at a given pixel
 * utilizing itkMovingWindowUImageFilter to manage which pixels are added and removed 
 * from the histogram.
 *
 * Rayleigh histogram is one of a family of statistical histograms.
 *
 * \author  Rachel Sparks
 * \ingroup Filtering
 */
template< class TInputPixel >
class RayleighHistogram
{



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
	 /** Standard class typedefs. */
	typedef RayleighHistogram              Self;
	typedef SmartPointer< Self>            Pointer;
	typedef SmartPointer< const Self>      ConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
	/** Constructor */
	RayleighHistogram();

	/** Destructor */
	virtual ~RayleighHistogram(){};
		
	/** What to do when adding pixels near the boundary */
	void AddBoundary(){}; //Do nothing for pixel outside the region

	/** What to do when removing pixels near the boundary */
	void RemoveBoundary(){}; //Do nothing for pixel outside the region

	
	/** What to do when adding pixel*/
	void AddPixel(const TInputPixel &p);

	/** What to do when removing pixel */
	void RemovePixel(const TInputPixel &p);

	/** How to calculate the final value */
	TInputPixel GetValue(const TInputPixel &); 
////////////////////////////////////////////////////////////////////////////////////////////////////



private:	
////////////////////////////////////////////////////////////////////////////////////////////////////
	double m_squareSum;
	signed int m_count;
    signed int m_nanCount; // adding this to keep track of nan pixels
////////////////////////////////////////////////////////////////////////////////////////////////////



};

} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRayleighHistogram.hxx"
#endif

#endif
