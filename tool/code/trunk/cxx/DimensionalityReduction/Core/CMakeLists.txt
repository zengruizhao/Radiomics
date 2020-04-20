


###########################################################################################

IF( ENABLE_DIMENSIONALITY_REDUCTION_CORE AND NOT ENABLE_COMMON_CORE )
  MESSAGE( "Dimensionality reduction core library "       )
  MESSAGE( "requires the common core library. Disabling " )
  MESSAGE( "the dimensionality reduction core library."   )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_CORE FALSE )
ENDIF()

IF ( ENABLE_DIMENSIONALITY_REDUCTION_CORE AND NOT BUILD_BOOST_PROJECTS )
  MESSAGE( "Dimensionality reduction core library "     )
  MESSAGE( "requires the boost library. Disabling the " )
  MESSAGE( "dimensionality reduction core library."     )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_CORE FALSE )
ENDIF ( ENABLE_DIMENSIONALITY_REDUCTION_CORE AND NOT BUILD_BOOST_PROJECTS )


IF ( ENABLE_DIMENSIONALITY_REDUCTION_CORE AND NOT BUILD_EIGEN_PROJECTS )
  MESSAGE( "Dimensionality reduction core library "     )
  MESSAGE( "requires the eigen library. Disabling the " )
  MESSAGE( "dimensionality reduction core library."     )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_CORE FALSE )
ENDIF ( ENABLE_DIMENSIONALITY_REDUCTION_CORE AND NOT BUILD_EIGEN_PROJECTS )

###########################################################################################



###########################################################################################
# Say where our files are
SET( DIMENSIONALITY_REDUCTION_CORE_INCLUDE_DIR
  "${DIMENSIONALITY_REDUCTION_CORE_DIR}/include"  )
SET( DIMENSIONALITY_REDUCTION_CORE_TEMPLATES_DIR
  "${DIMENSIONALITY_REDUCTION_CORE_DIR}/templates"  )
SET( DIMENSIONALITY_REDUCTION_CORE_SOURCE_DIR
  "${DIMENSIONALITY_REDUCTION_CORE_DIR}/source"  )
  
SET( DIMENSIONALITY_REDUCTION_INCLUDE_DIRS
  ${DIMENSIONALITY_REDUCTION_INCLUDE_DIRS}
  "${DIMENSIONALITY_REDUCTION_CORE_TEMPLATES_DIR}"
  "${DIMENSIONALITY_REDUCTION_CORE_INCLUDE_DIR}" )
###########################################################################################



###########################################################################################
# List our files
SET( DIMENSIONALITY_REDUCTION_CORE_FILES
  "${DIMENSIONALITY_REDUCTION_CORE_INCLUDE_DIR}/ccipdDimensionalityReductionGroups.h"
  "${DIMENSIONALITY_REDUCTION_CORE_INCLUDE_DIR}/ccipdMatrixConcatenation.h"
  "${DIMENSIONALITY_REDUCTION_CORE_INCLUDE_DIR}/ccipdPCADistribution.h"
  "${DIMENSIONALITY_REDUCTION_CORE_INCLUDE_DIR}/ccipdPCAProjections.h"
  "${DIMENSIONALITY_REDUCTION_CORE_INCLUDE_DIR}/ccipdPCATraining.h"
  "${DIMENSIONALITY_REDUCTION_CORE_INCLUDE_DIR}/ccipdPseudoInverse.h"
  "${DIMENSIONALITY_REDUCTION_CORE_TEMPLATES_DIR}/ccipdMatrixConcatenation.hxx"
  "${DIMENSIONALITY_REDUCTION_CORE_TEMPLATES_DIR}/ccipdPCADistribution.hxx"
  "${DIMENSIONALITY_REDUCTION_CORE_TEMPLATES_DIR}/ccipdPCAProjections.hxx"
  "${DIMENSIONALITY_REDUCTION_CORE_TEMPLATES_DIR}/ccipdPCATraining.hxx"
  "${DIMENSIONALITY_REDUCTION_CORE_TEMPLATES_DIR}/ccipdPseudoInverse.hxx"
  "${DIMENSIONALITY_REDUCTION_CORE_SOURCE_DIR}/ccipdMatrixConcatenation.cxx"
  "${DIMENSIONALITY_REDUCTION_CORE_SOURCE_DIR}/ccipdPCADistribution.cxx"
  "${DIMENSIONALITY_REDUCTION_CORE_SOURCE_DIR}/ccipdPCAProjections.cxx"
  "${DIMENSIONALITY_REDUCTION_CORE_SOURCE_DIR}/ccipdPCATraining.cxx"
  "${DIMENSIONALITY_REDUCTION_CORE_SOURCE_DIR}/ccipdPseudoInverse.cxx"
)
###########################################################################################


  
###########################################################################################
# Create our libraries
IF( ENABLE_DIMENSIONALITY_REDUCTION_CORE )

  OPTION ( SHARED_DIMENSIONALITY_REDUCTION_CORE
    "Build DimensionalityReduction Core as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
    
  IF ( SHARED_DIMENSIONALITY_REDUCTION_CORE )
    SET ( LIB_TYPE SHARED )
  ELSE()
    SET ( LIB_TYPE STATIC )
  ENDIF( SHARED_DIMENSIONALITY_REDUCTION_CORE )  
  
  ADD_LIBRARY( ccipdDimensionalityReductionCore
    ${LIB_TYPE} ${DIMENSIONALITY_REDUCTION_CORE_FILES} )
  
ENDIF( ENABLE_DIMENSIONALITY_REDUCTION_CORE )
###########################################################################################



###########################################################################################
# Link our libraries
IF( ENABLE_DIMENSIONALITY_REDUCTION_CORE )

  SET( DIMENSIONALITY_REDUCTION_CORE_LIBS
    "ccipdDimensionalityReductionCore"
  )

  TARGET_LINK_LIBRARIES( ccipdDimensionalityReductionCore
    ${DIMENSIONALITY_REDUCTION_CORE_LIBS} )

ENDIF( ENABLE_DIMENSIONALITY_REDUCTION_CORE )
###########################################################################################



###########################################################################################
IF(ENABLE_DIMENSIONALITY_REDUCTION_CORE)
MESSAGE(STATUS "Building DimensionalityReduction core library: " ${ENABLE_DIMENSIONALITY_REDUCTION_CORE} )
ELSE()
MESSAGE( "Building DimensionalityReduction core library: " ${ENABLE_DIMENSIONALITY_REDUCTION_CORE} )
ENDIF()
###########################################################################################
