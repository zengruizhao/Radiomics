


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFARenderingRefresh.h"
#include "ccipdMFAModel.h"
#include "ccipdMFALevelsetExtractor.h"
#include "ccipdSIMStorage.h"
#include "ccipdExtractFeatureImage.h"
#include "ccipdIsosurfaceRendering.h"

// vtk includes
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkActorCollection.h>

#ifdef _MSC_VER
#pragma warning( disable: 4310 )
#pragma warning( disable: 4512 )
#endif

// itk includes
#include <itkVectorImage.hxx>
#include <itkImage.h>

#ifdef _MSC_VER
#pragma warning( default: 4310 )
#pragma warning( default: 4512 )
#endif

// boost includes
#include <boost/foreach.hpp>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
vtkActorCollectionPointer RefreshRendering(
        vtkRenderWindow & renderWindow,
  const MFAModel        & model )
{

  // prepare the output
  const vtkActorCollectionPointer
    actors = vtkActorCollectionPointer::New();

  // clear the existing renderer
  const vtkRendererPointer
    renderer = renderWindow.GetRenderers()->GetFirstRenderer();
  if ( !renderer ) return actors;
  renderer->RemoveAllViewProps();

  // get the levelset extractor
  const MFALevelsetExtractorConstPointer
    levelsetExtractor = model.GetLevelsetExtractor();
  if ( !levelsetExtractor ) return actors;

  // get the trained MFA
  const MFAVolumeBaseConstPointer trainedMFA = model.GetTrainedSIM();
  if ( !trainedMFA ) return actors;

  // get the levelset features
  const FeatureIDConstCollection whichLevelsetFeatures = 
    levelsetExtractor->GetWhichFeatures();

  // loop through each levelset feature
  BOOST_FOREACH( const FeatureIDConstPointer & whichLevelsetFeature,
    whichLevelsetFeatures )
    {
    
    // extract the levelset values
    const auto levelsetFeatures =
      trainedMFA->GetMeanAndEigenvectors( whichLevelsetFeature );

    if (!levelsetFeatures) return actors;

    // extract the mean levelset
    const unsigned int meanIndex = 0;

    const VolumeImageConstPointer meanLevelset(
      ExtractFeatureImage( levelsetFeatures, meanIndex ) );

    // render it
    const InternalPixelType levelsetValue =
      -std::numeric_limits< InternalPixelType >::epsilon();
    const vtkActorPointer levelsetActor =
      GenerateVTKActorFromIsosurface( meanLevelset, levelsetValue );

    // add the actor to the scene
    renderer->AddActor( levelsetActor );

    // add it to our collection
    actors->AddItem( levelsetActor );

    } // for each feature

  // re-render
  renderer->ResetCamera();
  renderWindow.Render();
  renderWindow.Render();

  // output
  return actors;

} // RefreshRendering
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
