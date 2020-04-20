


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFATransformedReconstruction.h"
#include "ccipdFeatureID.h"

#ifndef NDEBUG
#include "ccipdDuplicateImage.h"
#endif

// itk includes
#include "itkMFAReconstructionFilter.h"
#include <itkImage.h>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer GetTransformedReconstruction(
  const MFAVolumeReconstructionFilter & filter,
  const FeatureIDConstPointer         & whichFeature )
{

  using std::cout;
  using std::cerr;
  using std::endl;
  
  // extract the data
  const bool verbose = filter.GetVerbose();
  
  const VolumeImagePointer badOutput;
  
  if ( !whichFeature )
    {
    if ( verbose )
      cerr << "Error: No feature." << endl;
    return badOutput;
    } // whichFeature
  
  // okay, now see if we have a reconstruction for that 
  if ( !filter.CanReconstructFeature( whichFeature ) )
    {
    if ( verbose )
      cerr << "Error: Unable to reconstruct feature " <<
        whichFeature->GetHash() << "!" << endl;
    return badOutput;
    }

  // alright, at this point, the filter has said
  // that it can reconstruct the feature
  if ( verbose ) cout << "Calculating reconstruction image:" << endl;
  const VolumeImagePointer reconstruction =
    filter.GetReconstructedImage( whichFeature );
  if ( verbose ) cout << "Calculating reconstruction image done." << endl;

  return reconstruction;

} // GetReconstruction
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
