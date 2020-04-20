#ifndef __itkReconstructionMetric_h
#define __itkReconstructionMetric_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFAReconstructionBase.h"


#include "ccipdDisableWarningsMacro.h"
  #include <itkImageToImageMetric.h>
#include "ccipdEnableWarningsMacro.h"

//std includes
#include <vnl/vnl_vector.h>
#include <vector>
#include <unordered_map> // for caching things
#include <memory>
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



/**
 * \brief Registration metric using the MFA.
 * 
 * In this class, the actual FixedImage input means nothing, but rather
 * the reconstruction (mean and basis images) is what is used as the fixed image,
 * so be sure to input that.
 * 
 * However, the fixed image mask, etc. WILL be used.
 * 
 * The image you wish to reconstruct (given different transformations)
 * should be set as the MovingImage.
 * TODO: multiple texture features.
 * 
 * Also, the SetSpatialSamples shouldn't be too low, or the PCA projections
 * which are calculated will be off.
 * 
 * The basis and mean images need to be set
 * 
 * Our basis images are actually our "fixed" images.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 */
template< class TPixelType, unsigned int Dimension > // template types
class MFAReconstructionMetric:
  public ImageToImageMetric<
    Image< TPixelType, Dimension >,
    Image< TPixelType, Dimension > >,
  public ccipd::MFAReconstructionBase< TPixelType, Dimension >
////////////////////////////////////////////////////////////////////////////////////////////////////
{



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Standard class typedefs. */
  typedef MFAReconstructionMetric Self;
  typedef TPixelType              PixelType;

  typedef Image<
    PixelType,
    Dimension
  > ImageType;

  typedef ImageType FixedImageType;
  typedef ImageType MovingImageType;

  typedef ImageToImageMetric<
    FixedImageType,
    MovingImageType
  > Superclass; ///< Superclass #1

  typedef ccipd::MFAReconstructionBase<
    PixelType,
    Dimension
  > MFASuperclass; ///< Superclass #2

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( MFAReconstructionMetric, Superclass );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Types transferred from the base class */
  typedef typename Superclass::TransformParametersType     TransformParametersType;
  typedef typename Superclass::FixedImageSampleContainer   FixedImageSampleContainer;
  typedef typename Superclass::TransformType               TransformType;
  typedef typename Superclass::InterpolatorType            InterpolatorType;
  typedef typename Superclass::MeasureType                 MeasureType;
  typedef typename Superclass::DerivativeType              DerivativeType;
  typedef typename MFASuperclass::SampleLocationsContainer SampleLocationsContainer;
  typedef typename TransformType::InputPointType           FixedImagePointType;
  typedef typename TransformType::OutputPointType          MovingImagePointType;

////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
  /**  Get the value for single valued optimizers. We must provide this. */
  MeasureType GetValue( const TransformParametersType & parameters) const;

  /** Get the reconstruction metric for a given feature. */
  MeasureType GetValue( const ccipd::FeatureIDConstPointer & whichFeature ) const;

  /** \todo: Figure out the derivative analytically. */
  void GetDerivative(
    const TransformParametersType & parameters,
          DerivativeType          & derivative) const;

  /** Initialize the metric. */
  virtual void Initialize() throw( ExceptionObject );

  /**
   * Sets the option to use NCC.
   * If true, NCC is returned by GetValue().
   * If false, SSD is returned by GetValue();
   */
  ccipdSetByvalDeclareMacro( UseNCC, bool )
  ccipdGetConstDeclareMacro( UseNCC, bool );
  itkBooleanMacro ( UseNCC );

////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////
  MFAReconstructionMetric();
  virtual ~MFAReconstructionMetric() {}
  void PrintSelf( std::ostream & os, Indent indent ) const;
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // we must override this to extract a list of points to be sampled
  virtual SampleLocationsContainer CalculateSampleLocations(); // maybe make this const?

////////////////////////////////////////////////////////////////////////////////////////////////////



private:
////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacroITK( MFAReconstructionMetric )
////////////////////////////////////////////////////////////////////////////////////////////////////



};
} // end namespace itk
#endif // __itkReconstructionMetric_h
