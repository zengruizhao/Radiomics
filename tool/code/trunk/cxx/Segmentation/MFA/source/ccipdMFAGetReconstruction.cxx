


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd
#include "ccipdMFAGetReconstruction.h"
#include "ccipdMFATransformedReconstruction.h"
#include "ccipdApplyTransformation.h"
#include "ccipdStudy.h"

// itk includes
#include "itkMFAReconstructionFilter.h"

#ifdef _MSC_VER
#pragma warning( disable: 4244 )
#pragma warning( disable: 4127 )
#endif

#include <itkImage.h>
#include <itkTransform.h>

#ifdef _MSC_VER
#pragma warning( default: 4244 )
#pragma warning( default: 4127 )
#endif

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer GetReconstruction(
  const MFAVolumeReconstructionFilter & filter,
  const FeatureIDConstPointer         & whichFeature,
  const VolumeImageBaseConstPointer   & outputTemplateImage,
  const VolumeTransformConstPointer   & finalTransform )
{

  const bool verbose = filter.GetVerbose();

  const VolumeImagePointer badOutput;
  
  using std::cerr;
  using std::endl;
  using std::cout;
  
  if ( !whichFeature )
    {
    if ( verbose ) cerr << "Error: no feature." << endl;
    return badOutput;
    }
    
  // remember, we used the new image as the moving image
  if ( verbose ) cout << "Calculating transformed reconstruction:" << endl;
  const VolumeImageConstPointer transformedReconstruction(
    GetTransformedReconstruction( filter, whichFeature ) );
  if ( verbose ) cout << "Calculating transformed reconstruction done." << endl;

  // the GetTransformedReconstruction function will
  // check if we've segmented this study and return
  // NULL if we haven't
  if ( !transformedReconstruction )
    {
    return badOutput;
    } // transformedReconstruction 

  // the transform is actually going to be the inverse
  // of the final transform

  const VolumeTransformConstPointer
    inverseTransform( finalTransform->GetInverseTransform() );
    
  if ( !inverseTransform )
    {
    if ( verbose )
      cerr << "Error: No inverse transform." << endl;
    return badOutput;
    } // inverse
    
  // we will extrapolate
  const bool extrapolate = true;

  // okay, apply the inverse transformation
  if ( verbose ) cout << "Untransforming reconstruction:" << endl;
  const VolumeImagePointer
    reconstruction = ApplyTransform(
      inverseTransform,
      transformedReconstruction,
      outputTemplateImage,
      extrapolate );
  if ( verbose ) cout << "Untransforming reconstruction." << endl;

  return reconstruction;

} // GetReconstruction
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
