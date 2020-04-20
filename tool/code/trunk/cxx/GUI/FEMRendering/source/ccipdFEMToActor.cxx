


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"
#include "ccipdFEMToActor.h"
#include "ccipdFEMToMesh.h"         // for creating a mesh from that FEM
#include "ccipdMeshToActor.h"       // for converting the mesh tetrahedrons to an actor
#include "ccipdLandmarksToActor.h"  // for converting the mesh points to an actor
#include "ccipdPolyDataToArrows.h"  // for converting the FEM loads to arrows
#include "ccipdPolyDataToSpheres.h" // for converting the FEM loads to spheres
#include "ccipdFEMNodeToElement.h"  // for extracting node index
#include "ccipdHashLandmark.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#pragma warning( disable: 4701 )
#pragma warning( disable: 4702 )
#endif // _MSC_VER

// vtk includes
#include <vtkSmartPointer.h>     // for memory management
#include <vtkActor.h>            // something to render
#include <vtkPointData.h>        // for defining magnitude and direction of loads
#include <vtkPolyData.h>         // defining our loads
#include <vtkFloatArray.h>       // for defining an array of forces
#include <vtkProperty.h>         // for changing colors

// itk includes
#include <itkFEMObject.h>
#include <itkFEMLoadNode.h>
#include <itkMesh.h>

// boost includes
#include <boost/foreach.hpp>

