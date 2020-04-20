


//////////////////////////////////////////////////////////////////////////////////////////
#include "TestQtUI.h"

#include "vtkSmartPointer.h"
  
#include "ccipdDisableWarningsMacro.h"
  #include <QApplication>
  #include <QMainWindow>
  #include "ui_TestQtUI.h"

  #include <vtkPolyDataMapper.h>
  #include <vtkRenderer.h>
  #include <vtkRenderWindow.h>
  #include <vtkSphereSource.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Constructor
SimpleView::SimpleView() 
{
  this->ui = new Ui_SimpleView;
  this->ui->setupUi(this);
 
  // sphere
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
  vtkSmartPointer<vtkActor> sphereActor = 
      vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);
 
  // VTK Renderer
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(sphereActor);
 
  // VTK/Qt wedded
  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
 
  // Set up action signals and slots
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
 
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SimpleView::slotExit() 
{
  qApp->exit();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
  // QT Stuff
  QApplication app( argc, argv );
 
  SimpleView mySimpleView;
  mySimpleView.show();
 
  return app.exec();
} // main
//////////////////////////////////////////////////////////////////////////////////////////
