


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCalculateMedialAxesRMSE.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateMedialAxesRMSE( 
  const MedialAxisSetType * const fixed,
	const MedialAxisSetType * const moved)
{
	//set up point set iterators
	typedef MedialAxisSetContainer::ConstIterator             MedialAxisIterator;
	typedef itk::Vector<MedialAxisType, MedialAxisDimension>  MedialAxisPointType;
	MedialAxisIterator    fixedIterator     = fixed->GetPoints()->Begin();
	MedialAxisIterator    movedIterator;
	MedialAxisIterator    fixedEnd			= fixed->GetPoints()->End();
	MedialAxisIterator	  movedEnd			= moved->GetPoints()->End();
	
	MedialAxisType 
		ssd = 0.0,
		dist,	
		minDist;

	bool foundMin;
	int count = 0;
	MedialAxisPointType point;

		while(fixedIterator!=fixedEnd)
		{
 
			minDist = std::numeric_limits< MedialAxisType >::max();
			foundMin = false;
			movedIterator=moved->GetPoints()->Begin();
			while(movedIterator!=movedEnd)
			{
				if (fixedIterator.Value()[3] < 0 && movedIterator.Value()[3] < 0)
				{
					point = fixedIterator.Value() - movedIterator.Value();
					dist	= 
							(point[0]*point[0])+
							(point[1]*point[1])+
							(point[2]*point[2]);

					if (dist  < minDist)
					{
						foundMin = true;
						minDist	= dist;
					}
				}
				++movedIterator;
			}

			if (foundMin)
			{
				ssd += minDist;
				count++;
			}
			++fixedIterator;
		}
	
	if (count>0)
		return static_cast<EvaluationValueType> (std::sqrt(ssd/moved->GetNumberOfPoints())); 
	else
	{
		std::cout << "Cannot compute distance" << std::endl;
		return static_cast<EvaluationValueType> ( -1 );  
	}

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateMedialAxesRMSE( 
  const LandmarkSetType * const fixed,
	const LandmarkSetType * const moved)
{
	 //set up point set iterators
  typedef LandmarkSetContainer::ConstIterator								MedialAxisIterator;
	MedialAxisIterator    fixedIterator = fixed->GetPoints()->Begin();
	MedialAxisIterator    movedIterator;
	MedialAxisIterator		fixedEnd			= fixed->GetPoints()->End();
	MedialAxisIterator		movedEnd			= moved->GetPoints()->End();
	
	MedialAxisType 
		ssd = 0.0,
	  dist,	
		minDist;

	bool foundMin;
	int count = 0;
	//LandmarkPointType point;

	while(fixedIterator!=fixedEnd)
	{
    minDist = std::numeric_limits< MedialAxisType >::max();
		foundMin = false;
		movedIterator=moved->GetPoints()->Begin();
		while(movedIterator!=movedEnd)
		{
			dist = 
				( fixedIterator.Value()[0] - movedIterator.Value()[0])*(fixedIterator.Value()[0] - movedIterator.Value()[0])+
				( fixedIterator.Value()[1] - movedIterator.Value()[1])*(fixedIterator.Value()[1] - movedIterator.Value()[1])+
				( fixedIterator.Value()[2] - movedIterator.Value()[2])*(fixedIterator.Value()[2] - movedIterator.Value()[2]);

			if (dist  < minDist)
			{
				foundMin = true;
				minDist	= dist;
			}
			++movedIterator;
		}

		if (foundMin)
		{
			ssd += minDist;
			count++;
		}
		++fixedIterator;
	}
	
	if (count>0)
		return static_cast<EvaluationValueType> (std::sqrt(ssd/moved->GetNumberOfPoints())); 
	else
	{
	  std::cout << "Cannot compute distance" << std::endl;
		return static_cast<EvaluationValueType> ( -1 );  
	}

}
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace ccipd
