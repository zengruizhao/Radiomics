


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCalculateSimpleMedialAxesRMSE.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateSimpleMedialAxesRMSE( 
  const MedialAxisSetType * const fixed,
	const MedialAxisSetType * const moved)
{
  //set up point set iterators
  typedef MedialAxisSetContainer::ConstIterator             MedialAxisIterator;
	typedef itk::Vector<MedialAxisType, MedialAxisDimension>  MedialAxisPointType;
	MedialAxisIterator    fixedIterator=fixed->GetPoints()->Begin();
	MedialAxisIterator    movedIterator=moved->GetPoints()->Begin();

	MedialAxisIterator fixedEnd = fixed->GetPoints()->End();
	
	MedialAxisType ssd=0;
	bool added = false;	

		if (moved->GetNumberOfPoints()!=fixed->GetNumberOfPoints())
		{
		   throw itk::ExceptionObject(__FILE__,__LINE__, 
		     "Point Sets must contain the same numbers of landmark points.");
		}
	
		while(fixedIterator!=fixedEnd)
		{
      MedialAxisPointType point;
      if (fixedIterator.Value()[3] < 0 && movedIterator.Value()[3] < 0)
      {
				added = true;
        point=fixedIterator.Value() - movedIterator.Value();
        ssd+= (point[0]*point[0])+(point[1]*point[1])+(point[2]*point[2]);
      }

			++fixedIterator;
			++movedIterator;
		}

	if (added)	
		return static_cast<EvaluationValueType> (std::sqrt(ssd/moved->GetNumberOfPoints())); 
	else
		return static_cast<EvaluationValueType> ( std::numeric_limits< EvaluationValueType >::max() );

}
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace ccipd
