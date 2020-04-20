


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSeguiWindow.h"
#include "ccipdLoadGUI.h"
#include "ccipdLoadVolumeGUI.h"
#include "ccipdSaveGUI.h"
#include "ccipdMFASegmenterOptionsGUI.h"
#include "ccipdMFASegmenterCorrection.h"
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdUpdateVTKImageProp.h"
#include "ccipdUpdateVTKImageSlice.h"
#include "ccipdUpdateVTKPropTransparency.h"
#include "ccipdUpdateVTKImageWindowLevel.h"
#include "ccipdUpdateWindowLevelSliders.h"
#include "ccipdStudy.h"
#include "ccipdRegisterMFAAlgorithm.h"
#include "ccipdRegistrationGUIObserver.h"
#include "ccipdMFARenderingRefresh.h"
#include "ccipdUniformDistribution.h" // for random generators
#include "ccipdFeatureID.h"
#include "ccipdTextureFeatureParser.h"
#include "ccipdIsosurfaceRendering.h"
#include "ccipdFeatureCollection.h"
#include "ccipdSaveImageGUI.h"
#include "ccipdPickPointsOnVolume.h"
#include "ccipdBoundingBoxCreation.h"
#include "ccipdBoundingBoxToActor.h" // for bounding box rendering
#include "ccipdDuplicateImage.h"
#include "ccipdLevelsetToSurface.h"
#include "ccipdResizeImage.h"
#include "ccipdLandmarksToActor.h"
#include "ccipdGetLargestContiguousRegion.h"

#include "ccipdDisableWarningsMacro.h"
 
// itk includes
#include <itkAddImageFilter.h>
#include <itkRGBPixel.h>

// vtk includes
#include <vtkProp.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkActorCollection.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QComboBox>
#include "ui_ccipdSeguiWindow.h"
#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER
// boost includes
#include <boost/foreach.hpp>

#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#endif

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
class SeguiWindow::SecretData : boost::noncopyable
{

public:
  explicit SecretData( SeguiWindow * const encapsulatingClass ); ///< Constructor

  SeguiWindow * const
    m_EncapsulatingClass;

  typedef MFASegmenterCorrection::Pointer SegmenterPointer;

  SegmenterPointer
    m_MFASegmenter;

  StudyConstPointer
    m_Study;
  
  /**
   * We only apply this when saving the output
   * because VTK doesn't render this correctly.
   */
  VolumeImageBase::DirectionType
    m_OriginalDirection;

  string
    m_FileName;
  
  vtkPropPointer
    m_ImageProp,    ///< Store the vtk prop for an image.
    m_LevelsetProp, ///< Store a prop for a levelset
    m_BoundingBoxProp,
    m_FixedCorrectionPointsProp;

  VolumeImageConstPointer
    m_CurrentImage; ///< Currently selected image (or null)
  
  vtkRendererPointer
    m_Renderer;

  vtkActorCollectionPointer
    m_LevelsetActors;

  typedef double ColorType;
  typedef UniformDistribution<
    ColorType
  > ColorDistributionType;
  ColorDistributionType::Pointer
    m_RandomColorDistribution;


  QString m_InputVolumeString;

  bool
    m_UpdatingComboList;

  /// Output the segmenter or give an error if none.
  ccipdGetConstDeclareMacro( MFASegmenter, SegmenterPointer )

  /// Update the title of the window with the segmenter file name.
  void UpdateTitle();

