


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdProstaFuseWindow.h"
#include "ccipdProstaFuseOptions.h"           // for showing options
#include "ccipdProstaFuseResults.h"           // for showing results
#include "ccipdMacros.h"
#include "ccipdVTKForwardDeclarations.h"      // vtk types
#include "ccipdVolumeTypes.h"                 // image types
#include "ccipdStudy.h"
#include "ccipdApplyTransformation.h"
#include "ccipdUpdateVTKImageSlice.h"
#include "ccipdUpdateVTKImageProp.h"
#include "ccipdUpdateVTKImageWindowLevel.h"
#include "ccipdUpdateWindowLevelSliders.h"
#include "ccipdSaveImageGUI.h"
#include "ccipdLoadVolumeGUI.h"
#include "ccipdLoadGUI.h"
#include "ccipdCreateTransformedTemplateImage.h"
#include "ccipdPreprocessMRI.h"
#include "ccipdPreprocessVolume.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkAffineTransform.h>
  #include <itkBSplineTransform.h>
  #include <itkImage.h>
  #include <itkImageBase.h>

  // Qt includes
  #include <QApplication>
  #include <qmessagebox.h>
  #include "ui_ccipdProstaFuseWindow.h"

  // vtk includes
  #include <vtkRenderer.h>
  #include <vtkRenderWindow.h>
  #include <vtkRendererCollection.h>
  #include <vtkSmartPointer.h>
  #include <vtkCamera.h>
  #include <vtkImageSlice.h>
  #include <vtkImageProperty.h>

#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interfact to ProstalignFEM
class ProstaFuseWindow::SecretData
{

public:
  SecretData(); ///< Constructor

  void ApplyStudyTransform(); /// To Apply Our Transform

  void LoadImage(
  const VolumeImageConstPointer & volume,
        QSlider                 & sliceSlider,
        QSlider                 & windowSlider,
        QSlider                 & levelSlider,
        vtkPropPointer          & imageProp,
  const vtkRenderWindowPointer  & renderWindow );

VolumeImageConstPointer // our images
  m_FixedImage, 
  m_MovingImage,
  m_MovedImage;


