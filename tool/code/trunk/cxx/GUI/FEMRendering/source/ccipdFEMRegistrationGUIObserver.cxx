


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdFEMRegistrationGUIObserver.h"
#include "ccipdFEMRenderer.h"
#include "ccipdResizeImage.h" // to resize like the template
#include "ccipdDuplicateFEM.h"

// itk includes
#include "itkFEMRegistrationFilterBase.h"
#include "itkFEMRegularizedMetric.h"
#include "itkFEMTransform.h"
//#include "itkFEMSolutionCache.h" // where the solutions are being stored

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkSingleValuedNonLinearOptimizer.h>
  #include <itkFEMObject.h>


// vtk includes
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdEnableWarningsMacro.h"


//////////////////////////////////////////////////////////////////////////////////////////
// "using" shortcuts
using std::cout;
using std::endl;
using std::cerr;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL interface to FEMRegistrationGUIObserver class
class FEMRegistrationGUIObserver::SecretData
{

public: // to the encapsulating class
  SecretData(); ///< Constructor

  // Variables

  /// We can observe volumetric FEM registrations.
  typedef itk::fem::FEMRegistrationFilterBase<
    VolumeImageType,
    VolumeImageType,
    FEMObjectType
  > FEMRegistrationFilterType;

  /// Store the FEM filter which calls this iteration event.
  FEMRegistrationFilterType::ConstPointer m_FEMFilter;

  /// Store an FEM to render
  FEMObjectConstPointer
    m_ExtractedFEM,  // this is just to see if we extract a new one
    m_DuplicatedFEM; // we duplicate it so that we can update

  /// To render the FEM
  FEMRendererPointer m_FEMRenderer;

  typedef itk::fem::LinearSystemWrapper SolutionType;

  /// A solved FEM
  const SolutionType * m_FEMSolution;

  /**
   * \brief Helper function to extract a FEM filter using dynamic_cast'ing
   * \returns true if extracted, false it not.
   */
  bool ExtractFEMFilter( const itk::Object * const caller );

  /**
   * \brief Helper function to extract a FEM using dynamic_cast'ing
   * \returns true if extracted, false it not.
   */
  bool ExtractFEMObjectAndSolution( const itk::Object * const caller );

