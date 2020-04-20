

// http://bit.ly/Is480q 



//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdVTKForwardDeclarations.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

#include <QApplication>
#include <QVTKWidget.h>
#include <QString>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char** argv)
{

  QApplication app(argc, argv);
 
  // setup sphere
  typedef vtkSmartPointer< vtkSphereSource > vtkSphereSourcePointer;
  const vtkSphereSourcePointer sphereSource = vtkSphereSourcePointer::New();
  sphereSource->Update();

  using ccipd::vtkPolyDataMapperPointer;
  const vtkPolyDataMapperPointer sphereMapper =  vtkPolyDataMapperPointer::New();

  sphereMapper->SetInputConnection( sphereSource->GetOutputPort() );
  using ccipd::vtkActorPointer;
  const vtkActorPointer sphereActor = vtkActorPointer::New();
  sphereActor->SetMapper(sphereMapper);
 
  // setup window
  using ccipd::vtkRenderWindowPointer;
  const vtkRenderWindowPointer renderWindow = vtkRenderWindowPointer::New();
 
  // setup renderer
  using ccipd::vtkRendererPointer;
  const vtkRendererPointer renderer = vtkRendererPointer::New();
  renderWindow->AddRenderer( renderer );
 
  renderer->AddActor( sphereActor );
  renderer->ResetCamera();
 
  // set up widget
  QVTKWidget widget;
  widget.resize( 256, 256 );
  widget.SetRenderWindow( renderWindow );
  widget.setWindowTitle ( "First Qt + VTK Application" );
  widget.show();
 
  app.exec();
 
  return EXIT_SUCCESS;

} // main
