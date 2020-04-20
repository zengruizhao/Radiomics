#ifndef __itkMFAReconstructionFilter_h
#define __itkMFAReconstructionFilter_h



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAReconstructionBase.h" // for computing projections, storing images, etc.   


#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageToImageFilter.h>    // base class
  #include <itkMacro.h>                 // for common macros
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <vector>
///////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Use the MFA to reconstruct a texture feature.
 * 
 * A trained MFA will be used to estimate
 * the linked parameters. These linked parameters
 * will then be used to reconstruct a feature
 * image, even if that feature image wasn't
 * input.
 * 
 * \todo Cache reconstructed result.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 */
template< class TPixelType, unsigned int Dimension >
class MFAReconstructionFilter :
  public ImageToImageFilter<
    Image< TPixelType, Dimension >,
    Image< TPixelType, Dimension > >,
  public ccipd::MFAReconstructionBase< TPixelType, Dimension >
////////////////////////////////////////////////////////////////////////////////////////////////////
{



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Standard class typedefs. */

  typedef TPixelType PixelType;

  /** The dimension of the image. */
  itkStaticConstMacro( ImageDimension, unsigned int, Dimension );

  typedef Image< PixelType, Dimension > ImageType;
  typedef ImageType                     InputImageType;
  typedef ImageType                     OutputImageType;

  typedef MFAReconstructionFilter Self;

  typedef ImageToImageFilter<
    InputImageType,
    OutputImageType
  > Superclass;

  typedef ccipd::MFAReconstructionBase<
    PixelType,
    ImageDimension
  > MFASuperclass; ///< Superclass #2

  typedef SmartPointer< Self >       Pointer;      ///< Point to ourself.
  typedef SmartPointer< const Self > ConstPointer; ///< Smart pointer to ourself.
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(MFAReconstructionFilter, ImageToImageFilter);

////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Output typedef's */
  typedef typename OutputImageType::Pointer OutputImagePointer;

  /// A collection of points (in real world coordinates) to sample.
  typedef typename MFASuperclass::SampleLocationsContainer SampleLocationsContainer;

////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * \returns Whether or not we can reconstruct a given feature.
   * 
   * \date 2012-05-15
   */
  virtual bool CanReconstructFeature(
    const ccipd::FeatureIDConstPointer & whichFeature ) const;

  /**
   * \brief Calculated a reconstructed image.
   * 
   * After calling Update(),
   * the calculation is not finish
   * because you may not want to
   * reconstruct all the features.
   * 
   * Therefore, this function should be
   * called after Update() ( or GenerateData() )
   * to actually finish the calculation for a
   * given feature.
   * 
   * \todo Cache reconstructed images.
   * 
   * \date 2012-05-14
   * 
   */
  virtual OutputImagePointer GetReconstructedImage(
    const ccipd::FeatureIDConstPointer & whichFeature ) const;

  /**
   * If this is set to true, and you actually give it another
   * reconstruction base, then it will use the sample
   * locations from that other base.  Otherwise,
   * it will use the entire image as samples.
   */
  ccipdSetByvalDeclareMacro( UseExistingLocations, bool )
  ccipdGetConstDeclareMacro( UseExistingLocations, bool )
  itkBooleanMacro ( UseExistingLocations );

  /**
   * You must set this if you want to 
   * manually set the locations to sample.
   * You also need to set m_UseExistingLocations to true.
   */
  ccipdSetByrefDeclareMacro( ExistingLocations, SampleLocationsContainer )
////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /* Protected functions. */

           MFAReconstructionFilter(); ///< Constructor
  virtual ~MFAReconstructionFilter(); ///< Destructor

  /**
   * \brief This is the main function called on Update().
   * 
   * This will perform the calculations necessary
   * to reconstruct any feature. However, it will
   * not finish the calculation (and will not store
   * anything in GetOutput) because you may not
   * wish to reconstruct all features.
   * 
   * Therefore, after you call Update() on this filter,
   * you should call GetReconstructedImage() instead
   * of GetOutput() to finish the calculation.
   * 
   */
  void GenerateData();

  /** We must override this to extract a list of points to be sampled.
   * in this filter, all the pixels in the image will be sampled to
   * extract our projections, because ideally this filter should only be
   * run once we have completed the segmentation, as a last step, and hence
   * we want it to be as accurate as possible.*/
  virtual SampleLocationsContainer CalculateSampleLocations();  

  /**
   * This function will get the locations
   * of every single pixel (disregarding
   * whether or not you have set UseExistingLocations
   * to true).
   */
  virtual SampleLocationsContainer CalculateAllSampleLocations();

  /** This will resample every pixel from every feature. */
  virtual void ResampleAllFeatures();
  
////////////////////////////////////////////////////////////////////////////////////////////////////



private:
////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
////////////////////////////////////////////////////////////////////////////////////////////////////


}; // class 
} // namespace itk
#endif // __itkMFAReconstructionFilter_h
