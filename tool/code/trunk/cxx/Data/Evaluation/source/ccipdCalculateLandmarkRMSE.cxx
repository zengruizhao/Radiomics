


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCalculateLandmarkRMSE.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateLandmarkRMSE( 
  const LandmarkSetType * const fixed,
	const LandmarkSetType * const moved,
  const bool verbose )
{
  //set up point set iterators
  typedef LandmarkSetContainer::ConstIterator           LandmarkIterator;
	typedef itk::Vector<LandmarkType, LandmarkDimension>  IteratorPointType;
	LandmarkIterator    fixedIterator=fixed->GetPoints()->Begin();
	LandmarkIterator    movedIterator=moved->GetPoints()->Begin();

	LandmarkIterator fixedEnd = fixed->GetPoints()->End();
	
	LandmarkType ssd=0;
	
  using std::cout;
  using std::endl;

  if (moved->GetNumberOfPoints()!=fixed->GetNumberOfPoints())
		{
		throw itk::ExceptionObject(__FILE__,__LINE__, 
		"Point Sets must contain the same numbers of landmark points.");
		}
	
	while(fixedIterator!=fixedEnd)
		{

    const auto
      fixedPoint = fixedIterator.Value(),
      movedPoint = movedIterator.Value();
    const IteratorPointType
      difference = fixedPoint - movedPoint;
	  ssd +=
      ( difference[ 0 ] * difference[ 0 ] ) + 
      ( difference[ 1 ] * difference[ 1 ] ) + 
      ( difference[ 2 ] * difference[ 2 ] );
		
    if ( verbose )
      {
      cout <<
        "Fixed point = " << fixedPoint << endl <<
        "Moved point = " << movedPoint << endl <<
        "Difference  = " << difference << endl <<
        "SSD         = " << ssd        << endl;
      } // verbose

    ++fixedIterator;
		++movedIterator;

		}
	
  const EvaluationValueType
    rmse = static_cast< EvaluationValueType >(
      std::sqrt(ssd/moved->GetNumberOfPoints() ) ); 

  if ( verbose )
    cout << "RMSE = " << rmse << endl;

  return rmse;

}
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace ccipd
