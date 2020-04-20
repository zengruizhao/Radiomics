


###########################################################################################
IF( LIB_BUILD_COMMON_MATH AND NOT BUILD_BOOST_PROJECTS )
   MESSAGE( "Common math library requires the "   ) 
   MESSAGE( "Boost library. Disabling the "       )
   MESSAGE( "common math library."                )
   SET ( ENABLE_COMMON_MATH FALSE )
ENDIF()

IF( LIB_BUILD_COMMON_MATH AND NOT BUILD_EIGEN_PROJECTS )
   MESSAGE( "Common math library requires the " ) 
   MESSAGE( "Eigen library. Disabling the "     )
   MESSAGE( "common math library."              )
   SET ( ENABLE_COMMON_MATH FALSE )
ENDIF()
###########################################################################################



###########################################################################################
# Say where our files are
SET( COMMON_MATH_INCLUDE_DIR   "${COMMON_MATH_DIR}/include"   )
SET( COMMON_MATH_TEMPLATES_DIR "${COMMON_MATH_DIR}/templates" )
SET( COMMON_MATH_SOURCE_DIR    "${COMMON_MATH_DIR}/source"    )
SET( COMMON_MATH_TEST_DIR      "${COMMON_MATH_DIR}/test"      )

SET( COMMON_INCLUDE_DIRS
  ${COMMON_INCLUDE_DIRS}
  "${COMMON_MATH_INCLUDE_DIR}"
  "${COMMON_MATH_TEMPLATES_DIR}"
)
###########################################################################################


###########################################################################################
# Set up the files
SET( COMMON_MATH_FILES
  "${COMMON_MATH_INCLUDE_DIR}/ccipdDistribution.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdDistributionTypes.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdGammaDistribution.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdGaussianDistribution.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdGetMaxPDF.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdMath.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdMatrixTypes.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdMatrixUtils.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdMultivariateGaussianDistribution.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdStandardDistribution.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdUnivariateDistribution.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdUnivariateGaussianDistribution.h"
  "${COMMON_MATH_INCLUDE_DIR}/ccipdUniformDistribution.h"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdDistribution.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdGammaDistribution.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdGaussianDistribution.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdGetMaxPDF.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdMatrixUtils.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdMultivariateGaussianDistribution.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdStandardDistribution.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdUnivariateDistribution.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdUnivariateGaussianDistribution.hxx"
  "${COMMON_MATH_TEMPLATES_DIR}/ccipdUniformDistribution.hxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdDistribution.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdGammaDistribution.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdGaussianDistribution.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdGetMaxPDF.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdMath.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdMultivariateGaussianDistribution.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdStandardDistribution.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdUnivariateDistribution.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdUnivariateGaussianDistribution.cxx"
  "${COMMON_MATH_SOURCE_DIR}/ccipdUniformDistribution.cxx"
)
SET( TEST_RANDOM_FILES "${COMMON_MATH_TEST_DIR}/TestRandom.cxx" )
SET( TEST_MATRIXUTILS_FILES "${COMMON_MATH_TEST_DIR}/TestMatrixUtils.cxx" )

###########################################################################################



###########################################################################################
CONFIGURE_FILE(
  "${COMMON_MATH_INCLUDE_DIR}/ccipdCommonMathOptions.h.in" # this is the template to create a real .h file
  "${Common_BINARY_DIR}/ccipdCommonMathOptions.h"    # this should be included in your project
) 
###########################################################################################



###########################################################################################
# Create the libraries
IF( ENABLE_COMMON_MATH )
  OPTION( SHARED_COMMON_MATH
    "Build common math as a shared (TRUE/checked) or static (FALSE/unchecked) library" OFF )
  IF( SHARED_MATH )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_MATH )
  ADD_LIBRARY( ccipdCommonMath ${LIB_TYPE} ${COMMON_MATH_FILES} )
ENDIF( ENABLE_COMMON_MATH )
###########################################################################################



###########################################################################################
IF( BUILD_TESTING AND ENABLE_COMMON_MATH )
   ADD_EXECUTABLE( TestRandom    ${TEST_RANDOM_FILES} )
  ADD_EXECUTABLE( TestMatrixUtils ${TEST_MATRIXUTILS_FILES} )
ENDIF()
###########################################################################################



###########################################################################################
# Link the libraries
IF( ENABLE_COMMON_MATH )
  SET( COMMON_MATH_LIBS "ccipdCommonMath" ${COMMON_CORE_LIBS} )
  TARGET_LINK_LIBRARIES( ccipdCommonMath ${COMMON_MATH_LIBS} )
ENDIF()

IF( BUILD_TESTING AND ENABLE_COMMON_MATH )
  TARGET_LINK_LIBRARIES( TestRandom ${COMMON_MATH_LIBS} ${Boost_LIBRARIES} )
  TARGET_LINK_LIBRARIES( TestMatrixUtils ${COMMON_MATH_LIBS} ${COMMON_CORE_LIBS} )
ENDIF()
###########################################################################################



###########################################################################################
IF(ENABLE_COMMON_MATH)
MESSAGE(STATUS "Building common math library: " ${ENABLE_COMMON_MATH} )
ELSE()
MESSAGE( "Building common math library: " ${ENABLE_COMMON_MATH} )
ENDIF()
###########################################################################################