  /// Update the rendered FEM.
  void UpdateFEMRendering();

}; // SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRegistrationGUIObserver::SecretData::SecretData() :
m_FEMRenderer( new FEMRenderer ),
m_FEMSolution( 0 )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRegistrationGUIObserver::FEMRegistrationGUIObserver() :
Superclass(),
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRegistrationGUIObserver::~FEMRegistrationGUIObserver()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRegistrationGUIObserver::SetRenderWindow(
  const vtkRenderWindowPointer & renderWindow )
{

  if ( renderWindow )
    {

    vtkRendererCollection * const renderers =
      renderWindow->GetRenderers();

    if ( renderers )
      {
      // give the renderer to our FEM renderer
      this->m_SecretData->m_FEMRenderer->SetRenderer(
        renderers->GetFirstRenderer() );
      }

    } // renderWindow

  // continue calling the superclass's version of this function
  this->Superclass::SetRenderWindow( renderWindow );

} // SetRenderWindow
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRegistrationGUIObserver::Execute(
  const itk::Object * caller, const itk::EventObject & event)
{

  // try to extract the relevant objects
  this->m_SecretData->ExtractFEMFilter           ( caller );
  this->m_SecretData->ExtractFEMObjectAndSolution( caller );

  // call the original execute function
  this->Superclass::Execute( caller, event );

  // if we are updating, re-render the FEM
  if ( this->GetUpdatingThisIteration() )
    this->m_SecretData->UpdateFEMRendering();

} // Execute
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FEMRegistrationGUIObserver::SecretData::ExtractFEMFilter(
  const itk::Object * const caller )
{

  // see if an FEM registration called us
  typedef SecretData::FEMRegistrationFilterType FEMRegistrationFilterType;

  // try to cast it as a known FEM registration filter
  const FEMRegistrationFilterType::ConstPointer
    FEMFilter =
      dynamic_cast< const FEMRegistrationFilterType * >( caller );

  if ( !FEMFilter )
    return false;

  // if successful, assign it
  this->m_FEMFilter = FEMFilter;

  return true;

} // caller
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FEMRegistrationGUIObserver::SecretData::ExtractFEMObjectAndSolution(
  const itk::Object * const caller )
{

  // try to cast it as an FEM regularized metric
  typedef itk::SingleValuedNonLinearOptimizer OptimizerType;
  const OptimizerType::ConstPointer optimizer =
    dynamic_cast< const OptimizerType * >( caller );

  if ( !optimizer )
    return false;

  // extract the cost function from the optimizer
  typedef OptimizerType::CostFunctionType CostFunctionType;
  const CostFunctionType::ConstPointer
    costFunction = optimizer->GetCostFunction();
      
  if ( !costFunction )
    return false;

  /*
  // try to cast it as an FEM regularized metric
  typedef itk::fem::RegularizedMetric<
    InternalPixelType, FEMDimension
  */
  typedef itk::ImageToImageMetric<
    ImageType, ImageType
  > MetricType;

  const MetricType::ConstPointer metric =
    dynamic_cast< const MetricType * >( costFunction.GetPointer() );

  if ( !metric )
    return false;

  // try to get the transform
  typedef MetricType::TransformType TransformType;
  const TransformType::ConstPointer
    transform = metric->GetTransform();

  if ( !transform )
    return false;

  // try to get an FEM transform
  typedef itk::fem::FEMTransform<
    TransformValueType, FEMDimension
  > FEMTransformType;

  const FEMTransformType::ConstPointer femTransform =
    dynamic_cast< const FEMTransformType * >( transform.GetPointer() );

  if ( !femTransform )
    return false;

  // pull out the FEM solution
  this->m_FEMSolution = femTransform->GetLinearSystem();

  // pull out the FEM
  const FEMObjectConstPointer FEM =
    femTransform->GetFEM();

  if ( !FEM || ( this->m_ExtractedFEM.GetPointer() == FEM.GetPointer() ) )
    return false;

  // assign it
  this->m_ExtractedFEM  = FEM;
  this->m_DuplicatedFEM = DuplicateFEM( FEM.GetPointer() );
  this->m_FEMRenderer->SetFEM( this->m_DuplicatedFEM );

  return true;

} // ExtractFEMObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer FEMRegistrationGUIObserver::GetMovedImage() const
{
 
  // check if we have an FEM
  typedef SecretData::FEMRegistrationFilterType FEMRegistrationFilterType;
  const FEMRegistrationFilterType::ConstPointer
    & FEMFilter = this->m_SecretData->m_FEMFilter;

  // just try the superclass's version
  if ( !FEMFilter )
    return this->Superclass::GetMovedImage();

  // we want to use the FEM's warp field to get the moved image
  cout << "Warping image to get the moved image:" << endl;
  const VolumeImageConstPointer movedImage(
    FEMFilter->WarpImageConst(
      //this->Superclass::GetMovingImage() ) );
      FEMFilter->GetOriginalMovingImage() ) );
  cout << "Warping image to get the moved image done." << endl;

  // if there's a template, resize it to be that
  // otherwise, just return the warped result

  const Superclass::TemplateImageConstPointer templateImage =
    this->Superclass::GetMovedTemplateImage();

  if ( templateImage )
    return ResizeImage( movedImage, templateImage ).GetPointer();
  else
    return movedImage;

} // GetMovedImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRegistrationGUIObserver::SecretData::UpdateFEMRendering()
{

  const SolutionType * const solution =
    this->m_FEMSolution;

  if ( !solution )
    return;

  try
    {

    // display some solution values
    const unsigned int numberOfValues = 10;
    cout << "First " << numberOfValues << " solution values = ";
    for ( unsigned int valueIndex = 0; valueIndex < numberOfValues; ++valueIndex )
      cout << solution->GetSolutionValue( valueIndex ) << " ";
    cout << endl;

    // now deform the FEM with the given solution
    if ( this->m_FEMRenderer )
      {
      cout << "Updating FEM rendering:" << endl;
      this->m_FEMRenderer->UpdateFEMRendering( *solution );
      cout << "Updating FEM rendering done." << endl;
      }

    } // try
  catch( ... ) { }

} // UpdateFEMRendering
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdGetConstImplementMacro( FEMRegistrationGUIObserver, FEMRenderer, FEMRendererPointer )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
