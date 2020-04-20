#ifndef __ccipdGetChannelByIndex_hxx__
#define __ccipdGetChannelByIndex_hxx__
#include "ccipdGetChannelByIndex.h"
#include "ccipdDisableWarningsMacro.h"

		// itk includes
		#include <itkImage.h>
		#include <itkVectorImage.h>
		#include <itkVectorIndexSelectionCastImageFilter.h>

		// std includes
		#include <iostream>		

#include "ccipdEnableWarningsMacro.h"

namespace ccipd
{


template< class TVectorImage, class TScalarImage >
typename TScalarImage::Pointer 
GetChannelByIndex(  const typename TVectorImage::Pointer input
                  , const unsigned int                   index 
			     )
{
	// safety checks
	if(!input)
	{
		std::cerr << "\nError(899-36-34): Invalid input ...";
		return NULL;//OutputImagePointer();
	}

	if ( index >= input->GetNumberOfComponentsPerPixel() )
	{
		std::cerr << "\nError(119-36-34): Invalid channel index ...";
		return NULL;//OutputImagePointer();
	}

	// objects
	typedef itk::VectorIndexSelectionCastImageFilter< TVectorImage, TScalarImage > FeatureExtractorType;
	typename FeatureExtractorType::Pointer featureExtractor = FeatureExtractorType::New();

	// inputs
	featureExtractor->SetInput( input );
	featureExtractor->SetIndex( index );

	// update and output
	featureExtractor->Update();
	return featureExtractor->GetOutput();

} // GetChannelByIndex




}


#endif