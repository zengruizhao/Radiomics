//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdSaveImage.h"      // for saving results
#include "ccipdLoadVolume.h"     // for loading images
#include "ccipdVolumeTypes.h"    // for image types
#include "ccipdStatistics.h"

// std includes
#include <iostream>             // for cout
#include <string>               // for filenames

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageSliceIteratorWithIndex.h>  // to access voxels
  #include <itkDiscreteGaussianImageFilter.h>
  #include <itkImage.h>      // for saving results
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

using ccipd::VolumeMaskImagePointer;
using ccipd::VolumeImagePointer;
using ccipd::VolumeMaskImageType;
//////////////////////////////////////////////////////////////////////////////////////////



#define epsilon 1e-6; 

int main( int argc, char * argv[] )
{

  if ( argc < 6 )
  {
    cout << endl << "Usage:" << 
      argv[ 0 ] << " MultiLabelFileName outputFileName LabelToExtract(int) [one of uved] [startSlice endSlice] " << endl;
    return EXIT_SUCCESS;
  }

  // extract the inputs
  const string
    imageFileName( argv[ 1 ] ),
    outputFileNamePrefix ( argv[ 2 ] );
 
  const int 
    intLabel  = argc > 3 ? static_cast< int >( strtod( argv[ 3 ], NULL ) ) : 1;

	string 
		options 	= argc > 4 ? argv[ 4 ]: "";

  int
    minSliceZ = argc > 5 ? static_cast< int >( strtod( argv[ 5 ], NULL ) ) : 0,
    maxSliceZ = argc > 6 ? static_cast< int >( strtod( argv[ 6 ], NULL ) ) : 0; 


	bool 
		addUrethra 				= false,
		addVeru 					= false,
		addEjacDucts			= false, 
		addDominantNodule = false;

	size_t
		found;

	found = options.find("u");
	if (found !=string::npos)
		addUrethra = true;

	found = options.find("n");
	if (found !=string::npos)
		addDominantNodule = true;

	found = options.find("v");
	if (found !=string::npos)
		addVeru = true;

	found = options.find("e");
	if (found !=string::npos)
		addEjacDucts = true;
  
	cout << "Add Urethra: " << 						addUrethra 					<< endl;
	cout << "Add Verumontanum: " << 			addVeru 						<< endl;
	cout << "Add Ejaculatory Ducts: " << 	addEjacDucts 				<< endl;
	cout << "Add DominantNodule: " << 		addDominantNodule 	<< endl;

  // load the image
  cout << "Loading " << imageFileName << ":" <<endl;
  VolumeImagePointer MultiLabelVol  = ccipd::LoadVolume( imageFileName ) ;

	int lableType	= 1; // first set of labels
	
	if (ccipd::GetMax(MultiLabelVol) > 50)
	{
		lableType = 2;
	}

	cout << "LabelType " << lableType << endl;


  if (maxSliceZ==0) // not assigned yet; then use all 
    maxSliceZ = static_cast< int >(
      MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 );

  VolumeImagePointer SingleLabelVol = ccipd::VolumeImageType::New();
  SingleLabelVol->SetRegions(MultiLabelVol->GetLargestPossibleRegion());
  SingleLabelVol->SetOrigin (MultiLabelVol->GetOrigin()               );
  SingleLabelVol->SetSpacing(MultiLabelVol->GetSpacing()              );
  SingleLabelVol->Allocate  ();

  if ( !MultiLabelVol )
    {
    cerr << "Loading " << imageFileName << " failed." <<endl;
    return EXIT_FAILURE;
    }
  cout << "Loading " << imageFileName << " done." <<endl;

 typedef itk::ImageSliceIteratorWithIndex< 
 const ccipd::VolumeImageType > IteratorType;

  IteratorType 
    itML  ( MultiLabelVol,  MultiLabelVol ->GetLargestPossibleRegion() ),
    itSL  ( SingleLabelVol, SingleLabelVol->GetLargestPossibleRegion() );

  itML.GoToBegin();
  itML.SetFirstDirection  ( 0 ); // 0=x, 1=y, 2=z
  itML.SetSecondDirection ( 1 ); // 0=x, 1=y, 2=z

  itSL.GoToBegin();
  itSL.SetFirstDirection  ( 0 ); // 0=x, 1=y, 2=z
  itSL.SetSecondDirection ( 1 ); // 0=x, 1=y, 2=z

  // because of the overlapping anatomic regions and multiple labels one has to extend the annotations
  // e.g. The prostate is label 1;2;4;5;13;27;37;40 within the slices that define the 
  vector<int> labels;

  switch (intLabel)
  {
    case 1: // add the prostate 
			if (lableType == 1)
			{
				labels.push_back(1);  // prostate
				labels.push_back(2);  // central gland
				labels.push_back(4);  // peripheral zone
				labels.push_back(5);  // seminal vesicles 
				labels.push_back(13); // urethra 
				labels.push_back(27); // dominant nodule
				labels.push_back(37); // ejaculatory ducts
				labels.push_back(40); // veramontanum
			}
			if (lableType == 2)
			{
				labels.push_back(1);  // prostate
				labels.push_back(4);  // peripheral zone
				labels.push_back(7);  // central gland
				//labels.push_back(4);  // peripheral zone
				//labels.push_back(126);  // seminal vesicles 
				labels.push_back(9); // urethra 
				//labels.push_back(46); // dominant nodule
				labels.push_back(152); // ejaculatory ducts
				labels.push_back(37); // veramontanum
			}
    break;
   case 2: // Central Gland
      labels.push_back(2);  // central gland
			if (addUrethra)
      	labels.push_back(13); // urethra 
			if (addDominantNodule)
      	labels.push_back(27); // dominant nodule
			if (addEjacDucts)
      	labels.push_back(37); // ejaculatory ducts
			if (addVeru)
      	labels.push_back(40); // veramontanum
    break;
    case 4: // Peripheral Zone	
		if (lableType == 1)
			{
      labels.push_back(4);  // peripheral zone
      if (addDominantNodule)
      	labels.push_back(27); // dominant nodule
			if (addEjacDucts)
      	labels.push_back(37); // ejaculatory ducts
      if(addVeru)
				labels.push_back(40); // veramontanum
			}
 		if (lableType == 2)
			{
      labels.push_back(4);  // peripheral zone
      if (addDominantNodule)
      	labels.push_back(27); // dominant nodule
			if (addEjacDucts)
      	labels.push_back(152); // ejaculatory ducts
      if(addVeru)
				labels.push_back(37); // veramontanum
			}
   break;
    case 5: // seminal vesicles
      labels.push_back(5);  // seminal vesicles   
      minSliceZ = 0;
      maxSliceZ = static_cast< int >(
        MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
    break;
		case 7: // Central Gland
				labels.push_back(7);  // central gland
				if (addUrethra)
					labels.push_back(9); // urethra 
				//if (addDominantNodule)
				//	labels.push_back(46); // dominant nodule
				if (addEjacDucts)
					labels.push_back(152); // ejaculatory ducts
				if (addVeru)
					labels.push_back(37); // veramontanum
			break;
	  case 9: //Urethra
			if (lableType == 2)
			{
				labels.push_back(9); // urethra 
				if (addVeru)
					labels.push_back(37); // veramontanum
				minSliceZ = 0;
				maxSliceZ = static_cast< int >(
          MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
			}
			break;

    case 13: //Urethra
			if (lableType == 1)
			{
				labels.push_back(13); // urethra 
				if (addVeru)
					labels.push_back(40); // veramontanum
				minSliceZ = 0;
				maxSliceZ = static_cast< int >(
          MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
			}

			if (lableType == 2)
			{
				labels.push_back(9); // urethra 
				if (addVeru)
					labels.push_back(37); // veramontanum
				minSliceZ = 0;
				maxSliceZ = static_cast< int >(
          MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
			}
			break;
    case 17:
      labels.push_back(17); // neurovascular bundles 
      minSliceZ = 0;
      maxSliceZ = static_cast< int >(
        MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
     break;
   case 27:
			if (lableType == 1)
      	labels.push_back(27); // dominant nodule
      /*
      // Rob's note 2012-10-28 - this code is after break and will never be reached
      minSliceZ = 0;
      maxSliceZ = MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1; 
      */
  case 32:
      labels.push_back(32); // neurovascular bundles 
      minSliceZ = 0;
      maxSliceZ = static_cast< int >(
        MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
     break;
    case 33:
      labels.push_back(33); // Penal bulb  
      minSliceZ = 0;
      maxSliceZ = static_cast< int >( MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
     break;
    case 37:
			if (lableType == 1)
			{
      	labels.push_back(37); // ejaculatory ducts 
      	minSliceZ = 0;
      	maxSliceZ = static_cast< int >(
          MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
			}
 			if (lableType == 2)
			{
      	labels.push_back(37); // verumontanum 
      	labels.push_back(152); // verumontanum 
			}
     break;
    case 40:
      labels.push_back(40); // veramontanum
      minSliceZ = 0;
      maxSliceZ = static_cast< int >(
        MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
			break; 
    case 41:
      labels.push_back(13); // urethra 
      labels.push_back(37); // ejaculatory ducts 
      labels.push_back(40); // veramontanum 
      minSliceZ = 0;
      maxSliceZ = static_cast< int >(
        MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
     break;
		case 46:
			if (lableType == 2)
			{
      	labels.push_back(46); // cancer/index lesion
      	labels.push_back(68); // extraglandular extension  
			}
      minSliceZ = 0;
      maxSliceZ = static_cast< int >(
        MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
			break;
     case 126: //Seminal vesicles
      labels.push_back(126);  
      minSliceZ = 0;
      maxSliceZ = static_cast< int >(
        MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
     break;
   case 152: //Ejaculatory ducts
      labels.push_back(152); // ejaculatory ducts 

      minSliceZ = 0;
      maxSliceZ = static_cast< int >(
        MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] - 1 ); 
     break;
 }


	cout << "Slices from [" << minSliceZ << " " << maxSliceZ << "]; Lable classes:" << labels.size() << endl; 

  bool add;

  int currentSlice; 
  ccipd::InternalPixelType v0;
  while( !itML.IsAtEnd() ) // going through slice on z
  {
    currentSlice = static_cast< int >( itML.GetIndex()[2] );
    while( !itML.IsAtEndOfSlice() )
    {
      while( !itML.IsAtEndOfLine() )
      {
        v0 = itML.Get();

        add = false;
        for (unsigned int i=0; i<labels.size(); i++)
          if ( abs(v0 - labels[i]) < 1e-6  && currentSlice >= minSliceZ && currentSlice <= maxSliceZ ) 
            add = true;

        if (add)  
          itSL.Set( 1 );
        else
          itSL.Set( 0 );

        ++itML;
        ++itSL;
      }
      itML.NextLine ();
      itSL.NextLine();
    }
    itML.NextSlice  ();
    itSL.NextSlice ();
   }
/*
  typedef itk::DiscreteGaussianImageFilter<
    VolumeMaskImageType, ccipd::VolumeImageType >  filterType;
  const float variance = 0.1; 
  filterType::Pointer gaussianFilter = filterType::New();
  gaussianFilter->SetInput( SingleLabelVol );
  //gaussianFilter->SetInput( MultiLabelVol );
  gaussianFilter->SetVariance(variance);
  gaussianFilter->Update();
  */



	SingleLabelVol->SetDirection( MultiLabelVol->GetDirection() );
  // save the result
  cout << "Saving extraction result:" << endl;
  const bool savedSuccessfully =
    ccipd::SaveImage( SingleLabelVol.GetPointer(), outputFileNamePrefix, true );
    //ccipd::SaveImage( gaussianFilter->GetOutput(), outputFileNamePrefix, true );
  if ( !savedSuccessfully )
    {
    cerr << "Saving extraction result failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Saving extraction result done." << endl;

  return EXIT_SUCCESS;

} // main
