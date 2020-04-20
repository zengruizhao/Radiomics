#ifndef __ccipdVTKForwardDeclarations_h
#define __ccipdVTKForwardDeclarations_h



//////////////////////////////////////////////////////////////////////////////////////////
template< class T >
class vtkSmartPointer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class vtkDicomReader;
class vtkUnstructuredGrid;
class vtkImageData;
class vtkPolyData;
class vtkPoints;
class vtkProp;
class vtkActor;
class vtkActorCollection;
class vtkProperty;
class vtkImageSlice;
class vtkDataSetMapper;
class vtkPolyDataMapper;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkVolume;
class vtkImageViewer2;
class vtkImageActor;
class vtkInteractorStyle;
class vtkInteractorStyleTrackballCamera;
class vtkEventQtSlotConnect;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
namespace ccipd
{
  typedef vtkSmartPointer< vtkImageData              > vtkImageDataPointer;
  typedef vtkSmartPointer< vtkImageActor             > vtkImageActorPointer;
  typedef vtkSmartPointer< vtkUnstructuredGrid       > vtkUnstructuredGridPointer;
  typedef vtkSmartPointer< vtkPolyData               > vtkPolyDataPointer;
  typedef vtkSmartPointer< vtkPoints                 > vtkPointsPointer;
  typedef vtkSmartPointer< vtkProp                   > vtkPropPointer;
  typedef vtkSmartPointer< vtkActor                  > vtkActorPointer;
  typedef vtkSmartPointer< vtkActorCollection        > vtkActorCollectionPointer;
  typedef vtkSmartPointer< vtkProperty               > vtkPropertyPointer;
  typedef vtkSmartPointer< vtkDataSetMapper          > vtkDataSetMapperPointer;
  typedef vtkSmartPointer< vtkPolyDataMapper         > vtkPolyDataMapperPointer;
  typedef vtkSmartPointer< vtkRenderer               > vtkRendererPointer;
  typedef vtkSmartPointer< vtkRenderWindow           > vtkRenderWindowPointer;
  typedef vtkSmartPointer< vtkRenderWindowInteractor > vtkRenderWindowInteractorPointer;
  typedef vtkSmartPointer< vtkVolume                 > vtkVolumePointer;
  typedef vtkSmartPointer< vtkImageViewer2           > vtkImageViewer2Pointer;

  typedef vtkSmartPointer< vtkInteractorStyle               > vtkInteractorStylePointer;
  typedef vtkSmartPointer<vtkInteractorStyleTrackballCamera > vtkInteractorStyleTrackballCameraPointer;

  typedef vtkSmartPointer<vtkEventQtSlotConnect>              vtkQtConnectorPointer;


} // namespace ccipd
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdVTKForwardDeclarations_h
