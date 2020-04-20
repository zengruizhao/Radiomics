


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdProstalignTPS.h"
#include "ccipdLoadRGBSliceGUI.h"                 // for opening images
#include "ccipdVTKForwardDeclarations.h"          // vtk types
#include "ccipdRenderPointPicker.h"               // for setting up the render window
#include "ccipdMouseInteractorStylePointPicker.h" // how we picked the points
#include "ccipdThinPlateSplines.h"                // for registering
#include "ccipdApplyTransformation.h"             // for applying the registration
#include "ccipdProstalignResults.h"               // for displaying the results
#include "ccipdProstalignTPSOptions.h"           // for showing options

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// itk includes
#include <itkImage.h>
#include <itkTransform.h>
#include <itkRGBPixel.h>
#include <itkPointSet.h>

// Qt includes
#include <QApplication>
#include <qmessagebox.h>
#include "ui_ccipdProstalignTPS.h"

// vtk includes
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkCamera.h>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interfact to ProstalignTPS
class ProstalignTPS::SecretData
{

public:
  SecretData(); ///< Constructor

  RGBSliceConstPointer
    m_FixedImage,  ///< Fixed image.
    m_MovingImage; ///< Moving image.

  bool
    m_KeepLandmarks; ///< Retain the landmarks when loading a new image?

  /// Clear landmarks if you aren't keeping them and reload a fixed image.
  void ReloadImageWidget(
          vtkRenderWindow * const renderWindow,
    const RGBSliceType    * const image ) const;

  /// Definitely clear the landmarks and reload a fixed image.
  void ClearLandmarks(
          vtkRenderWindow * const renderWindow,
    const RGBSliceType    * const image );

