#ifndef __itkVectorImageToVectorImageFilter_h
#define __itkVectorImageToVectorImageFilter_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include <itkImageToImageFilter.h>
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{

/** \class VectorImageToVectorImageFilter
 *  \brief Applies a filter to each component of a VectorImage
 *
 * Computes a VectorImage where each component is calculated
 * according to a user provided filter separately. The output 
 * is then a composite of these individual outputs.
 *
 * The user provided filter must be of the form 
 * ImageToImageFilter<ComponentImageType, ComponentImageType>.
 * 
 * \author  Rachel Sparks
 * \ingroup Filtering
 * \date 07-01-2012
 *
 */
template< class TInputImage, class TOutputImage, class TFilter >
class ITK_EXPORT VectorImageToVectorImageFilter:
  public ImageToImageFilter< TInputImage, TOutputImage >
{

public:
  /** Standard class typedefs. */
  typedef VectorImageToVectorImageFilter                        Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage >       Superclass;
  typedef SmartPointer< Self >                                  Pointer;
  typedef SmartPointer< const Self >                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  itkTypeMacro(VectorImageToVectorImageFilter, ImageToImageFilter);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage               InputImageType;
  typedef TOutputImage              OutputImageType;
  typedef TFilter                   FilterType;
  typedef typename TFilter::Pointer FilterPointer;

  /** I am defining the pixel type this way so that
    * this is generalizable to both itk::VectorImages<>
	* and Image<itk::Vector<>> types.
	*/
  typedef typename InputImageType::PixelType::ComponentType PixelComponentType;
  typedef Image<
	PixelComponentType, 
    InputImageType::ImageDimension
  > ComponentImageType;

  typedef typename ComponentImageType::ConstPointer  ComponentImageConstPointer;


  /** Methods to get/set the image filter */ 
  itkSetObjectMacro  ( ComponentFilter, FilterType );
  itkGetObjectMacro  ( ComponentFilter, FilterType );

/** Method to generate the filter output
   * for a single component. 
   */
virtual ComponentImageConstPointer GetComponent( unsigned int index) const;

protected: 
 /** Constructor*/
 VectorImageToVectorImageFilter();
 
 /** Destructor */
 virtual ~VectorImageToVectorImageFilter();

 /**Method to generate the output */
 virtual void GenerateData();

private:
  VectorImageToVectorImageFilter(const Self &){} //purposely not implemented
  void operator=(const Self &){}                 //purposely not implemented
  virtual void PrintSelf(std::ostream & os, Indent indent ) const;
  FilterPointer m_ComponentFilter;
};

} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorImageToVectorImageFilter.hxx"
#endif

#endif //__itkVectorImageToVectorImageFilter_h