#ifndef __ccipdDuplicateLandmarks_h
#define __ccipdDuplicateLandmarks_h





//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief   Simple wrapper to dupcliate our common landmark types.
 * \ingroup Common
 */
LandmarkSetPointer DuplicateLandmarkSet( const LandmarkSetType * landmarks );

/**
 * \brief   Simple wrapper to dupcliate our common landmark types.
 * \ingroup Common
 */
LandmarkSetContainerPointer DuplicateLandmarkContainer(
  const LandmarkSetContainer * landmarkContainer );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This actually performs the duplication of any landmark type.
 * Include ccipdDuplicateLandmarks.hxx 
 * if you want to use this for non-standard landmarks types.
 * 
 * \author  Rachel Sparks
 * \ingroup Common
 */
template< class LandmarkType, unsigned int Dimension, class MeshTraits >
typename itk::SmartPointer< 
  itk::PointSet< LandmarkType, Dimension, MeshTraits > 
> DuplicateLandmarkSet ( const itk::PointSet< LandmarkType, Dimension, MeshTraits> * points );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This actually performs the duplication of any landmark type.
 * Include ccipdDuplicateLandmarks.hxx
 * if you want to use this for non-standard landmarks types.
 * 
 * \author  Rachel Sparks
 * \ingroup Common
 */
template< class LandmarkType, unsigned int Dimension, class MeshTraits >
typename itk::SmartPointer< 
  itk::VectorContainer< 
    itk::SizeValueType, 
    itk::Point< LandmarkType, Dimension >
  > 
> DuplicateLandmarkContainer( 
    const typename itk::VectorContainer< 
	  itk::SizeValueType, 
      itk::Point< LandmarkType, Dimension > 
	> * container );
//////////////////////////////////////////////////////////////////////////////////////////




} // namespace
#endif // __ccipdDuplicateLandmarks_h