  /**
   * \brief Extract landmarks from a render window.
   * 
   * If !parent, then no warnings or errors will be
   * displayed.
   */
  static LandmarkSetPointer GetLandmarks(
    vtkRenderWindow * const renderWindow,
     QWidget        * const parent );

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstalignTPS::SecretData::SecretData() :
m_KeepLandmarks( false )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstalignTPS::ProstalignTPS() :
ccipdNewPIMPLMacro
{

  // set up the user interface
  this->ui = new Ui_ProstalignTPS;
  this->ui->setupUi( this );
 
  // sphere
  typedef vtkSmartPointer< vtkSphereSource > vtkSphereSourcePointer;
  const vtkSphereSourcePointer sphereSource = vtkSphereSourcePointer::New();

  sphereSource->SetThetaResolution( 15 );
  sphereSource->SetPhiResolution  ( 15 );
  sphereSource->Update();

  vtkPolyDataMapperPointer sphereMapper = vtkPolyDataMapperPointer::New();
  sphereMapper->SetInputConnection( sphereSource->GetOutputPort() );

  const vtkActorPointer sphereActor = vtkActorPointer::New();
  sphereActor->SetMapper( sphereMapper );
 
  // VTK Renderer
  const vtkRendererPointer
    leftRenderer  = vtkRendererPointer::New(),
    rightRenderer = vtkRendererPointer::New();

  leftRenderer ->AddActor( sphereActor );
  rightRenderer->AddActor( sphereActor );

  const double
    backgroundR = 0,
    backgroundG = 0,
    backgroundB = 0;
  leftRenderer ->SetBackground( backgroundR, backgroundG, backgroundB );
  rightRenderer->SetBackground( leftRenderer->GetBackground() );

  // VTK/Qt wedded
  this->ui->m_FixedImageWidget ->GetRenderWindow()->AddRenderer( leftRenderer  );
  this->ui->m_MovingImageWidget->GetRenderWindow()->AddRenderer( rightRenderer );

  // Set up action signals and slots
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT( SlotExit() ));
 
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstalignTPS::~ProstalignTPS()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SecretData::ReloadImageWidget(
          vtkRenderWindow * const renderWindow,
    const RGBSliceType    * const image ) const
{

  // remember the landmarks
  const LandmarkSetConstPointer landmarks(
    this->GetLandmarks( renderWindow, 0 ) );

  // clear the existing renderer
  vtkRenderer * const oldRenderer = renderWindow->GetRenderers()->GetFirstRenderer();

  if ( oldRenderer )
    renderWindow->RemoveRenderer( oldRenderer );

  // set up our point picker
  const bool         verbose = true; // show output?
  const unsigned int slice   = 0; // 2D image

  if ( this->m_KeepLandmarks )
    {
    // use the initial landmarks
    RenderPointPicker( image, slice, verbose, renderWindow, landmarks );
    }
  else
    {
    // don't use any initial landmarks
    RenderPointPicker( image, slice, verbose, renderWindow );
    }

} // ReloadFixedWidget
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SecretData::ClearLandmarks(
          vtkRenderWindow * const renderWindow,
    const RGBSliceType    * const image )
{

  const bool oldKeepLandmarks = this->m_KeepLandmarks;
  this->m_KeepLandmarks = false; // we are clearing them!

  this->ReloadImageWidget( renderWindow, image );

  // turn our option back on
  this->m_KeepLandmarks = oldKeepLandmarks;

} // ClearLandmarks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SlotOpenFixedImage() 
{

  // try to load it
  const RGBSliceConstPointer fixedImage( LoadRGBSlice( this, true ) );
  
  if ( !fixedImage )
    return;

  // if so, assign it
  this->m_SecretData->m_FixedImage = fixedImage;

  // update the render window
  this->m_SecretData->ReloadImageWidget(
    this->ui->m_FixedImageWidget->GetRenderWindow(),
    fixedImage );

} // slotOpenFixedImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SlotOpenMovingImage()
{

  // try to load it
  const RGBSliceConstPointer movingImage( LoadRGBSlice( this, true ) );
  
  if ( !movingImage )
    return;

  // if so, assign it
  this->m_SecretData->m_MovingImage = movingImage;
  
  // update the render window
  this->m_SecretData->ReloadImageWidget(
    this->ui->m_MovingImageWidget->GetRenderWindow(),
    movingImage );

} // slotOpenMovingImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SlotResetFixedImage()
{

  this->m_SecretData->ClearLandmarks(
    this->ui->m_FixedImageWidget->GetRenderWindow(),
    this->m_SecretData->m_FixedImage );

} // SlotResetFixedImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SlotResetMovingImage()
{

  this->m_SecretData->ClearLandmarks(
    this->ui->m_MovingImageWidget->GetRenderWindow(),
    this->m_SecretData->m_MovingImage );

} // SlotResetMovingImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SlotRegister() 
{
  
  // first, error checking with the images
  const RGBSliceConstPointer
    & fixedImage  = this->m_SecretData->m_FixedImage,
    & movingImage = this->m_SecretData->m_MovingImage;

  if ( !fixedImage || !movingImage )
    {
    QMessageBox::warning(
      this,
      "Invalid Images",
      "No fixed or moving images. Not registering.",
      QMessageBox::Cancel );
    return;
    }

  // get the landmarks
  const LandmarkSetConstPointer
    fixedLandmarks  = this->m_SecretData->GetLandmarks(
      this->ui->m_FixedImageWidget->GetRenderWindow(), this ).GetPointer(),
    movingLandmarks = this->m_SecretData->GetLandmarks(
      this->ui->m_MovingImageWidget->GetRenderWindow(), this ).GetPointer();

  // check the number of landmarks
  const auto numberOfLandmarks = fixedLandmarks->GetNumberOfPoints();

  if ( numberOfLandmarks == 0 )
    {
    QMessageBox::warning(
      this,
      "No Landmarks",
      "You haven't selected any landmarks on the fixed image.",
      QMessageBox::Cancel );
    return;
    }

  if ( movingLandmarks->GetNumberOfPoints() != numberOfLandmarks )
    {
    QMessageBox::warning(
      this,
      "Unparied Landmarks",
      "You have selected a different number of landmarks on the moving image.",
      QMessageBox::Cancel );
    return;
    }

  // okay, register
  std::cout << "Registering:" << std::endl;

  // calculate the TPS transform
  const SliceTransformConstPointer transform(
    CalculateThinPlateSplines2D( fixedLandmarks, movingLandmarks ) );

  // apply the TPS to the moving image
  const RGBSliceConstPointer movedImage(
    ApplyTransform( transform, movingImage, fixedImage ) );

  std::cout << "Registering done." << std::endl;

  // show the results
  ProstalignResults results( fixedImage, movingImage, movedImage, transform );
  results.exec();

} // SlotRegister
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer ProstalignTPS::SecretData::GetLandmarks(
  vtkRenderWindow * const renderWindow,
   QWidget        * const parent )
{

  // get the point picker interactor styles
  typedef MouseInteractorStylePointPicker StyleType;

  const StyleType * style = dynamic_cast< StyleType * >(
    renderWindow->GetInteractor()->GetInteractorStyle() );
  
  if ( !style )
    {
    if ( parent )
      {
      QMessageBox::critical(
        parent,
        "Point Picker Style",
        "Critical error: Unable to extract point picker style!",
        QMessageBox::Cancel );
      }
    return LandmarkSetPointer();
    }

  // get the landmarks
  const LandmarkSetPointer landmarks( style->GetLandmarks() );

  if ( !landmarks )
    {
    if ( parent )
      {
      QMessageBox::critical(
        parent,
        "No Landmarks",
        "Critical error: NULL landmark pointer.",
        QMessageBox::Cancel );
      }
    return LandmarkSetPointer();
    }

  return landmarks;

} // GetLandmarks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SlotOptions() 
{

  // extract the data I want to modify
  bool & keepLandmarks = this->m_SecretData->m_KeepLandmarks;

  // set up my options dialog
  ProstalignTPSOptions options( keepLandmarks );

  // display it
  if ( !options.exec() )
    return; // the user pressed cancel

  // update the values
  keepLandmarks = options.GetKeepLandmarks();

} // SlotOptions
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignTPS::SlotExit() 
{

  this->hide();
  qApp->exit();

} // SlotExit
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
