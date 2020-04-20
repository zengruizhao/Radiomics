#ifndef __ccipdVTKUtils_hxx
#define __ccipdVTKUtils_hxx


//#include "ccipdVTKForwardDeclarations.h"
#include <QVTKWidget.h>
#include <itkImageBase.h>
#include <vtkProp.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>


namespace ccipd
{
	//template <class TVolumeImageType>
	//void renderVolumeAsSlices(typename TVolumeImageType::Pointer pVolumeImage, QVTKWidget* pWidget, vtkPropPointer & pProp, unsigned int sliceNumber)
	//{
	//	if (!pWidget || !pVolumeImage || pProp)
	//	{
	//		//statusBar()->showMessage("Invalid rendering parameters ...");
	//		return;
	//	}

	//   const vtkRenderWindowPointer renderWindow = pWidget->GetRenderWindow();
	//   const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	//   renderer->SetBackground ( 0.0, 0.0, 0.0 );    //Define the background color of rendering window
	//   renderer->ResetCamera();

	//   renderITKImage<TVolumeImageType>(pVolumeImage, renderWindow, pProp, sliceNumber);
	//}
}


#endif //