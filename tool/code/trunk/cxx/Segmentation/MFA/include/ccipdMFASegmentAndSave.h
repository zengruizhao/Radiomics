#ifndef __ccipdMFASegmentAndSave_h
#define __ccipdMFASegmentAndSave_h


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdPatientTypes.h" // for our study
#include "ccipdMFATypes.h"     // to do the actual segmenting

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
 * \brief Segment a study and save all the results.
 * 
 * This is a helper function to take a study,
 * and segment it. The MFA segmentation parameters will
 * be loaded from a file, and the entire
 * set of reconstructions and 
 * segmentation results will be saved in a specific
 * output directory.
 * 
 * \param[in] study
 *   Which study to segment.
 * 
 * \param[in] outputDirectory
 *   Where to save the results.
 * 
 * \param[in] outputPrefix
 *   This optional parameter will
 *   allow you to specific a prefix
 *   to prepend to all output files.
 *   This may be useful if you
 *   want the study description, or
 *   patient name, to be prepended to
 *   the output.
 * 
 * \param[in] verbose
 *   Whether or not to show progress
 *   and errors in this function.
 *   Note that this is different
 *   from the "verbose" parameter
 *   in the MFA itself, which will
 *   dictate whether or not progress
 *   will be shown during the actual
 *   segmentation.
 * 
 * \returns false if any errors occur
 * 
 * \note
 *   If the segmenter has a SegmentationObject
 *   set, then nothing else will be output
 *   (i.e. saved). Only the segmentation
 *   of that specific object.
 *   This can be more efficient, but give
 *   less understanding of why a segmentation
 *   did or did not perform well.
 * 
 * \date    2012-06-01
 * \author  Rob Toth
 * \ingroup MFA
 */
bool MFASegmentAndSave(
  const StudyPointer & study,
        MFASegmenter & segmenter,
  const std::string  & outputDirectory,
  const std::string  & outputPrefix = "MFA_",
  const bool           verbose = true );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFASegmentAndSave_h
