#ifndef __itkMeanHistogram_h
#define __itkMeanHistogram_h



namespace itk
{
/**\class MeanHistogram
 * \brief Calculated mean over a moving window, adding and removing only 
 * select pixels.
 *
 * Computes the mean for a window centered at a given pixel
 * utilizing itkMovingWindowImageFilter to manage which pixels are added and removed 
 * from the histogram.
 *
 * Mean histogram is one of a family of statistical histograms.
 *
 * \author  Rachel Sparks
 * \ingroup Filtering
 */
template< class TInputPixel >
class MeanHistogram
{
public:
	 /** Standard class typedefs. */
	typedef MeanHistogram                  Self;
	typedef SmartPointer< Self>            Pointer;
	typedef SmartPointer< const Self>      ConstPointer;

	MeanHistogram();

	virtual ~MeanHistogram(){};
		
	void AddBoundary(){}; //Do nothing for pixel outside the region

	void RemoveBoundary(){}; //Do nothing for pixel outside the region

	void AddPixel(const TInputPixel &p);

	void RemovePixel(const TInputPixel &p);

	TInputPixel GetValue(const TInputPixel &); 

private:	
	float m_sum;
	signed int m_count;
    signed int m_nanCount; // adding this to keep track of nan pixels
};
} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMeanHistogram.hxx"
#endif

#endif
