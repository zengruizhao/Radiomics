


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdProstalignFEM.h"
#include "ccipdPatientTypes.h"               // for our loaded patient
#include "ccipdPatient.h"                    // for our loaded patient
#include "ccipdStudy.h"                      // for the fixed and moving data
#include "ccipdLoadGUI.h"                    // for loading a patient
#include "ccipdVTKForwardDeclarations.h"     // for VTK types
#include "ccipdUpdateVTKImageProp.h"         // for displaying an image
#include "ccipdUpdateVTKImageSlice.h"        // for changing the slice
#include "ccipdUpdateVTKPropTransparency.h"  // for changing transparency
#include "ccipdUpdateVTKImageWindowLevel.h"  // for changing window/level
#include "ccipdUpdateVTKImageBlinds.h"       // for changing the blinds
#include "ccipdUpdateWindowLevelSliders.h"   // for changing the sliders
#include "ccipdFEMRegistrationGUIObserver.h" // for watching progress
#include "ccipdCropImage.h"                  // for cropping ROI
#include "ccipdResizeImage.h"                // for resizing the mask
#include "ccipdIntensityStandardization.h"   // for standardizing moving to fixed
#include "ccipdSaveLinearTransformGUI.h"     // for saving the transform
#include "ccipdSaveImageGUI.h"               // for saving the moved image
#include "ccipdSaveLandmarksGUI.h"           // for saving landmarks
#include "ccipdApplyTransformation.h"        // for applying the transform to a new image
#include "ccipdFEMMaterialsDialog.h"         // for editing the materials
#include "ccipdGradientDescentOptionsGUI.h"
#include "ccipdParticleSwarmOptionsGUI.h"
#include "ccipdFEMTransformOptionsGUI.h"
#include "ccipdRegisterComboFEMImages.h"
#include "ccipdRegisterComboFEMMasks.h"
#include "ccipdLandmarksToActor.h"
#include "ccipdFEMRenderer.h"                // for rendering the FEM
#include "ccipdCalculateWarpField.h"         // for calculating the deformation field
#include "ccipdGetDirectionTransform.h"
#include "ccipdDuplicateImage.h"
#include "ccipdDisableWarningsMacro.h"

// itk includes
#include <itkAffineTransform.h>        // for affine
#include <itkVersorRigid3DTransform.h> // for rigid
#include <itkStatisticsImageFilter.h>  // for min, max, mean intensities
#include <itkRGBPixel.h>
// Qt includes
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QComboBox>
#include <QColorDialog>
#include "ui_ccipdProstalignFEM.h"

// vtk includes
#include <vtkImageSlice.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCamera.h>
#include <vtkProp.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>

// boost includes
#include <boost/foreach.hpp> // for loops

#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interfact to ProstalignFEM
class ProstalignFEM::SecretData
{

public:
  SecretData(); ///< Constructor

  /// The patient we are registering.
  /// Which we want to be sorted (in the same order as the
  /// combo boxes).
  typedef std::vector< StudyConstPointer > StudyVectorType;
  StudyVectorType
    m_Studies;

  // our studies
  StudyConstPointer
    m_FixedStudy,  ///< The study containing the fixed volume.
    m_MovingStudy, ///< The study containing the moving volume.
    m_MovedStudy;  ///< For after registering

  VolumeTransformPointer
    m_FinalTransform;
  VolumeLinearTransformPointer
    m_MovedLinearTransform;

  vtkPropPointer
    m_Image1Prop, //< Store the vtk prop for the first image.
    m_Image2Prop; //< Store the vtk prop for the second image.

  vtkActorPointer
    m_Image1Landmarks,
    m_Image2Landmarks;

  bool
    m_NeverRegistered;

  /// The default FEM material
  MaterialPointer
    m_DefaultMaterial;

  /// Non-default FEM materials
  MaskMaterialMap
    m_Materials;

  /// For rendering the FEM
  FEMRendererPointer
    m_FEMRenderer;

  typedef RegisterComboFEMImageBase    RegisterType;
  typedef RegisterType::OptimizerType  OptimizerType;
  typedef RegisterType::TransformType  TransformType;

  RegisterType::Pointer
    m_RegisterImageLinear,
    m_RegisterMaskLinear,
    m_RegisterFEM;

  /**
   * \brief Sets a given study to the study contained in the combo box's value.
   * 
   * \param[in] combo
   *   The combo box containing studies.
   * 
   * \param[out] study
   *   The study to store the output.
   * 
   * \date 2012-05-22
   */
  void SetStudy(
    const QComboBox         & combo,
          StudyConstPointer & study );

  /**
   * \brief Change which image is shown.
   */
  void ChangeImage(
    const QString                & whichImage,
          QSlider                & sliceSlider,
          QSlider                & windowSlider,
          QSlider                & levelSlider,
          vtkPropPointer         & imageProp,
    const vtkRenderWindowPointer & renderWindow );

  /**
   * \brief Change which landmarks are shown.
   */
  void ChangeLandmarks(
    const QString & whichImage,
    vtkActorPointer & landmarksActor,
    const vtkRenderWindowPointer & renderWindow );

  static const float m_SliderFactor;

  /**
   * This will output
   * the given volume for
   * one of the image props.
   * \returns NULL on an error
   */
  VolumeImageConstPointer GetVolume(
    const QString & whichImage ) const;

  /**
   * This will output the landmarks
   * for one of the images.
   * \returns null pointer on an error.
   */
  LandmarkSetConstPointer GetLandmarks(
    const QString & whichImage ) const;

  /**
   * \brief Update what type of blinds to show.
   */
  void ChangeImageBlinds(
    const QComboBox       & blindsCombo,
          QSpinBox        & blindsSpin,
    const VolumeImageType & image,
    const vtkPropPointer  & imageProp );

  /// If !faceColor, it will ignore opacity and change the edge color
  void ChangeColorAndOpacity(
    const bool                    faceColor,
    const FEMRenderer::WhichActor actor,
          QAbstractSlider * const sliderToUpdate = 0 );

  /// Extract all the properties and update the FEM renderer
  void UpdateFEMRendererProperties(
    const Ui_ProstalignFEM & ui );

  /**
   * Get (in this order):
   *   - The user-selected mask object
   *   - A random mask from moving study
   *   - An empty string if none of the above exists.
   * 
   */
  MaskObjectType GetFEMMaskObject(
    const Ui_ProstalignFEM & ui ) const;

