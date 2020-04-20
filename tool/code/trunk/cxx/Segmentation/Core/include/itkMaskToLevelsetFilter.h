#ifndef __itkMaskToLevelsetFilter_h
#define __itkMaskToLevelsetFilter_h



/*
 * This filter will input a mask,
 * and convert it to a levelset, where
 * negative values represent inside
 * the object of interest, and
 * positive values represent outside
 * the object of interest.
 */



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  #include <itkImageToImageFilter.h> // base class
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



/**
 * \brief Convert a mask to a levelset.
 * 
 * \date    2012-02-16
 * \author  Rob Toth
 * \ingroup Segmentation
 */
template< class TMaskImage, class TLevelsetImage >       // template types
class ITK_EXPORT MaskToLevelsetFilter :                  // class declaration
public ImageToImageFilter < TMaskImage, TLevelsetImage > // the type of filter we are
{



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Input and output image related type definitions. */
  typedef TLevelsetImage LevelsetImageType;
  typedef TMaskImage     MaskImageType;
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Standard class typedefs. */
  typedef MaskToLevelsetFilter         Self;
  typedef ImageToImageFilter<
    MaskImageType, LevelsetImageType > Superclass;
  typedef SmartPointer< Self >         Pointer;
  typedef SmartPointer< const Self >   ConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(MaskToLevelsetFilter, ImageToImageFilter);

  /** The dimension of the output image. */
  itkStaticConstMacro(
    ImageDimension,
    unsigned int,
    LevelsetImageType::ImageDimension );
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Set/Get methods */

  /**
   * Should we use a sigmoid
   * based levelset (as opposed to distance)?
   * 
   * default = false
   */
  itkSetMacro     ( Sigmoid, bool );
  itkGetConstMacro( Sigmoid, bool );
  itkBooleanMacro ( Sigmoid );

  /**
   * Should we normalize the
   * levelset to have unit variance?
   * 
   * default = true
   */
  itkSetMacro     ( Normalize, bool );
  itkGetConstMacro( Normalize, bool );
  itkBooleanMacro ( Normalize );

  /**
   * Should we smooth the levelset?
   * 
   * default = true
   */
  itkSetMacro     ( Smooth, bool );
  itkGetConstMacro( Smooth, bool );
  itkBooleanMacro ( Smooth );
  
  /**
   * "W" in the following equation:
   * http://tinyurl.com/8xdscw7
   * (L is levelset, D is signed distance)
   * 
   * default = 4
   */
  typedef float SigmoidWidthType;
  itkSetMacro     ( SigmoidWidth, SigmoidWidthType );
  itkGetConstMacro( SigmoidWidth, SigmoidWidthType );
  
  /**
   * How much to smooth? (Variance in mm^2)
   * 
   * default = 4
   */
  typedef float SmoothingVarianceType;
  itkSetMacro     ( SmoothingVariance, SmoothingVarianceType );
  itkGetConstMacro( SmoothingVariance, SmoothingVarianceType );
////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////

  MaskToLevelsetFilter(); // constructor
  void GenerateData   (); // where the actual processing goes

  /** Each of the subfunctions will return this type. */
  typedef itk::SmartPointer<
    itk::ImageSource<
      TLevelsetImage
  > > PipelineOutput;


  /**
   * This function will normalize a levelset and return
   * the last filter in the normalization pipeline.
   */
  static PipelineOutput NormalizeLevelset( const LevelsetImageType * levelset );

  /**
   * This function will smooth a levelset.
   */
  static PipelineOutput SmoothLevelset(
    const LevelsetImageType   * levelset,
    const SmoothingVarianceType variance );
  
  /**
   * This function will calculate the signed distances from a map.
   */
  static PipelineOutput CalculateDistances( const MaskImageType * mask );

  /**
   * This function will transform a levelset to a sigmoid shape.
   * http://tinyurl.com/8xdscw7
   */
  static PipelineOutput CalculateSigmoid(
    const LevelsetImageType * levelset,
    const SigmoidWidthType    width );

////////////////////////////////////////////////////////////////////////////////////////////////////



private:
////////////////////////////////////////////////////////////////////////////////////////////////////
  MaskToLevelsetFilter( const Self & ); //purposely not implemented
  void operator=      ( const Self & ); //purposely not implemented
////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////
  // Variables
  bool
    m_Sigmoid,
    m_Normalize,
    m_Smooth;
  SmoothingVarianceType
    m_SmoothingVariance;
  SigmoidWidthType
    m_SigmoidWidth;
////////////////////////////////////////////////////////////////////////////////////////////////////



}; // class 
} // namespace itk



////////////////////////////////////////////////////////////////////////////////////////////////////
// this is where the implementation is actually included in the header
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMaskToLevelsetFilter.hxx"
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////



#endif // __itkMaskToLevelsetFilter_h
