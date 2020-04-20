



//////////////////////////////////////////////////////////////////////////////////////////
 
// ccipd includes
#include "ccipdPickPointsDialog.h"
#include "ccipdUpdateWindowLevelSliders.h"
#include "ccipdUpdateVTKImageWindowLevel.h"
#include "ccipdRenderPointPicker.h"
#include "ccipdMouseInteractorStylePointPicker.h" // how we picked the points
#include "ccipdLandmarksToActor.h" // for rendering landmarks

#include "ccipdDisableWarningsMacro.h"

// Qt includes
#include "ui_ccipdPickPointsDialog.h"

// itk includes
#include <itkImage.h>
#include <itkPointSet.h>

// vtk includes
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkProp.h>
#include <vtkPropCollection.h>
#include <vtkSmartPointer.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkLookupTable.h> // for colors

// boost includes
#include <boost/noncopyable.hpp>

#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
class PickPointsDialog::SecretData : boost::noncopyable
{

public:
  explicit SecretData( const VolumeImageConstPointer image );

  const VolumeImageConstPointer
    m_Image; ///< Store the image
  
  vtkPropPointer
    m_ImageProp; ///< Store the vtk prop for an image.

  vtkActorPointer
    m_LandmarksActor; ///< Store the actor for the landmarks

  static const float
    m_SliderFactor;

