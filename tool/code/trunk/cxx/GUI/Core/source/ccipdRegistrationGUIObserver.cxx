
//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdRegistrationGUIObserver.h"
#include "ccipdApplyTransformation.h" // for creating the moved image
#include "ccipdUpdateVTKImageProp.h"  // for updating the display

// std includes
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegistrationMethod.h> // for storing the images, etc.
  #include <itkRGBPixel.h>
  // vtk includes
  #include <vtkSmartPointer.h>
  #include <vtkProp.h>             // general base class
  #include <vtkImageSlice.h>       // our specific prop
  #include <vtkImageSliceMapper.h> // for helping us pick the slice #
  #include <vtkImageProperty.h>    // for storing the properties
  #include <vtkRenderWindow.h>     // where we update
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
class RegistrationGUIObserver::SecretData
{

public: // to the encapsulating class
  SecretData(); ///< Constructor

  // Variables

  /// What prop should we be updating every iteration?
  vtkPropPointer m_ImagePropToUpdate;

  /// Where should we output?
  vtkRenderWindowPointer m_RenderWindow;

  /// What type of image will we store?
  typedef RegistrationGUIObserver::ImageConstPointer ImageConstPointer;

  /// Store the image to update.
  ImageConstPointer m_MovingImage;

  /// What type of image is the template?
  typedef RegistrationGUIObserver::TemplateImageConstPointer TemplateImageConstPointer;

  /// Store the template image.
  TemplateImageConstPointer m_MovedTemplateImage;

  /// What type of transform will we use?
  typedef RegistrationGUIObserver::TransformPointer TransformPointer;

  /// Store the transform to update.
  TransformPointer m_Transform;

