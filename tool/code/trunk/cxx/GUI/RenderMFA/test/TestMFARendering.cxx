


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"
#include "ccipdMFASegmenterOptionsGUI.h"
#include "ccipdMFASegmenter.h"
#include "ccipdRegisterMFAAlgorithm.h"
#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER
 
// qt includes
#include <QApplication>

// std includes
#include <cstdlib>
#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////


int main( int argc, char * argv[] )
{

  // "using" declarations
  using ccipd::MFASegmenter;
  using ccipd::ModifyMFASegmenterOptions;
  using ccipd::RegisterMFAAlgorithm;
  using ccipd::RegisterMFAAlgorithmPointer;

  // create our qt application
  QApplication application( argc, argv );

  // create a new segmenter object
  const MFASegmenter::Pointer segmenterPointer = MFASegmenter::New();
  MFASegmenter & segmenter = *segmenterPointer;

  // set some properties of the segmenter
  segmenter.SetSegmentationObject( "Prostate" );

  // create some registration algorithms for the segmenter
  const RegisterMFAAlgorithmPointer
    algorithm1( RegisterMFAAlgorithm::New() ),
    algorithm2( RegisterMFAAlgorithm::New() );

  // make different properties of each algorithm
  typedef RegisterMFAAlgorithm::InterpolatorType InterpolatorType;

  const  RegisterMFAAlgorithm::InterpolatorPointer interpolator1  = 
    std::dynamic_pointer_cast< InterpolatorType >( algorithm1->GetInterpolator() ); 
  interpolator1->SetWhichInterpolator( InterpolatorType::LinearInterpolator );

    const  RegisterMFAAlgorithm::InterpolatorPointer interpolator2  = 
    std::dynamic_pointer_cast< RegisterMFAAlgorithm::InterpolatorType >( algorithm2->GetInterpolator() ); 
    interpolator2->SetWhichInterpolator( InterpolatorType::NearestNeighborInterpolator );

    // get the transform class
    RegisterMFAAlgorithm::TransformPointer transformClass1(
      std::dynamic_pointer_cast< RegisterMFAAlgorithm::TransformType >( 
        algorithm1->GetTransform() ) );
    transformClass1->SetWhichTransform( RegisterMFAAlgorithm::TransformType::RigidTransform );

    RegisterMFAAlgorithm::TransformPointer transformClass2(
      std::dynamic_pointer_cast< RegisterMFAAlgorithm::TransformType >( 
        algorithm2->GetTransform() ) );
    transformClass2->SetWhichTransform( RegisterMFAAlgorithm::TransformType::AffineTransform );

  // give the algorithms to the segmenter
  ccipd::RegisterMFAAlgorithmCollection algorithms;
  algorithms.push_back( algorithm1 );
  algorithms.push_back( algorithm2 );

  segmenter.SetRegistrationAlgorithms( algorithms );

  // show the GUI to modify the segmenter
  ModifyMFASegmenterOptions( segmenter );

  // exit
  return EXIT_SUCCESS;

} // main
