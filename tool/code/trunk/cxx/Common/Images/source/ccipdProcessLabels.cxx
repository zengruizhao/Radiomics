
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdProcessLabels.h"
#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageSliceIteratorWithIndex.h>  // to access voxels
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{

 /**
  * Extract one label from a file with multiple labels
  **/
VolumeMaskImageConstPointer ExtractLabels(const VolumeImageType * MultiLabelVol, std::vector<int> inLabels, int outLabel)
{
  VolumeMaskImagePointer  SingleLabelVol = VolumeMaskImageType::New();
  SingleLabelVol->SetRegions  (MultiLabelVol->GetLargestPossibleRegion());
  SingleLabelVol->SetOrigin   (MultiLabelVol->GetOrigin()               );
  SingleLabelVol->SetSpacing  (MultiLabelVol->GetSpacing()              );
  SingleLabelVol->Allocate    ();
  SingleLabelVol->SetDirection(MultiLabelVol->GetDirection() );

  std::cout << inLabels.size() << " " << outLabel << std::endl;
  typedef itk::ImageSliceIteratorWithIndex< 
    const ccipd::VolumeImageType > IteratorTypeVol;

  typedef itk::ImageSliceIteratorWithIndex< 
    const ccipd::VolumeMaskImageType > IteratorTypeMask;

  IteratorTypeVol 
    itML  ( MultiLabelVol,  MultiLabelVol ->GetLargestPossibleRegion() );

  IteratorTypeMask
    itSL  ( SingleLabelVol, SingleLabelVol->GetLargestPossibleRegion() );

  itML.GoToBegin();
  itML.SetFirstDirection  ( 0 ); // 0=x, 1=y, 2=z
  itML.SetSecondDirection ( 1 ); // 0=x, 1=y, 2=z

  itSL.GoToBegin();
  itSL.SetFirstDirection  ( 0 ); // 0=x, 1=y, 2=z
  itSL.SetSecondDirection ( 1 ); // 0=x, 1=y, 2=z

  bool add;
  ccipd::InternalPixelType v0;
  while( !itML.IsAtEnd() ) // going through slice on z
  {
    while( !itML.IsAtEndOfSlice() )
    {
      while( !itML.IsAtEndOfLine() )
      {
        v0 = itML.Get();

        add = false;
        for (unsigned int i=0; i<inLabels.size(); i++)
        {
          if ( abs(v0 - inLabels[i]) < 1e-6 )
            add = true; 
        }

        if (add) 
          itSL.Set( static_cast< ccipd::MaskPixelType >( outLabel ) );
        else
          itSL.Set( 0 );

        ++itML;
        ++itSL;
      }
      itML.NextLine ();
      itSL.NextLine();
    }
    itML.NextSlice();
    itSL.NextSlice();
  }
  return static_cast<const  VolumeMaskImageType * >(SingleLabelVol);

};



/**
 * Merge two labels in the same file uses outValue1 for the first volume and outValue2 for the second volume output
 **/
VolumeMaskImageConstPointer MergeLabels(const VolumeMaskImageConstPointer a, const VolumeMaskImageConstPointer b, int outValue1, int outValue2  )
{
  VolumeMaskImagePointer c = VolumeMaskImageType::New();
  c->SetRegions  (a->GetLargestPossibleRegion());
  c->SetOrigin   (a->GetOrigin()               );
  c->SetSpacing  (a->GetSpacing()              );
  c->Allocate    ();
  c->SetDirection(a->GetDirection() );

  typedef itk::ImageSliceIteratorWithIndex< 
    const ccipd::VolumeMaskImageType > IteratorTypeMask;

  IteratorTypeMask
    itA  ( a, a->GetLargestPossibleRegion() ),
    itB  ( b, b->GetLargestPossibleRegion() ),
    itC  ( c, c->GetLargestPossibleRegion() );

  itA.GoToBegin();
  itA.SetFirstDirection  ( 0 ); // 0=x, 1=y, 2=z
  itA.SetSecondDirection ( 1 ); // 0=x, 1=y, 2=z

  itB.GoToBegin();
  itB.SetFirstDirection  ( 0 ); // 0=x, 1=y, 2=z
  itB.SetSecondDirection ( 1 ); // 0=x, 1=y, 2=z

  itC.GoToBegin();
  itC.SetFirstDirection  ( 0 ); // 0=x, 1=y, 2=z
  itC.SetSecondDirection ( 1 ); // 0=x, 1=y, 2=z

  bool printedOnce = false;
  ccipd::InternalPixelType v0, v1;
  while( !itA.IsAtEnd() ) // going through slice on z
  {
    while( !itA.IsAtEndOfSlice() )
    {
      while( !itA.IsAtEndOfLine() )
      {
        v0 = itA.Get();
        v1 = itB.Get();

        if (v1 != 0 && v0 != 0 && !printedOnce)
        {
          std::cout << "************* Two labels at the same location ***************" << itA.GetIndex() << std::endl;
          printedOnce = true;
        }

        if (v1 != 0) 
          itC.Set( static_cast< ccipd::MaskPixelType >( outValue2 ) );
        if (v0 != 0) 
          itC.Set( static_cast< ccipd::MaskPixelType >( outValue1 ) );
        if (v1==0 && v0 ==0)
          itC.Set( static_cast< ccipd::MaskPixelType >( 0 ) );

        ++itA;
        ++itB;
        ++itC;
      }
      itA.NextLine ();
      itB.NextLine();
      itC.NextLine();
    }
    itA.NextSlice();
    itB.NextSlice();
    itC.NextSlice();
  }

  return static_cast<const  VolumeMaskImageType * >(c);
};
}
