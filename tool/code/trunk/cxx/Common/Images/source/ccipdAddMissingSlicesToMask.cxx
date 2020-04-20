#include "ccipdAddMissingSlicesToMask.h" 

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageSliceIteratorWithIndex.h> // for loading masks
#include "ccipdEnableWarningsMacro.h"

///////////////  // boost includes
  #include <boost/range/algorithm.hpp>     // for hashing
#///////////////////////////////////////////////////////////////////////////
namespace ccipd
{
using std::vector;
using std::cout;
using std::endl;

vector<int> AddMissingData(VolumeMaskImagePointer mask, vector<int> slices)
{
  typedef itk::ImageSliceIteratorWithIndex
    < VolumeMaskImageType > SliceIteratorType;
  vector<int> 
    modifiedSlices,
    slicesBefore,
    slicesAfter;

  int
    radius = 10;
  VolumeImageType::IndexType 
    start,end;
  VolumeImageType::SizeType 
    size;
  size[0] = radius*2+1;
  size[1] = radius*2+1;
  size[2] = radius*2+1;

  if (slices.size()<=1)
  {
    cout << "Data will not be filled as there are to few slices" << endl;
    return modifiedSlices;
  }

  //sort slices to make sure that first slice has lowest number
  boost::sort(slices);  

  //expect that slices are in the same order
  int 
    sliceDiff;  

  for (unsigned int i = 1; i < slices.size(); i++)
  {
    sliceDiff = abs(slices[i-1]-slices[i]); // assumed order

    if (sliceDiff == 2) 
    {
      //slices are in the txt file between 1..n, but c 0 to n-1
      modifiedSlices. push_back(slices[i-1] + (slices[i]-slices[i-1])/2);
      slicesBefore.   push_back(slices[i-1] - 1);
      slicesAfter.    push_back(slices[i  ] - 1);
    }
  }

  SliceIteratorType itMaskBefore    ( mask,     mask->GetLargestPossibleRegion()    );
  SliceIteratorType itMask          ( mask,     mask->GetLargestPossibleRegion()    );
  SliceIteratorType itMaskAfter     ( mask,     mask->GetLargestPossibleRegion()    );


  //previous slice
  itMaskBefore.SetFirstDirection  (0); // x?
  itMaskBefore.SetSecondDirection (1); // y?

  //current slice
  itMask.SetFirstDirection        (0); // x?
  itMask.SetSecondDirection       (1); // y?

  //following slice
  itMaskAfter.SetFirstDirection   (0); // x?
  itMaskAfter.SetSecondDirection  (1); // y?

  EvaluationValueType value;
  auto iSize = modifiedSlices.size();
  for(unsigned int i=0; i<iSize; i++)
  {
    cout << " *** Filling missing cancer annotation in slice " << modifiedSlices[i] << endl;
 
    itMaskBefore.GoToBegin();

    itMask.GoToBegin();
    itMask.NextSlice();
    
    itMaskAfter.GoToBegin();
    itMaskAfter.NextSlice(); 
    itMaskAfter.NextSlice();

    //go to the right slice; 
    while( !itMaskAfter.IsAtEnd() && itMaskAfter.GetIndex()[2] != slicesAfter[i] )
    {
      itMaskBefore.   NextSlice();
      itMask.         NextSlice();
      itMaskAfter.    NextSlice();
    }
    
    if (!itMaskAfter.IsAtEnd()) // not at the end then replace
    {
      while( !itMaskAfter.IsAtEndOfSlice() )
      {
        while( !itMaskAfter.IsAtEndOfLine() )
        {
          if (itMaskAfter.Get() != itMaskBefore.Get()) // different valued voxels - then interpolate
          {
            start[0] = itMaskBefore.GetIndex()[0]-radius>=0 ? itMaskBefore.GetIndex()[0]-radius : 0;
            start[1] = itMaskBefore.GetIndex()[1]-radius>=0 ? itMaskBefore.GetIndex()[1]-radius : 0;
            start[2] = itMaskBefore.GetIndex()[2];

            size[0] = itMaskBefore.GetIndex()[0]+radius*2+1 >= (int)mask->GetLargestPossibleRegion().GetSize()[0] ?   
                - itMaskBefore.GetIndex()[0] + mask->GetLargestPossibleRegion().GetSize()[0]  : 
                radius*2+1;
            size[1] = itMaskBefore.GetIndex()[1]+radius*2+1 >= (int)mask->GetLargestPossibleRegion().GetSize()[1] ?   
                - itMaskBefore.GetIndex()[1] + mask->GetLargestPossibleRegion().GetSize()[1]  : 
                radius*2+1;
            size[2] = 1;

            end [0] = start[0] + size[0];
            end [1] = start[1] + size[1];
            end [2] = start[2] + size[2];
            
            VolumeImageType::RegionType region1(start, size);
            SliceIteratorType itMaskPatchBefore          (mask,     region1); 

            start[2] = itMaskAfter.GetIndex()[2] ;
            VolumeImageType::RegionType region2(start,size);
            SliceIteratorType itMaskPatchAfter          (mask,     region2);  

            itMaskPatchBefore.SetFirstDirection(0); // x?
            itMaskPatchBefore.SetSecondDirection(1); // y?

            itMaskPatchAfter.SetFirstDirection(0); // x?
            itMaskPatchAfter.SetSecondDirection(1); // y?


            float sum     = 0.0;
            int   count       = 0;

            itMaskPatchBefore.GoToBegin();
            itMaskPatchAfter.GoToBegin();

            while (!itMaskPatchAfter.IsAtEnd())
            {
            while( !itMaskPatchAfter.IsAtEndOfSlice() )
            {
              while( !itMaskPatchAfter.IsAtEndOfLine() )
              {
                auto diff =itMaskPatchAfter.GetIndex() - itMaskAfter.GetIndex();
                if (diff[0]*diff[0]+diff[1]*diff[1]+diff[2]*diff[2] < radius*radius)
                {
                  sum += (itMaskPatchBefore.Get() + itMaskPatchAfter.Get());
                  count += 2;
                }
                ++itMaskPatchAfter;
                ++itMaskPatchBefore;
              }
              itMaskPatchBefore .NextLine();
              itMaskPatchAfter  .NextLine();
            }

            itMaskPatchBefore .NextSlice();
            itMaskPatchAfter  .NextSlice();
            }

            value = (sum/(float)count<=0.5 )? 0.0:1.0;
          }
          else
          {
            value = itMaskAfter.Get();  
          }

          itMask.Set( static_cast< unsigned char >( value ) ); //Rachel's note masks are chars not floats.

          ++itMaskBefore;
          ++itMask;
          ++itMaskAfter;
        }
       itMaskBefore .NextLine();
       itMask       .NextLine();
       itMaskAfter  .NextLine();
      }
    }   
  }


  return modifiedSlices;  
};

} //ccipd name space
