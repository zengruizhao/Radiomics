#ifndef __ccipdSegmenter_h
#define __ccipdSegmenter_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdSegmentationDLL.h"
#include "ccipdSegmentationTypes.h"
#include "ccipdPatientTypes.h"
#include "ccipdVolumeTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
  * This is the abstract base class for all segmentation
  * objects.  It provide a set of functions which the child
  * segmenter class must override, for segmenting different
  * image types.
  * 
  * \ingroup Segmentation
  * \author  Rob Toth
  */
class Segmentation_EXPORT Segmenter // class declaration
{



////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( Segmenter )

public:
  typedef std::shared_ptr<       Self > Pointer;
  typedef std::shared_ptr< const Self > ConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////

  /** Must provide the ability to segment a study. */
  virtual bool Segment(
    const StudyConstPointer             & inputStudy,
    const SegmentationModelConstPointer & trainedModel ) = 0;

  /** Must provide the ability to return a segmentation. */
  virtual VolumeMaskImagePointer GetSegmentation(
    const MaskObjectType    & whichObject,
    const StudyConstPointer & whichStudy ) const = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace
#endif // __ccipdSegmenter_h
