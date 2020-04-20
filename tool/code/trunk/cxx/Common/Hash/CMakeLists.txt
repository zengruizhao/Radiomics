


###########################################################################################
# Say where our files are
SET( COMMON_HASH_INCLUDE_DIR   "${COMMON_HASH_DIR}/include"   )
SET( COMMON_HASH_TEMPLATES_DIR "${COMMON_HASH_DIR}/templates" )
SET( COMMON_HASH_SOURCE_DIR    "${COMMON_HASH_DIR}/source"    )
SET( COMMON_HASH_TEST_DIR      "${COMMON_HASH_DIR}/test"      )

SET( COMMON_INCLUDE_DIRS
  ${COMMON_INCLUDE_DIRS}
  "${COMMON_HASH_INCLUDE_DIR}"
  "${COMMON_HASH_TEMPLATES_DIR}"
)
###########################################################################################



###########################################################################################

SET( COMMON_HASH_FILES
  "${COMMON_HASH_INCLUDE_DIR}/ccipdHashImage.h"
  "${COMMON_HASH_INCLUDE_DIR}/ccipdHashable.h"
  "${COMMON_HASH_TEMPLATES_DIR}/ccipdHashImage.hxx"
  "${COMMON_HASH_SOURCE_DIR}/ccipdHashable.cxx"
  "${COMMON_HASH_SOURCE_DIR}/ccipdHashImage.cxx"
)

IF ( BUILD_BOOST_PROJECTS )
 # Common files which depend on boost
  SET( COMMON_HASH_FILES
    ${COMMON_HASH_FILES}
    "${COMMON_HASH_INCLUDE_DIR}/ccipdHashCollection.h"
    "${COMMON_HASH_INCLUDE_DIR}/ccipdHashImageIndex.h"
    "${COMMON_HASH_INCLUDE_DIR}/ccipdHashOptimizerParameters.h"
    "${COMMON_HASH_INCLUDE_DIR}/ccipdHashLandmark.h"
    "${COMMON_HASH_INCLUDE_DIR}/ccipdHashLandmarks.h"
    "${COMMON_HASH_TEMPLATES_DIR}/ccipdHashCollection.hxx" 
    "${COMMON_HASH_TEMPLATES_DIR}/ccipdHashImageIndex.hxx"
    "${COMMON_HASH_TEMPLATES_DIR}/ccipdHashOptimizerParameters.hxx" 
    "${COMMON_HASH_TEMPLATES_DIR}/ccipdHashLandmark.hxx"
    "${COMMON_HASH_TEMPLATES_DIR}/ccipdHashLandmarks.hxx"
    "${COMMON_HASH_SOURCE_DIR}/ccipdHashCollection.cxx" 
    "${COMMON_HASH_SOURCE_DIR}/ccipdHashImageIndex.cxx"
    "${COMMON_HASH_SOURCE_DIR}/ccipdHashLandmark.cxx"
    "${COMMON_HASH_SOURCE_DIR}/ccipdHashLandmarks.cxx"
    "${COMMON_HASH_SOURCE_DIR}/ccipdHashOptimizerParameters.cxx" 
  )
ENDIF ( BUILD_BOOST_PROJECTS )
###########################################################################################



###########################################################################################
# Create the libraries
# Note that all libraries below should appear in
# "GenerateCommonHashConfig.cmake"
IF( ENABLE_COMMON_HASH )
  OPTION( SHARED_COMMON_HASH
    "Build common hash as a shared (TRUE/checked) or static (FALSE/unchecked) library" OFF )
  IF( SHARED_COMMON_HASH )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_COMMON_HASH )
  ADD_LIBRARY( ccipdCommonHash ${LIB_TYPE} ${COMMON_HASH_FILES} )
ENDIF( ENABLE_COMMON_HASH )
###########################################################################################



###########################################################################################
# Link the libraries
IF( ENABLE_COMMON_HASH )
  SET( COMMON_HASH_LIBS
    "ccipdCommonHash"
    ${ITK_LIBRARIES}
  )
  
  TARGET_LINK_LIBRARIES( ccipdCommonHash ${COMMON_HASH_LIBS} )
ENDIF( ENABLE_COMMON_HASH )
###########################################################################################



###########################################################################################
IF(ENABLE_COMMON_HASH)
MESSAGE(STATUS "Building common hash library: " ${ENABLE_COMMON_HASH} )
ELSE()
MESSAGE( "Building common hash library: " ${ENABLE_COMMON_HASH} )
ENDIF()

###########################################################################################


