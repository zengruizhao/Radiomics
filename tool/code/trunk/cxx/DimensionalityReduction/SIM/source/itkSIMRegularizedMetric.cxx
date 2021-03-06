


////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"

// itk includes
#include "itkSIMRegularizedMetric.hxx"
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/** This file just compiles the IntermediateRegularizedMetric for our existing data types. */
template class itk::SIMRegularizedMetric<
  ccipd::InternalPixelType,
  ccipd::VolumeDimension >;
////////////////////////////////////////////////////////////////////////////////////////////////////
