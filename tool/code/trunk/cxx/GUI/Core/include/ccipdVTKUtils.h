#ifndef __ccipdVTKUtils_h
#define __ccipdVTKUtils_h

#include "ccipdUpdateVTKImageProp.hxx"

#include "ccipdIsosurfaceRendering.h"
#include "ccipdRenderITKImage.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdDisableWarningsMacro.h"


#include <QVTKWidget.h>

#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include "ccipdRenderITKImage.h"


#include "ccipdEnableWarningsMacro.h"

namespace ccipd
{
	template <class TVolumeImageType>
	void renderVolumeAsSlices(  typename TVolumeImageType::Pointer pVolumeImage
		                      , QVTKWidget* pWidget
							  , vtkPropPointer & pProp
							  , const unsigned int sliceNumber = 0
                              , const bool makeZeroTransparent = false
							 )
	{
		if (!pWidget || !pVolumeImage)
		{
			cerr << "\n" << "Error(2307): Invalid rendering parameters ... " << "\n";
			return;
		}
	   
	   const vtkRenderWindowPointer renderWindow = pWidget->GetRenderWindow();
	   if (!(bool)renderWindow)
	   {
		   cout << "\nError(30-26): Invalid Render Window in the passed qWidget ...\n";
           return;
	   }
	   const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	   
	   
	   
	   renderer->SetBackground ( 0.0, 0.0, 0.0 );    //Define the background color of rendering window
	   renderer->ResetCamera();

	   renderITKImage<TVolumeImageType>(pVolumeImage, renderWindow, pProp, sliceNumber, makeZeroTransparent);
	}

  /** \brief  : This function renders a volume image as a levelset iso surface 
  * \Created: Ahmad Algohary 10/03/2013
  * \Updated: Ahmad Algohary 10/08/2013 
  * \ingroup: GUI Core
  * \param  : pVolumeImage: The input volume image to be rendered.
  * \param  : pWidget     : A pointer to the QVTKWidget on which the pVolumeImage is rendered. 
  * \param  : isBinaryMask: A boolean which tells whether the input pVolumeImage is a binary image, if true, then a 
                            levelset volume image is created (by calling the MaskToLevelset Function), otherwise, 
                            the pVolumeImage will be considered the required levelset to create the surface.
  * \param  : levelsetThresholdValue: This is the isoValue, all values below this value will be ignored when creating the surface.
  * \param  : levelsetOpacity       : The opacity of the rendered surface, ranges from 0.0 to 1.0.    
  * \param  : maskSmoothing         : The smoothing variance (i.e. the degree of smoothing applied to the created surface), 0.0 means no smoothing applied.      
  * \param  : normalize             : boolean tells whether to normalize the pVolumeImage.          
  * \param  : resizeIsotropically   : if true, created surface will be resized isotropically (spacing will be equal alont the three directions), this will most probably interpolate along the z-direction.
  * \param  : levelsetSurfaceR      : Red Component level of the created surface color (ranges from 0.0 to 1.0).
  * \param  : levelsetSurfaceG      : Green Component level of the created surface color (ranges from 0.0 to 1.0).		
  * \param  : levelsetSurfaceB      : Blue Component level of the created surface color (ranges from 0.0 to 1.0).		
  * \param  : renderEdge            : boolean to decide if edges of the created surface will be rendered too.         
  * \param  : levelsetEdgeR         : Red Component level of the created surface edge color (ranges from 0.0 to 1.0), works only if renderEdge is true.		  
  * \param  : levelsetEdgeG         : Green Component level of the created surface edge color (ranges from 0.0 to 1.0), works only if renderEdge is true.		  
  * \param  : levelsetEdgeB         : Blue Component level of the created surface edge color (ranges from 0.0 to 1.0), works only if renderEdge is true.			  
  * \return : smart Pointer to a templated Histogram type (itk::Statistics::Histogram)
  * \Notes  : This function assumes a .csv file with data either coming in 1 column of frequencies for each histogram bin
              All decimal points of the given frequencies will be ignored - frequencies' type are restricted to unsigned int
  */
	template <class TVolumeImageType>
	vtkActorPointer renderVolumeAsSurface(  typename TVolumeImageType::Pointer pVolumeImage
										                    ,          QVTKWidget*               pWidget
										                    , const    bool                      isBinaryMask           = false
										                    , const    float                     levelsetThresholdValue = 0.0f
										                    , const    float                     levelsetOpacity        = 0.1f
										                    , const    float                     maskSmoothing          = 3.0f
										                    , const    bool                      normalize              = false
										                    , const    bool                      resizeIsotropically    = false
										                    , const    float                     levelsetSurfaceR		    = 0.2f
										                    , const    float                     levelsetSurfaceG		    = 0.8f
										                    , const    float                     levelsetSurfaceB		    = 0.6f
										                    , const    bool                      renderEdge             = false
										                    , const    float                     levelsetEdgeR		      = 0.0f
										                    , const    float                     levelsetEdgeG		      = 0.0f
										                    , const    float                     levelsetEdgeB		      = 1.0f
										                   )
	{
		if (!pWidget || !pVolumeImage)
		{
			cerr << "\n" << "Error(8607): Invalid rendering parameters ... " << "\n";
			return NULL;
		}

		//if (levelsetValue < GetMin(pVolume) || levelsetValue > GetMax(pVolume))
		if (levelsetThresholdValue < 0.0f || levelsetThresholdValue > 1.0f) // Assuming only this range for speeding the process
		{
			//statusBar()->showMessage("Error: Levelset out of range ...");
			return NULL;
		}

		// Check if the given image is binary Mask
		ccipd::vtkActorPointer levelsetActor;
		if (isBinaryMask)
		{
			typename TVolumeImageType::Pointer levelset(ccipd::MaskToLevelset< typename TVolumeImageType::PixelType>(
				pVolumeImage.GetPointer(), normalize, false, 0, maskSmoothing, resizeIsotropically));

			levelsetActor = ccipd::GenerateVTKActorFromIsosurface(levelset, levelsetThresholdValue);

		}
		else
		{
			levelsetActor = ccipd::GenerateVTKActorFromIsosurface(pVolumeImage, levelsetThresholdValue);

		}

		levelsetActor->GetProperty()->SetOpacity(levelsetOpacity);
		levelsetActor->GetProperty()->SetColor(levelsetSurfaceR, levelsetSurfaceG, levelsetSurfaceB);

		levelsetActor->GetProperty()->SetEdgeVisibility(renderEdge);
		levelsetActor->GetProperty()->SetEdgeColor(levelsetEdgeR, levelsetEdgeG, levelsetEdgeB);


		const vtkRenderWindowPointer renderWindow = pWidget->GetRenderWindow();
		const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();

		renderer->AddActor(levelsetActor);

		renderer->SetBackground(0.0, 0.0, 0.0);
		renderer->ResetCamera();
		renderWindow->AddRenderer(renderer);
		renderWindow->Render();

		return levelsetActor;

	} // function
	

	bool initializeQVTKWidget(QVTKWidget* pWidget);


} // namespace


#endif // __ccipdVTKUtils_h