  /// Make 0-valued pixels transparent?
  bool m_MakeZeroTransparent;

}; // SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegistrationGUIObserver::SecretData::SecretData() :
m_MakeZeroTransparent( false )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegistrationGUIObserver::RegistrationGUIObserver() :
Superclass(),
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegistrationGUIObserver::~RegistrationGUIObserver()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegistrationGUIObserver::Execute(
  const itk::Object * caller, const itk::EventObject & event)
{

  // call the original execute function
  this->Superclass::Execute( caller, event );

  // if we shouldn't update this iteration, exit
  if ( !this->Superclass::GetUpdatingThisIteration() )
    return;

  // typedef's
  typedef SecretData::ImageConstPointer ImageConstPointer;

  // extract the data
  const vtkRenderWindowPointer & renderWindow =
    this->m_SecretData->m_RenderWindow;
  vtkPropPointer & prop =
    this->m_SecretData->m_ImagePropToUpdate;
  const ImageConstPointer & movingImage =
    this->m_SecretData->m_MovingImage;

  // some simple error checks
  if ( !prop || !renderWindow || !movingImage )
    {
    cerr << "Error: You must set the " <<
      "prop, render window, moving image, and transform " <<
      "prior to using this observer!" << endl;
    return;
    } // prop

  // try to convert it to an image slice
  cout << "Casting to image slice:" << endl;

  vtkImageSlice * const imageSlice =
    dynamic_cast< vtkImageSlice * >( prop.GetPointer() );

  if ( !imageSlice )
    {
    cerr << "Error: unable to cast to image slice." << endl;
    return;
    } // imageSlice

  cout << "Casting to image slice done." << endl;

  // save the properties of this slice
  const vtkSmartPointer< vtkImageProperty > properties =
    imageSlice->GetProperty();
  
  // try to cast the mapper into the correct type
  cout << "Casting to image slice mapper:" << endl;

  vtkImageSliceMapper * const mapper =
    dynamic_cast< vtkImageSliceMapper * >( imageSlice->GetMapper() );

  if ( !mapper )
    {
    cerr << "Error: unable to cast to image slice mapper." << endl;
    return;
    } // mapper

  cout << "Casting to image slice mapper done." << endl;

  // extract the slice number
  const int sliceNumber = mapper->GetSliceNumber();

  // get the moved image
  cout << "Calculating moved image:" << endl;
  const VolumeImageConstPointer movedImage = this->GetMovedImage();
  if ( !movedImage )
    {
    cerr << "Unable to calculate moved image." << endl;
    return;
    }
  cout << "Calculating moved image done." << endl;

  // okay, now update the display with the desired slice of the moved image
  const bool
    makeZeroTransparent = this->GetMakeZeroTransparent(),
    verbose             = true;
  cout << "Updating display:" << endl;
  UpdateVTKImageProp(
    renderWindow,
    prop,
    movedImage,
    sliceNumber,
    makeZeroTransparent,
    verbose );
  cout << "Updating display done." << endl;

  // set the properties
  vtkImageSlice * const outputImageSlice =
    dynamic_cast< vtkImageSlice * >( prop.GetPointer() );
  if ( imageSlice )
    {
    //cout << "Old properties = " << *outputImageSlice->GetProperty() << endl;
    cout << "Setting properties:" << /*endl << *properties <<*/ endl;
    outputImageSlice->SetProperty( properties );
    properties->Modified(); // we need this to update the images
    cout << "Setting properties done." << endl;
    }
  
  // rerender
  cout << "Rerendering:" << endl;
  renderWindow->Render();
  renderWindow->Render();
  cout << "Rerendering done." << endl;
  
} // Execute
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer RegistrationGUIObserver::GetMovedImage() const
{

  // now, we must apply the current transformation
  
  // extract the parameters
  typedef Superclass::ParametersType ParametersType;
  const ParametersType parameters = 
    this->Superclass::GetParameters();

  const TemplateImageConstPointer
    templateImage = this->GetMovedTemplateImage();

  const ImageConstPointer
    movingImage = this->GetMovingImage();

  const TransformPointer
    & transform = this->m_SecretData->m_Transform;

  // check to make sure we have everything
  if ( !movingImage || !transform )
    return ImageConstPointer();

  // now, we must update the rendered image display
  
  // extract the transform
  cout << "Cloning the transform and setting the parameters:" << endl;

  // give the parameters to the transform
  try
    {
    transform->SetParameters( parameters );
    }
  catch( ... )
    {
    // ignore errors
    cerr << "Error creating the transform." << endl;
    return ImageConstPointer();
    }

  cout << "Cloning the transform and setting the parameters done." << endl;

  // create the moved image
  cout << "Apply transform to moving image to create moved image:" << endl;

  const bool hasTemplateImage = templateImage.IsNotNull();
  if ( !hasTemplateImage ) cout << "(Note: No template image set)" << endl;
  
  const ImageConstPointer movedImage(
    hasTemplateImage ?
      ApplyTransform( transform, movingImage, templateImage ) :
      ApplyTransform( transform, movingImage ) );

  cout << "Apply transform to moving image to create moved image done." << endl;

  // output
  return movedImage;

} // GetMovedImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Set/Get functions
ccipdSetByrefImplementMacro( RegistrationGUIObserver, ImagePropToUpdate,
  vtkPropPointer )
ccipdSetByrefImplementMacro( RegistrationGUIObserver, RenderWindow,
  vtkRenderWindowPointer )
ccipdSetByvalImplementMacro( RegistrationGUIObserver, MovingImage,
  RegistrationGUIObserver::ImageConstPointer )
ccipdGetConstImplementMacro( RegistrationGUIObserver, MovingImage,
  RegistrationGUIObserver::ImageConstPointer )
ccipdSetByvalImplementMacro( RegistrationGUIObserver, MovedTemplateImage,
  RegistrationGUIObserver::TemplateImageConstPointer )
ccipdGetConstImplementMacro( RegistrationGUIObserver, MovedTemplateImage,
  RegistrationGUIObserver::TemplateImageConstPointer )
ccipdSetByrefImplementMacro( RegistrationGUIObserver, Transform,
  RegistrationGUIObserver::TransformPointer )
ccipdGetConstImplementMacro( RegistrationGUIObserver, MakeZeroTransparent,
  bool )
ccipdSetByvalImplementMacro( RegistrationGUIObserver, MakeZeroTransparent,
  bool )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
