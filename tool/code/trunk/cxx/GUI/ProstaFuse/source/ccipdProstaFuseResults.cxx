


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdProstaFuseResults.h"
#include "ccipdProstaFuseResultsOptions.h"    // for showing options
#include "ccipdMacros.h"
#include "ccipdVTKForwardDeclarations.h"      // vtk types
#include "ccipdVolumeTypes.h"                 // image types
#include "ccipdImageToProp.h"
#include "ccipdCopySliderParameters.h"
#include "ccipdUpdateVTKImageSlice.h"
#include "ccipdUpdateVTKImageProp.h"
#include "ccipdUpdateVTKImageBlinds.h"
#include "ccipdUpdateVTKImageWindowLevel.h"
#include "ccipdResizeImage.h"
#include "ccipdSaveImageGUI.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

#include <itkImage.h>

// Qt includes
#include <QApplication>
#include <qmessagebox.h>
#include "ui_ccipdProstaFuseResults.h"

// vtk includes
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkCamera.h>
#include <vtkRendererCollection.h>
#include <vtkInteractorStyleImage.h>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interfact to ProstalignMain
class ProstaFuseResults::SecretData
{

public:

#ifdef _MSC_VER
  SecretData & operator=( const SecretData & other )
  {
    return const_cast< SecretData & >( other );
  }
#endif // _MSC_VER

  /// Constructor
  SecretData(
    const VolumeImageType       * const fixedImage,
    const VolumeImageType       * const movedImage,
    const float                         slideFactor,
          ProstaFuseResults     * const containerClass );

  ProstaFuseResults * const m_ContainerClass; ///< What are we a part of?

  /// Update what images are displayed in the widget
  void UpdateImageWidget();

  /// Create a vtk prop to add, with all the desired properties.
  vtkPropPointer CreateImageProp(     
    const VolumeImageType  * const image, 
    const int                      whichSlice,
    const bool                     useCheckerboard );


  const VolumeImageConstPointer
    m_MovedImage;
  VolumeImageConstPointer
    m_FixedImage;  ///< Fixed image gets resized to match moved
  
  const float m_SlideFactor;

  vtkPropPointer
    m_MovedImageProp,
    m_FixedImageProp;

  /// Controlling how many checkerboard blinds to display.
  typedef ProstaFuseResultsOptions::NumberOfBlindsType NumberOfBlindsType;
  
  NumberOfBlindsType
    m_HorizontalBlinds, ///< How many blinds to show horizontally.
    m_VerticalBlinds;   ///< How many blinds to show vertically.
 
  /// State of whether or not we are overlaying the fixed image
  bool 
    m_DisplayFixedImage;

  int 
    m_WhichSlice;
  
}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstaFuseResults::SecretData::SecretData(
    const VolumeImageType    * const fixedImage,
    const VolumeImageType    * const movedImage,
    const float                      slideFactor,
          ProstaFuseResults  * const containerClass ) :
