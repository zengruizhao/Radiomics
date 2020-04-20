
IF(ENABLE_COMMON_LANDMARKS)
	IF(NOT ENABLE_COMMON_IMAGES )
		MESSAGE( "Common Landmarks requires Common Images.")
		SET( ENABLE_COMMON_LANDMARKS FALSE )
	ENDIF()

ENDIF()

IF(NOT ENABLE_COMMON_LANDMARKS)
	MESSAGE("Disabling Common Landmarks library.")
	RETURN()
ENDIF()


###########################################################################################
# Say where our files are
SET( COMMON_LANDMARKS_INCLUDE_DIR   "${COMMON_LANDMARKS_DIR}/include"   )
SET( COMMON_LANDMARKS_TEMPLATES_DIR "${COMMON_LANDMARKS_DIR}/templates" )
SET( COMMON_LANDMARKS_SOURCE_DIR    "${COMMON_LANDMARKS_DIR}/source"    )
SET( COMMON_LANDMARKS_TEST_DIR      "${COMMON_LANDMARKS_DIR}/test"      )

SET( COMMON_INCLUDE_DIRS  ${COMMON_INCLUDE_DIRS}
                         "${COMMON_LANDMARKS_INCLUDE_DIR}"
                         "${COMMON_LANDMARKS_TEMPLATES_DIR}"
						 "${DATA_CORE_INCLUDE_DIR}"
)
###########################################################################################



###########################################################################################
# Set up the files
SET( COMMON_LANDMARKS_FILES
  "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdConcatenateLandmarks.h"
  "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdDuplicateLandmarks.h"
  "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdGetControlPointsFromBSpline.h"
  "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdGetCornerPoints.h"
  "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdGetBSplineFromControlPoints.h"
  "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdLandmarksToMesh.h"
  "${COMMON_LANDMARKS_TEMPLATES_DIR}/ccipdDuplicateLandmarks.hxx" 
  "${COMMON_LANDMARKS_TEMPLATES_DIR}/ccipdGetBSplineFromControlPoints.hxx"
  "${COMMON_LANDMARKS_TEMPLATES_DIR}/ccipdGetControlPointsFromBSpline.hxx"  
  "${COMMON_LANDMARKS_TEMPLATES_DIR}/ccipdGetCornerPoints.hxx"
  "${COMMON_LANDMARKS_TEMPLATES_DIR}/ccipdLandmarksToMesh.hxx"
  "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdConcatenateLandmarks.cxx"
  "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdDuplicateLandmarks.cxx"
  "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdGetBSplineFromControlPoints.cxx"
  "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdGetControlPointsFromBSpline.cxx"
  "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdGetCornerPoints.cxx"
  "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdLandmarksToMesh.cxx"
)

IF( BUILD_VTK_PROJECTS )
 
 SET( COMMON_LANDMARKS_FILES
    ${COMMON_LANDMARKS_FILES}
    "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdDelaunay3D.h"
    "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdInternalPointsFromGrid.h"
    "${COMMON_LANDMARKS_INCLUDE_DIR}/ccipdSurfacePointsFromGrid.h"
    "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdDelaunay3D.cxx"
    "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdInternalPointsFromGrid.cxx"
    "${COMMON_LANDMARKS_SOURCE_DIR}/ccipdSurfacePointsFromGrid.cxx"
  )
ENDIF( BUILD_VTK_PROJECTS )
###########################################################################################

###########################################################################################
# Testing files
SET( TEST_CONCATENATE_LANDMARKS_FILES
  "${COMMON_LANDMARKS_TEST_DIR}/TestConcatenateLandmarks.cxx"
)
SET( TEST_INDEX_TO_POINT_FILES
  "${COMMON_LANDMARKS_TEST_DIR}/TestIndexToPoint.cxx"
)
###########################################################################################


###########################################################################################
# Create the libraries
IF( ENABLE_COMMON_LANDMARKS )
  OPTION( SHARED_COMMON_LANDMARKS
    "Build common landmarks as a shared (TRUE/checked) or static (FALSE/unchecked) library" OFF )
  IF( SHARED_COMMON_LANDMARKS )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_COMMON_LANDMARKS )
  ADD_LIBRARY( ccipdCommonLandmarks ${LIB_TYPE} ${COMMON_LANDMARKS_FILES} )
ENDIF( ENABLE_COMMON_LANDMARKS )
###########################################################################################

###########################################################################################
IF( BUILD_TESTING AND ENABLE_COMMON_LANDMARKS )
  # Create test programs
  ADD_EXECUTABLE( TestConcatenateLandmarks ${TEST_CONCATENATE_LANDMARKS_FILES} )
  
  	IF(ENABLE_DATA_CORE )
		ADD_EXECUTABLE( TestIndexToPoint ${TEST_INDEX_TO_POINT_FILES}  )
	ENDIF()

ENDIF( BUILD_TESTING AND ENABLE_COMMON_LANDMARKS )
###########################################################################################



###########################################################################################
# Link the libraries
IF( ENABLE_COMMON_LANDMARKS )

  IF( BUILD_VTK_PROJECTS )
    # The delaunay3d code depends on VTK
    SET( COMMON_LANDMARKS_LIBS
      "ccipdCommonLandmarks"
      ${COMMON_IMAGES_LIBS}
      ${VTK_LIBRARIES}
    )
  ELSE()
    SET( COMMON_LANDMARKS_LIBS
      "ccipdCommonLandmarks"
    )
  ENDIF( BUILD_VTK_PROJECTS )
  
  TARGET_LINK_LIBRARIES( ccipdCommonLandmarks ${COMMON_LANDMARKS_LIBS} )
ENDIF( ENABLE_COMMON_LANDMARKS )
###########################################################################################


IF( BUILD_TESTING AND ENABLE_COMMON_LANDMARKS )
  TARGET_LINK_LIBRARIES( 
  TestConcatenateLandmarks
    ${COMMON_CORE_LIBS}
    ${COMMON_LANDMARKS_LIBS}
    ${ITK_LIBRARIES}
     )
   
	 
  	IF(ENABLE_DATA_CORE )
		TARGET_LINK_LIBRARIES( TestIndexToPoint ${DATA_CORE_LIBS} )
	ENDIF()
     
ENDIF( BUILD_TESTING AND ENABLE_COMMON_LANDMARKS )
###########################################################################################



###########################################################################################

MESSAGE(STATUS "Building common landmarks library: "  ${ENABLE_COMMON_LANDMARKS} )


###########################################################################################


