// ccipd includes
#include "ccipdVTKUtils.h"
#include "ccipdMouseInteractorStylePointPicker.h"
#include "ccipdDisableWarningsMacro.h"

// vtk includes
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkInteractorStyleUser.h>
#include <QVTKInteractor.h>
#include <vtkEventQtSlotConnect.h>

#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{

//static void leftClickCallbackFunction ( vtkObject* /*caller*/, long unsigned int /*eventId*/, void* /*clientData*/, void* /*callData*/ )
//{
//  std::cout << "Left Click callback" << std::endl;
//
//  static bool leftButtonpressed = true;
//
//
//  // We can get the calling object like this:
//  // vtkRenderWindowInteractor *iren = 
//  //  static_cast<vtkRenderWindowInteractor*>(caller);
//}

//static void mouseMoveCallbackFunction ( vtkObject* caller, long unsigned int eventId, void* /*clientData*/, void* /*callData*/ )
//{
//
//  cout << eventId << endl;
//  
//  if(eventId == QVTKInteractor::DragMoveEvent)
//  {
//  
//
//
//  std::cout << "Mouse Move callback" << std::endl;
//
//  static int prev_x;
//  static int prev_y; 
//         
//
//  //((QVTKInteractor*)caller)->GetMousePosition(&current_x, &current_y);
//
//  const int * const eventPosition = ((QVTKInteractor*)caller)->GetEventPosition();
//    const int current_x = eventPosition[ 0 ],
//              current_y = eventPosition[ 1 ];
//              //current_z = 0; // always 0
//
//    
//  int deltaX = current_x - prev_x;
//  int deltaY = current_y - prev_y;
//
//  cout << "deltaX = " << deltaX << "    deltaY = " << deltaY << endl;
//
//    prev_x = current_x;
//    prev_y = current_y;
//  //this->Interactor->GetControlKey()
//
//  // We can get the calling object like this:
//  // vtkRenderWindowInteractor *iren = 
//  //  static_cast<vtkRenderWindowInteractor*>(caller);
//
//  }
//}

//static void mouseWheelForwardCallbackFunction ( vtkObject* /*caller*/, long unsigned int /*eventId*/, void* /*clientData*/, void* /*callData*/ )
//{
//  //void mouseEvent(QMouseEvent* event);
//  //((QVTKInteractor*)caller)->MouseWheelBackwardEvent(
//   // connect(caller, SIGNAL(mouseEvent(QMouseEvent* event)), caller,SLOT());
//
//  cout << "f\n";
//
//}

//static void mouseWheelBackwardCallbackFunction ( vtkObject* /*caller*/, long unsigned int /*eventId*/, void* /*clientData*/, void* /*callData*/ )
//{
//
//  cout << "b\n";
//
//}



  bool initializeQVTKWidget(QVTKWidget* pWidget)
	{
		if (!pWidget)
		{
			cout << "\nError(141-34): NULL qWidget Pointer ...\n";
			return false;
		}

		// Create VTK Renderer
		const vtkRendererPointer renderer = vtkRendererPointer::New();
		renderer->SetBackground( 1.00, 1.00, 1.00);

		// Create an interactor style
		//const vtkInteractorStylePointer interactorStyle = vtkInteractorStyleTrackballCameraPointer::New();
		//vtkSmartPointer<MouseInteractorStylePointPicker> interactorStyle =  vtkSmartPointer<MouseInteractorStylePointPicker>::New();
    //vtkSmartPointer<vtkInteractorStyleImage> interactorStyle =  vtkSmartPointer<vtkInteractorStyleImage>::New();
    //vtkSmartPointer<vtkInteractorStyleUser> interactorStyle =  vtkSmartPointer<vtkInteractorStyleUser>::New();
    vtkSmartPointer<vtkInteractorStyleCustomized> interactorStyle =  vtkSmartPointer<vtkInteractorStyleCustomized>::New();

    //interactorStyle->SetInteractionModeToImage3D();


    vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();


		// Get the render window
		const vtkRenderWindowPointer renderWindow = pWidget->GetRenderWindow();

		// make sure we have an interactor
		vtkRenderWindowInteractorPointer interactor = renderWindow->GetInteractor();
		if (!interactor)
		{
			interactor = vtkRenderWindowInteractorPointer::New();
			interactor->SetRenderWindow( renderWindow );
		} // !interactor
		
    interactor->SetPicker(pointPicker);
		//interactor->GetPicker

		// update the style
		renderWindow->GetInteractor()->SetInteractorStyle(interactorStyle/*.GetPointer()*/);


     // vtkSmartPointer<vtkCallbackCommand> leftClickCallback = vtkSmartPointer<vtkCallbackCommand>::New();
     // leftClickCallback->SetCallback(leftClickCallbackFunction);

     // vtkSmartPointer<vtkCallbackCommand> mouseMoveCallback = vtkSmartPointer<vtkCallbackCommand>::New();
     //mouseMoveCallback->SetCallback(mouseMoveCallbackFunction);

    //renderWindow->GetInteractor()->AddObserver ( vtkCommand::LeftButtonPressEvent, leftClickCallback );
    //renderWindow->GetInteractor()->AddObserver ( vtkCommand::MouseMoveEvent      , mouseMoveCallback );


    // I need, not the direct parent of pWidget, but the Uppermost Parent window of it where the slot is implemented
    //QObject *pParentObj = pWidget; 
    //do
    //{
    //  pParentObj = pParentObj->parent();
    //}
    //while(pParentObj->parent());

    //vtkSmartPointer<vtkEventQtSlotConnect> Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    //  
    //  
    //  Connections->Connect(pWidget->GetRenderWindow()->GetInteractor()//  interactor
    //  , vtkCommand::LeftButtonPressEvent,
    //  pParentObj,
    //  SLOT(slot_clicked(vtkObject*, unsigned long, void*, void*)));

      //pParentObj->slot_clicked(NULL, 1, NULL, NULL);


    //vtkSmartPointer<vtkCallbackCommand> mouseWheelForwardCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    //mouseWheelForwardCallback->SetCallback(mouseWheelForwardCallbackFunction);
    //renderWindow->GetInteractor()->AddObserver ( vtkCommand::MouseWheelForwardEvent , mouseWheelForwardCallback );
    //
    
    //
    //vtkSmartPointer<vtkCallbackCommand> mouseWheelBackwardCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    //mouseWheelBackwardCallback->SetCallback(mouseWheelBackwardCallbackFunction);
    //renderWindow->GetInteractor()->AddObserver ( vtkCommand::MouseWheelBackwardEvent, mouseWheelBackwardCallback );


		// add the renderer
		renderWindow->AddRenderer( renderer );
		renderWindow->Render();
    renderWindow->Render();
		return true;
  }

}