  vtkPropPointer
    m_FixedImageProp, //< Store the vtk prop for fixedImage.
    m_MovingImageProp; //< Store the vtk prop for movingImage.

StudyConstPointer // the study point to load in the transforms
  m_TransformStudy;

bool // which set of transforms to apply
  m_UseAffineTransform,
  m_UseElasticTransform,
  m_CropMovedImage;

const float m_SlideFactor;
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstaFuseWindow::SecretData::SecretData() :
m_FixedImage ( VolumeImagePointer() ),
m_MovingImage ( VolumeImagePointer() ),
m_MovedImage ( VolumeImagePointer() ),
m_TransformStudy( StudyPointer() ),
m_UseAffineTransform( true ),
m_UseElasticTransform( false ),
m_CropMovedImage( false ),
m_SlideFactor( 100 )
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstaFuseWindow::ProstaFuseWindow() :
ccipdNewPIMPLMacro
{

    // set up the user interface
  this->ui = new Ui_ProstaFuseWindow;
  this->ui->setupUi( this );
 
  // VTK Renderer
  const vtkRendererPointer
    fixedRenderer  = vtkRendererPointer::New(),
    movingRenderer = vtkRendererPointer::New();

  // setting background image
  const double
    backgroundR = 0,
    backgroundG = 0,
    backgroundB = 0;
  fixedRenderer ->SetBackground( backgroundR, backgroundG, backgroundB );
  movingRenderer->SetBackground( fixedRenderer->GetBackground() );

  // VTK/Qt wedded
  this->ui->m_FixedImageWidget ->GetRenderWindow()->AddRenderer( fixedRenderer  );
  this->ui->m_MovingImageWidget->GetRenderWindow()->AddRenderer( movingRenderer );

  // Set up action signals and slots
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT( SlotExit() ));

} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstaFuseWindow::~ProstaFuseWindow()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void 
ProstaFuseWindow::SecretData::ApplyStudyTransform()
{
  // re initialize the moved image
  this->m_MovedImage = NULL;

  if ( !this->m_UseAffineTransform && !this->m_UseElasticTransform )
    {
    std::cout << "No transform type selected. Unable to apply transform." << this->m_UseAffineTransform << std::endl;
    return;
    }
  if ( !this->m_MovingImage )
    {
    std::cout << "No moving image exists. Unable to apply transform." << std::endl;
    return;
    } //  ( !this->m_MovingImage )
  if ( !this->m_FixedImage )
    {
    std::cout << "No fixed image exists. Unable to apply transform." << std::endl;
    return;
    } // ( !this->m_FixedImage )

  if( !this->m_TransformStudy )
    {
    std::cout << "No transformation exists. Unable to apply transform." << std::endl;
    return;
    } // ( !this->m_TransformStudy )

  if ( this->m_UseAffineTransform )
    {
    if ( !this->m_TransformStudy->GetAffineTransform() )
      {
      std::cout << "No affine transformation exists. Unable to apply affine transform." << std::endl;
      return;
      }

  
  // get the template image
  VolumeImageBaseConstPointer templateImage;

  if(  m_CropMovedImage )
    templateImage = this->m_FixedImage;
  else
    templateImage = CreateTransformedTemplateImage(
      *(this->m_MovingImage),
      *(this->m_TransformStudy->GetAffineTransform() ) ); 

    this->m_MovedImage = ApplyTransform( 
      this->m_TransformStudy->GetAffineTransform(), 
      this->m_MovingImage, 
       templateImage,  
       false );

  if ( this->m_UseElasticTransform )
    {
      if ( !this->m_TransformStudy->GetBSplineTransform() )
      {
      std::cout << "No elastic transformation exists. Unable to apply elastic transform." << std::endl;
      return;
      }

    this->m_MovedImage = ApplyTransform( 
      this->m_TransformStudy->GetBSplineTransform(), 
      this->m_MovedImage, 
       templateImage );
    } // m_UseElasticTransform

    } // m_UseAffineTransform
} // ApplyTransform
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
void 
ProstaFuseWindow::SecretData::LoadImage(
  const VolumeImageConstPointer & volume,
        QSlider                 & sliceSlider,
        QSlider                 & windowSlider,
        QSlider                 & levelSlider,
        vtkPropPointer          & imageProp,
  const vtkRenderWindowPointer  & renderWindow )
{

  if ( volume )
    {

    // get the # of slices
    const unsigned int numberOfSlices = static_cast< unsigned int >(
      volume->GetBufferedRegion().GetSize()[ 2 ] );
    cout << "Number of slices = " << numberOfSlices << endl;

    // update the slider
    sliceSlider.setMaximum( numberOfSlices - 1 );
    sliceSlider.setValue( numberOfSlices/2 ); // initialize image in midgland    

    // update the display image
    const bool
      makeZeroTransparent = true,
      verbose             = false;
    const unsigned int
      slice = sliceSlider.value();

    UpdateVTKImageProp(
      renderWindow,
      imageProp,
      volume.GetPointer(),
      slice,
      makeZeroTransparent,
      verbose );

    if ( imageProp )
      {
      imageProp->VisibilityOn();
      imageProp->PickableOn();
      
      }
    UpdateWindowLevelSliders( volume, &windowSlider, &levelSlider, this->m_SlideFactor );

    } // volume
  else
    {
    if ( imageProp )
      {
      imageProp->VisibilityOff();
      imageProp->PickableOff();
      }
    }

  vtkRenderer * const renderer = renderWindow.GetPointer()->GetRenderers()->GetFirstRenderer();

  renderer->ResetCamera(); // view the whole scene
    // make sure the camera is correct since itk flips the y axis
  vtkCamera * const camera = renderer->GetActiveCamera();
  camera->SetViewUp( 0, -1, 0 );

  const double * const position = camera->GetPosition();
  camera->SetPosition( position[ 0 ], position[ 1 ], -position[ 2 ] );

  renderer->ResetCameraClippingRange();

  // re-render
  renderWindow->Render();
  renderWindow->Render(); // once more for good measure

} // LoadImage
//////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotOptions() 
{
    // extract the data I want to modify
  bool & useAffineTransform  = this->m_SecretData->m_UseAffineTransform;
  bool & useElasticTransform = this->m_SecretData->m_UseElasticTransform;
  bool & cropMovedImage      = this->m_SecretData->m_CropMovedImage;
  // set up my options dialog
  ProstaFuseOptions options( useAffineTransform, 
                             useElasticTransform,
                              cropMovedImage      );

  // display it
  if ( !options.exec() )
    return; // the user pressed cancel

  // update the values
  useAffineTransform  = options.GetUseAffineTransform();
  useElasticTransform = options.GetUseElasticTransform();
  cropMovedImage      = options.GetCropMovedImage();
} // SlotOptions
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotChangeFixedImageSlice( const int whichSlice )
{

  // change the slice
  UpdateVTKImageSlice( this->m_SecretData->m_FixedImageProp, whichSlice, false );

  // re-render
  this->ui->m_FixedImageWidget->GetRenderWindow()->Render();

} // SlotChangeFixedSlice
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotChangeMovingImageSlice( const int whichSlice )
{

  // change the slice
  UpdateVTKImageSlice( this->m_SecretData->m_MovingImageProp, whichSlice, false );

  // re-render
  this->ui->m_MovingImageWidget->GetRenderWindow()->Render();

} // SlotChangeMovingSlice
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotOpenFixedTRUS() 
{
  
  // try to load it
  const VolumeImageConstPointer 
    fixedImage( RescaleIntensitiesVolume( LoadVolume( this, true ) ) );
  
  if ( !fixedImage )
    return;

  // if so, assign it
  this->m_SecretData->m_FixedImage = fixedImage;

  // update the renderer
  this->m_SecretData->LoadImage( 
    fixedImage, 
    *this->ui->m_FixedSliceSlider, 
    *this->ui->m_FixedWindowSlider,
    *this->ui->m_FixedLevelSlider,
    this->m_SecretData->m_FixedImageProp,
    this->ui->m_FixedImageWidget->GetRenderWindow() );
} // SlotOpenFixedTRUS
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotOpenMovingMRI() 
{
    
  // try to load it
  const VolumeImageConstPointer  
    movingImage( RescaleIntensitiesMRI( LoadVolumeFromDirectory( this, true ) ) );
  
  if ( !movingImage )
    return;

  // if so, assign it
  this->m_SecretData->m_MovingImage = movingImage;
  this->m_SecretData->LoadImage( 
    movingImage, 
    *this->ui->m_MovingSliceSlider,
    *this->ui->m_MovingWindowSlider,
    *this->ui->m_MovingLevelSlider, 
    this->m_SecretData->m_MovingImageProp,
    this->ui->m_MovingImageWidget->GetRenderWindow() );
} // SlotOpenMovingMRI
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotSaveFixedTRUS() 
{
  const bool saved = SaveImage< VolumeImageType >( this->m_SecretData->m_FixedImage, this, true );

  if ( saved )
    cout << "Saving fixed image done." << endl;
  else
    cout << "Saving fixed image failed." << endl;

} // SlotSaveFixedTRUS
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotChangeFixedWindow() 
{
    // change it
  UpdateVTKImageWindowLevel(
    this->m_SecretData->m_FixedImageProp,
    this->ui->m_FixedWindowSlider->value()/this->m_SecretData->m_SlideFactor,
    this->ui->m_FixedLevelSlider ->value()/this->m_SecretData->m_SlideFactor );

  // re-render
  this->ui->m_FixedImageWidget->GetRenderWindow()->Render();

} // SlotSaveFixedTRUS
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotChangeFixedLevel() 
{
    // change it
  UpdateVTKImageWindowLevel(
    this->m_SecretData->m_FixedImageProp,
    this->ui->m_FixedWindowSlider->value()/this->m_SecretData->m_SlideFactor,
    this->ui->m_FixedLevelSlider ->value()/this->m_SecretData->m_SlideFactor);

  // re-render
  this->ui->m_FixedImageWidget->GetRenderWindow()->Render();

} // SlotSaveFixedTRUS
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotChangeMovingWindow() 
{
    // change it
  UpdateVTKImageWindowLevel(
    this->m_SecretData->m_MovingImageProp,
    this->ui->m_MovingWindowSlider->value()/this->m_SecretData->m_SlideFactor,
    this->ui->m_MovingLevelSlider ->value()/this->m_SecretData->m_SlideFactor );

  // re-render
  this->ui->m_MovingImageWidget->GetRenderWindow()->Render();

} // SlotSaveFixedTRUS
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotChangeMovingLevel() 
{
    // change it
  UpdateVTKImageWindowLevel(
    this->m_SecretData->m_MovingImageProp,
    this->ui->m_MovingWindowSlider->value()/this->m_SecretData->m_SlideFactor,
    this->ui->m_MovingLevelSlider ->value()/this->m_SecretData->m_SlideFactor );

  // re-render
  this->ui->m_MovingImageWidget->GetRenderWindow()->Render();

} // SlotSaveFixedTRUS
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotSaveMovingMRI() 
{

const bool saved =  
 ( this->m_SecretData->m_MovedImage ) ? // if we have a moved image save it; otherwise save the moving image
  SaveImage< VolumeImageType >( this->m_SecretData->m_MovedImage, this, true ) :
  SaveImage< VolumeImageType >( this->m_SecretData->m_MovingImage, this, true );

  if ( saved )
    cout << "Saving moved image done." << endl;
  else
    cout << "Saving moved image failed." << endl;

} // SlotSaveMovingMRI
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotLoadTransform() 
{
  // Transform will be loaded from a patient study
  cout << "Begin loading transformation." << endl;


  // try to load a patient
  const StudyConstPointer study(
    LoadObjectGUI< Study >( this, true ) );

  // if not successful, return
  if ( !study ) return;

  this->m_SecretData->m_TransformStudy = study;

  // Transform will be loaded from a patient study
  cout << "Done loading transformation." << endl;

} // SlotLoadTransform
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotApplyTransform() 
{
  std::cout << "Begin applying transformation." << std::endl;

    this->m_SecretData->ApplyStudyTransform();
  
  std::cout << "Done applying transformation." << std::endl;

  // show the results if we have created a moved image
  if ( this->m_SecretData->m_MovedImage )
    {
    ProstaFuseResults results( this->m_SecretData->m_FixedImage,
                               *this->ui->m_FixedWindowSlider,
                               *this->ui->m_FixedLevelSlider,  
                               this->m_SecretData->m_MovedImage,
                               *this->ui->m_MovingWindowSlider,
                               *this->ui->m_MovingLevelSlider,
                               *this->ui->m_MovingSliceSlider,
                               this->m_SecretData->m_SlideFactor);
    results.exec();
     }
  else
   QMessageBox::warning( this, 
      "No Transform Selected",
      "There is no transform applied to the study. Please load and select appropriate options.",
      QMessageBox::Cancel );

} // SlotApplyTransform
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotShowAbout()
{
  
  QMessageBox::about( this, tr( "About ProstaFuse" ),
    tr( "\
ProstaFuse is a research tool to apply and visualize the results from MAPPER  \
(Multi-atribute Probablistic Propstate Elastic Registration) .\n\n\
The ProstaFuse software is intended for educational, research, and informational purposes only. \
ProstaFuse, or information derived from ProstaFuse, may be used only for these purposes \
and may not under any circumstances whatsoever be used for clinical purposes.\
ProstaFuse has not been approved by the FDA and is not intended for treating or diagnosing human \
subjects, and the recipient and user will not use the ProstaFuse software for such purposes. \n\n\
Author: Rachel Sparks" ) );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseWindow::SlotExit() 
{

  this->hide();
  qApp->exit();

} // SlotExit
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd