



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdProstalignResults.h"
#include "ccipdRGBSliceTypes.h"
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdImageToProp.h"
#include "ccipdCalculateWarpField.h"       // for generating a warp field from transform
#include "ccipdApplyWarpField.hxx"           // for deforming for the animation
#include "ccipdAnimateWarpField.h"         // for outputting animation frames
#include "ccipdProstalignResultsOptions.h" // for showing options
#include "ccipdSaveImageGUI.h"             // for saving the result
#include "ccipdUpdateVTKImageBlinds.h"     // for creating "checkerboard" blinds

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// Qt includes
#include <QFileDialog>  // for selecting a directory
#include <QInputDialog> // for selecting # of frames
#include "ui_ccipdProstalignResults.h"

// itk includes
#include <itkImage.h>
#include <itkTransform.h>
#include <itkRGBPixel.h>

// vtk includes
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkProp.h>
#include <vtkImageSlice.h>
#include <vtkInteractorStyleImage.h> // our mouse interactor style
#include <vtkCamera.h>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////




namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interfact to ProstalignMain
class ProstalignResults::SecretData
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
    const RGBSliceType       * const fixedImage,
    const RGBSliceType       * const movingImage,
    const RGBSliceType       * const movedImage,
    const SliceTransformType * const transform,
          ProstalignResults  * const containerClass );

  const RGBSliceConstPointer
    m_FixedImage,  ///< Fixed image.
    m_MovingImage, ///< Moving image.
    m_MovedImage;  ///< Moved image.
  
  ProstalignResults * const m_ContainerClass; ///< What are we a part of?

  RGBSliceConstPointer
    m_LeftImage,  ///< The image specified by the left combo box.
    m_RightImage; ///< The image specified by the right combo box.

  SliceTransformConstPointer
    m_Transform; ///< The transform from the moving to the moved.

  SliceWarpFieldConstPointer
    m_WarpField; ///< Generated from transform.

  /// Possible states of the left and right images as checkerboards
  enum CheckerboardState
  {
    NoCheckerboard,
    HorizontalCheckerboard,
    VerticalCheckerboard,
    CombinedCheckerboard
  };

  CheckerboardState
    m_LeftCheckerboard,  ///< The checkerboard state of the left image.
    m_RightCheckerboard; ///< The checkerboard state of the right image.

  /// Controlling how many checkerboard blinds to display.
  typedef ProstalignResultsOptions::NumberOfBlindsType NumberOfBlindsType;
  
  NumberOfBlindsType
    m_HorizontalBlinds, ///< How many blinds to show horizontally.
    m_VerticalBlinds;   ///< How many blinds to show vertically.

  /// Display the left and right images in the results window.
  void UpdateResultsWidget();

  /// Update either the left or right image.
  void UpdateImage(
    const QString              & imageString,
          RGBSliceConstPointer & whichImage );

  /// Update either the left or right checkerboard state.
  void UpdateCheckerboard(
    const QString           & checkerboardString,
          CheckerboardState & whichCheckerboard );

  /// Create a vtk prop to add, with all the desired properties.
  vtkPropPointer CreateImageProp(
    const RGBSliceType * const image,
    const double               zTranslation = 0,
    const CheckerboardState    checkerboard = NoCheckerboard ) const;

  /// This will take m_Transform and calculate m_WarpField from it.
  void CalculateWarpField();

  /// This will return an existing warp field if it exists, or calculate it or not.
  SliceWarpFieldConstPointer GetWarpField();

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
ProstalignResults::SecretData::SecretData(
  const RGBSliceType       * const fixedImage,
  const RGBSliceType       * const movingImage,
  const RGBSliceType       * const movedImage,
  const SliceTransformType * const transform,
        ProstalignResults  * const containerClass ) :
m_FixedImage       ( fixedImage     ),
m_MovingImage      ( movingImage    ),
m_MovedImage       ( movedImage     ),
m_ContainerClass   ( containerClass ),
m_Transform        ( transform      ),
m_LeftCheckerboard ( NoCheckerboard ),
m_RightCheckerboard( NoCheckerboard ),
m_HorizontalBlinds ( 32 ),
m_VerticalBlinds   ( 32 )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstalignResults::ProstalignResults(
  const RGBSliceType       * const fixedImage,
  const RGBSliceType       * const movingImage,
  const RGBSliceType       * const movedImage,
  const SliceTransformType * const transform ) :
m_SecretData( new SecretData( fixedImage, movingImage, movedImage, transform, this ) )
{

  // set up the user interface
  this->ui = new Ui_ProstalignResults;
  this->ui->setupUi( this );
 
  // VTK Renderer
  const vtkRendererPointer resultsRenderer = vtkRendererPointer::New();

  // create an image based interactor
  typedef vtkInteractorStyleImage StyleType;
  typedef vtkSmartPointer< StyleType > StylePointer;
  const StylePointer style = StylePointer::New();
  style->SetInteractionModeToImage2D();

  // give everythign to the render window
  vtkRenderWindow * const renderWindow = this->ui->resultsWidget->GetRenderWindow();
  renderWindow->GetInteractor()->SetInteractorStyle( style );
  renderWindow->AddRenderer( resultsRenderer );

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ProstalignResults::~ProstalignResults()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SlotRightImageComboChanged( const QString & comboItem )
{

  this->m_SecretData->UpdateImage( comboItem, this->m_SecretData->m_RightImage );

} // SlotRightComboChanged
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SlotLeftImageComboChanged( const QString & comboItem )
{

  this->m_SecretData->UpdateImage( comboItem, this->m_SecretData->m_LeftImage );

} // SlotLeftComboChanged
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SlotRightCheckerboardComboChanged( const QString & comboItem )
{

  this->m_SecretData->UpdateCheckerboard(
    comboItem, this->m_SecretData->m_RightCheckerboard );

} // SlotRightCheckerboardComboChanged
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SlotLeftCheckerboardComboChanged( const QString & comboItem )
{

  this->m_SecretData->UpdateCheckerboard(
    comboItem, this->m_SecretData->m_LeftCheckerboard );

} // SlotLeftCheckerboardComboChanged
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SecretData::UpdateCheckerboard(
  const QString           & checkerboardString,
        CheckerboardState & whichCheckerboard )
{

  if ( checkerboardString.compare( "No Checkerboard" ) == 0 )
    {
    whichCheckerboard = NoCheckerboard;
    }
  else if ( checkerboardString.compare( "Horizontal Checkerboard" ) == 0 )
    {
    whichCheckerboard = HorizontalCheckerboard;
    }
  else if ( checkerboardString.compare( "Vertical Checkerboard" ) == 0 )
    {
    whichCheckerboard = VerticalCheckerboard;
    }
  else if ( checkerboardString.compare( "Combined Checkerboard" ) == 0 )
    {
    whichCheckerboard = CombinedCheckerboard;
    }
  else
    {
    return;
    }

  // if the checkerboard state has changed, update the results
  this->UpdateResultsWidget();

} // UpdateCheckerboard
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SecretData::UpdateImage(
  const QString        & imageString,
  RGBSliceConstPointer & whichImage )
{

  if ( imageString.compare( "Fixed Image" ) == 0 )
    {
    whichImage = this->m_FixedImage;
    }
  else if ( imageString.compare( "Moving Image" ) == 0 )
    {
    whichImage = this->m_MovingImage;
    }
  else if ( imageString.compare( "Moved Image" ) == 0 )
    {
    whichImage = this->m_MovedImage;
    }
  else if ( imageString.compare( "(None)" ) == 0 )
    {
    whichImage = RGBSliceConstPointer(); // NULL
    }
  else
    {
    return;
    }
  
  // update the display
  this->UpdateResultsWidget();

} // SlotLeftComboChanged
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SecretData::UpdateResultsWidget()
{

  // extract the data
  const RGBSliceConstPointer
    & leftImage  = this->m_LeftImage,
    & rightImage = this->m_RightImage;

  // clear the existing renderer
  vtkRenderWindow * const renderWindow =
    this->m_ContainerClass->ui->resultsWidget->GetRenderWindow();

  vtkRenderer * const oldRenderer = renderWindow->GetRenderers()->GetFirstRenderer();

  if ( oldRenderer )
    renderWindow->RemoveRenderer( oldRenderer );

  // create a new renderer
  const vtkRendererPointer newRenderer = vtkRendererPointer::New();
  
  // make sure the camera is correct since itk flips the y axis
  vtkCamera * const camera = newRenderer->GetActiveCamera();
  camera->SetViewUp( 0, -1, 0 );

  // add the images
  if ( leftImage )
    {
    newRenderer->AddViewProp(
      this->CreateImageProp(
        leftImage, 0, this->m_LeftCheckerboard ) );
    } // leftImage

  if ( rightImage )
    {
    const double epsilon = -1e-2; // negative because we move our camera
    newRenderer->AddViewProp(
      this->CreateImageProp(
        rightImage, epsilon, this->m_RightCheckerboard ) );
    } // rightImage

  // the x axis is actually also flipped,
  // so we will place the camera "behind" the image
  newRenderer->ResetCamera(); // view the whole scene
  const double * const position = camera->GetPosition();
  camera->SetPosition( position[ 0 ], position[ 1 ], -position[ 2 ] );

  // add the renderer
  renderWindow->AddRenderer( newRenderer );

  // rerender
  renderWindow->Render();
  renderWindow->Render(); // once more for good measure

} // UpdateResultsWidget
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
vtkPropPointer ProstalignResults::SecretData::CreateImageProp(
  const RGBSliceType * const image,
  const double               zTranslation,
  const CheckerboardState    checkerboard ) const
{

  // first, just create the prop
  const bool makeZeroTransparent = true;
  const vtkPropPointer imageProp = CreatePropFromImage( image, makeZeroTransparent );

  if ( zTranslation != 0 || checkerboard != NoCheckerboard )
    {

    // cast it to a prop so we can translate and create a checkerboard
    // we are going to want to translate this slightly
    typedef vtkSmartPointer< vtkImageSlice > vtkImageSlicePointer;
    const vtkImageSlicePointer imageSlice =
      dynamic_cast< vtkImageSlice * >( imageProp.GetPointer() );

    if ( imageSlice )
      {

      // translate
      if ( zTranslation != 0 )
        imageSlice->AddPosition( 0, 0, zTranslation );

      if ( checkerboard != NoCheckerboard )
        {
        // which type is it?
        const bool
          isHorizontal = checkerboard == HorizontalCheckerboard,
          isVertical   = checkerboard == VerticalCheckerboard;

        // create the blinds
        UpdateVTKImageBlinds(
          *image,
          *imageSlice,
          ( !isVertical   ) ? this->m_HorizontalBlinds : 0,
          ( !isHorizontal ) ? this->m_VerticalBlinds   : 0 );
        
        } // NoCheckerboard

      } // imageSlice

    } // casting

  // output
  return imageProp;

} // UpdateResultsWidget
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SlotOptions()
{

  // extract the data I want to modify
  typedef SecretData::NumberOfBlindsType NumberOfBlindsType;
  NumberOfBlindsType
    & horizontalBlinds = this->m_SecretData->m_HorizontalBlinds,
    & verticalBlinds   = this->m_SecretData->m_VerticalBlinds;

  // set up my options dialog
  ProstalignResultsOptions options( horizontalBlinds, verticalBlinds );

  // display it
  if ( !options.exec() )
    return; // the user pressed cancel

  // update the values
  horizontalBlinds = options.GetHorizontalBlinds();
  verticalBlinds   = options.GetVerticalBlinds  ();

  // update the display
  this->m_SecretData->UpdateResultsWidget();

} // SlotOptions
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SlotSaveMoved()
{

  // save the result
  SaveImage< RGBSliceType >(
    this->m_SecretData->m_MovedImage , this, true /* verbose */ );

} // SlotSaveMoved
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SlotSaveWarpField()
{

  // save the warp field
  SaveImage< SliceWarpFieldType >(
    this->m_SecretData->GetWarpField(), this, true );

} // SlotSaveMoved
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SlotSaveAnimation()
{

  // select a directory
  const string directory = QFileDialog::getExistingDirectory(
    this, tr( "Select a directory to save the animation frames") ).toStdString();

  if ( directory.empty() )
    return;

  // how many frames?

  bool validNumberOfFrames = true;
  const unsigned int
    defaultFrames  = 24,
    minFrames      = 3,
    maxFrames      = 2147483647,
    framesStep     = 1,
    numberOfFrames = static_cast< unsigned int >(
      QInputDialog::getInt(
        this,
        tr( "Number of Frames" ),
        tr( "Please input the number of frames to generate." ),
        defaultFrames,
        minFrames,
        maxFrames,
        framesStep,
        &validNumberOfFrames ) );

  if ( !validNumberOfFrames )
    return;

  const SliceWarpFieldConstPointer
    warpField = this->m_SecretData->GetWarpField();

  cout << "Animating:" << endl;

  // now, animate
  const MaskPixelType movingImageTransparency = 
    static_cast< MaskPixelType >( 0.75 * 255 );
  const bool makeBlackTransparent = true;

  GenerateWarpFieldAnimationFrames(
    warpField,
    this->m_SecretData->m_FixedImage,
    this->m_SecretData->m_MovingImage,
    numberOfFrames,
    movingImageTransparency,
    makeBlackTransparent,
    directory );

  cout << "Animating done." << endl;

} // SlotSaveAnimation
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SliceWarpFieldConstPointer ProstalignResults::SecretData::GetWarpField()
{


  // we might have to calculate the warp field
  if ( !this->m_WarpField )
    {
    cout << "Calculating warp field:" << endl;
    this->CalculateWarpField();
    cout << "Calculating warp field done." << endl;
    }

  return this->m_WarpField;

} // GetWarpField
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ProstalignResults::SecretData::CalculateWarpField()
{

  // extract the data
  const RGBSliceConstPointer
    & fixedImage  = this->m_FixedImage,
    & movingImage = this->m_MovingImage;
  SliceWarpFieldConstPointer
    & warpField = this->m_WarpField;
  const SliceTransformConstPointer
    & transform = this->m_Transform;

  // turn it into a warp field
#ifdef NDEBUG
  const unsigned int numberOfInversePointsPerDimension = 24; // more accurate
#else
  const unsigned int numberOfInversePointsPerDimension = 8; // faster
#endif
  const unsigned int subSamplingFactor =
    static_cast< unsigned int >(
      movingImage->GetBufferedRegion().GetSize()[ 0 ] /
      numberOfInversePointsPerDimension );

  warpField = CalculateInverseWarpField(
    transform, fixedImage, subSamplingFactor ).GetPointer();
  
} // CalculateWarpField
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
