#ifndef __ccipdSplitRegion_hxx
#define __ccipdSplitRegion_hxx



///////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdSplitRegion.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegionSplitterSlowDimension.h>
#include "ccipdEnableWarningsMacro.h"
///////////////////////////////////////////////////////////////////////////



namespace ccipd
{



///////////////////////////////////////////////////////////////////////////
template< unsigned int Dimension >
std::vector< itk::ImageRegion< Dimension > >
SplitRegion(
  const itk::ImageRegion< Dimension > & region,
  const unsigned int                    numberOfRequestedSubRegions )
{

  // typedefs
  typedef itk::ImageRegion<
    Dimension
  > RegionType;
  typedef itk::ImageRegionSplitterSlowDimension
    SplitterType;
  typedef itk::ImageRegionSplitterBase
    SplitterBase;
  typedef std::vector<
    RegionType
  > OutputType;

  // create a splitter
  const typename SplitterBase::Pointer splitter( SplitterType::New() );

  // how many subregions can we actually split into?
  const unsigned int numberOfSubRegions =
    splitter->GetNumberOfSplits( region, numberOfRequestedSubRegions );

  // prepare our output
  OutputType subRegions;
  subRegions.reserve( numberOfSubRegions );

  // now split up
  for ( unsigned int subRegionIndex = 0;
    subRegionIndex < numberOfSubRegions; ++subRegionIndex )
    {

    // extract and add it to our vector
    
    // The new version requires a copy of the
    // original region to be modified into
    // a new "split" subregion
    RegionType currentRegion = region;
    
    splitter->GetSplit(
      subRegionIndex, numberOfSubRegions, currentRegion );

    subRegions.push_back( currentRegion );
    
    } // subRegionIndex

  return subRegions;

} // SplitRegion
///////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdSplitRegion_hxx
