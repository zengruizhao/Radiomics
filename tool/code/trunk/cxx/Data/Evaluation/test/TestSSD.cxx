


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdLandmarkTypes.h"
#include "ccipdLoadTextLandmarks.h"
#include "ccipdApplyTransformation.h"
#include "ccipdCalculateLandmarkRMSE.h"

//std includes
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkAffineTransform.h>
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////


int main( int argc, char *argv[] )
{

  if ( argc < 1 )
    return EXIT_FAILURE;

  const std::string landmarks1(argv[1]);
  ccipd::LandmarkSetConstPointer       fixedPointSet;
  ccipd::LandmarkSetConstPointer       movedPointSet;

	//first load in the landmark points
  try
  {
    fixedPointSet=ccipd::LoadLandmarkSetFromTextFile(landmarks1);

  }

  // Since you aren't using err, Visual Studio gives a warning
  // about it being unused.  So I have replaced it with catching
  // any possible error (not just itk errors).
  // The alternative is to do std::cerr << err << std::endl so
  // feel free to change that if you want.

  //catch(itk::ExceptionObject & err)
  catch( ... )
  {
    std::cerr << "Unable to load point sets" << std::endl;
    return EXIT_FAILURE;
  }
  typedef itk::AffineTransform<
    double, ccipd::LandmarkDimension > LandmarkTransformType;
  typedef LandmarkTransformType::ParametersType      LandmarkParametersType;

  LandmarkTransformType::Pointer transform = LandmarkTransformType::New();
  LandmarkParametersType initialParameters(
    static_cast< LandmarkParametersType::SizeValueType >(
      transform->GetNumberOfParameters() ) );

  initialParameters[0]  =1;
  initialParameters[4]  =1;
  initialParameters[8]  =1;
  initialParameters[1] =0;
  initialParameters[2] =0;
  initialParameters[3] =0;
  initialParameters[5] =0;
  initialParameters[6] =0;
  initialParameters[7]  =0;
  //add in translation in the x dimension
  initialParameters[9]   =10;
  initialParameters[10]  =0;
  initialParameters[11]  =0;

  transform->SetParameters(initialParameters);
  ccipd::VolumeTransformConstPointer constTransform=
    static_cast< ccipd::VolumeTransformConstPointer >( transform );

 movedPointSet = ccipd::ApplyTransform(transform, fixedPointSet);


  try
  {
     const auto ssd= ccipd::CalculateLandmarkRMSE(fixedPointSet,movedPointSet);
     std::cout << std::endl << "RMSE = " << ssd << std::endl << std::endl;
  }
  catch(...)
  {
    std::cerr << "Unable to calculate RMSE" << std::endl;
    return EXIT_FAILURE;
  }
  return 0;
}