  /**
   * Get the mask associated with the mask object
   * or NULL if none exists.
   */
  VolumeMaskImageConstPointer GetFEMMask(
    const Ui_ProstalignFEM & ui ) const;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
const float ProstalignFEM::SecretData::m_SliderFactor = 1000.0f;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstalignFEM::SecretData::SecretData() :
m_FixedStudy ( StudyPointer() ),
m_MovingStudy( StudyPointer() ),
m_MovedStudy ( StudyPointer() ),
m_NeverRegistered( true ),
m_FEMRenderer( new FEMRenderer ),
m_RegisterImageLinear( RegisterComboFEMImages::New() ),
m_RegisterMaskLinear( RegisterComboFEMMasks::New() ),
m_RegisterFEM   ( RegisterComboFEMImages::New() )
{
  // set some default registration options for linear
  OptimizerType::Pointer optimizerLinear = std::dynamic_pointer_cast< OptimizerType >
    ( m_RegisterImageLinear->GetOptimizer() );

  optimizerLinear->SetWhichParticleSwarm(
    OptimizerType/*::WhichParticleSwarm*/::InitializationBiased );

  // set some default registration options for FEM
  OptimizerType::Pointer optimizerFEM = std::dynamic_pointer_cast< OptimizerType >
    ( m_RegisterFEM->GetOptimizer() );
  optimizerFEM->SetWhichParticleSwarm(
    OptimizerType/*::WhichParticleSwarm*/::MultiThreaded );
    
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstalignFEM::ProstalignFEM() :
ccipdNewPIMPLMacro
{

  // set up the user interface
  this->ui = new Ui_ProstalignFEM;
  this->ui->setupUi( this );
  
  // VTK Renderer
  const vtkRendererPointer
    renderer = vtkRendererPointer::New();

  const double
    backgroundR = 0,
    backgroundG = 0,
    backgroundB = 0;
  renderer->SetBackground( backgroundR, backgroundG, backgroundB );

  // create an interactor style
  typedef vtkSmartPointer< vtkInteractorStyle > vtkInteractorStylePointer;
  const vtkInteractorStylePointer interactorStyle =
    vtkSmartPointer<
      vtkInteractorStyleTrackballCamera
      //vtkInteractorStyleImage
    >::New();

  // get the render window
  const vtkRenderWindowPointer renderWindow =
    this->ui->m_OutputVTKWidget->GetRenderWindow();

  // make sure we have an interactor
  vtkRenderWindowInteractorPointer
    interactor = renderWindow->GetInteractor();

  if ( !interactor )
    {
    interactor = vtkRenderWindowInteractorPointer::New();
    interactor->SetRenderWindow( renderWindow );
    } // !interactor

  // update the style
  renderWindow->GetInteractor()->SetInteractorStyle( interactorStyle );

  // add the renderer
  renderWindow->AddRenderer( renderer );

  renderWindow->Render();

  // set up the custom signals/slots
  /*
  QWidget * const saveButton = this->ui->m_SaveButton;
  saveButton->setContextMenuPolicy(Qt::CustomContextMenu);
  connect( saveButton, SIGNAL( customContextMenuRequested( const QPoint & )),
    this, SLOT( SlotSaveResults( const QPoint & ) ) );
  */

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstalignFEM::~ProstalignFEM()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SecretData::SetStudy(
  const QComboBox         & combo,
        StudyConstPointer & study )
{

  // extract the current index
  const std::size_t currentIndex = combo.currentIndex();
  
  // extract the data
  const StudyVectorType & studies = this->m_Studies;

  // make sure we have a valid index
  if ( currentIndex >= studies.size() )
    return;

  // set the study of that index
  study = studies[ currentIndex ];

} // SetStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage1( const QString & value )
{

  // extract the data
  QSlider
    & sliceSlider  = *this->ui->m_Image1SliceSlider,
    & windowSlider = *this->ui->m_Image1WindowSlider,
    & levelSlider  = *this->ui->m_Image1LevelSlider;
  vtkPropPointer & imageProp =
    this->m_SecretData->m_Image1Prop;
  vtkActorPointer & landmarksActor =
    this->m_SecretData->m_Image1Landmarks;
  const vtkRenderWindowPointer renderWindow =
    this->ui->m_OutputVTKWidget->GetRenderWindow();

  // change the image
  this->m_SecretData->ChangeImage(
    value,
    sliceSlider,
    windowSlider,
    levelSlider,
    imageProp,
    renderWindow );

  this->SlotChangeImage1WindowLevel();
  this->SlotChangeImage1Blinds();

  // update the landmarks
  this->m_SecretData->ChangeLandmarks(
    value, landmarksActor, renderWindow );

  // update the colors
  if ( landmarksActor )
    {
    landmarksActor->GetProperty()->SetColor( 1, 0, 0 );
    }
  
  renderWindow->Render();


} // SlotChangeImage1
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage2( const QString & value )
{

  // extract the data
  QSlider
    & sliceSlider  = *this->ui->m_Image2SliceSlider,
    & windowSlider = *this->ui->m_Image2WindowSlider,
    & levelSlider  = *this->ui->m_Image2LevelSlider;
  vtkPropPointer & imageProp =
    this->m_SecretData->m_Image2Prop;
  vtkActorPointer & landmarksActor =
    this->m_SecretData->m_Image2Landmarks;
  const vtkRenderWindowPointer renderWindow =
    this->ui->m_OutputVTKWidget->GetRenderWindow();

  // change the image
  this->m_SecretData->ChangeImage(
    value,
    sliceSlider,
    windowSlider,
    levelSlider,
    imageProp,
    renderWindow );
  
  this->SlotChangeImage2WindowLevel();
  this->SlotChangeImage2Blinds();

  // update the landmarks
  this->m_SecretData->ChangeLandmarks(
    value, landmarksActor, renderWindow );

  // update the colors
  if ( landmarksActor )
    {
    landmarksActor->GetProperty()->SetColor( 0, 0, 1 );
    }
  
  renderWindow->Render();

} // SlotChangeImage2
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage1()
{
  return this->SlotChangeImage1( this->ui->m_Image1Combo->currentText() );
} // SlotChangeImage1
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage2()
{
  return this->SlotChangeImage2( this->ui->m_Image2Combo->currentText() );
} // SlotChangeImage1
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer ProstalignFEM::SecretData::GetVolume(
    const QString & whichImage ) const
{

  // this will store the volume
  VolumeImageConstPointer volume( 0 );

  if ( whichImage.compare( "(None)" ) == 0 )
    {
    // just hide it
    }
  else if ( whichImage.compare( "Fixed Image" ) == 0 )
    {

    const StudyConstPointer fixedStudy =
      this->m_FixedStudy;
    if ( fixedStudy )
      volume = fixedStudy->GetVolume();

    } // fixed
  else if ( whichImage.compare( "Moving Image" ) == 0 )
    {

    const StudyConstPointer movingStudy =
      this->m_MovingStudy;
    if ( movingStudy )
      volume = movingStudy->GetVolume();

    } // moving
  else if ( whichImage.compare( "Moved Image" ) == 0 )
    {

    const StudyConstPointer movedStudy =
      this->m_MovedStudy;
    if ( movedStudy )
      volume = movedStudy->GetVolume();

    } // moved

  // output
  return volume;

} // GetVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetConstPointer ProstalignFEM::SecretData::GetLandmarks(
    const QString & whichImage ) const
{

  // this will store the volume
  LandmarkSetConstPointer landmarks;

  if ( whichImage.compare( "(None)" ) == 0 )
    {
    // just hide it
    }
  else if ( whichImage.compare( "Fixed Image" ) == 0 )
    {

    const StudyConstPointer fixedStudy =
      this->m_FixedStudy;
    if ( fixedStudy )
      landmarks = fixedStudy->GetLandmarkSet();

    } // fixed
  else if ( whichImage.compare( "Moving Image" ) == 0 )
    {

    const StudyConstPointer movingStudy =
      this->m_MovingStudy;
    if ( movingStudy )
      landmarks = movingStudy->GetLandmarkSet();

    } // moving
  else if ( whichImage.compare( "Moved Image" ) == 0 )
    {

    const StudyConstPointer movedStudy =
      this->m_MovedStudy;
    if ( movedStudy )
      landmarks = movedStudy->GetLandmarkSet();

    } // moved

  // output
  return landmarks;

} // GetLandmarks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SecretData::ChangeImage(
  const QString                & whichImage,
        QSlider                & sliceSlider,
        QSlider                & windowSlider,
        QSlider                & levelSlider,
        vtkPropPointer         & imageProp,
  const vtkRenderWindowPointer & renderWindow )
{

  // extract the volume
  const VolumeImageConstPointer volume =
    this->GetVolume( whichImage );

  if ( volume )
    {

    // update the # of slices

    // get the # of slices
    const unsigned int numberOfSlices = static_cast< unsigned int >(
      volume->GetBufferedRegion().GetSize()[ 2 ] );
    cout << "Number of slices = " << numberOfSlices << endl;

    // update the slider
    sliceSlider.setMaximum( numberOfSlices - 1 );

    // update the window/level sliders
    UpdateWindowLevelSliders(
      volume, &windowSlider, &levelSlider, this->m_SliderFactor );

    // update the display image 
    const bool
      makeZeroTransparent = true,
      verbose             = true;
    const unsigned int
      slice = sliceSlider.value();
    /*
    const VolumeImagePointer volumeWithoutDirection =
      DuplicateImage( volume.GetPointer() );
    auto direction = volumeWithoutDirection->GetDirection();
    direction.SetIdentity();
    volumeWithoutDirection->SetDirection( direction );
    */


    UpdateVTKImageProp(renderWindow, imageProp,
      volume,
      //volumeWithoutDirection,
      slice, makeZeroTransparent, verbose );

    if ( imageProp )
      {
      imageProp->VisibilityOn();
      imageProp->PickableOn();
      
      }
    
    } // volume
  else
    {
    if ( imageProp )
      {
      imageProp->VisibilityOff();
      imageProp->PickableOff();
      }
    }

  // re-render
  renderWindow->Render();
  renderWindow->Render(); // once more for good measure

} // ChangeImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SecretData::ChangeLandmarks(
  const QString                & whichImage,
        vtkActorPointer        & landmarksActor,
  const vtkRenderWindowPointer & renderWindow )
{

  if ( !renderWindow ) return;

  // remove any existing actor from the scene
  const vtkRendererPointer
    renderer = renderWindow->GetRenderers()->GetFirstRenderer();
  if ( !renderer ) return;

  renderer->RemoveActor( landmarksActor );

  // extract the landmarks
  LandmarkSetConstPointer landmarks =
    this->GetLandmarks( whichImage );

  // we need to remove the direction component
  // before displaying the landmark
  const VolumeImageConstPointer volume = this->GetVolume( whichImage );
  if ( volume && landmarks )
    {

    // get the direction as an affine transform
    const auto direction = volume->GetDirection();
    const VolumeAffineTransformConstPointer
      directionTransform( GetDirectionTransform( *volume ) );

    cout << "Direction = " << direction << endl;    
    cout << "Direction transform = " << directionTransform << endl;

    // apply the transform
    landmarks = ApplyTransform(
      directionTransform, landmarks );

    } // volume
    
  // update the actor
  landmarksActor = GenerateActorFromLandmarks( landmarks );

  if ( landmarksActor )
    {
    landmarksActor->GetProperty()->SetOpacity( 0.50 );
    }

  renderer->AddActor( landmarksActor );

  // rerender
  renderWindow->Render();

} // ChangeLandmarks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeFixedStudy( const int itkNotUsed( whichIndex ) )
{

  // try to update the fixed study
  StudyConstPointer & fixedStudy = this->m_SecretData->m_FixedStudy;

  this->m_SecretData->SetStudy(
    *this->ui->m_FixedStudyCombo,
    fixedStudy );

  if ( this->m_SecretData->m_NeverRegistered )
    {

    // the "moved" image will be
    // in the same reference frame
    // as the "fixed" image
    this->m_SecretData->m_MovedStudy = fixedStudy;

    } // never registered

  // now, update the image itself by calling the
  // combo box which selects the images
  this->SlotChangeImage1();
  this->SlotChangeImage2();

} // SlotChangeFixedStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeMovingStudy( const int itkNotUsed( whichIndex ) )
{

  // try to update the fixed study
  StudyConstPointer & movingStudy = this->m_SecretData->m_MovingStudy;

  this->m_SecretData->SetStudy(
    *this->ui->m_MovingStudyCombo,
    movingStudy );
  
  // now, update the image itself by calling the
  // combo box which selects the images
  this->SlotChangeImage1();
  this->SlotChangeImage2();

} // SlotChangeMovingStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage1Transparency( const int opacity )
{

  // change the transparency
  const bool verbose = false;
  UpdateVTKPropTransparency(
    this->m_SecretData->m_Image1Prop,
    static_cast< unsigned int >( opacity ),
    verbose );

  // re-render
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotChangeImage1Transparency
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage2Transparency( const int opacity )
{

  // change the transparency
  const bool verbose = false;
  UpdateVTKPropTransparency(
    this->m_SecretData->m_Image2Prop,
    static_cast< unsigned int >( opacity ),
    verbose );

  // re-render
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotChangeImage2Transparency
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage1WindowLevel()
{

  const float sliderFactor =
    this->m_SecretData->m_SliderFactor;

  // change it
  UpdateVTKImageWindowLevel(
    this->m_SecretData->m_Image1Prop,
    this->ui->m_Image1WindowSlider->value() / sliderFactor,
    this->ui->m_Image1LevelSlider ->value() / sliderFactor );

  // re-render
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotChangeImage1WindowLevel
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage2WindowLevel()
{
  
  const float sliderFactor =
    this->m_SecretData->m_SliderFactor;

  // change it
  UpdateVTKImageWindowLevel(
    this->m_SecretData->m_Image2Prop,
    this->ui->m_Image2WindowSlider->value() / sliderFactor,
    this->ui->m_Image2LevelSlider ->value() / sliderFactor );

  // re-render
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotChangeImage2WindowLevel
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage1Slice( const int whichSlice )
{

  // change the slice
  UpdateVTKImageSlice( this->m_SecretData->m_Image1Prop, whichSlice, true );

  // re-render
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotChangeFixedSlice
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage2Slice( const int whichSlice )
{

  // change the slice
  UpdateVTKImageSlice( this->m_SecretData->m_Image2Prop, whichSlice, true );

  // re-render
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotChangeMovingSlice
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SecretData::ChangeImageBlinds(
  const QComboBox       & blindsCombo,
        QSpinBox        & blindsSpin,
  const VolumeImageType & image,
  const vtkPropPointer  & imageProp )
{
  
  if ( !imageProp ) return; // error checking

  // extract the type of blinds as a string
  const QString & blindsValue = blindsCombo.currentText();

  // which type of blinds are we showing?
  const bool
    hasCheckerboard = 
      blindsValue.compare( tr( "Checkerboard Blinds" ) ) == 0,
    hasHorizontalBlinds = hasCheckerboard || 
      blindsValue.compare( tr( "Horizontal Blinds" ) ) == 0,
    hasVerticalBlinds = hasCheckerboard ||
      blindsValue.compare( tr( "Vertical Blinds" ) ) == 0;

  // make sure the spin box is enabled
  // if we are showing some blinds
  blindsSpin.setEnabled( hasHorizontalBlinds || hasVerticalBlinds );

  // how many blinds should we show?
  const unsigned int
    numberOfBlinds   = blindsSpin.value(),
    horizontalBlinds = hasHorizontalBlinds ? numberOfBlinds : 0,
    verticalBlinds   = hasVerticalBlinds   ? numberOfBlinds : 0;

  // update
  UpdateVTKImageBlinds( image, *imageProp, horizontalBlinds, verticalBlinds );

} // ChangeImageBlinds
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage1Blinds()
{

  // get the volume
  const VolumeImageConstPointer volume =
    this->m_SecretData->GetVolume(
      this->ui->m_Image1Combo->currentText() );

  if ( !volume ) return; // error checking

  // change the blinds of image 1
  this->m_SecretData->ChangeImageBlinds(
    *this->ui->m_Image1BlindsCombo,
    *this->ui->m_Image1BlindsSpin,
    *volume,
    this->m_SecretData->m_Image1Prop );

  // re-render
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotChangeImage1Blinds
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeImage2Blinds()
{

  // get the volume
  const VolumeImageConstPointer volume =
    this->m_SecretData->GetVolume(
      this->ui->m_Image2Combo->currentText() );

  if ( !volume ) return; // error checking
  
  // change the blinds of image 1
  this->m_SecretData->ChangeImageBlinds(
    *this->ui->m_Image2BlindsCombo,
    *this->ui->m_Image2BlindsSpin,
    *volume,
    this->m_SecretData->m_Image2Prop );

  // re-render
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotChangeImage2Blinds
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotRegister() 
{
  
  /*const*/ bool verbose = true;
  
  // extract the studies
  const StudyConstPointer
    fixedStudy  = this->m_SecretData->m_FixedStudy,
    movingStudy = this->m_SecretData->m_MovingStudy;

  // make sure we have the study
  if ( !fixedStudy )
    {
    QMessageBox::warning(
      this,
      "Invalid fixed study",
      "No fixed study. Not registering.",
      QMessageBox::Cancel );
    return;
    }

  if ( !movingStudy )
    {
    QMessageBox::warning(
      this,
      "Invalid moving study",
      "No moving study. Not registering.",
      QMessageBox::Cancel );
    return;
    }

  // process the volume
  VolumeImageConstPointer
    fixedVolume  = fixedStudy ->GetVolume(),
    movingVolume = movingStudy->GetVolume();

  if ( this->ui->m_StandardizeCheck->isChecked() )
    {
      movingVolume = StandardizeImage(const_cast<VolumeImageType*>(movingVolume.GetPointer()), const_cast<VolumeImageType*>(fixedVolume.GetPointer()) );
    } // standardize

  // we crop the volume to get a new
  // fixed study
  const double
    cropStartX = this->ui->m_FromStartCornerXSpin->value(),
    cropEndX   = this->ui->m_FromEndCornerXSpin  ->value(),
    cropStartY = this->ui->m_FromStartCornerYSpin->value(),
    cropEndY   = this->ui->m_FromEndCornerYSpin  ->value(),
    cropStartZ = this->ui->m_FromStartCornerZSpin->value(),
    cropEndZ   = this->ui->m_FromEndCornerZSpin  ->value();

  fixedVolume = CropVolume(
    fixedStudy->GetVolume(),
    cropStartX, cropEndX,
    cropStartY, cropEndY,
    cropStartZ, cropEndZ );

  // create new studies to pass in
  const StudyPointer
    newFixedStudy  = Study::New( *fixedStudy  ),
    newMovingStudy = Study::New( *movingStudy );

  newFixedStudy ->SetVolume( fixedVolume  );
  newMovingStudy->SetVolume( movingVolume );
  
  newFixedStudy ->SetLandmarkSet(
    fixedStudy ->GetLandmarkSet() );
  newMovingStudy->SetLandmarkSet(
    movingStudy->GetLandmarkSet() );

  // loop through all the masks
  BOOST_FOREACH( const MaskObjectType & whichObject, fixedStudy->GetMaskObjects() )
    {
  // set the masks
  const VolumeMaskImageConstPointer // resize the fixed mask
    fixedMask( ResizeMask(
      fixedStudy->GetMask( whichObject ), fixedVolume.GetPointer() ) );
  newFixedStudy ->SetMask( fixedMask, whichObject );
    } // for each fixed mask

  
  BOOST_FOREACH( const MaskObjectType & whichObject, movingStudy->GetMaskObjects() )
    {
    // set the masks
  newMovingStudy->SetMask( movingStudy->GetMask( whichObject ), whichObject );
    } // for each fixed mask

  // prepare the moved prop
  if ( verbose ) cout << "Preparing moved prop:" << endl;

  // extract data
  const vtkRenderWindowPointer renderWindow =
    this->ui->m_OutputVTKWidget->GetRenderWindow();

  vtkPropPointer defaultProp;
  vtkPropPointer & movedProp =
    this->ui->m_Image1Combo->currentText().compare( "Moved Image" ) == 0 ?
      this->m_SecretData->m_Image1Prop :
    this->ui->m_Image2Combo->currentText().compare( "Moved Image" ) == 0 ?
      this->m_SecretData->m_Image2Prop :
      defaultProp; // by default, no prop

  const bool makeZeroTransparent = true;

  if ( !movedProp )
    {
    UpdateVTKImageProp(
      renderWindow,
      movedProp,
      movingStudy->GetVolume(),
      0,
      makeZeroTransparent,
      verbose );

    } // movedProp

  // make it visible
  movedProp->VisibilityOn();
  renderWindow->Render();
  renderWindow->Render();

  if ( verbose ) cout << "Preparing moved prop done." << endl;

  // set up the observer
  if ( verbose ) cout << "Setting up observer:" << endl;

  // create an observer
  // to watch progress
  const FEMRegistrationGUIObserver::Pointer
    observer = FEMRegistrationGUIObserver::New();

  float displayPercent = static_cast< float >(
    this->ui->m_AffineDisplayProbabilitySpin->value() );

  observer->SetMakeZeroTransparent( makeZeroTransparent );
  observer->SetImagePropToUpdate  ( movedProp    );
  observer->SetMovingImage        ( movingVolume );
  observer->SetMovedTemplateImage ( fixedVolume.GetPointer() );
  observer->SetProbabilityOfOutput(displayPercent );
  observer->SetRenderWindow( renderWindow );

  if ( verbose ) cout << "Setting up observer done." << endl;

  // delete the old renderer's actors
  if ( verbose ) cout << "Removing existing FEM actors:" << endl;
  if ( this->m_SecretData->m_FEMRenderer )
    this->m_SecretData->m_FEMRenderer->RemoveActors();
  renderWindow->Render();
  if ( verbose ) cout << "Removing existing FEM actors done." << endl;

  if ( verbose ) cout << "Setting up FEM renderer:" << endl;

  // store the new FEM renderer used in the observer
  this->m_SecretData->m_FEMRenderer = observer->GetFEMRenderer();

  // update
  this->m_SecretData->UpdateFEMRendererProperties( *this->ui );

  if ( verbose ) cout << "Setting up FEM renderer done." << endl;

  // okay so we first try to use the mask that
  // the user selected

  // see what masks they have in common
  std::unordered_set< std::string > masksInCommon;
  BOOST_FOREACH( const MaskObjectType & fixedObject, newFixedStudy->GetMaskObjects() )
    {
    if ( !fixedObject.empty() && newMovingStudy->HasMask( fixedObject ) )
      masksInCommon.insert( fixedObject );
    }

  MaskObjectType whichObject;

  // set up the main registerer
  typedef SecretData::RegisterType RegisterType;
  RegisterType::Pointer registerer;

  // if none, we can't register masks!
  if ( !masksInCommon.empty() )
    {
    const MaskObjectType whichObjectFromGUI =
      this->ui->m_MaskObjectText->text().toStdString();
    if( whichObjectFromGUI.empty() )
      {
      whichObject = *masksInCommon.cbegin();
      cout << "Warning: no object set by user. " <<
      "Using object " << whichObject << "." << endl;
      }
    else
      {
      // see if both studies have that gui-selected mask object
      if ( masksInCommon.find( whichObjectFromGUI ) != masksInCommon.cend() )
        {
        whichObject = whichObjectFromGUI;
        }
      else
        {
        whichObject = *masksInCommon.cbegin();
        cout << "Warning: fixed and moving studies do not both have object " <<
        whichObjectFromGUI << ". Using object " << whichObject << "." << endl;
        }
      }
    } // masks in common
  else
    {
    cerr << "Error: No masks in common between studies! Ignoring masks." << endl;
    }

  // set the type of registration to perform
  const QString whatToRegisterString(
    this->ui->m_WhatToRegisterCombo->currentText() );

  if ( whatToRegisterString.compare( QString( "Register Masks" ) ) == 0 )
    {
    // set mask-specific parameters
    const RegisterComboFEMMasks::Pointer
      registererMasks( 
        std::dynamic_pointer_cast< RegisterComboFEMMasks> ( 
          this->m_SecretData->m_RegisterMaskLinear ) 
      );

    if ( !registererMasks )
      {
      cerr << "Error: No registerer!" << endl;
      return;
      }

    // set all the study parameters
    registererMasks->SetObserver       ( observer.GetPointer() );
    registererMasks->SetFixedStudy     ( newFixedStudy  );
    registererMasks->SetMovingStudy    ( movingStudy    );
    
    registererMasks->SetWhichFixedMask ( whichObject    );
    registererMasks->SetWhichMovingMask( whichObject    );
    registererMasks->SetWhichMovedMask ( whichObject    );

    registererMasks->SetCropMovedOutput( false          );
    registererMasks->SetDisplayPercent ( displayPercent );

    if(!registererMasks->HasFixedMask() || !registererMasks->HasMovingMask() )
      {
      cerr << "Error: No masks!" << endl;
      return;
      }

    // update the minimum % of pixels in the mask to be counted
    RegisterComboFEMMasks::MetricBasePointer metric = RegisterComboFEMMasks::MetricBaseType::New(); 
    metric->SetMinimumPercentOfPixelsCounted(
      static_cast< float >( this->ui->m_MinPercentPixelsSpin->value() ) );
    registererMasks->SetMetric( metric );

    // the masks use the particle swarm optimizer
    typedef RegisterComboFEMMasks::OptimizerType OptimizerType;
    OptimizerType::Pointer optimizer = std::dynamic_pointer_cast< OptimizerType > 
      ( registererMasks->GetOptimizer() );

    optimizer->SetWhichOptimizer(
      OptimizerType::/*WhichOptimizer::*/ParticleSwarmOptimizer );

    // output what we will use to register
    registerer = registererMasks->Clone();

    } // Register Masks
  else // Register Images
    {
    const RegisterComboFEMImages::Pointer
      registererImages( 
        std::dynamic_pointer_cast< RegisterComboFEMImages >( 
          this->m_SecretData->m_RegisterImageLinear ) 
      );

    if ( !registererImages )      
      {
      cerr << "Error: No registerer!" << endl;
      return;
      }

    // set all the study parameters
    registererImages->SetObserver       ( observer.GetPointer() );
    registererImages->SetFixedStudy     ( newFixedStudy  );
    registererImages->SetMovingStudy    ( movingStudy    );
    
    registererImages->SetWhichFixedMask ( whichObject    );
    registererImages->SetWhichMovingMask( whichObject    );
    registererImages->SetWhichMovedMask ( whichObject    );

    registererImages->SetCropMovedOutput( false          );
    registererImages->SetDisplayPercent ( displayPercent );

    // the images use the gradient descent optimizer
    typedef RegisterComboFEMImages::OptimizerType OptimizerType;
    OptimizerType::Pointer optimizer = std::dynamic_pointer_cast< OptimizerType > 
      ( registererImages->GetOptimizer() );

     // the images use the gradient descent optimizer
    optimizer->SetWhichOptimizer(
      OptimizerType::/*WhichOptimizer::*/GradientDescentOptimizer );
    
    // set the metric-specific properties
    RegisterComboFEMImages::MetricPointer metric = 
      std::dynamic_pointer_cast< RegisterComboFEMImages::MetricType > ( registererImages->GetMetric() );

    metric->SetWhichMetric(
      RegisterComboFEMImages::MetricType/*::WhichMetric*/::MutualInformationMetric );


    metric->SetHistogramBins( this->ui->m_HistogramBinsSpin->value() );

    if ( whatToRegisterString.compare( QString( "Register Images" ) ) == 0 )
      {
      registererImages->SetCropImagesByMasks( false );
      }
    else if ( whatToRegisterString.compare(
      QString( "Register Images Cropped by Masks" ) ) == 0 )
      {
       
      registererImages->SetCropImagesByMasks( true );
      if ( !registererImages->HasFixedMask() ||
           !registererImages->HasMovingMask() )
        {
        cerr << "Error: No masks!" << endl;
        return;
        }
      }
    else
      {
      cerr << "Error: Unknown registration type!" << endl;
      return;
      }

    // we will use the images
    registerer = registererImages->Clone();

    } // Register Images

  // prepare the rigid registration
 typedef  RegisterType::TransformType  TransformType;
 TransformType::Pointer transform( 
   std::dynamic_pointer_cast< TransformType >( 
     registerer->GetTransform() ) 
   );
 
 // outside of DoRigidCheck to make sure we have a transform type set up to retrieve final transform
 transform->SetWhichTransform( TransformType/*::WhichTransform*/::RigidTransform );

  if ( this->ui->m_DoRigidCheck->isChecked() )
    {  
    registerer->Register();

    transform->SuperclassLinear::SetInitialTransform(
      dynamic_cast< const TransformType::LinearTransformType * >(
      registerer->GetFinalTransform().GetPointer() ) );
    } // do rigid


  if ( this->ui->m_DoAffineCheck->isChecked() )
    {  
    transform->SetWhichTransform( TransformType::/*WhichTransform::*/AffineTransform );
    registerer->Register();
    }

  // assign the output
 StudyConstPointer movedStudy = registerer->GetMovedStudy();

  if( !movedStudy )
    {
    cout << "No moved study exists! Using moving study instead." << endl;
    movedStudy = ccipd::Study::ConstNew( *newMovingStudy );
    }

  this->m_SecretData->m_MovedStudy = movedStudy;

  this->m_SecretData->m_FinalTransform =
    registerer->GetFinalTransform().GetPointer();

  this->m_SecretData->m_MovedLinearTransform =
    dynamic_cast< VolumeLinearTransformType * >(
      this->m_SecretData->m_FinalTransform.GetPointer() );
      
  // we've registered at least once
  this->m_SecretData->m_NeverRegistered = false;

  // make sure these are updated
  this->SlotChangeImage1();
  this->SlotChangeImage2();

  // do the FEM next
  const bool doFEM = this->ui->m_DoFEMCheck->isChecked();
  if ( doFEM )
    {

    // Set the fixed and moved studies  
    registerer->SetFixedStudy( newFixedStudy  );
    // the new "moving" study is the linearly "moved" study
    registerer->SetMovingStudy( movedStudy );

    // Set our FEM-specific parameters-- transform and optimizer       
    TransformType::Pointer transform( 
      std::dynamic_pointer_cast< TransformType >( 
        this->m_SecretData->m_RegisterFEM->GetTransform() ) 
      );
    transform->SetMaterials      ( this->m_SecretData->m_Materials       );
    transform->SetDefaultMaterial( this->m_SecretData->m_DefaultMaterial );
    
    registerer->SetTransform( transform );


    // the FEM uses the particle swarm optimizer
    typedef RegisterType::OptimizerType  OptimizerType;
    OptimizerType::Pointer
      optimizer = std::dynamic_pointer_cast< OptimizerType > ( this->m_SecretData->m_RegisterFEM->GetOptimizer() );
    optimizer->SetWhichOptimizer(
      OptimizerType::/*WhichOptimizer::*/ParticleSwarmOptimizer );
    optimizer->SetTransform( transform );

    registerer->SetOptimizer( optimizer ); 

    // update the observer with how frequently to update
    displayPercent = static_cast< float >(
      this->ui->m_FEMDisplayProbabilitySpin->value() );
    registerer->SetDisplayPercent( displayPercent );

    // update the moving image of the observer
    observer->SetMovingImage( movedStudy->GetVolume() );

    // register
    registerer->Register();

    // output
    this->m_SecretData->m_MovedStudy =
      registerer->GetMovedStudy();

    this->m_SecretData->m_FinalTransform =
      registerer->Superclass::Superclass::GetFinalTransform().GetPointer();
    
    // This overwrites the register FEM with
    // the linear registration pointer, which
    // is incorrect.

    //this->m_SecretData->m_RegisterFEM = registerer;

    } // doFEM
  
  // make sure these are updated
  this->SlotChangeImage1();
  this->SlotChangeImage2();
  
  cout << "Registration done." << endl;

} // SlotRegister
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotLoadPatient() 
{

  // try to load a patient
  const PatientConstPointer patient(
    LoadObjectGUI< Patient >( this, true ) );

  // if not successful, return
  if ( !patient ) return;

  // go through each study
  const StudyCollectionType currentStudies =
    patient->GetStudiesWithVolume();

  // we need 2 studies
  if ( currentStudies .size() < 2 )
    {

    QMessageBox::warning(
      this,
      tr( "Invalid Patient" ),
      tr( "The patient must have at least 2 studies to register." ),
      QMessageBox::Cancel );

    return;

    } // not enough studies

  // prepare to assign it
  SecretData::StudyVectorType & assignedStudies = this->m_SecretData->m_Studies;
  assignedStudies.clear();

  // get the fixed and moving image list
  QComboBox
    * const fixedCombo  = this->ui->m_FixedStudyCombo,
    * const movingCombo = this->ui->m_MovingStudyCombo;

  fixedCombo ->clear();
  movingCombo->clear();

  // we are loading a new patient we haven't registered yet
  this->m_SecretData->m_NeverRegistered = true;

  // add each study to the list
  BOOST_FOREACH( const StudyPointer & study, currentStudies )
    {
    
    const VolumeImageConstPointer volume = study->GetVolume();
    if ( study->GetNumberOfMasks() == 0 )
      {

      cout << "Creating default mask:" << endl;

      // if no mask exists, we will create one the same size
      // as the volume
      const VolumeMaskImagePointer mask = VolumeMaskImageType::New();
      mask->SetOrigin   ( volume->GetOrigin        () );
      mask->SetSpacing  ( volume->GetSpacing       () );
      mask->SetDirection( volume->GetDirection     () );
      mask->SetRegions  ( volume->GetBufferedRegion() );

      // fill with a non-zero value to indicate valid pixels
      mask->Allocate();
      mask->FillBuffer( 1 );

      study->SetMask( mask );

      cout << "Creating default mask done." << endl;

      } // no masks

    // get the description of the study
    const QString description = tr( study->GetDescription().c_str() );

    // each item will have the index
    // of the collection index
    fixedCombo ->addItem( description );
    movingCombo->addItem( description );

    // add it to our collection
    assignedStudies.push_back( study );

    } // for each study

  // select one
  fixedCombo ->setCurrentIndex( 0 );
  movingCombo->setCurrentIndex( 1 );

  // make sure the slots are triggered
  this->SlotChangeFixedStudy ( 0 );
  this->SlotChangeMovingStudy( 1 );

  // make sure these are updated
  this->SlotChangeImage1();
  this->SlotChangeImage2();
  this->SlotChangeImage1WindowLevel();
  this->SlotChangeImage2WindowLevel();

} // SlotLoadPatient
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotResetGUI() 
{

  // extract out render window
  const vtkRenderWindowPointer renderWindow =
    this->ui->m_OutputVTKWidget->GetRenderWindow();

  // reset the camera
  renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();

  // re-render
  renderWindow->Render();

} // SlotResetGUI
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotSaveResults()// const QPoint & position )
{

  // first, find out if we've registered
  if ( this->m_SecretData->m_NeverRegistered )
    {
    QMessageBox::warning(
      this,
      "Never Registered",
      "You haven't registered yet; nothing to save.",
      QMessageBox::Cancel );
    return;
    } // never registered

  const QWidget * const saveButton = this->ui->m_SaveButton;

  // where do we show the popup menu?
  const QPoint
    localPosition( 0, saveButton->height() ),
    globalPosition = saveButton->mapToGlobal( localPosition );

  // what items to show in the menu?
  const QString
    saveAffineText         ( "Save 4x4 Affine Matrix..."   ),
    saveCroppedMovedText   ( "Save cropped moved image..." ),
    saveMaskText           ( "Save moved mask..."          ),
    saveDeformationText    ( "Save deformation field..."   ),
    saveFullMovedText      ( "Save full moved image..."    ),
    saveFixedLandmarksText ( "Save fixed landmarks..."     ),
    saveMovingLandmarksText( "Save moving landmarks..."    ),
    saveMovedLandmarksText ( "Save moved landmarks..."     );

  // create a popup menu to show
  QMenu saveMenu;
  saveMenu.addAction( saveAffineText          );
  saveMenu.addAction( saveCroppedMovedText    );
  saveMenu.addAction( saveMaskText            );
  saveMenu.addAction( saveDeformationText     );
  saveMenu.addAction( saveFullMovedText       );
  saveMenu.addAction( saveFixedLandmarksText  );
  saveMenu.addAction( saveMovingLandmarksText );
  saveMenu.addAction( saveMovedLandmarksText  );

  const QAction * const selectedItem = saveMenu.exec( globalPosition );

  // nothing selected
  if ( !selectedItem ) return;

  const QString selectedText = selectedItem->text();

  // extract the data to possibly save
  const VolumeLinearTransformPointer
    & linearTransform = this->m_SecretData->m_MovedLinearTransform;

  const StudyConstPointer
      & movedStudy  = this->m_SecretData->m_MovedStudy,
      & movingStudy = this->m_SecretData->m_MovingStudy,
      & fixedStudy  = this->m_SecretData->m_FixedStudy;

  if ( !linearTransform || !movedStudy || !movingStudy || !fixedStudy )
    {
    QMessageBox::critical(
      this,
      "No Registration Result",
      "Unable to load the registration result to save!",
      QMessageBox::Cancel );
    return;
    } // registration result

  if ( selectedText.compare( saveAffineText ) == 0 )
    {

    // save the affine matrix
    cout << "Saving affine matrix:" << endl;

    const bool saved = SaveLinearTransform(
      *linearTransform, this, true );

    if ( saved )
      cout << "Saving affine matrix done." << endl;
    else
      cout << "Saving affine matrix failed." << endl;

    }
  else if ( selectedText.compare( saveDeformationText ) == 0 )
    {
    
    const VolumeWarpFieldConstPointer warpField(
      CalculateWarpField(
        this->m_SecretData->m_FinalTransform.GetPointer(),
        movedStudy->GetVolume().GetPointer() ) );
    
    const bool saved = SaveImage< VolumeWarpFieldType >(
      warpField, this, true );
    
    if ( saved )
      cout << "Saving warp field done." << endl;
    else
      cout << "Saving warp field failed." << endl;

    }
  else if ( selectedText.compare( saveFixedLandmarksText ) == 0 )
    {
    const LandmarkSetConstPointer fixedLandmarks = fixedStudy->GetLandmarkSet();
    if ( fixedLandmarks )
      {
      cout << "Saving fixed landmarks:" << endl;
      const bool saved = SaveLandmarks( *fixedLandmarks, this, true );
      if ( saved )
        {
        cout << "Saving fixed landmarks done." << endl;
        } // saved
      else
        {
        cerr << "Saving fixed landmarks failed." << endl;
        } // saved
      } // fixedLandmarks
    else
      {
      cerr << "Error: No fixed landmarks!" << endl;
      }
    } // save moving landmarks
    else if ( selectedText.compare( saveMovingLandmarksText ) == 0 )
    {
    const LandmarkSetConstPointer movingLandmarks = movingStudy->GetLandmarkSet();
    if ( movingLandmarks )
      {
      cout << "Saving moving landmarks:" << endl;
      const bool saved = SaveLandmarks( *movingLandmarks, this, true );
      if ( saved )
        {
        cout << "Saving moving landmarks done." << endl;
        } // saved
      else
        {
        cerr << "Saving moving landmarks failed." << endl;
        } // saved
      } // movingLandmarks
    else
      {
      cerr << "Error: No moving landmarks!" << endl;
      }
    } // save moving landmarks
    else if ( selectedText.compare( saveMovedLandmarksText ) == 0 )
    {
    const LandmarkSetConstPointer movedLandmarks = movedStudy->GetLandmarkSet();
    if ( movedLandmarks )
      {
      cout << "Saving moved landmarks:" << endl;
      const bool saved = SaveLandmarks( *movedLandmarks, this, true );
      if ( saved )
        {
        cout << "Saving moved landmarks done." << endl;
        } // saved
      else
        {
        cerr << "Saving moved landmarks failed." << endl;
        } // saved
      } // movedlandmarks
    else
      {
      cerr << "Error: No moving landmarks!" << endl;
      }
    } // save moved landmarks
  else if ( selectedText.compare( saveMaskText ) == 0 )
    {

    // save the moved mask
    cout << "Saving moved mask:" << endl;

    // choose which mask to savef
    const MaskObjectCollectionType possibleMasks = movedStudy->GetMaskObjects();
    if ( possibleMasks.empty() )
      {
      QMessageBox::warning(
        this,
        "No Masks",
        "No moved masks to save.",
        QMessageBox::Cancel );
      } // empty
    else
      {

      // create a list of possible masks to save
      QStringList possibleMasksList;
      BOOST_FOREACH( const MaskObjectType & possibleMask, possibleMasks )
        {
        possibleMasksList.push_back( possibleMask.c_str() );
        }

      // choose a mask
      bool userPressedOkay = true;
      const QString selectedMaskObject = QInputDialog::getItem(
        this,
        tr( "Select Mask" ),
        tr( "Choose a mask object to save." ),
        possibleMasksList,
        0,
        false, // editable
        &userPressedOkay );

      const MaskObjectType whichObject = selectedMaskObject.toStdString();
      if ( userPressedOkay && movedStudy->HasMask( whichObject ) )
        {

    const bool saved = SaveImage< VolumeMaskImageType >(
          movedStudy->GetMask( whichObject ), this, true );

    if ( saved )
      cout << "Saving moved mask done." << endl;
    else
      cout << "Saving moved mask failed." << endl;

        } // user pressed okay
        

      } // not empty


    } // save mask
  else if ( selectedText.compare( saveCroppedMovedText ) == 0 )
    {

    // save the moved image
    cout << "Saving cropped moved image:" << endl;

    const bool saved = SaveImage< VolumeImageType >(
      movedStudy->GetVolume(), this, true );
    
    if ( saved )
      cout << "Saving cropped moved image done." << endl;
    else
      cout << "Saving cropped moved image failed." << endl;

    }
  else if ( selectedText.compare( saveFullMovedText ) == 0 )
    {

    // save the full moved image
    cout << "Saving full moved image:" << endl;

    // apply the transform
    cout << "Applying transform:" << endl;
    const VolumeImageConstPointer
      movingVolume = movingStudy->GetVolume(),
      fixedVolume  = fixedStudy ->GetVolume();
    if ( !movingVolume || !fixedVolume ) return;
    const VolumeImageConstPointer
      movedVolume( ApplyTransform(
        linearTransform, movingVolume, fixedVolume ) );
    if ( !movedVolume ) return;
    cout << "Applying transform done." << endl;

    // save it
    const bool saved = SaveImage< VolumeImageType >(
      movedVolume, this, true );
    
    if ( saved )
      cout << "Saving full moved image done." << endl;
    else
      cout << "Saving full moved image failed." << endl;

    }
  /*
  else if ( selectedText.compare( saveAnotherImageText ) == 0 )
    {
    // apply it to another image and save
    cout << "Saving another image:" << endl;
    cout << "Saving another image done." << endl;
    }
  */

} // SlotSaveResults
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectType ProstalignFEM::SecretData::GetFEMMaskObject(
  const Ui_ProstalignFEM & ui ) const
{

  MaskObjectType selectedMaskObject;

  // get the mask we want
  const MaskObjectType
    guiMaskObject = ui.m_FEMMaskObjectText->text().toStdString();

  const StudyConstPointer movingStudy = this->m_MovingStudy;

  if ( movingStudy )
    {
    if ( !guiMaskObject.empty() && movingStudy->HasMask( guiMaskObject ) )
      {
      cout << "Note: Using GUI-selected mask " << guiMaskObject << endl;
      selectedMaskObject = guiMaskObject;
      }
    else if ( movingStudy->GetNumberOfMasks() > 0 )
      {
      cout << "Note: Using a random mask from the moving study." << endl;
      selectedMaskObject = *movingStudy->GetMaskObjects().cbegin();
      }
    else
      {
      cerr << "Error: No masks in the moving study." << endl;
      }

    } // movingStudy
  else
    {
    cerr << "Error: No moving study." << endl;
    } // moving study

  return selectedMaskObject;

} // GetFEMMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImageConstPointer ProstalignFEM::SecretData::GetFEMMask(
  const Ui_ProstalignFEM & ui ) const
{

  const MaskObjectType whichMask = this->GetFEMMaskObject( ui );
  const StudyConstPointer movingStudy = this->m_MovingStudy;

  if ( !whichMask.empty() && movingStudy )
    {
    return movingStudy->GetMask( whichMask );
    }
  else
    {
    return VolumeMaskImageConstPointer();
    }

} // GetFEMMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotEditMaterials()
{
  
  const VolumeMaskImageConstPointer mask = THIS->GetFEMMask( *this->ui );

  // set up my options dialog
  FEMMaterialsDialog materialsDialog(
    mask,
    this->m_SecretData->m_DefaultMaterial,
    this->m_SecretData->m_Materials );

  // display it
  if ( !materialsDialog.exec() )
    return; // the user pressed cancel

  // update the output materials
  this->m_SecretData->m_DefaultMaterial = materialsDialog.GetDefaultMaterial();
  this->m_SecretData->m_Materials       = materialsDialog.GetMaterials();

} // SlotEditMaterials
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotExit() 
{

  this->hide();
  qApp->exit();

} // SlotExit
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeLoadsSpheresColor()
{
  this->m_SecretData->ChangeColorAndOpacity(
    true,
    FEMRenderer::LoadsSpheresActor,
    this->ui->m_LoadsSpheresOpacitySlider );
} // SlotChangeLoadsSpheresColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeLoadsArrowsColor()
{
  // update the color
  this->m_SecretData->ChangeColorAndOpacity(
    true,
    FEMRenderer::LoadsArrowsActor,
    this->ui->m_LoadsArrowsOpacitySlider );
} // SlotChangeLoadsArrowsColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeTrianglesFaceColor()
{
  this->m_SecretData->ChangeColorAndOpacity(
    true,
    FEMRenderer::TrianglesActor,
    this->ui->m_TrianglesOpacitySlider );
} // SlotChangeTrianglesFaceColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeTrianglesEdgeColor()
{
  this->m_SecretData->ChangeColorAndOpacity(
    false, FEMRenderer::TrianglesActor );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeLandmarksColor()
{
  this->m_SecretData->ChangeColorAndOpacity(
    true,
    FEMRenderer::LandmarksActor,
    this->ui->m_LandmarksOpacitySlider );
} // SlotChangeLandmarksColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SecretData::ChangeColorAndOpacity(
  const bool                    faceColor,
  const FEMRenderer::WhichActor actor,
        QAbstractSlider * const sliderToUpdate )
{

  /*const*/ bool verbose = true;

  // extract the renderer
  const FEMRendererPointer
    & renderer = this->m_FEMRenderer;

  if ( !renderer )
    return;

  // extract the current color
  const FEMRenderer::ColorType currentColor =
    faceColor ?
      renderer->GetFaceColor( actor ) :
      renderer->GetEdgeColor( actor );
  const FEMRenderer::RealPropertiesType oldOpacity =
    faceColor ?
      renderer->GetOpacity( actor ) :
      1.0;

  const QColor currentRGBA(
    currentColor[ 0 ], currentColor[ 1 ], currentColor[ 2 ],
    static_cast< int >( floor( 0.5 + oldOpacity * 255.0 ) ) );

  const QColor newRGBA = QColorDialog::getColor(
    currentRGBA, NULL, "Select color",
    ( faceColor ? QColorDialog::ShowAlphaChannel :
      QColorDialog::ColorDialogOptions( 0 ) ) );

  // did the user select cancel?
  if ( !newRGBA.isValid() )
    return;

  // update the properties
  FEMRenderer::ColorType newColor;
  newColor[ 0 ] = static_cast< FEMRenderer::RGBValueType >( newRGBA.red  () );
  newColor[ 1 ] = static_cast< FEMRenderer::RGBValueType >( newRGBA.green() );
  newColor[ 2 ] = static_cast< FEMRenderer::RGBValueType >( newRGBA.blue () );

  const FEMRenderer::RealPropertiesType newOpacityPercent =
    static_cast< FEMRenderer::RealPropertiesType >(
      newRGBA.alpha() ) / 255.0;

  if ( faceColor )
    {
    renderer->SetFaceColor( actor, newColor );
    renderer->SetOpacity( actor, newOpacityPercent );

    if ( sliderToUpdate )
      {
      // set the opacity slider
      sliderToUpdate->setValue( static_cast< int >(
        floor( 0.5 + newOpacityPercent * SecretData::m_SliderFactor ) ) );
      }

    if ( verbose )
      cout << "Old opacity = " << oldOpacity * 100.0 << "%" << endl <<
        "New opacity = " << newOpacityPercent * 100.0 << "%" << endl;
    } // faceColor
  else  // edgeColor
    {
    renderer->SetEdgeColor( actor, newColor );
    } // edgeColor

} // SlotChangeLandmarksColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotChangeArrowsScale()
{
  this->m_SecretData->UpdateFEMRendererProperties( *this->ui );
}
void ProstalignFEM::SlotChangeLandmarksTransparency()
{
  this->m_SecretData->UpdateFEMRendererProperties( *this->ui );
} 
void ProstalignFEM::SlotChangeTrianglesTransparency()
{
  this->m_SecretData->UpdateFEMRendererProperties( *this->ui );
} 
void ProstalignFEM::SlotChangeLoadsSpheresTransparency()
{
  this->m_SecretData->UpdateFEMRendererProperties( *this->ui );
} 
void ProstalignFEM::SlotChangeLoadsArrowsTransparency()
{
  this->m_SecretData->UpdateFEMRendererProperties( *this->ui );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SecretData::UpdateFEMRendererProperties(
  const Ui_ProstalignFEM & ui )
{
  
  // extract the renderer
  const FEMRendererPointer
    & renderer = this->m_FEMRenderer;

  if ( !renderer )
    return;

  // update all the opacities
  renderer->SetOpacity(
    FEMRenderer::LandmarksActor,
    static_cast< FEMRenderer::RealPropertiesType >(
      ui.m_LandmarksOpacitySlider->value() ) / SecretData::m_SliderFactor );
  renderer->SetOpacity(
    FEMRenderer::TrianglesActor,
    static_cast< FEMRenderer::RealPropertiesType >(
      ui.m_TrianglesOpacitySlider->value() ) / SecretData::m_SliderFactor );
  renderer->SetOpacity(
    FEMRenderer::LoadsSpheresActor,
    static_cast< FEMRenderer::RealPropertiesType >(
      ui.m_LoadsSpheresOpacitySlider->value() ) / SecretData::m_SliderFactor );
  renderer->SetOpacity(
    FEMRenderer::LoadsArrowsActor,
    static_cast< FEMRenderer::RealPropertiesType >(
      ui.m_LoadsArrowsOpacitySlider->value() ) / SecretData::m_SliderFactor );

  // update the scale
  const FEMRenderer::RealPropertiesType
    mantissa = ui.m_ArrowsScaleMantissaSpin->value(),
    exponent = ui.m_ArrowsScaleExponentSpin->value(),
    base     = 10.0,
    scale    = mantissa * std::pow( base, exponent );

  renderer->SetArrowScaling( scale );

  // re-render
  ui.m_OutputVTKWidget->GetRenderWindow()->Render();

} // UpdateFEMRendererProperties
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotGradientDescentOptions()
{
  
  // get the parameters
  const SecretData::RegisterType::Pointer
    registerer = this->m_SecretData->m_RegisterImageLinear;
  if ( !registerer ) return;

  const SecretData::OptimizerType::Pointer optimizerBase = 
    std::dynamic_pointer_cast< SecretData::OptimizerType > ( registerer->GetOptimizer() );
  if ( !optimizerBase ) return; 

  ModifyGradientDescentOptions( *optimizerBase );

} // SlotGradientDescentOptions
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotParticleSwarmLinearOptions()
{
  
  // get the parameters
  const SecretData::RegisterType::Pointer
    registerer = this->m_SecretData->m_RegisterMaskLinear;
  if ( !registerer ) return;

  const SecretData::OptimizerType::Pointer optimizerBase = 
    std::dynamic_pointer_cast< SecretData::OptimizerType > ( registerer->GetOptimizer() );
  if ( !optimizerBase ) return; 

  ModifyParticleSwarmOptions( *optimizerBase );

} // SlotParticleSwarmLinearOptions
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotParticleSwarmFEMOptions()
{
  
  // get the parameters
  const SecretData::RegisterType::Pointer
    registerer = this->m_SecretData->m_RegisterFEM;
  if ( !registerer ) return;

    const SecretData::OptimizerType::Pointer optimizerBase = 
    std::dynamic_pointer_cast< SecretData::OptimizerType > ( registerer->GetOptimizer() );
  if ( !optimizerBase ) return; 

  ModifyParticleSwarmOptions( *optimizerBase );

} // SlotParticleSwarmFEMOptions
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignFEM::SlotFEMTransformOptions()
{
  
  // get the parameters
  const SecretData::RegisterType::Pointer
    registerer = this->m_SecretData->m_RegisterFEM;
  if ( !registerer ) return;

 const SecretData::TransformType::Pointer transformBase = 
    std::dynamic_pointer_cast< SecretData::TransformType > ( registerer->GetTransform() );
  if ( !transformBase ) return; 

  ModifyFEMTransformOptions( *transformBase );

} // SlotFEMTransformOptions
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