#ifdef _MSC_VER
#pragma warning( default: 4701 )
#pragma warning( default: 4702 )
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
struct HashLandmarkFunctor
{
  HashValueType operator()( const LandmarkPointType & landmark ) const
  {
    return HashLandmark( landmark );
  }
  bool operator()(
    const LandmarkPointType & landmark1,
    const LandmarkPointType & landmark2 ) const
  {
    return
      landmark1[ 0 ] == landmark2[ 0 ] &&
      landmark1[ 1 ] == landmark2[ 1 ] &&
      landmark1[ 2 ] == landmark2[ 2 ];
  }
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
vtkPolyDataPointer GeneratePolyDataFromFEMLoads( const FEMObjectType * const FEM )
{

  // store the poly data for each load point
  const vtkPointsPointer loadNodePoints = vtkPointsPointer::New();

  // store the force for each load point
  typedef vtkSmartPointer< vtkFloatArray > vtkFloatArrayPointer;
  const vtkFloatArrayPointer loadNodeForces = vtkFloatArrayPointer::New();
  loadNodeForces->SetNumberOfComponents( FEMDimension );

  // store the force for each load point
  using itk::fem::Element;

  // we will remember which boundary conditions
  // we have for each dimension
  typedef vnl_vector< Element::Float > ForceType;
  typedef std::unordered_map<
    LandmarkPointType,
    ForceType,
    HashLandmarkFunctor,
    HashLandmarkFunctor 
  > CoordinatesToValueMap;
  CoordinatesToValueMap coordinatesToValue;

  const double forceFactor = -1; // we want them facing towards the point

  // extract the load locations
  const unsigned int numberOfLoads = FEM->GetNumberOfLoads();
  for ( unsigned int loadIndex = 0; loadIndex < numberOfLoads; ++loadIndex )
    {

    // extract the current load
    using itk::fem::Load;
    using itk::fem::LoadNode;
    using itk::fem::LoadBC;

    const Load::ConstPointer currentLoad(
      FEM->GetLoad( loadIndex ) );

    const LoadNode::ConstPointer currentLoadNode(
      dynamic_cast< const LoadNode * >( currentLoad.GetPointer() ) );
    const LoadBC::ConstPointer currentLoadBC(
      dynamic_cast< const LoadBC * >( currentLoad.GetPointer() ) );

    // if successful, we have a load node
    if ( currentLoadNode )
      {

      // which vertex of the current element (e.g. tetrahedron) are we at?
      const int vertexIndex = currentLoadNode->GetNode();

      // extract the coordinates
      const Element::VectorType & coordinates =
        currentLoad->GetElement()->GetNodeCoordinates( vertexIndex );

      // add the coordinates to the collection of points
      loadNodePoints->InsertNextPoint(
        coordinates[ 0 ],
        coordinates[ 1 ],
        coordinates[ 2 ] );

      // remember the force
      const ForceType currentForce = currentLoadNode->GetForce();

      loadNodeForces->InsertNextTuple3(
        forceFactor * currentForce[ 0 ],
        forceFactor * currentForce[ 1 ],
        forceFactor * currentForce[ 2 ] );

      //loadNodeForcesArray.SetNextTuple( 0, 

      } // currentLoadNode
    else if ( currentLoadBC )
      {
      unsigned int dimension = 0;
      const unsigned long vertexIndex =
        GetNodeIndexFromLocalDegreeOfFreedom(
          *currentLoad->GetElement(),
          currentLoadBC->GetDegreeOfFreedom(),
          dimension );
      
      // extract the coordinates
      const Element::VectorType & coordinatesAsVector =
        currentLoad->GetElement()->GetNodeCoordinates( vertexIndex );
      LandmarkPointType coordinatesAsPoint;
      
      for ( unsigned int index = 0; index < 3; ++index )
        coordinatesAsPoint[ index ] =
          static_cast< LandmarkPointType::ValueType >(
            coordinatesAsVector[ index ] );

      // see if we have those coordinates in the map
      if ( coordinatesToValue.find( coordinatesAsPoint ) == coordinatesToValue.cend() )
        {
        // no - initialize
        coordinatesToValue[ coordinatesAsPoint ] = ForceType( FEMDimension, 0.0 );
        }
      
      // update our coordinates
      coordinatesToValue[ coordinatesAsPoint ][ dimension ] =
        currentLoadBC->GetValue()[ 0 ];

      } // currentLoadBC

    } // loadIndex

  // now add in any boundary conditions we've stored
  BOOST_FOREACH( auto & boundaryConditionPair, coordinatesToValue )
    {
      

    // add the coordinates to the collection of points
    loadNodePoints->InsertNextPoint(
      boundaryConditionPair.first[ 0 ],
      boundaryConditionPair.first[ 1 ],
      boundaryConditionPair.first[ 2 ] );

    loadNodeForces->InsertNextTuple3(
      forceFactor * boundaryConditionPair.second[ 0 ],
      forceFactor * boundaryConditionPair.second[ 1 ],
      forceFactor * boundaryConditionPair.second[ 2 ] );

    } // for each boundary condition pair

  // create some poly data from the points
  const vtkPolyDataPointer polyData = vtkPolyDataPointer::New();

  // give the points to the polydata
  polyData->SetPoints( loadNodePoints );

  // give the forces to the poly data
  polyData->GetPointData()->SetVectors( loadNodeForces );

  return polyData;

} // GeneratePolyDataFromFEMLoads
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer GenerateArrowsActorFromFEMLoads(
  const FEMObjectType * const FEM,
  const double                scaleFactor )
{

  // extract the poly data
  const vtkPolyDataPointer polyData = GeneratePolyDataFromFEMLoads( FEM );

  // extract an actor in which the arrows point towards the point
  const vtkActorPointer loadsActor = GenerateArrowsFromPolyData(
    polyData, false, scaleFactor );

  return loadsActor;

} // GenerateVTKActorsFromFEM
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void GenerateVTKActorsFromFEM(
  const FEMObjectType * const FEM,
        vtkActorPointer & landmarksActor,    // output #1
        vtkActorPointer & trianglesActor,    // output #2
        vtkActorPointer & loadsArrowsActor,  // output #3
        vtkActorPointer & loadsSpheresActor, // output #4
  const double            landmarkSphereRadius,
  const double            loadSphereRadius,
  const double            loadArrowScalingFactor,
  const bool              verbose )
{

  if ( verbose ) cout << "FEM to Mesh:" << endl;
  const VolumeMeshConstPointer mesh( ConvertFEMToMesh( FEM ) );
  if ( verbose ) cout << "FEM to Mesh done." << endl;

  // generate landmarks actor
  if ( verbose ) cout << "Mesh to Landmarks Actor:" << endl;
  landmarksActor = GenerateActorFromLandmarks( mesh, landmarkSphereRadius );
  if ( verbose ) cout << "Mesh to Landmarks Actor done." << endl;

  // generate triangles actor
  if ( verbose ) cout << "Mesh to Triangles Actor:" << endl;
  trianglesActor = GenerateTrianglesActorFromMesh( mesh );
  if ( verbose ) cout << "Mesh to Triangles Actor done." << endl;

  // generate poly data
  const vtkPolyDataPointer loadsPolyData = GeneratePolyDataFromFEMLoads( FEM );

  // generate loads arrow actor
  if ( verbose ) cout << "FEM to Loads Actor:" << endl;
  loadsArrowsActor = GenerateArrowsFromPolyData(
    loadsPolyData, false, loadArrowScalingFactor );
  if ( verbose ) cout << "Mesh to Triangles Actor done." << endl;

  // generate loads sphere actor
  if ( verbose ) cout << "FEM to Loads Actor:" << endl;
  loadsSpheresActor = GenerateSpheresFromPolyData( loadsPolyData, loadSphereRadius );
  loadsSpheresActor->GetProperty()->SetColor( 1.0, 0.2, 0.0 );
  if ( verbose ) cout << "Mesh to Triangles Actor done." << endl;

} // GenerateVTKActorsFromFEM
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
