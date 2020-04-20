

////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMouseInteractorStylePointPicker.h"
#include "ccipdDuplicateLandmarks.h"
#include "ccipdLandmarkToCursor.h" // for drawing a crosshair

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkPointSet.h>

  // vtk includes
  #include <vtkAbstractPicker.h>
  #include <vtkRenderWindow.h>
  #include <vtkRendererCollection.h>
  #include <vtkRenderWindowInteractor.h>
  #include <vtkActor.h>
  #include <vtkSmartPointer.h>

  // boost includes
  #include <boost/foreach.hpp>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////
class MouseInteractorStylePointPicker::SecretData
{

public:

  /// Constructor
  SecretData();

  /// Constructor for when you already have some landmarks.
  SecretData( const LandmarkSetConstPointer & initialLandmarks );

  /// A running collection of the points picked.
  LandmarkSetContainerPointer m_Landmarks;

  /// Whether to show the points to std::cout
  bool m_Verbose;

  static vtkActorPointer GetCrosshairs( const LandmarkPointType & landmark );

}; // class SecretData
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
MouseInteractorStylePointPicker::MouseInteractorStylePointPicker() :
ccipdNewPIMPLMacro
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
MouseInteractorStylePointPicker::MouseInteractorStylePointPicker(
  const LandmarkSetConstPointer &       initialLandmarks,
        vtkRenderWindow         * const renderWindow ) :
m_SecretData( new SecretData( initialLandmarks ) )
{
  
  // get the landmarks
  const LandmarkSetContainerPointer & landmarks = this->m_SecretData->m_Landmarks;

  // extract the rendering objects
  vtkRenderer * const renderer = renderWindow->GetRenderers()->GetFirstRenderer();

  // loop through each landmark
  BOOST_FOREACH( const LandmarkPointType & landmark, *landmarks )
    {

    // add some crosshairs to the scene
    renderer->AddActor( this->m_SecretData->GetCrosshairs( landmark ) );

    } // for each landmark

  // re-render
  renderWindow->Render();
  
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
MouseInteractorStylePointPicker::SecretData::SecretData() :
m_Landmarks( LandmarkSetContainer::New() )
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
MouseInteractorStylePointPicker::SecretData::SecretData(
  const LandmarkSetConstPointer & initialLandmarks ) :
m_Landmarks( LandmarkSetContainer::New() )
{

  // we must duplicate them
  if ( initialLandmarks && initialLandmarks->GetPoints() )
    this->m_Landmarks = DuplicateLandmarkContainer( initialLandmarks->GetPoints() );

} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
MouseInteractorStylePointPicker::~MouseInteractorStylePointPicker()
{
} // Destructor
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
MouseInteractorStylePointPicker * MouseInteractorStylePointPicker::New()
{
  return new MouseInteractorStylePointPicker;
} // New
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
void MouseInteractorStylePointPicker::OnRightButtonDown()
{

  // show output?
  const bool verbose = this->GetVerbose();

  const int controlKey = this->Interactor->GetControlKey();
  if ( !controlKey )
    {
    if ( verbose )
      std::cout << "No control key. Ignoring event." << endl;
    }
  else
    {

    // extract the position
    const int * const eventPosition = this->Interactor->GetEventPosition();
    const int
      eventPositionX = eventPosition[ 0 ],
      eventPositionY = eventPosition[ 1 ],
      eventPositionZ = 0; // always 0

    if ( verbose )
      {
      std::cout << "Pixel position = " <<
        eventPositionX << " " << eventPositionY << std::endl;
      } // verbose
      
    // extract the rendering objects
    vtkRenderWindowInteractor * const
      interactor = this->Interactor;
    vtkRenderWindow * const
      renderWindow = interactor->GetRenderWindow();
    vtkRenderer * const
      renderer = renderWindow->GetRenderers()->GetFirstRenderer();

    // pick where the point was
    this->Interactor->GetPicker()->Pick(
      eventPositionX,
      eventPositionY,
      eventPositionZ,
      renderer );
  
    // convert it to real world coordinates
    double realWorldCoordinates[ LandmarkDimension ];
    this->Interactor->GetPicker()->GetPickPosition( realWorldCoordinates );

    if ( verbose )
      {
      std::cout << "Real world coordiantes = " <<
        realWorldCoordinates[ 0 ] << " " <<
        realWorldCoordinates[ 1 ] << " " <<
        realWorldCoordinates[ 2 ] << std::endl;
      } // verbose
  
    // convert it to the landmark type
    LandmarkPointType landmark = LandmarkPointType();
    for ( unsigned int dimension = 0; dimension < LandmarkDimension; ++dimension )
      landmark[ dimension ] = static_cast< LandmarkType >(
        realWorldCoordinates[ dimension ] );

    // add it to our display
    renderer->AddActor( this->m_SecretData->GetCrosshairs( landmark ) );
    
    // re-render
    renderWindow->Render();

    // give the landmark to our collection
    LandmarkSetContainer & landmarks = *this->m_SecretData->m_Landmarks;
    const LandmarkSetContainer::ElementIdentifier
      elementID = landmarks.Size();
    landmarks.InsertElement( elementID, landmark );

    if ( verbose )
      {
      cout << "After adding this landmark, there are " <<
        elementID + 1 << " landmarks." << endl;
      }

    } // control key

  // Forward events
  Superclass::OnRightButtonDown();
      
} // OnRightButtonDown
////////////////////////////////////////////////////////////////////////////////////////////////

    void MouseInteractorStylePointPicker::OnLeftButtonDown() 
    {
      std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1] << std::endl;
      this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0], 
                         this->Interactor->GetEventPosition()[1], 
                         0,  // always zero.
                         this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
      double picked[3];
      this->Interactor->GetPicker()->GetPickPosition(picked);
      std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
      // Forward events
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

////////////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer MouseInteractorStylePointPicker::SecretData::GetCrosshairs(
  const LandmarkPointType & landmark )
{

  // define our size
  const LandmarkType
    epsilon = 1e-2f,
    xRadius = 10,
    yRadius = 10,
    zRadius = -epsilon * 2;

  // create a landmark slightly in front of the image
  LandmarkPointType landmarkToDisplay = landmark;
  landmarkToDisplay[ 2 ] += epsilon;

  const vtkActorPointer cursorActor = CreateCursorAtLandmark(
    landmarkToDisplay, xRadius, yRadius, zRadius );

  return cursorActor;

} // AddCrosshairs
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer MouseInteractorStylePointPicker::GetLandmarks() const
{

  // extract the landmarks
  const LandmarkSetContainerPointer & landmarkContainer =
    this->m_SecretData->m_Landmarks;

  // create an output type
  const LandmarkSetPointer landmarkSet = LandmarkSetType::New();

  // we must clone the points so that the landmarkSet can modify them
  // without messing up the original stored points
  landmarkSet->SetPoints( DuplicateLandmarkContainer( landmarkContainer ) );

  // output
  return landmarkSet;

} // GetLandmarks
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
// Get/Set functions
ccipdSetByvalImplementMacro( MouseInteractorStylePointPicker, Verbose, bool )
ccipdGetConstImplementMacro( MouseInteractorStylePointPicker, Verbose, bool )
////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace

 