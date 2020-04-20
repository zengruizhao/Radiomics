


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdFEMRenderer.h"
#include "ccipdFEMToActor.h" // for generating the VTK actors
#include "ccipdDeformFEM.h"  // for deforming the FEM with a solution

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkFEMObject.h>

  // vtk includes
  #include <vtkSmartPointer.h>    // smart pointers
  #include <vtkActor.h>           // for the FEM actors
  #include <vtkActorCollection.h> // for all the existing actors
  #include <vtkGlyph3D.h>         // for modifying the glyph scales
  #include <vtkMapper.h>          // for modifying the glyph scales
  #include <vtkProperty.h>        // for changing colors, etc.
  #include <vtkRenderer.h>        // where to display the result


// std includes
#include <limits> // for max color value

// boost includes
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
class FEMRenderer::SecretData
{

public:
  SecretData(); ///< Constructor

  typedef std::size_t IndexType;

  static const IndexType m_NumberOfActors = 4;

  typedef std::unordered_map<
    WhichActor, IndexType,
    std::hash< int >
  > ActorToIndexMap;

  static const ActorToIndexMap m_ActorToIndex;

  /// collection to store the actors
  typedef std::array<
    vtkActorPointer,
    m_NumberOfActors
  > ActorCollection;

  ActorCollection m_Actors;

  /// properties of the actors
  FEMRenderer::RealPropertiesType
    m_LandmarkSphereRadius,
    m_LoadSphereRadius,
    m_ArrowScaling;

  typedef std::array< FEMRenderer::RealPropertiesType,
    m_NumberOfActors
  > OpacityCollection;

  OpacityCollection m_Opacities;

  typedef std::array<
    FEMRenderer::ColorType,
    m_NumberOfActors
  > ColorCollection;

  ColorCollection
    m_FaceColors,
    m_EdgeColors;

  FEMObjectConstPointer m_FEM;      ///< Store the FEM
  vtkRendererPointer    m_Renderer; ///< Where to display the output

  bool m_Verbose; ///< Show progress?

  /// Regenerate the actors for the scene.
  void RegenerateActors();

  /// Make sure all the actors are in the scene
  void AddActors();

  /// Update things like color, opacity, etc.
  void UpdateActorProperties();

