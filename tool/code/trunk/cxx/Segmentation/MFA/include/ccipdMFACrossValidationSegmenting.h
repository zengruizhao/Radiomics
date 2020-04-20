#ifndef __ccipdMFACrossValidationSegmenting_h
#define __ccipdMFACrossValidationSegmenting_h



//////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// std includes
#include <string>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Run the segmentation on the next fold.
 * 
 * This class assumes you have already prepared
 * the directory structure for the cross validation.
 * 
 * It will find the next available cross validation
 * fold directory which hasn't been segmented yet,
 * and isn't currently in the process of
 * being segmenting (using a "lock" file),
 * and has been trained (using a "trained" file).
 * 
 * An MFASEgmenter.xml file should exist in
 * the root cross validation directory, which
 * will be used as a template for the current fold.
 * 
 * The output will be a saved MFAModel.xml
 * in the current fold's directory, as well
 * as the trained MFA image files in that
 * directory.
 * 
 * A TestingPatients.xml file must exist
 * 
 * A "locked" file will be created in the
 * directory to indicate that no other
 * process should work on this directory.
 * 
 * \returns
 *   false if any errors are encountered.
 *   true if it successfully segmented one study.
 * 
 * \author  Rob Toth
 * \date    2012-06-01
 * \ingroup MFACrossValidation
 */
bool RunMFACrossValidationSegmenting(
  const std::string & crossValidationRootDirectory,
  const bool          verbose = true );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFACrossValidationSegmenting_h
