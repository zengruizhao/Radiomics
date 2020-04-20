#ifndef __itkMFARegularizedKnownValues_h
#define __itkMFARegularizedKnownValues_h



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdFeatureTypes.h"
#include "ccipdMFAForwardDeclarations.h"

// itk includes
#include "itkParameterRegularizedMetric.h"

////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



/**
 * \brief Regularize an MFA metric with known feature values.
 * 
 * \todo
 *   Find a more efficient way
 *   to represent this class, since this
 *   uses much of the existing functionality
 *   of the MFAReconstructionBase!!!
 *
 * This regularization assumes that you know
 * certain feature values, and want to
 * heavily regularized based on them.
 * 
 * The traditional MFA metric also assumes
 * you know certain "driving" feature values, and
 * attempts to reconstruct those
 * driving feature values. The reconstruction
 * is then compared to the known feature
 * values in terms of NCC, SSD, etc.
 * 
 * This performs almost the same task, but
 * allows a few known feature values to be
 * extremely heavily weighted.
 * 
 * In addition, one other example
 * is that this uses the existing MFA's
 * linked projection (z-score) vector
 * from the previous reconstruction
 * to reconstruct the few feature values.
 * 
 * An example of this, is you are using
 * thousands of known texture values
 * to drive the registration, and get a
 * single NCC value representing the
 * reconstruction of those.
 * However, you also have a couple
 * known levelset values.
 * You want to add those into the metric
 * so that it goes towards those few points.
 * 
 * \author  Rob Toth
 * \date    2013-03-31
 * \ingroup MFA
 */
template<
  typename     TPixelType,
  unsigned int TDimension > // template types
class MFARegularizedKnownValues:
  public itk::ParameterRegularizedMetric< TPixelType, TDimension >
{

  

public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  
  typedef ::itk::MFARegularizedKnownValues<
    TPixelType,
    TDimension
  > Self;

  typedef ::itk::ParameterRegularizedMetric<
    TPixelType,
    TDimension
  > Superclass;

  typedef SmartPointer<       Self > Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  typedef typename Superclass::TransformParametersType TransformParametersType;
  typedef typename Superclass::MeasureType             MeasureType;
  typedef typename Superclass::InputPointType          PointType;

  typedef MFAReconstructionMetric< TPixelType, TDimension > MFAMetricType;
  typedef SmartPointer< const MFAMetricType >               MFAMetricConstPointer;

////////////////////////////////////////////////////////////////////////////////////////////////////


  
public:
////////////////////////////////////////////////////////////////////////////////////////////////////

  /** We are abstract so no "New" function. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( MFARegularizedKnownValues, Superclass );

////////////////////////////////////////////////////////////////////////////////////////////////////


  
public: // set/get
////////////////////////////////////////////////////////////////////////////////////////////////////


  /**
   * You must set the metric if
   * you want the regularization to work.
   * 
   * \note
   *   You should also explicitly
   *   set the underlying metric separately!
   */
  ccipdSetByrefDeclareMacro( MFAMetric, MFAMetricConstPointer )

  /**
   * If you know the value at a certain
   * point, include it. You must provide
   * the point in space for the known value,
   * the value that you know to be correct,
   * and which feature that value corresponds to.
   * 
   * If the feature is not in the MFA, then
   * it will be ignored.
   */
  virtual void AddKnownValue(
    const PointType            where,
    const TPixelType           value,
    const ::ccipd::FeatureID & whichFeature );

  /// Clear all known values that are stored.
  virtual void ClearKnownValues();

////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////

           MFARegularizedKnownValues();
  virtual ~MFARegularizedKnownValues();

  /**
   * We provide a regularization of
   * the given transform parameters.
   * 
   * See the class description
   * for how this is done.
   * 
   * This will output the regularized
   * value for the known feature
   * values at the input points.
   * 
   * \returns
   *   The mean absolute difference
   *   between the known values
   *   and the reconstructed values.
   *   Or, NaN if any errors occur
   *   such as invalid features,
   *   outside the image, etc.
   */
  virtual MeasureType GetRegularization( const TransformParametersType & parameters) const;

////////////////////////////////////////////////////////////////////////////////////////////////////
  


////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacroITK( MFARegularizedKnownValues )
////////////////////////////////////////////////////////////////////////////////////////////////////



}; // class MFARegularizedKnownValues:
} // namespace itk
#endif // __itkMFARegularizedKnownValues_h