m_ContainerClass( containerClass ),
m_MovedImage( movedImage ),
m_SlideFactor( slideFactor ),
m_HorizontalBlinds ( 4 ),
m_VerticalBlinds   ( 4 ), 
m_DisplayFixedImage( false ),
m_WhichSlice( 0 )
{
this->m_FixedImage = ResizeImage( fixedImage, movedImage );
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseResults::SecretData::UpdateImageWidget()
{

  // clear the existing renderer
  vtkRenderWindow * const renderWindow =
    this->m_ContainerClass->ui->m_ImageWidget->GetRenderWindow();

  vtkRenderer * const oldRenderer = renderWindow->GetRenderers()->GetFirstRenderer();

  if ( oldRenderer )
    renderWindow->RemoveRenderer( oldRenderer );

  // create a new renderer
  const vtkRendererPointer newRenderer = vtkRendererPointer::New();

  m_MovedImageProp = this->CreateImageProp( this->m_MovedImage, this->m_WhichSlice, false );
  newRenderer->AddViewProp( m_MovedImageProp );
  UpdateVTKImageWindowLevel(
      this->m_MovedImageProp,
      this->m_ContainerClass->ui->m_MovedWindowSlider->value()/this->m_SlideFactor,
      this->m_ContainerClass->ui->m_MovedLevelSlider ->value()/this->m_SlideFactor );

  if ( this->m_DisplayFixedImage )
    {
    m_FixedImageProp = this->CreateImageProp( this->m_FixedImage, this->m_WhichSlice, true );
    newRenderer->AddViewProp( m_FixedImageProp );

    UpdateVTKImageWindowLevel(
      this->m_FixedImageProp,
      this->m_ContainerClass->ui->m_FixedWindowSlider->value()/this->m_SlideFactor,
      this->m_ContainerClass->ui->m_FixedLevelSlider ->value()/this->m_SlideFactor );
    }
  else
    {
    m_FixedImageProp = NULL; // clear the fixedImageProp if we are not using it
    }
  // make sure the camera is correct since itk flips the x andy axis
  // so we will place the camera "behind" the image
  newRenderer->ResetCamera(); // view the whole scene
  vtkCamera * const camera = newRenderer->GetActiveCamera();
  camera->SetViewUp( 0, -1, 0 );

  const double * const position = camera->GetPosition();
  camera->SetPosition( position[ 0 ], position[ 1 ], -position[ 2 ] );

    // add the renderer
  renderWindow->AddRenderer( newRenderer );

  // rerender
  renderWindow->Render();
  renderWindow->Render(); // once more for good 

} // UpdateImageWidget
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
vtkPropPointer ProstaFuseResults::SecretData::CreateImageProp(     
  const VolumeImageType  * const image, 
  const int                      whichSlice,
  const bool                     useCheckerboard )
{
// first, just create the prop
  const bool makeZeroTransparent = true;
   
  vtkPropPointer imageProp( CreatePropFromImage( image, makeZeroTransparent ) );

  // Now change to the appropriate slice
  UpdateVTKImageSlice( imageProp , whichSlice, false );

  if ( useCheckerboard )
    // create the blinds
    UpdateVTKImageBlinds(
      *image,
      *imageProp,
      this->m_HorizontalBlinds,
      this->m_VerticalBlinds );

  return imageProp;
} // CreateImageProp()
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstaFuseResults::ProstaFuseResults(
    const VolumeImageType    * const fixedImage,
    QSlider                  &       fixedWindowSlider,
    QSlider                  &       fixedLevelSlider,
    const VolumeImageType    * const movedImage,
    QSlider                  &       movedWindowSlider,
    QSlider                  &       movedLevelSlider,
    QSlider                  &       movedSliceSlider,
    const                    float   slideFactor        ):
m_SecretData( new SecretData( fixedImage, movedImage, slideFactor, this ) )
{

  // set up the user interface
  this->ui = new Ui_ProstaFuseResults;
  this->ui->setupUi( this );

  // VTK Renderer
  this->m_SecretData->UpdateImageWidget();

  // get the # of slices
  const unsigned int numberOfSlices = static_cast< unsigned int >(
    movedImage->GetBufferedRegion().GetSize()[ 2 ] );
  cout << "Number of slices = " << numberOfSlices << endl;

  // update the slider
  QSlider * sliceSlider = this->ui->m_MovedImageSlider;
  sliceSlider->setMaximum( numberOfSlices - 1 );
  sliceSlider->setValue( movedSliceSlider.value() ); // initialize image in midgland    

  CopySliderParameters( fixedWindowSlider, *this->ui->m_FixedWindowSlider );
  CopySliderParameters( fixedLevelSlider, *this->ui->m_FixedLevelSlider );
  CopySliderParameters( movedWindowSlider, *this->ui->m_MovedWindowSlider );
  CopySliderParameters( movedLevelSlider, *this->ui->m_MovedLevelSlider );


} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstaFuseResults::~ProstaFuseResults()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseResults::SlotChangeFixedWindow()
{
  if ( this->m_SecretData->m_DisplayFixedImage ) // only if we are displaying the fixed image
    {
    // change it
    UpdateVTKImageWindowLevel(
      this->m_SecretData->m_FixedImageProp,
      this->ui->m_FixedWindowSlider->value()/this->m_SecretData->m_SlideFactor,
      this->ui->m_FixedLevelSlider ->value()/this->m_SecretData->m_SlideFactor );

    // re-render
    this->ui->m_ImageWidget->GetRenderWindow()->Render();
    }
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseResults::SlotChangeFixedLevel()
{
  if ( this->m_SecretData->m_DisplayFixedImage ) // only if we are displaying the fixed image
    {
    // change it
    UpdateVTKImageWindowLevel(
      this->m_SecretData->m_FixedImageProp,
      this->ui->m_FixedWindowSlider->value()/this->m_SecretData->m_SlideFactor,
      this->ui->m_FixedLevelSlider ->value()/this->m_SecretData->m_SlideFactor );

    // re-render
    this->ui->m_ImageWidget->GetRenderWindow()->Render();
    }
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseResults::SlotChangeMovedWindow()
{
    // change it
    UpdateVTKImageWindowLevel(
      this->m_SecretData->m_MovedImageProp,
      this->ui->m_MovedWindowSlider->value()/this->m_SecretData->m_SlideFactor,
      this->ui->m_MovedLevelSlider ->value()/this->m_SecretData->m_SlideFactor );

    // re-render
    this->ui->m_ImageWidget->GetRenderWindow()->Render();
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseResults::SlotChangeMovedLevel()
{
    // change it
    UpdateVTKImageWindowLevel(
      this->m_SecretData->m_MovedImageProp,
      this->ui->m_MovedWindowSlider->value()/this->m_SecretData->m_SlideFactor,
      this->ui->m_MovedLevelSlider ->value()/this->m_SecretData->m_SlideFactor );

    // re-render
    this->ui->m_ImageWidget->GetRenderWindow()->Render();
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseResults::SlotOptions() 
{
    // extract the data I want to modify
  typedef SecretData::NumberOfBlindsType NumberOfBlindsType;
  NumberOfBlindsType
    & horizontalBlinds = this->m_SecretData->m_HorizontalBlinds,
    & verticalBlinds   = this->m_SecretData->m_VerticalBlinds;

  bool & displayFixedImage = this->m_SecretData->m_DisplayFixedImage;

  // set up my options dialog
  ProstaFuseResultsOptions options( displayFixedImage, 
                                    horizontalBlinds,
                                    verticalBlinds   );

  // display it
  if ( !options.exec() )
    return; // the user pressed cancel

  // update the values
  displayFixedImage  = options.GetDisplayFixedTRUS();
  horizontalBlinds   = options.GetHorizontalBlinds();
  verticalBlinds     = options.GetVerticalBlinds();

  // update the display
  this->m_SecretData->UpdateImageWidget();

} // SlotOptions
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseResults::SlotChangeImageSlice( const int whichSlice )
{

  // change the slice
  this->m_SecretData->m_WhichSlice = whichSlice;
 
   // Now change proprs to the appropriate slice
  if ( this->m_SecretData->m_MovedImageProp )
    UpdateVTKImageSlice( this->m_SecretData->m_MovedImageProp , whichSlice, false );

  if ( this->m_SecretData->m_DisplayFixedImage && this->m_SecretData->m_FixedImageProp )
    UpdateVTKImageSlice( this->m_SecretData->m_FixedImageProp , whichSlice, false );

  // re-render
  this->ui->m_ImageWidget->GetRenderWindow()->Render();

} // SlotChangeMovingSlice
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstaFuseResults::SlotSaveMovedImage() 
{

const bool saved =  
  SaveImage< VolumeImageType >( this->m_SecretData->m_MovedImage, this, true );

  if ( saved )
    cout << "Saving moved image done." << endl;
  else
    cout << "Saving moved image failed." << endl;

} // SlotSaveMovedImage
//////////////////////////////////////////////////////////////////////////////////////////




} // namespace ccipd