  /// Rerender the scene
  void ReRender();

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// constants
const FEMRenderer::SecretData::ActorToIndexMap
  FEMRenderer::SecretData::m_ActorToIndex =
    boost::assign::map_list_of
      ( LandmarksActor   , 0 )
      ( TrianglesActor   , 1 )
      ( LoadsSpheresActor, 2 )
      ( LoadsArrowsActor , 3 );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRenderer::SecretData::SecretData() :
m_LandmarkSphereRadius( 0.15 ),
m_LoadSphereRadius    ( 0.20 ),
m_ArrowScaling        ( 1.00 ),
m_Verbose( true )
{

  const SecretData::ActorToIndexMap
    & actorToIndex = SecretData::m_ActorToIndex;

  // initialize the colors
  FEMRenderer::ColorType color;

  color[ 0 ] = 255;
  color[ 1 ] = 0;
  color[ 2 ] = 0;
  this->m_FaceColors[ actorToIndex.at( LandmarksActor ) ] = color;
  
  color[ 0 ] = 0;
  color[ 1 ] = 100;
  color[ 2 ] = 255;
  this->m_FaceColors[ actorToIndex.at( TrianglesActor ) ] = color;

  color[ 0 ] = 0;
  color[ 1 ] = 255;
  color[ 2 ] = 0;
  this->m_FaceColors[ actorToIndex.at( LoadsArrowsActor ) ] = color;

  color[ 0 ] = 255;
  color[ 1 ] = 255;
  color[ 2 ] = 255;
  this->m_FaceColors[ actorToIndex.at( LoadsSpheresActor ) ] = color;

  // initialize the edge colors
  color[ 0 ] = 200;
  color[ 1 ] = 200;
  color[ 2 ] = 200;
  this->m_EdgeColors.fill( color );

  // set the opacities
  this->m_Opacities[ actorToIndex.at( LandmarksActor    ) ] = 0.90;
  this->m_Opacities[ actorToIndex.at( TrianglesActor    ) ] = 0.50;
  this->m_Opacities[ actorToIndex.at( LoadsSpheresActor ) ] = 1.00;
  this->m_Opacities[ actorToIndex.at( LoadsArrowsActor  ) ] = 1.00;

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRenderer::FEMRenderer() :
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRenderer::~FEMRenderer()
{

  // here we will delete all the actors
  const bool verbose = this->m_SecretData->m_Verbose;

  if ( verbose )
    cout << "Destructor called." << endl << "Removing FEM actors:" << endl;

  this->RemoveActors();

  if ( verbose )
    cout << "Removing FEM actors done." << endl;

} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SecretData::RegenerateActors()
{

  const bool verbose = this->m_Verbose;

  // extract our FEM
  const FEMObjectConstPointer & FEM = this->m_FEM;

  // make sure we have everything
  if ( !FEM )
    {
    if ( verbose )
      cerr << "Warning: No FEM. Returning." << endl;
    return;
    }

  // regenerate everything
  const SecretData::ActorToIndexMap
    & actorToIndex = SecretData::m_ActorToIndex;

  GenerateVTKActorsFromFEM(
    FEM,
    this->m_Actors[ actorToIndex.at( LandmarksActor    ) ],
    this->m_Actors[ actorToIndex.at( TrianglesActor    ) ],
    this->m_Actors[ actorToIndex.at( LoadsArrowsActor  ) ],
    this->m_Actors[ actorToIndex.at( LoadsSpheresActor ) ],
    this->m_LandmarkSphereRadius,
    this->m_LoadSphereRadius,
    this->m_ArrowScaling,
    verbose );

} // RegenerateActors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SecretData::ReRender()
{
  
  const bool verbose = this->m_Verbose;

  // extract our renderer
  const vtkRendererPointer & renderer = this->m_Renderer;

  if ( !renderer )
    {
    if ( verbose )
      cerr << "Warning: No renderer. Returning." << endl;
    return;
    } // renderer
  
  // update the properties
  this->UpdateActorProperties();

  // rerender
  if ( renderer->GetRenderWindow() )
    {
    renderer->Render();
    }
  else
    {
    if ( verbose )
      cerr << "Warning: No render window. Returning." << endl;
    }
} // ReRender
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SecretData::UpdateActorProperties()
{

  const ActorCollection
    & actors = this->m_Actors;
  const OpacityCollection
    & opacities = this->m_Opacities;
  const ColorCollection
    & faceColors = this->m_FaceColors,
    & edgeColors = this->m_EdgeColors;

  BOOST_FOREACH( const ActorToIndexMap::value_type & iter, this->m_ActorToIndex )
    {

    // extract the relevant data for this iteration
    const FEMRenderer::WhichActor
      whichActor = iter.first;
    const IndexType
      index = iter.second;

    // get the actor
    const vtkActorPointer actor = actors[ index ];

    if ( !actor )
      continue;

    // get the properties
    const vtkPropertyPointer properties = actor->GetProperty();

    // set the opacitiy
    properties->SetOpacity( opacities[ index ] );

    const FEMRenderer::RealPropertiesType maxColorValue =
      static_cast< FEMRenderer::RealPropertiesType >(
        std::numeric_limits< FEMRenderer::RGBValueType >::max() );

    // set the face color
    const FEMRenderer::ColorType faceColor = faceColors[ index ];

    properties->SetColor(
      static_cast< FEMRenderer::RealPropertiesType >(
        faceColor[ 0 ] ) / maxColorValue,
      static_cast< FEMRenderer::RealPropertiesType >(
        faceColor[ 1 ] ) / maxColorValue,
      static_cast< FEMRenderer::RealPropertiesType >(
        faceColor[ 2 ] ) / maxColorValue );

    // we only define the edge for the triangles
    if ( whichActor == FEMRenderer::TrianglesActor )
      {

      // set the edge color
      const FEMRenderer::ColorType edgeColor = edgeColors[ index ];

      properties->SetEdgeColor(
        static_cast< FEMRenderer::RealPropertiesType >(
          edgeColor[ 0 ] ) / maxColorValue,
        static_cast< FEMRenderer::RealPropertiesType >(
          edgeColor[ 1 ] ) / maxColorValue,
        static_cast< FEMRenderer::RealPropertiesType >(
          edgeColor[ 2 ] ) / maxColorValue );

      properties->SetEdgeVisibility( 1 );

      } // TrianglesActor
    else if ( whichActor == FEMRenderer::LoadsArrowsActor )
      {

      // we define a scale factor for the arrows

      // first extract the glyph representing the arrows
      typedef vtkGlyph3D ArrowsType;
      typedef vtkSmartPointer< ArrowsType > ArrowsPointer;

      // try to cast the algorithm as an arrows type
      const ArrowsPointer arrows =
        dynamic_cast< ArrowsType * >(
          actor->GetMapper()->GetInputAlgorithm() );

      if ( arrows )
        {
        arrows->SetScaleFactor( this->m_ArrowScaling );
        } // arrows
      
      } // LoadsArrowsActor

    } // for each actor

} // UpdateActorProperties
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SecretData::AddActors()
{

  const bool verbose = this->m_Verbose;
  
  const ActorCollection & ourActors = this->m_Actors;

  // extract our renderer
  const vtkRendererPointer & renderer = this->m_Renderer;

  if ( !renderer )
    {
    if ( verbose )
      cerr << "Warning: No renderer. Returning." << endl;
    return;
    } // renderer

  // extract all the existing actors in the scene
  typedef vtkSmartPointer< vtkActorCollection > vtkActorCollectionPointer;
  const vtkActorCollectionPointer
    existingActors = renderer->GetActors();
  
  // loop through our actors
  BOOST_FOREACH( const ActorToIndexMap::value_type & iter, SecretData::m_ActorToIndex )
    {

    const IndexType index = iter.second;

    // get the actor
    const vtkActorPointer actor = ourActors[ index ];

    const bool addActor = actor && !existingActors->IsItemPresent( actor );

    if ( addActor )
      renderer->AddActor( actor );

    }

} // AddActors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::RemoveActors()
{
  
  const bool verbose = this->m_SecretData->m_Verbose;
  
  const SecretData::ActorCollection
    & ourActors = this->m_SecretData->m_Actors;

  // extract our renderer
  const vtkRendererPointer & renderer = this->m_SecretData->m_Renderer;

  if ( !renderer )
    {
    if ( verbose )
      cerr << "Warning: No renderer. Returning." << endl;
    return;
    } // renderer

  // extract all the existing actors in the scene
  typedef vtkSmartPointer< vtkActorCollection > vtkActorCollectionPointer;
  const vtkActorCollectionPointer
    existingActors = renderer->GetActors();
  
  if ( !existingActors )
    return;

  // loop through our actors
  BOOST_FOREACH( const SecretData::ActorToIndexMap::value_type & iter,
    SecretData::m_ActorToIndex )
    {

    const SecretData::IndexType index = iter.second;

    // get the actor
    const vtkActorPointer & actor = ourActors[ index ];

    const bool removeActor = actor && existingActors->IsItemPresent( actor );

    if ( removeActor )
      renderer->RemoveActor( actor );

    } // for each iter

} // RemoveActors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SetFEM( const FEMObjectConstPointer & FEM )
{

  // first remove existing actors
  this->RemoveActors();

  // set the FEM
  this->m_SecretData->m_FEM = FEM;

  // generate the actors and update
  this->m_SecretData->RegenerateActors();
  this->m_SecretData->AddActors();
  this->m_SecretData->ReRender();

} // SetFEM
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SetRenderer( const vtkRendererPointer & renderer )
{

  this->m_SecretData->m_Renderer = renderer;

  this->m_SecretData->AddActors();
  this->m_SecretData->ReRender();

} // SetRenderer
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SetFaceColor(
  const WhichActor  actor,
  const ColorType & color )
{
  this->m_SecretData->m_FaceColors[
    SecretData::m_ActorToIndex.at( actor ) ] = color;
  this->m_SecretData->ReRender();
} // SetFaceColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SetEdgeColor(
  const WhichActor  actor,
  const ColorType & color )
{
  this->m_SecretData->m_EdgeColors[
    SecretData::m_ActorToIndex.at( actor ) ] = color;
  this->m_SecretData->ReRender();
} // SetEdgeColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SetOpacity(
  const WhichActor         actor,
  const RealPropertiesType opacity )
{
  this->m_SecretData->m_Opacities[
    SecretData::m_ActorToIndex.at( actor ) ] = opacity;
  this->m_SecretData->ReRender();
} // SetOpacity
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRenderer::ColorType FEMRenderer::GetFaceColor(
  const WhichActor actor ) const
{
  return this->m_SecretData->m_FaceColors[
    SecretData::m_ActorToIndex.at( actor ) ];
} // GetFaceColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRenderer::ColorType FEMRenderer::GetEdgeColor(
  const WhichActor actor ) const
{
  return this->m_SecretData->m_EdgeColors[
    SecretData::m_ActorToIndex.at( actor ) ];
} // GetEdgeColor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMRenderer::RealPropertiesType FEMRenderer::GetOpacity(
  const WhichActor actor ) const
{
  return this->m_SecretData->m_Opacities[
    SecretData::m_ActorToIndex.at( actor ) ];
} // GetOpacity
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::SetArrowScaling( const RealPropertiesType scaling )
{
  this->m_SecretData->m_ArrowScaling = scaling;
  this->m_SecretData->ReRender();
} // SetArrowScaling
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMRenderer::UpdateFEMRendering( const itk::fem::LinearSystemWrapper & solution )
{

  const bool verbose = this->m_SecretData->m_Verbose;

  const FEMObjectConstPointer startingFEM = this->m_SecretData->m_FEM;

  if ( !startingFEM )
    {
    if ( verbose ) cerr << "Warning: No starting FEM! Returning." << endl;
    return;
    } // startingFEM

  // deform the FEM with the given solution
  if ( verbose ) cout << "Deforming original FEM:" << endl;
  const FEMObjectPointer deformedFEM(
    DeformFEM( startingFEM.GetPointer(), solution ) );
  if ( verbose ) cout << "Deforming original FEM done." << endl;

  // update the FEM loads
  if ( verbose ) cout << "Updating FEM loads:" << endl;
  UpdateFEMLoads( *deformedFEM, solution );
  if ( verbose ) cout << "Updating FEM loads done." << endl;

  // update with the deformed FEM
  if ( verbose ) cout << "Setting deformed FEM:" << endl;
  this->SetFEM( deformedFEM.GetPointer() );
  if ( verbose ) cout << "Setting deformed FEM done." << endl;

  // now that we're done, replace the original FEM
  // so that next time when we deform, we have the original

  // set the FEM
  this->m_SecretData->m_FEM = startingFEM;

} // UpdateFEMRendering
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( FEMRenderer, Verbose, bool )
ccipdGetConstImplementMacro( FEMRenderer, ArrowScaling, FEMRenderer::RealPropertiesType )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
