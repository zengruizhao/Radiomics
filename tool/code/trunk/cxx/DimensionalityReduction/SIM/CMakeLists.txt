


###########################################################################################
IF( ENABLE_DIMENSIONALITY_REDUCTION_SIM AND NOT ENABLE_DATA_PATIENTS )
  MESSAGE( "Dimensionality reduction SIM library "          )
  MESSAGE( "requires the data patients library. Disabling " )
  MESSAGE( "the dimensionality reduction SIM library."      )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_SIM FALSE )
ENDIF() 

IF ( ENABLE_DIMENSIONALITY_REDUCTION_SIM AND NOT ENABLE_DIMENSIONALITY_REDUCTION_CORE )
  MESSAGE( "Dimensionality reduction SIM library "                 )
  MESSAGE( "requires the dimensionality reduction core. Disabling" )
  MESSAGE( " the dimensionality reduction SIM library."            )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_SIM FALSE )
ENDIF ( ENABLE_DIMENSIONALITY_REDUCTION_SIM AND NOT ENABLE_DIMENSIONALITY_REDUCTION_CORE )


IF( ENABLE_DIMENSIONALITY_REDUCTION_SIM AND NOT ENABLE_REGISTRATION_METRICS)
  MESSAGE( "Dimensionality reduction SIM library "                 )
  MESSAGE( "requires the registration metrics library. Disabling"  )
  MESSAGE( " the dimensionality reduction SIM library."            )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_SIM FALSE )
ENDIF()
IF( ENABLE_DIMENSIONALITY_REDUCTION_SIM AND NOT ENABLE_REGISTRATION_ALGORITHMS )
  MESSAGE( "Dimensionality reduction SIM library "                 )
  MESSAGE( "requires the registration algorithms library. "        )
  MESSAGE( "Disabling the dimensionality reduction SIM library."   )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_SIM FALSE )
ENDIF()
###########################################################################################



###########################################################################################
# Say where our files are
SET( DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR
  "${DIMENSIONALITY_REDUCTION_SIM_DIR}/include"  )
SET( DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR
  "${DIMENSIONALITY_REDUCTION_SIM_DIR}/source"  )
SET( DIMENSIONALITY_REDUCTION_SIM_TEMPLATES_DIR
  "${DIMENSIONALITY_REDUCTION_SIM_DIR}/templates"  )
  
SET( DIMENSIONALITY_REDUCTION_INCLUDE_DIRS
  ${DIMENSIONALITY_REDUCTION_INCLUDE_DIRS}
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}" 
  "${DIMENSIONALITY_REDUCTION_SIM_TEMPLATES_DIR}")
###########################################################################################



###########################################################################################
# List our files
SET( DIMENSIONALITY_REDUCTION_SIM_FILES
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMBoundingBoxExtractor.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMBoundingBoxUnion.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMEstimator.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMFeatureExtractor.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMForwardDeclarations.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMFeatureExtractorInsertion.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMFeaturePreparerUsingExtractors.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMFeaturePreparerInterface.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMModel.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMTypes.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMSampleImage.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMStorage.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMStudiesAligner.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/ccipdSIMStudiesMaskAligner.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/itkPCAShapeModelEstimatorEigen.h"
  "${DIMENSIONALITY_REDUCTION_SIM_INCLUDE_DIR}/itkSIMRegularizedMetric.h"
  "${DIMENSIONALITY_REDUCTION_SIM_TEMPLATES_DIR}/ccipdSIMSampleImage.hxx"
  "${DIMENSIONALITY_REDUCTION_SIM_TEMPLATES_DIR}/ccipdSIMStorage.hxx"
  "${DIMENSIONALITY_REDUCTION_SIM_TEMPLATES_DIR}/itkPCAShapeModelEstimatorEigen.hxx"
  "${DIMENSIONALITY_REDUCTION_SIM_TEMPLATES_DIR}/itkSIMRegularizedMetric.hxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMBoundingBoxExtractor.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMBoundingBoxUnion.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMEstimator.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMFeatureExtractor.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMFeatureExtractorInsertion.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMFeaturePreparerUsingExtractors.cxx"  
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMModel.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMSampleImage.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMStorage.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMStudiesAligner.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/ccipdSIMStudiesMaskAligner.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/itkPCAShapeModelEstimatorEigen.cxx"
  "${DIMENSIONALITY_REDUCTION_SIM_SOURCE_DIR}/itkSIMRegularizedMetric.cxx"
)
###########################################################################################


  
###########################################################################################
# Create our libraries
IF( ENABLE_DIMENSIONALITY_REDUCTION_SIM )

  OPTION ( SHARED_DIMENSIONALITY_REDUCTION_SIM
    "Build DimensionalityReduction SIM as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
    
  IF ( SHARED_DIMENSIONALITY_REDUCTION_SIM )
    SET ( LIB_TYPE SHARED )
  ELSE()
    SET ( LIB_TYPE STATIC )
  ENDIF( SHARED_DIMENSIONALITY_REDUCTION_SIM )  
  
  ADD_LIBRARY( ccipdDimensionalityReductionSIM
    ${LIB_TYPE} ${DIMENSIONALITY_REDUCTION_SIM_FILES} )
  
ENDIF( ENABLE_DIMENSIONALITY_REDUCTION_SIM )
###########################################################################################



###########################################################################################
# Link our libraries
IF( ENABLE_DIMENSIONALITY_REDUCTION_SIM )

  SET( DIMENSIONALITY_REDUCTION_SIM_LIBS
    "ccipdDimensionalityReductionSIM"
    ${DIMENSIONALITY_REDUCTION_CORE_LIBS}
    ${REGISTRATION_ALGORITHMS_LIBS}
  )

  TARGET_LINK_LIBRARIES( ccipdDimensionalityReductionSIM
    ${DIMENSIONALITY_REDUCTION_SIM_LIBS} )

ENDIF( ENABLE_DIMENSIONALITY_REDUCTION_SIM )
###########################################################################################



###########################################################################################
IF(ENABLE_DIMENSIONALITY_REDUCTION_SIM)
MESSAGE(STATUS "Building DimensionalityReduction sim library: " ${ENABLE_DIMENSIONALITY_REDUCTION_SIM} )
ELSE()
MESSAGE( "Building DimensionalityReduction sim library: " ${ENABLE_DIMENSIONALITY_REDUCTION_SIM} )
ENDIF()
###########################################################################################