  /// Populate the image combo with the possible images
  void UpdateImageCombo();

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////
SeguiWindow::SeguiWindow() :
m_SecretData( new SecretData( this ) )
{
  this->m_SecretData->m_InputVolumeString = tr( "Input Volume" );
  this->ui = new Ui_SeguiWindow;
  this->ui->setupUi( this );

  this->m_SecretData->UpdateTitle();
  this->m_SecretData->UpdateImageCombo();
  
  // VTK Renderer
  const vtkRendererPointer
    renderer = vtkRendererPointer::New();

  const double
    backgroundR = 0,
    backgroundG = 0,
    backgroundB = 0;

  renderer->SetBackground( backgroundR, backgroundG, backgroundB );
  
  // create an interactor style
  typedef vtkSmartPointer<
    vtkInteractorStyleTrackballCamera
  > vtkInteractorStylePointer;
  const vtkInteractorStylePointer interactorStyle =
    vtkInteractorStylePointer::New();

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

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SeguiWindow::SecretData::SecretData( SeguiWindow * const encapsulatingClass ) :
m_EncapsulatingClass( encapsulatingClass ),
m_LevelsetActors( vtkActorCollectionPointer::New() ),
m_RandomColorDistribution( ColorDistributionType::New() ),
m_UpdatingComboList( false )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SeguiWindow::~SeguiWindow()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SecretData::UpdateImageCombo()
{

  // declare that we are currently updating
  this->m_UpdatingComboList = true;

  const bool verbose = true;
  if ( verbose ) cout << "Clearing image combo:" << endl;
  QComboBox & imageCombo = *this->m_EncapsulatingClass->ui->m_ImageCombo;
  imageCombo.clear();
  if ( verbose ) cout << "Clearing image combo done." << endl;

  // add images from the study
  if ( this->m_Study )
    {
    if ( verbose ) cout << "Adding input volume:" << endl;
    imageCombo.addItem( this->m_InputVolumeString );
    if ( verbose ) cout << "Adding input volume done." << endl;
    }

  const SegmenterPointer segmenter = this->m_MFASegmenter;

  if ( !segmenter )
    {
    if ( verbose ) cout << "No segmenter." << endl;
    }
  else
    {
    // we will store all the features to add to our dropdown
    FeatureIDConstCollection whichFeatures;

    // extract the features to use
    const StudyPointer movedStudy = segmenter->GetMovedStudy();
    if ( !movedStudy )
      {
      if ( verbose ) cout << "Error: No moved study." << endl;
      } // movedStudy
    else
      {
      const FeatureCollectionPointer featureCollection =
        movedStudy->GetFeatureCollection();
      if ( !featureCollection )
        {
        if ( verbose ) cout << "Error: No feature collection." << endl;
        } // featureCollection
      else
        {

        if ( verbose ) cout << "Getting output features:" << endl;
        const FeatureIDConstCollection whichOutputFeatures =
          featureCollection->GetWhichFeatures();
        if ( verbose ) cout << "Getting output features done." << endl;
        
        // insert the output features into our collection
        whichFeatures.insert(
          whichFeatures.begin(),
          whichOutputFeatures.cbegin(),
          whichOutputFeatures.cend() );

        } // featureCollection

      // add in the reconstructable features from the segmenter
      const FeatureIDConstCollection
        whichReconstructableFeatures = segmenter->GetAllFeatures();

      whichFeatures.insert(
        whichFeatures.begin(),
        whichReconstructableFeatures.cbegin(),
        whichReconstructableFeatures.cend() );

      // loop through each feature
      BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
        {

        if ( !whichFeature ) continue;

        // we will store the hash for the feature
        const HashValueType currentFeatureIDHash = whichFeature->GetHash();

        if ( verbose ) cout << "Adding feature " << currentFeatureIDHash << endl;

        // we store whether it is a levelset or texture
        const bool isTexture = whichFeature->HasTextureFeature();

        // the string is either the texture
        // name or the feature description
        const FeatureDescriptionType
          inputDescription = whichFeature->GetDescription(),
          description = ( inputDescription.empty() && isTexture ) ?
            TextureFeatureParser::GetTextureFeatureString(
              *whichFeature->GetTextureFeature() ) :
            inputDescription;

        // the data for the combo item will be the hash of the feature
        const QVariant hashData = QVariant::fromValue( currentFeatureIDHash );
        imageCombo.addItem( description.c_str(), hashData );

        if ( verbose ) cout << "Adding feature done." << endl;

        } // for each feature

      } // movedStudy

    } // MFA segmenter

  // we are done updating
  this->m_UpdatingComboList = false;

} // UpdateImageCombo
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SecretData::UpdateTitle()
{

  const QString
    coreTitle = QString( "Segui" ) + QChar( 0x2122 ), // trademark symbol
    fileName = this->m_FileName.c_str();

  // do we have an unsaved segmenter without a file name?
  const bool
    noFileName = fileName.isEmpty(),
    untitled = noFileName && this->m_MFASegmenter;

  const QString
    additionalTitle = 
      untitled ? " - Untitled*" :
        noFileName ? "" : 
          QString( " - " ) + fileName,
    finalTitle = coreTitle + additionalTitle;

  // set the title
  this->m_EncapsulatingClass->setWindowTitle( finalTitle );

} // UpdateTitle
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotUpdateWhichImage()
{

  // don't do anything while we are actually updating the list of combo items
  if ( this->m_SecretData->m_UpdatingComboList )
    return;

  // we need to get the image
  const QComboBox & imageCombo = *this->ui->m_ImageCombo;
  const QString text = imageCombo.currentText();

  // first check if we have the input volume

  VolumeImageConstPointer image;
  FeatureIDConstPointer   whichFeature;
  
  const SecretData::SegmenterPointer segmenter = 
    this->m_SecretData->m_MFASegmenter;

  const bool transformed = this->ui->m_TransformedCheck->isChecked();

  if ( text.compare( this->m_SecretData->m_InputVolumeString ) == 0 )
    {
    const StudyConstPointer study = ( transformed && segmenter ) ?
      segmenter->GetMovedStudy() : this->m_SecretData->m_Study;
    if ( study )
      {
      image = study->GetVolume().GetPointer();
      }
    else
      {
      cout << "Warning: no study." << endl;
      }
    } // input volume
  else if ( segmenter )
    {

    // we need a feature from the data
    // stored in the item
    const auto index = imageCombo.currentIndex();
    if ( index < 0 ) return;
    const QVariant data = imageCombo.itemData( index );

    // extract the hash of the feature
    const HashValueType featureHash =
      data.value< HashValueType >();

    // get the existing feature id's and find the hash

    // we will include all the moved, cached features
    const FeatureCollectionPointer
      movedFeatures = segmenter->GetMovedStudy()->GetFeatureCollection();
      
    FeatureIDConstCollection possibleFeatures =
      movedFeatures->GetWhichFeatures();

    // we will add in the reconstruction features
    const FeatureIDConstCollection reconstructableFeatures =
      segmenter->GetAllFeatures();

    possibleFeatures.insert(
      possibleFeatures.begin(),
      reconstructableFeatures.cbegin(),
      reconstructableFeatures.cend() );

    BOOST_FOREACH( const FeatureIDConstPointer & possibleFeature,
      possibleFeatures )
      {

      if ( possibleFeature && possibleFeature->GetHash() == featureHash )
        {
        whichFeature = possibleFeature;
        cout << "Feature = " << featureHash << endl;
        break;
        } // found

      } // possibleFeature

    // if we've already stored this
    // feature in our feature collection,
    // then use it
    if ( movedFeatures->HasFeature( whichFeature ) )
      {
      cout << "Using cached moved feature." << endl;
      image = movedFeatures->GetFeature( whichFeature ).at(0);
      }
    else
      {
      // nope - we need to compute it
      cout << "Computing feature reconstruction:" << endl;

      if ( transformed )
        {
        image = segmenter->GetTransformedReconstruction( whichFeature );
        }
      else
        {
        image = segmenter->GetReconstruction( whichFeature );
        }

      cout << "Computing feature reconstruction done." << endl;

      } // has feature

    } // not input volume

  // store the image
  this->m_SecretData->m_CurrentImage = image;
  
  if ( !image )
    {
    cout << "Warning: No image for the feature." << endl;
    return;
    }
    
  const bool
    makeZeroTransparent = false,
    verbose             = true;

  if ( verbose ) cout << "Updating sliders:" << endl;

  // update our sliders
  const auto numberOfSlices = 
    image->GetBufferedRegion().GetSize()[ 2 ];

  if ( verbose ) cout << "# of slices = " << numberOfSlices << endl;

  this->ui->m_SliceSlider->setMaximum( static_cast< int >( numberOfSlices ) );

  UpdateWindowLevelSliders(
    image,

    this->ui->m_ImageWindowSlider,
    this->ui->m_ImageLevelSlider,
    100 );

  // middle slice
  const unsigned int slice = 
    static_cast< unsigned int >( numberOfSlices / 2 );

  const vtkRenderWindowPointer
    renderWindow = this->ui->m_OutputVTKWidget->GetRenderWindow();
  const vtkRendererPointer
    renderer = renderWindow->GetRenderers()->GetFirstRenderer();
  
  if ( verbose ) cout << "Rendering image:" << endl;

  UpdateVTKImageProp(
    renderWindow,
    this->m_SecretData->m_ImageProp,
    image,
    slice,
    makeZeroTransparent,
    verbose );

  // update the slice slider
  this->ui->m_SliceSlider->setValue( slice );

  if ( verbose ) cout << "Rendering image done." << endl;

  if ( verbose ) cout << "Updating levels / transparency:" << endl;

  this->SlotUpdateImageWindowLevel();
  this->SlotUpdateTransparency();

  if ( verbose ) cout << "Updating levels / transparency done." << endl;

  // special considerations for levelsets
  const bool isLevelset = whichFeature && !whichFeature->HasTextureFeature();
  
  if ( isLevelset )
    {
    
    cout << "This is a levelset feature." << endl;
    
    // remove the prop
    if ( verbose ) cout << "Removing existing levelset prop:" << endl;
    this->SlotClearLevelset();
    if ( verbose ) cout << "Removing existing image levelset done." << endl;

    // generate the levelset actor
    if ( verbose ) cout << "Generating levelset actor:" << endl;
    const InternalPixelType levelsetValue =
      -std::numeric_limits< InternalPixelType >::epsilon();
    const vtkActorPointer levelsetActor =
      GenerateVTKActorFromIsosurface( image, levelsetValue );
    if ( verbose ) cout << "Generating levelset actor done." << endl;

    // render the levelset
    if ( verbose ) cout << "Adding levelset actor:" << endl;
    this->m_SecretData->m_LevelsetProp = levelsetActor;
    renderer->AddActor( levelsetActor );
    if ( verbose ) cout << "Adding levelset actor done." << endl;
    
    // update transparency
    if ( verbose ) cout << "Updating transparency:"<< endl;
    this->SlotUpdateTransparency();
    if ( verbose ) cout << "Updating transparency done."<< endl;

    } // isLevelset

  // rerender
  renderer->Render();
  
} // SlotUpdateWhichImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// Save the image
void SeguiWindow::SlotSaveCurrentImage()
{

  // apply it to the image
  const VolumeImageConstPointer
    currentImage = this->m_SecretData->m_CurrentImage;

  if ( !currentImage ) return;

  const VolumeImagePointer
    imageWithDirection = DuplicateImage( currentImage.GetPointer() );

  if ( !imageWithDirection ) return;

  // apply the old direction that we stored
  const VolumeImageBase::DirectionType
    direction = this->m_SecretData->m_OriginalDirection;

  imageWithDirection->SetDirection( direction );

  SaveImage( imageWithDirection.GetPointer(), this, true );

} // SlotSaveCurrentImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotClearLevelset()
{

  if ( this->m_SecretData->m_LevelsetProp )
    {

    const vtkRenderWindowPointer
      renderWindow = this->ui->m_OutputVTKWidget->GetRenderWindow();
    const vtkRendererPointer
      renderer = renderWindow->GetRenderers()->GetFirstRenderer();
        
    renderer->RemoveActor( this->m_SecretData->m_LevelsetProp );
    
    renderWindow->Render();
    
    } // levelset prop
  else
    {
    cout << "Note: No levelset to clear." << endl;
    }
      
} // SlotClearLevelset
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotUpdateTransparency()
{

  // get our levelset actors
  const vtkActorCollectionPointer
    levelsetActors = this->m_SecretData->m_LevelsetActors;
  if ( !levelsetActors ) return;

  // get our transparency values
  const float
    denominator         = 100.0f,
    levelsetSliderValue = static_cast< float >( this->ui->m_LevelsetTransparencySlider->value() ),
    levelsetOpacity     = levelsetSliderValue / denominator,
    imageSliderValue    = static_cast< float >( this->ui->m_ImageTransparencySlider->value() ),
    imageOpacity        = imageSliderValue / denominator;

  // loop through each actor in our collection
  vtkCollectionSimpleIterator iterator;
  vtkActorPointer levelsetActor;
  for ( levelsetActors->InitTraversal( iterator ); 
    levelsetActor = levelsetActors->GetNextActor( iterator ); )
    {
    UpdateVTKPropTransparency( levelsetActor, levelsetOpacity );
    } // for each levelset actor

  UpdateVTKPropTransparency( this->m_SecretData->m_LevelsetProp, levelsetOpacity );
  
  // update the image transparency
  const vtkPropPointer & imageProp = this->m_SecretData->m_ImageProp;
  if ( imageProp )
    {
    UpdateVTKPropTransparency( imageProp, imageOpacity );
    } // imageProp

  // rerender
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotUpdateTransparency
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotRandomizeColors()
{

  // get our levelset actors
  const vtkActorCollectionPointer
    levelsetActors = this->m_SecretData->m_LevelsetActors;

  if ( !levelsetActors ) return;

  // create a random distribution
  SecretData::ColorDistributionType & randomDistribution =
    *this->m_SecretData->m_RandomColorDistribution;
  randomDistribution.SetMinimumValue( 0.0 );
  randomDistribution.SetMaximumValue( 1.0 );
  
  // loop through each actor in our collection
  vtkCollectionSimpleIterator iterator;
  vtkActorPointer levelsetActor;
  for ( levelsetActors->InitTraversal( iterator ); 
    levelsetActor = levelsetActors->GetNextActor( iterator ); )
    {

    // set the transparency property
    const SecretData::ColorType
      red   = randomDistribution.GetRandomValue(),
      green = randomDistribution.GetRandomValue(),
      blue  = randomDistribution.GetRandomValue();

    levelsetActor->GetProperty()->SetColor( red, green, blue );

    } // for each levelset actor

  // rerender
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotRandomizeColors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotExit() 
{

  this->hide();
  qApp->exit();

} // SlotExit
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotAbout() 
{

  QMessageBox::about( this, tr( "About Segui" ),
    tr( "\
Segui (Segmentation Graphical User Interface) \
is a program to use the MFA segmenter.\n\n\
Author: Rob Toth" ) );

} // SlotAbout
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotNewMFASegmenter()
{

  this->m_SecretData->m_MFASegmenter =
    SecretData::SegmenterPointer::element_type::New();

  this->m_SecretData->m_FileName.clear();
  this->m_SecretData->UpdateTitle();

} // SlotNewMFASegmenter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotOpenMFASegmenter() 
{

  // try to load the segmenter
  const bool verbose = true;
  
  cout << "Loading segmenter:" << endl;
  
  const auto loadedSegmenterAndFileName =
    LoadObjectAndFileNameGUI<
      SecretData::SegmenterPointer::element_type >( this, verbose );

  cout << "Loading segmenter done." << endl;
  
  // if we loaded it, set it
  const SecretData::SegmenterPointer
    loadedSegmenter = loadedSegmenterAndFileName.first;

  if ( loadedSegmenter )
    {
    this->m_SecretData->m_MFASegmenter = loadedSegmenter;

    // also set the file name
    this->m_SecretData->m_FileName = loadedSegmenterAndFileName.second;

    this->m_SecretData->UpdateTitle();

    // rerender
    cout << "Initializing MFA:" << endl;
    loadedSegmenter->Initialize();
    cout << "Initializing MFA done." << endl;

    cout << "Rerendering:" << endl;
    this->SlotRerender();
    cout << "Rerendering done." << endl;

    } // loadedSEgmenter

} // SlotOpenMFASegmenter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotCloseMFASegmenter() 
{

  // clear all our variables
  this->m_SecretData->m_MFASegmenter.reset();
  this->m_SecretData->m_FileName.clear();
  this->m_SecretData->m_LevelsetActors->RemoveAllItems();

  // update our ui
  this->m_SecretData->UpdateTitle();
  this->SlotRerender();

} // SlotCloseMFASegmenter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotOpenStudy() 
{

  // try to load the segmenter
  const bool verbose = true;

  if ( verbose ) cout << "Loading study:" << endl;

  const StudyPointer loadedStudy =
    LoadObjectGUI< Study >( this, verbose );

  if ( !loadedStudy )
    {
    cerr << "Error: No study." << endl;
    return;
    }

  if ( verbose ) cout << "Loading study done." << endl;

  VolumeImageBase::DirectionType identityDirection;
  identityDirection.SetIdentity();

  // remove the orientation and store it for later
  if ( loadedStudy->HasVolume() )
    {

    // get the volume
    const VolumeImageConstPointer originalVolume( loadedStudy->GetVolume() );

    this->m_SecretData->m_OriginalDirection = originalVolume->GetDirection();

    const VolumeImagePointer volumeWithoutDirection =
      DuplicateImage( originalVolume.GetPointer() );
    // create an identity direction
    
    // remove direction
    volumeWithoutDirection->SetDirection( identityDirection );

    // set it with a new volume
    loadedStudy->SetVolume( volumeWithoutDirection );

    } // has Volume

  // if we loaded it, set it
  if ( loadedStudy )
    {

    this->m_SecretData->m_Study = loadedStudy;

    cout << "# of masks = " << loadedStudy->GetNumberOfMasks() << endl;

    const MaskObjectCollectionType maskObjects = loadedStudy->GetMaskObjects();

    BOOST_FOREACH( const MaskObjectType & maskObject, maskObjects )
      {

      // get the mask
      const VolumeMaskImageConstPointer originalMask(
        loadedStudy->GetMask( maskObject ) );

      // get the direction
      const auto maskDirection = originalMask->GetDirection();

      if ( !( maskDirection == this->m_SecretData->m_OriginalDirection ) )
        {
        cerr << "Warning: Mask direction is not the same as the volume direction!" <<
          endl << "Volume direction = " << endl <<
          this->m_SecretData->m_OriginalDirection << endl <<
          "Mask direction = " << endl << maskDirection << endl;
        } // !=

      // duplicate it
      const VolumeMaskImagePointer maskWithoutDirection =
        DuplicateImage( originalMask.GetPointer() );

      // remove direction
      maskWithoutDirection->SetDirection( identityDirection );

      // set it with the new mask
      loadedStudy->SetMask( maskWithoutDirection, maskObject );

      } // for each mask object

    } // loadedStudy

  // update the image
  this->m_SecretData->UpdateImageCombo();
  
  // clear existing things from the old study
  this->SlotClearBoundingBox();
  this->SlotClearCorrectionAndPoints();

  // rerender
  this->SlotRerender();

} // SlotOpenStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotOpenDICOM() 
{

    // try to load the segmenter
  const bool verbose = true;

  if ( verbose ) cout << "Loading volume:" << endl;

  const VolumeImageConstPointer loadedVolume(
    LoadVolumeFromDirectory( this, verbose ) );
  
  if ( !loadedVolume )
    {
    cerr << "Error: No volume loaded." << endl;
    return;
    }

  if ( verbose ) cout << "Loading volume done." << endl;


  VolumeImageBase::DirectionType identityDirection;
  identityDirection.SetIdentity();

  // remove the orientation and store it for later
  const VolumeImageConstPointer originalVolume( loadedVolume );

  this->m_SecretData->m_OriginalDirection = originalVolume->GetDirection();

  const VolumeImagePointer volumeWithoutDirection =
    DuplicateImage( originalVolume.GetPointer() );

    
  // remove direction
  volumeWithoutDirection->SetDirection( identityDirection );

    // set study with a new volume
  const StudyPointer newStudy = Study::New();
  newStudy->SetVolume( volumeWithoutDirection );

  this->m_SecretData->m_Study = newStudy;

  // update the image
  this->m_SecretData->UpdateImageCombo();
  
  // clear existing things from the old study
  this->SlotClearBoundingBox();
  this->SlotClearCorrectionAndPoints();

  // rerender
  this->SlotRerender();

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotEditMFASegmenter() 
{

  // if we have a segmenter
  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();

  if ( segmenter )
    {

    // edit the options now
    ModifyMFASegmenterOptions( *segmenter );

    } // segmenter

} // SlotOpenMFASegmenter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SeguiWindow::SecretData::SegmenterPointer
SeguiWindow::SecretData::GetMFASegmenter() const
{
  
  // if we have a segmenter
  const SegmenterPointer segmenter = this->m_MFASegmenter;

  if ( !segmenter )
    {

    QMessageBox::warning(
      this->m_EncapsulatingClass,
      "No MFA Segmenter",
      "There is no MFA segmenter. Please load or create one first.",
      QMessageBox::Cancel );

    } // !segmenter

  return segmenter;

} // GetMFASegmenter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotSaveMFASegmenter()
{
  
  const string & fileName = this->m_SecretData->m_FileName;

  // do we have a file name already?
  if ( fileName.empty() )
    {

    // nope - just call the "Save As" function
    return SlotSaveMFASegmenterAs();

    } // file name

  const bool verbose = true;

  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();

  if ( segmenter )
    {

    // try to save
    SaveObjectGUI( *segmenter, fileName, this, verbose );

    } // segmenter


} // SlotSaveMFASegmenter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotSaveMFASegmenterAs()
{
  
  const bool verbose = true;

  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();

  if ( !segmenter )
    {
    return;
    }

  // try to save
  const string savedFileName = SaveObjectGUI( *segmenter, this, verbose );

  // if successful, update the file name
  if ( !savedFileName.empty() )
    {
    this->m_SecretData->m_FileName = savedFileName;
    this->m_SecretData->UpdateTitle();
    }

} // SlotSaveMFASegmenterAs
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotRerender()
{

  // extract the render window
  const vtkRenderWindowPointer
    renderWindow = this->ui->m_OutputVTKWidget->GetRenderWindow();
  if ( !renderWindow ) return;

  // clear the render window
  const vtkRendererPointer
    renderer = renderWindow->GetRenderers()->GetFirstRenderer();
  if ( !renderer ) return;
  renderer->RemoveAllViewProps();
  this->m_SecretData->m_ImageProp = vtkPropPointer();

  // render the segmenter
  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->m_MFASegmenter;

  if ( segmenter )
    {

    // get the model
    const auto model = segmenter->GetTrainedModel();

    if ( model )
      {

      cout << "Rerendering MFA:" << endl;

      this->m_SecretData->m_LevelsetActors =
        RefreshRendering( *renderWindow, *model );

      cout << "Rerendering MFA done." << endl;
      } // model

    } // segmenter

  // render the image
  this->SlotUpdateWhichImage();

  // update the transparencies
  this->SlotUpdateTransparency();

  // update the colors
  this->SlotRandomizeColors();

} // SlotRerenderMFA
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotUpdateImageWindowLevel()
{

  // get the values
  const double
    denominator       = 100.0,
    windowSliderValue = this->ui->m_ImageWindowSlider->value(),
    levelSliderValue  = this->ui->m_ImageLevelSlider->value(),
    window            = windowSliderValue / denominator,
    level             = levelSliderValue / denominator;

  cout << "Updating window / level to " << window << " / " << level << ":" << endl;

  // update the windowing / level
  UpdateVTKImageWindowLevel(
    this->m_SecretData->m_ImageProp, window, level );

  // rerender
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

  cout << "Updating window / level done." << endl;

} // SlotUpdateImageWindowLevel
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotUpdateSlice()
{
  
  const bool verbose = true;
  const unsigned int slice =
    this->ui->m_SliceSlider->value();

  // change the slice
  UpdateVTKImageSlice( this->m_SecretData->m_ImageProp, slice, verbose );

  // rerender
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} //SlotUpdateSlice
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotSegment()
{

  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();

  if ( !segmenter )
    return;

  // rerender
  cout << "Initializing MFA:" << endl;
  segmenter->Initialize();
  cout << "Initializing MFA done." << endl;

  cout << "Rerendering:" << endl;
  this->SlotRerender();
  cout << "Rerendering done." << endl;

  const auto model = segmenter->GetTrainedModel();

  if (!model )
  {
  QMessageBox::warning(
    this,
    "No model",
    "There is no model to perform the segmentation. Please load one first.",
    QMessageBox::Cancel );

    return;
  }

  // we don't reconstruct (since we might want to correct)
  segmenter->ReconstructFeaturesInMovedOff();

  const StudyConstPointer study = this->m_SecretData->m_Study;

  if ( !study )
    {

    QMessageBox::warning(
      this,
      "No study",
      "There is no study to segment. Please load one first.",
      QMessageBox::Cancel );

    return;

    } // !study
  
  // extract the render window
  const vtkRenderWindowPointer
    renderWindow = this->ui->m_OutputVTKWidget->GetRenderWindow();

  // create a GUI observer
  const RegistrationGUIObserver::Pointer
    observer = RegistrationGUIObserver::New();

  const vtkPropPointer
    & movedProp = this->m_SecretData->m_ImageProp;
  
  const bool
    makeZeroTransparent = false;

  observer->SetMakeZeroTransparent( makeZeroTransparent );
  observer->SetImagePropToUpdate  ( movedProp    );
  observer->SetRenderWindow       ( renderWindow );

  // extract the algorithms
  const RegisterMFAAlgorithmCollection algorithms =
    segmenter->GetRegistrationAlgorithms();
  
  // extract the segmentation objects
  const MaskObjectCollectionType allSegmentationObjects =
    segmenter->GetAllSegmentationObjects();

  // set up each algorithm
  BOOST_FOREACH( const auto & algorithm, algorithms )
    {
    algorithm->SetObserver( observer.GetPointer() );

    // clear the known points for this algorithm
    RegisterMFAAlgorithm::MetricPointer metric = 
      std::dynamic_pointer_cast< RegisterMFAAlgorithm::MetricType > ( algorithm->GetMetric() );
    metric->ClearKnownPoints();

    // set the known points for the algorithm
    BOOST_FOREACH( const MaskObjectType & segmentationObject,
      allSegmentationObjects )
      {
      const LandmarkSetConstPointer knownPoints(
        segmenter->GetKnownSurfacePoints( segmentationObject ) );

      metric->SetKnownPoints( knownPoints, segmentationObject );

      } // for each segmentation object

    } // for each algorithm
  
  QApplication::setOverrideCursor( Qt::WaitCursor );
  this->setDisabled( true );
  const bool segmented = segmenter->Resegment( study );
  this->setDisabled( false );
  QApplication::restoreOverrideCursor();
  
  if ( segmented )
    {

    QMessageBox::information(
      this,
      "Segmentation succeeded.",
      "MFA segmentation completed successfully.",
      QMessageBox::Ok );

    }
  else
    {

    QMessageBox::warning(
      this,
      "Segmentation failed",
      "Warning: The segmentation failed.",
      QMessageBox::Ok );

    } // !segmented

  // update the possible images
  cout << "Updating image combo:" << endl;
  this->m_SecretData->UpdateImageCombo();
  cout << "Updating image combo done." << endl;

  // update the displayed image
  cout << "Updating displayed image:" << endl;
  this->SlotUpdateWhichImage();
  cout << "Updating displayed image done." << endl;
  
} // SlotSegment
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotCorrectSegmentation()
{

  // get the study
  const StudyConstPointer study = this->m_SecretData->m_Study;
  if ( !study )
    {
    cout << "No study." << endl;
    return;
    } // study  
  
  // get the volume
  if ( !study->HasVolume() )
    {
    cout << "No volume." << endl;
    return;
    } // study
  const VolumeImageConstPointer volume( study->GetVolume() );

  // get the segmenter
  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();
  if ( !segmenter ) return;

  // select which object
  const MaskObjectCollectionType
    segmentationObjects = segmenter->GetAllSegmentationObjects();

  if ( segmentationObjects.empty() )
    {
    cout << "No segmentation objects." << endl;
    return;
    }

  QStringList items;
  BOOST_FOREACH( const MaskObjectType & segmentationObject, segmentationObjects )
    {
    items.append( QString( segmentationObject.c_str() ) );
    } // for each object

  // select the item
  bool userPressedOkay = true;
  const int defaultItem = 0;
  const bool editable = false;
  const QString item = 
    QInputDialog::getItem(
      this, tr( "Segmentation object" ),
      tr( "Please select a segmentation object.\n \
You will then have the opportunity to select\n \
points on the surface of this object from\n \
the input image on the next screen." ),
      items, defaultItem, editable, &userPressedOkay );

  if ( !userPressedOkay )
    {
    cout << "User canceled." << endl;
    return;
    }

  // convert it to the mask object type
  const MaskObjectType segmentationObject = item.toStdString();
  
  cout << "Merging levelset with image:" << endl;
  const VolumeImageConstPointer
    levelset( ResizeImageIsotropically(
      segmenter->GetReconstructedLevelsetWithoutCorrection( segmentationObject ) ) ),
    mergedImage( MergeImageWithSurfaceImage(
      ResizeImageIsotropically( volume.GetPointer() ).GetPointer(),
      levelset.GetPointer() ) );
  cout << "Merging levelset with image done." << endl;

  // get the landmarks for this object
  // abnd select more landmarks
  const LandmarkSetConstPointer
    existingLandmarks( segmenter->GetKnownSurfacePoints( segmentationObject ) ),
    updatedLandmarks ( PickPointsOnImage( mergedImage, existingLandmarks ) );

  if ( !updatedLandmarks )
    {
    cout << "No landmarks." << endl;
    return;
    }

  // give the landmarks to the segmenter corrector
  segmenter->SetKnownSurfacePoints( segmentationObject, updatedLandmarks );

  // regenerate
  cout << "Regenerating correction:" << endl;
  this->SlotRegenerateCorrection();
  cout << "Regenerating correction done." << endl;

  // update the image
  cout << "Updating image:" << endl;
  this->SlotUpdateWhichImage();
  cout << "Updating image done." << endl;

} // SlotCorrectSegmentation
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotRegenerateCorrection()
{
  
  // get the segmenter
  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();
  if ( !segmenter ) return;

  // regenerate
  cout << "Regenerating correction:" << endl;
  segmenter->FixCornersOn();
  segmenter->RegenerateCorrection();
  cout << "Regenerating correction done." << endl;

  // store the fixed points

  const vtkPropPointer existingFixedPointsProp =
    this->m_SecretData->m_FixedCorrectionPointsProp;

  const vtkRenderWindowPointer renderWindow =
    this->ui->m_OutputVTKWidget->GetRenderWindow();
  vtkRendererPointer renderer;
  if ( renderWindow ) 
    renderer = renderWindow->GetRenderers()->GetFirstRenderer();

  // remove existing fixed points actor
  if ( existingFixedPointsProp && renderer )
    {
    cout << "Removing existing points:" << endl;
    renderer->RemoveActor( existingFixedPointsProp );
    cout << "Removing existing points done." << endl;
    } // existingFixedPointsProp

  // get the new fixed points
  const LandmarkSetConstPointer newFixedPoints =
    segmenter->GetRandomFixedPoints();

  if ( !newFixedPoints )
    {
    cout << "Error: No fixed points." << endl;
    return;
    } // newFixedPoints

  const auto numberOfPoints = newFixedPoints->GetNumberOfPoints();
  cout << "# of fixed points = " << numberOfPoints << endl;
  if ( numberOfPoints > 0 )
    {
    cout << "Fixed point #1 = " << endl << newFixedPoints->GetPoint( 0 ) << endl;
    } // numberOfPoints

  // create an actor from these
  cout << "Generating points actor:" << endl;
  const vtkPropPointer newFixedPointsProp = 
    GenerateActorFromLandmarks( newFixedPoints );
  cout << "Generating points actor done." << endl;

  if ( !newFixedPointsProp )
    {
    cout << "Error: No fixed points prop." << endl;
    return;
    } // newFixedPointsProp 

  // update our stored prop
  this->m_SecretData->m_FixedCorrectionPointsProp = newFixedPointsProp;

  // add them to the renderer
  if ( renderer )
    {
    cout << "Adding points actor:" << endl;
    renderer->AddActor( newFixedPointsProp );
    cout << "Adding points actor done." << endl;
    }

  // display them
  cout << "Showing fixed points:" << endl;
  this->SlotShowFixedPoints();
  cout << "Showing fixed points done." << endl;

} // SlotRegenerateCorrection
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotClearCorrectionAndPoints()
{

  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->m_MFASegmenter;
  if ( segmenter )
    {
    segmenter->ClearCorrectionAndKnownSurfacePoints();
    cout << "Cleared TPS correction and surface points." << endl;
    }

} // SlotClearCorrectionAndPoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotClearCorrection()
{

  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->m_MFASegmenter;
  if ( segmenter )
    {
    segmenter->ClearCorrection();
    cout << "Cleared TPS correction." << endl;
    }

} // SlotClearCorrection
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotClearCorrectionPoints()
{

  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->m_MFASegmenter;
  if ( segmenter )
    {
    segmenter->ClearKnownSurfacePoints();
    cout << "Cleared known surface points." << endl;
    }

} // SlotClearCorrectionPoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotSetNumberOfFixedPoints()
{

  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();
  if ( !segmenter ) return;

  // get the current # of fixed points
  const unsigned int originalFixedPoints =
    segmenter->GetNumberOfRandomFixedPoints();

  // display a popup
  bool userPressedOkay = true;
  const unsigned int newFixedPoints = static_cast< unsigned int >(
    QInputDialog::getInt( this, tr( "# of Fixed Points" ),
      tr( "Please input the number of random fixed points to include in the correction." ),
      originalFixedPoints, 0, 999, 5, &userPressedOkay ) );

  if ( !userPressedOkay ) return;

  segmenter->SetNumberOfRandomFixedPoints( newFixedPoints );

  this->SlotRegenerateCorrection();

} // SlotSetNumberOfFixedPoints
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotSetBoundingBox()
{

  // get the study
  const StudyConstPointer study = this->m_SecretData->m_Study;
  if ( !study )
    {
    cout << "No study." << endl;
    return;
    } // study  
  
  // get the volume
  if ( !study->HasVolume() )
    {
    cout << "No volume." << endl;
    return;
    } // study
  const VolumeImageConstPointer volume( study->GetVolume() );

  // get the segmenter
  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();
  if ( !segmenter ) return;

  // get the landmarks for this object
  // abnd select more landmarks
  const LandmarkSetConstPointer
    boundingBoxLandmarks( PickPointsOnImage( volume ) );

  if ( !boundingBoxLandmarks )
    {
    cout << "No landmarks." << endl;
    return;
    }

  // give the landmarks to the segmenter corrector
  const VolumeRegionType boundingBox = 
    CreateBoundingBox( *boundingBoxLandmarks, *volume );

  if ( boundingBox.GetNumberOfPixels() == 0 )
    {
    cout << "0-sized bounding box." << endl;
    return;
    } // no pixels
  else
    {
    cout << "Manual bounding box = " << endl << boundingBox << endl;
    }

  // set the bounding box of the segmenter
  segmenter->SetBoundingBox( boundingBox );

  // create an actor from it
  cout << "Rendering bounding box:" << endl;
  const vtkPropPointer
    oldBoundingBoxProp = this->m_SecretData->m_BoundingBoxProp,
    newBoundingBoxProp = GenerateVTKActorFromBoundingBox(
      boundingBox, volume.GetPointer() );
  cout << "Rendering bounding box done." << endl;

  // replace the old one
  // get the render window
  const vtkRenderWindowPointer renderWindow =
    this->ui->m_OutputVTKWidget->GetRenderWindow();
  if ( !renderWindow ) return;
  const vtkRendererPointer renderer =
    renderWindow->GetRenderers()->GetFirstRenderer();
  if ( !renderer ) return;
  
  // add it to our scene
  renderer->RemoveActor( oldBoundingBoxProp );
  renderer->AddActor   ( newBoundingBoxProp );

  // save it
  this->m_SecretData->m_BoundingBoxProp = newBoundingBoxProp;

  // show it
  this->SlotShowBoundingBox();

} // SlotSetBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotClearBoundingBox()
{

  // get the segmenter
  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->m_MFASegmenter;
  if ( segmenter )
    {
    segmenter->ClearBoundingBox();
    }

  
  // make it visible
  const vtkPropPointer boundingBoxProp = this->m_SecretData->m_BoundingBoxProp;
  if ( boundingBoxProp )
    {
    // remove it from rendering
    const vtkRenderWindowPointer renderWindow =
      this->ui->m_OutputVTKWidget->GetRenderWindow();
    if ( renderWindow )
      {
      const vtkRendererPointer renderer =
        renderWindow->GetRenderers()->GetFirstRenderer();
      if ( renderer )
        {
        // add it to our scene
        renderer->RemoveActor( boundingBoxProp );
        } // renderer

      } // renderWindow

    } // boundingBoxProp

  this->m_SecretData->m_BoundingBoxProp = vtkPropPointer();

  /*
  QMessageBox::information(
    this,
    tr( "Bounding box cleared." ),
    tr( "The input volume no longer has a manual bounding box set." ),
    QMessageBox::Ok );
  */
  cout << "Bounding box cleared." << endl;

} // SlotClearBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotShowBoundingBox()
{
  // make it visible
  const vtkPropPointer boundingBoxProp = this->m_SecretData->m_BoundingBoxProp;
  if ( boundingBoxProp )
    boundingBoxProp->VisibilityOn();

  // rerender
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotShowBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotHideBoundingBox()
{

  // make it invisible
  const vtkPropPointer boundingBoxProp = this->m_SecretData->m_BoundingBoxProp;
  if ( boundingBoxProp )
    boundingBoxProp->VisibilityOff();

  // rerender
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotShowBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotShowFixedPoints()
{
  // make it visible
  const vtkPropPointer fixedPointsProp =
    this->m_SecretData->m_FixedCorrectionPointsProp;

  if ( fixedPointsProp )
    fixedPointsProp->VisibilityOn();

  // rerender
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotShowBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotHideFixedPoints()
{
  
  // make it visible
  const vtkPropPointer fixedPointsProp =
    this->m_SecretData->m_FixedCorrectionPointsProp;

  if ( fixedPointsProp )
    fixedPointsProp->VisibilityOff();

  // rerender
  this->ui->m_OutputVTKWidget->GetRenderWindow()->Render();

} // SlotShowBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotSetForbiddenRadius()
{

  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();
  if ( !segmenter ) return;

  // get the current # of fixed points
  const TransformValueType originalRadius =
    segmenter->GetForbiddenRadius();

  // display a popup
  bool userPressedOkay = true;
  const TransformValueType newRadius = static_cast< TransformValueType >(
    QInputDialog::getDouble( this, tr( "Forbidden Radius" ),
      tr( "Please input the radius around known surface points in which no anchor points can lie." ),
      originalRadius, 0, 999, 2, &userPressedOkay ) );

  if ( !userPressedOkay ) return;

  segmenter->SetForbiddenRadius( newRadius );

  this->SlotRegenerateCorrection();

} // SlotSetForbiddenRadius
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotSetNumberOfSurfacePoints()
{

  // get the segmenter
  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();
  if ( !segmenter ) return;

  // select which object
  const MaskObjectCollectionType
    segmentationObjects = segmenter->GetAllSegmentationObjects();

  if ( segmentationObjects.empty() )
    {
    cout << "No segmentation objects." << endl;
    return;
    }

  QStringList items;
  BOOST_FOREACH( const MaskObjectType & segmentationObject, segmentationObjects )
    {
    items.append( QString( segmentationObject.c_str() ) );
    } // for each object

  // select the item
  bool userPressedOkay = true;
  const int defaultItem = 0;
  const bool editable = false;
  const QString item = 
    QInputDialog::getItem(
      this, tr( "Segmentation object" ),
      tr( "Please select a segmentation object." ),
      items, defaultItem, editable, &userPressedOkay );

  if ( !userPressedOkay )
    {
    cout << "User canceled." << endl;
    return;
    }

  // convert it to the mask object type
  const MaskObjectType segmentationObject = item.toStdString();
  
  // get the current # of fixed points
  const unsigned int originalSurfacePoints =
    segmenter->GetNumberOfAnchorSurfacePoints( segmentationObject );

  // display a popup
  userPressedOkay = true;
  const unsigned int newSurfacePoints = static_cast< unsigned int >(
    QInputDialog::getInt( this, tr( "# of Surface Points" ),
      tr( "Please input the number of random anchor surface points to include in the correction." ),
      originalSurfacePoints, 0, 999, 5, &userPressedOkay ) );

  if ( !userPressedOkay ) return;

  segmenter->SetNumberOfAnchorSurfacePoints( segmentationObject, newSurfacePoints );

  this->SlotRegenerateCorrection();

} // SlotSetNumberOfSurfacePoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SeguiWindow::SlotSaveSummedLabels()
{

  // we need to select two labels

  // get the segmenter
  const SecretData::SegmenterPointer
    segmenter = this->m_SecretData->GetMFASegmenter();
  if ( !segmenter ) return;

  // select which object
  const MaskObjectCollectionType
    segmentationObjects = segmenter->GetAllSegmentationObjects();

  if ( segmentationObjects.empty() )
    {
    cout << "No segmentation objects." << endl;
    return;
    }

  QStringList items;
  BOOST_FOREACH( const MaskObjectType & segmentationObject, segmentationObjects )
    {
    items.append( QString( segmentationObject.c_str() ) );
    } // for each object

  // select the item
  bool userPressedOkay = true;
  const int defaultItem = 0;
  const bool editable = false;

  const QString item1 = 
    QInputDialog::getItem(
      this, tr( "Segmentation object 1/2" ),
      tr( "Please select segmentation object #1." ),
      items, defaultItem, editable, &userPressedOkay );

  if ( !userPressedOkay )
    {
    cout << "User canceled." << endl;
    return;
    }
  
  const QString item2 = 
    QInputDialog::getItem(
      this, tr( "Segmentation object 2/2" ),
      tr( "Please select segmentation object #2." ),
      items, defaultItem, editable, &userPressedOkay );

  if ( !userPressedOkay )
    {
    cout << "User canceled." << endl;
    return;
    }


  // convert it to the mask object type
  const MaskObjectType
    segmentationObject1 = item1.toStdString(),
    segmentationObject2 = item2.toStdString();
  
  // get the masks
  const VolumeMaskImageConstPointer
    segmentation1( segmenter->GetSegmentation( segmentationObject1 ) ),
    segmentation2( segmenter->GetSegmentation( segmentationObject2 ) );

  if ( segmentation1.IsNull() || segmentation2.IsNull() )
    {

    QMessageBox::warning(
      this,
      tr( "No segmentation." ),
      tr( "There is no output segmentation!" ),
      QMessageBox::Cancel );

    return;

    } // null segmentation

  typedef itk::AddImageFilter<
    VolumeMaskImageType, VolumeMaskImageType, VolumeMaskImageType
  > AddImageFilterType;

  cout << "Adding segmentations:" << endl;
  const AddImageFilterType::Pointer adder = AddImageFilterType::New();
  adder->SetInput1( segmentation1 );
  adder->SetInput2( segmentation2 );
  adder->Update();
  cout << "Adding segmentations done." << endl;

  VolumeMaskImagePointer summedVolume = adder->GetOutput();

  if ( !summedVolume )
    {

    QMessageBox::warning(
      this,
      tr( "No segmentation." ),
      tr( "There is no summed segmentation!" ),
      QMessageBox::Cancel );

    return;

    } // summed volume

  // remove speckle by getting the largest regions
  summedVolume = GetLargestContiguousRegions( summedVolume.GetPointer() );

  // apply the old direction that we stored
  const VolumeImageBase::DirectionType
    direction = this->m_SecretData->m_OriginalDirection;

  summedVolume->SetDirection( direction );

  // try to save it
  SaveImage( summedVolume.GetPointer(), this );

} // SlotSaveSummedLabels
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