  // for color lookup
  typedef vtkSmartPointer< vtkLookupTable > vtkLookupTablePointer;
  vtkLookupTablePointer
    m_ColorMap;

}; // SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
const float PickPointsDialog::SecretData::m_SliderFactor = 100;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PickPointsDialog::PickPointsDialog( const VolumeImageConstPointer & image ) :
ui( new Ui_PickPointsDialog ), // set up the ui
m_SecretData( new SecretData( image ) )
{
  // initialize with no landmarks
  this->Initialize( image, LandmarkSetConstPointer() );
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
PickPointsDialog::PickPointsDialog(
  const VolumeImageConstPointer & image,
  const LandmarkSetConstPointer & initialLandmarks ) :
ui( new Ui_PickPointsDialog ), // set up the ui
m_SecretData( new SecretData( image ) )
{
  this->Initialize( image, initialLandmarks );
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
void PickPointsDialog::Initialize(
  const VolumeImageConstPointer & image,
  const LandmarkSetConstPointer & initialLandmarks )
{
  // create the interface
  Ui_PickPointsDialog & interface = *this->ui;
  interface.setupUi( this ); // we need to do this first

  // we will create an event filter to
  // capture any time the user clicks in the qtvtkwidget
  // so we can rerender the landmarks
  interface.m_VTKOutput->installEventFilter( this );

  if ( !image )
    {
    cerr << "Error: No image." << endl;
    return;
    }

  // render the image based stuff
  
  // update our sliders
  const unsigned int
    numberOfSlices = static_cast< unsigned int >(
      image->GetBufferedRegion().GetSize()[ 2 ] ),
    whichSlice     = numberOfSlices / 2;

  // render the point picker with our initial landmarks
  if ( initialLandmarks )
    {
    RenderPointPicker(
      image.GetPointer(),
      whichSlice,
      true, // verbose
      interface.m_VTKOutput->GetRenderWindow(),
      initialLandmarks );
    } // initialLandmarks

  interface.m_SliceSlider->setMaximum( static_cast< int >( numberOfSlices ) );
  interface.m_SliceSlider->setValue  ( static_cast< int >( whichSlice     ) );

  const float sliderFactor = SecretData::m_SliderFactor;

  UpdateWindowLevelSliders(
    image,
    interface.m_WindowSlider,
    interface.m_LevelSlider,
    sliderFactor );

  // update the landmark opacity slider
  const unsigned int maxLandmarksOpacitySlider =
    static_cast< unsigned int >( sliderFactor );
  interface.m_LandmarksOpacitySlider->setMinimum( 0 );
  interface.m_LandmarksOpacitySlider->setMaximum( maxLandmarksOpacitySlider );
  interface.m_LandmarksOpacitySlider->setValue  ( maxLandmarksOpacitySlider / 2 );

  // update the landmark size slider
  const unsigned int maxLandmarkSizeSlider =
    static_cast< unsigned int >( 15 * sliderFactor );
  interface.m_LandmarksSizeSlider->setMinimum( 0 );
  interface.m_LandmarksSizeSlider->setMaximum( maxLandmarkSizeSlider );
  interface.m_LandmarksSizeSlider->setValue  ( maxLandmarkSizeSlider / 5 );

  // update the landmarks color slider
  const unsigned int maxLandmarksColorSlider = maxLandmarksOpacitySlider;
  interface.m_LandmarksColorSlider->setMinimum( 0 );
  interface.m_LandmarksColorSlider->setMaximum( maxLandmarksColorSlider );
  interface.m_LandmarksColorSlider->setValue  ( maxLandmarksColorSlider / 2 );

  // update the color map
  const auto colorMap = SecretData::vtkLookupTablePointer::New();
  colorMap->SetTableRange      ( 0, maxLandmarksColorSlider );
  colorMap->SetNumberOfTableValues( maxLandmarksColorSlider );
  colorMap->SetRampToLinear();
  colorMap->IndexedLookupOff();
  colorMap->Build();
  this->m_SecretData->m_ColorMap = colorMap;

  this->SlotUpdateSlice();
  this->SlotUpdateWindowLevel();
  
} // Initialize
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool PickPointsDialog::eventFilter( QObject * object, QEvent * event )
{

  // this captures all events occuring on the dialog

  // the triggering event was out vtk widget
  if ( object == this->ui->m_VTKOutput &&
    event->type() == QEvent::MouseButtonRelease )
    {

    cout << "Captured click event, and updating landmarks:" << endl;

    // rerender our landmarks
    this->SlotUpdateLandmarksSize();

    cout << "Updating landmarks done." << endl;

    } // event type

  // we might have already handled this event, but still pass it through
  return false;

} // eventFilter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void PickPointsDialog::SlotUpdateSlice()
{

  const VolumeImageConstPointer
    image = this->m_SecretData->m_Image;
  
  if ( !image )
    {
    cerr << "Error: No image." << endl;
    return;
    }

  const unsigned int whichSlice =
    static_cast< unsigned int >(
      this->ui->m_SliceSlider->value() );

  // render the point picker
  const vtkRenderWindowPointer
    renderWindow = this->ui->m_VTKOutput->GetRenderWindow();

  if ( !renderWindow )
    {
    cerr << "Error: No render window." << endl;
    return;
    }

  // extract the renderer
  const vtkRendererPointer
    oldRenderer = renderWindow->GetRenderers()->GetFirstRenderer();

  typedef vtkSmartPointer< vtkCamera > vtkCameraPointer;
  vtkCameraPointer camera;

  if ( oldRenderer )
    {
    
    // remember the camera
    camera = oldRenderer->GetActiveCamera();

    // remove the renderer
    renderWindow->RemoveRenderer( oldRenderer );

    } // oldRenderer

  // render the point picker
  RenderPointPicker(
    image.GetPointer(),
    whichSlice,
    true, // verbose
    renderWindow,
    this->GetLandmarks().GetPointer() );

  const vtkRendererPointer
    newRenderer = renderWindow->GetRenderers()->GetFirstRenderer();

  // update the camera
  if ( camera )
    newRenderer->SetActiveCamera( camera );

  if ( !newRenderer )
    {
    cout << "Error: No renderer!" << endl;
    return;
    }

  const auto props = newRenderer->GetViewProps();
  props->InitTraversal();

  cout << "# of props = " << props->GetNumberOfItems() << endl;
  
  // extract the (hopefully only?) prop
  this->m_SecretData->m_ImageProp = props->GetNextProp();
    
  cout << "Image prop = " << this->m_SecretData->m_ImageProp << endl;

  // update window and level
  this->SlotUpdateWindowLevel();

  // add back in landmarks since we've deleted the old renderer
  this->SlotUpdateLandmarksSize();

  // re-render
  renderWindow->Render();

} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PickPointsDialog::SecretData::SecretData( const VolumeImageConstPointer image ) :
m_Image( image )
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
PickPointsDialog::~PickPointsDialog()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void PickPointsDialog::SlotUpdateWindowLevel()
{

  // get the values
  const double
    denominator       = SecretData::m_SliderFactor,
    windowSliderValue = this->ui->m_WindowSlider->value(),
    levelSliderValue  = this->ui->m_LevelSlider ->value(),
    window            = windowSliderValue / denominator,
    level             = levelSliderValue  / denominator;

  cout << "Updating window / level to " << window << " / " << level << ":" << endl;

  // update the windowing / level
  UpdateVTKImageWindowLevel(
    this->m_SecretData->m_ImageProp, window, level );

  // rerender
  this->ui->m_VTKOutput->GetRenderWindow()->Render();
  this->ui->m_VTKOutput->GetRenderWindow()->Render();

  cout << "Updating window / level done." << endl;

} // SlotUpdateImageWindowLevel
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void PickPointsDialog::SlotUpdateLandmarksColor()
{
  
  // get the landmarks actor
  const vtkActorPointer landmarksActor = this->m_SecretData->m_LandmarksActor;
  if ( !landmarksActor ) return;

  // get the slider's value
  const double sliderValue =
    static_cast< double >( this->ui->m_LandmarksColorSlider->value() );
    
  const auto colorMap = this->m_SecretData->m_ColorMap;
  if ( !colorMap ) return;

  // convert it to a color
  double rgb[ 3 ];
  colorMap->GetColor( sliderValue, rgb );

  // update the landmarks color
  landmarksActor->GetProperty()->SetColor( rgb );

  /*
  cout << "Slider = " << sliderValue << endl;
  cout << "Color = [" <<
    rgb[ 0 ] << ", " << rgb[ 1 ] << ", " << rgb[ 2 ] << "]" << endl;
  */

  // rerender
  this->ui->m_VTKOutput->GetRenderWindow()->Render();

} // SlotUpdateLandmarksColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void PickPointsDialog::SlotUpdateLandmarksOpacity()
{

  // get the landmarks actor
  const vtkActorPointer landmarksActor = this->m_SecretData->m_LandmarksActor;
  if ( !landmarksActor ) return;

  // get the opacity value
  const float
    sliderValue = static_cast< float >( this->ui->m_LandmarksOpacitySlider->value() ),
    opacity     = sliderValue / SecretData::m_SliderFactor;

  // set it
  landmarksActor->GetProperty()->SetOpacity( opacity );

  // rerender
  this->ui->m_VTKOutput->GetRenderWindow()->Render();
  
} // SlotUpdateLandmarksOpacity
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void PickPointsDialog::SlotUpdateLandmarksSize()
{

  // get the renderer

  const vtkRenderWindowPointer
    renderWindow = this->ui->m_VTKOutput->GetRenderWindow();
  if ( !renderWindow ) return;

  const vtkRendererPointer
    renderer = renderWindow->GetRenderers()->GetFirstRenderer();
  if ( !renderer ) return;

  // remove existing landmarks
  const vtkActorPointer oldLandmarksActor = this->m_SecretData->m_LandmarksActor;
  if ( oldLandmarksActor )
    renderer->RemoveActor( oldLandmarksActor );

  // get the landmarks size
  const double
    sliderValue = static_cast< double >( this->ui->m_LandmarksSizeSlider->value() ),
    sphereRadius = sliderValue / SecretData::m_SliderFactor;

  // get the landmarks
  // on the given slice
  const unsigned int whichSlice =
    static_cast< unsigned int >( this->ui->m_SliceSlider->value() );
  const LandmarkSetConstPointer
    landmarksOnSlice = this->GetLandmarks( whichSlice );

  // create new landmarks actor with the given size

  vtkActorPointer newLandmarksActor =
    GenerateActorFromLandmarks( landmarksOnSlice, sphereRadius );
  if ( !newLandmarksActor ) return;

  this->m_SecretData->m_LandmarksActor = newLandmarksActor;

  // add it to the scene
  renderer->AddActor( newLandmarksActor );
  
  // update the opacity
  this->SlotUpdateLandmarksOpacity();

  // update the colors
  this->SlotUpdateLandmarksColor();

  // rerender
  renderWindow->Render();

} // SlotUpdateLandmarksSize
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer PickPointsDialog::GetLandmarks() const
{

  // get the point picker interactor styles
  typedef MouseInteractorStylePointPicker StyleType;

  const vtkRenderWindowPointer
    renderWindow = this->ui->m_VTKOutput->GetRenderWindow();

  const LandmarkSetPointer badOutput;
  
  if ( !renderWindow )
    {
    cerr << "Error: No render window!" << endl;
    return badOutput;
    }

  const StyleType * style = dynamic_cast< StyleType * >(
    renderWindow->GetInteractor()->GetInteractorStyle() );
  
  if ( !style )
    {
    cerr << "Error: No point picker style!" << endl;
    return badOutput;
    }

  // get the landmarks
  const LandmarkSetPointer landmarks( style->GetLandmarks() );

  if ( !landmarks )
    {
    cerr << "Error: Null landmark pointer!" << endl;
    return badOutput;
    }

  return landmarks;

} // GetLandmarks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetConstPointer PickPointsDialog::GetLandmarks(
  const unsigned int whichSlice ) const
{

  // get the image
  const VolumeImageBaseConstPointer image( this->m_SecretData->m_Image );
  if ( !image ) return LandmarkSetConstPointer();
  
  // get the real coordinates of that slice
  VolumeImageBase::IndexType indexOnSlice;
  indexOnSlice[ 0 ] = 0; // x
  indexOnSlice[ 1 ] = 0; // y
  indexOnSlice[ 2 ] = whichSlice; // z

  VolumePointType pointOnSlice;
  image->TransformIndexToPhysicalPoint( indexOnSlice, pointOnSlice );

  // get the z coordinate
  // and how "close" do we have to be to the z coordinate?
  typedef VolumePointType::ValueType CoordinateValueType;
  const CoordinateValueType
    sliceZCoordinate = pointOnSlice[ 2 ],
    coordinatePrecision = static_cast< CoordinateValueType >(
      image->GetSpacing()[ 2 ] / 1000.0 );

  // prepare the output landmark set
  const LandmarkSetPointer landmarksOnSlice = LandmarkSetType::New();

  // extract the existing landmarks
  const LandmarkSetConstPointer allLandmarks( this->GetLandmarks() );
  if ( !allLandmarks ) return allLandmarks;

  // loop through each existing point
  typedef LandmarkSetType::PointIdentifier PointIdentifier;
  const PointIdentifier numberOfLandmarks = allLandmarks->GetNumberOfPoints();
  PointIdentifier outputLandmarkIndex = 0;
  for ( PointIdentifier inputLandmarkIndex = 0;
    inputLandmarkIndex < numberOfLandmarks; ++inputLandmarkIndex )
    {

    // get the landmark
    const LandmarkPointType inputLandmark =
      allLandmarks->GetPoint( inputLandmarkIndex );

    // get the z coordinate
    const CoordinateValueType inputZCoordinate = inputLandmark[ 2 ];

    // see if it's close enough
    const CoordinateValueType
      difference = std::abs( inputZCoordinate - sliceZCoordinate );

    const bool currentLandmarkIsOnSlice = difference < coordinatePrecision;

    if ( currentLandmarkIsOnSlice )
      {

      // add it to our collection
      landmarksOnSlice->SetPoint( outputLandmarkIndex++, inputLandmark );

      } // currentLandmarkIsOnSlice 

    } // for inputLandmarkIndex 

  // output the landmarks on this slice
  return LandmarkSetConstPointer( landmarksOnSlice );

} // GetLandmarks (slice)
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
