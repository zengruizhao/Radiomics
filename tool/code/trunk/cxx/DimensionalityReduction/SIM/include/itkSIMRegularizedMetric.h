#ifndef __itkSIMRegularizedMetric_h
#define __itkSIMRegularizedMetric_h



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMacros.h"
#include "ccipdSIMForwardDeclarations.h"

// our own itk includes
#include "itkParameterRegularizedMetric.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{

/** \brief Allows for an underlying metric to be regularized by a SIM model.
  * 
  * This class controls the SIM model and the determination of which points to sample.
  * A child class is responsible for calculating the regularizer value for a given 
  * set of parameters.
  *
  * \date 2013-02-25
  * \author Rachel Sparks
  * \ingroup Registration
  */
template<
  class        TPixelType,
  unsigned int TDimension > // template types
class SIMRegularizedMetric :
  public itk::ParameterRegularizedMetric<
    TPixelType, TDimension >
{



public:
////////////////////////////////////////////////////////////////////////////////////////////////////

  typedef ::itk::SIMRegularizedMetric<
    TPixelType,
    TDimension
  > Self;

  typedef ::itk::ParameterRegularizedMetric<
    TPixelType,
    TDimension
  > Superclass;
  
  typedef ccipd::SIMStorage< 
   TPixelType,
   TDimension
  > SIMStorageType;
  
  typedef SmartPointer<       Self > Pointer;
  typedef SmartPointer< const Self > ConstPointer;

//////////////////////////////////////////////////////////////////////////////////////////////////// 



public:
////////////////////////////////////////////////////////////////////////////////////////////////////

  /** We are abstract so no "New" function. */
  //itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( SIMRegularizedMetric, Superclass );

////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  typedef typename Superclass::TransformParametersType    TransformParametersType;
  typedef typename TransformParametersType::ValueType     ParametersValueType;
  typedef typename Superclass::MeasureType                MeasureType;
  typedef typename Superclass::DerivativeType             DerivativeType;
  
  typedef typename Superclass::FixedImageSampleContainer  FixedImageSampleContainer;
  typedef itk::ImageBase< TDimension >                    ImageBaseType;
  typedef typename ImageBaseType::PointType               PointType;
  /// Types for sample locations.
  typedef std::vector< PointType >                        SampleLocationsContainer;
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Initialize the metric. */
  virtual void Initialize() throw( ExceptionObject );

  ccipdSetByrefDeclareMacro( SIM, SIMStorageType )
  ccipdGetConstDeclareMacro( SIM, SIMStorageType )
////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////
  // constructor/desctructor
           SIMRegularizedMetric();
  virtual ~SIMRegularizedMetric();

  // Get the SIM features for the samples considered
  virtual void SampleSIM(); 

////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacroITK( SIMRegularizedMetric )
////////////////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace itk

#endif //__itkSIMRegularizedMetric_h