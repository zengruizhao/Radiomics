#ifndef __ccipdDuplicateLandmarks_hxx
#define __ccipdDuplicateLandmarks_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDuplicateLandmarks.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class LandmarkType, unsigned int Dimension, class MeshTraits >
typename itk::SmartPointer< 
  itk::PointSet< LandmarkType, Dimension, MeshTraits > 
> DuplicateLandmarkSet( 
  const itk::PointSet< LandmarkType, Dimension, MeshTraits> * points )
{

typedef itk::PointSet< LandmarkType, Dimension, MeshTraits > 
PointSetType;

//Prepare PointSet Copy
typename PointSetType::Pointer copyPoints= PointSetType::New();
copyPoints->CopyInformation(points);  
  
//Duplicate The Point Set Container
typename PointSetType::PointsContainer::Pointer 
  copyPointsContainer = DuplicateLandmarkContainer< LandmarkType, Dimension, MeshTraits > ( points->GetPoints() );
copyPoints->SetPoints(copyPointsContainer);

//Return Points
return copyPoints;
} //DuplicateLandmarkSet
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
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
	> * container )
{

typedef typename itk::PointSet< LandmarkType, Dimension, MeshTraits 
>::PointsContainer ContainerType;

//set up my new continer
typename ContainerType::Pointer copyContainer=ContainerType::New();
copyContainer->resize( container->Size() );

//set up my iterators
typename ContainerType::ConstIterator containerIter= container->Begin();
typename ContainerType::ConstIterator containerEnd  = container->End();
unsigned int i=0;

//iterate through and copy each point
for(; containerIter != containerEnd; ++containerIter, ++i)
  {

  copyContainer->SetElement(i,static_cast <itk::Point< LandmarkType, Dimension > >( containerIter.Value() ));

  }

return copyContainer;
} // DuplicateLandmarkContainer
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdDuplicateImage_hxx
