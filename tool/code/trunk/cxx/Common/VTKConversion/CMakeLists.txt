


###########################################################################################
# Will we be building the VTK conversion library?
IF( LIB_BUILD_COMMON_VTK_CONVERSION AND NOT BUILD_VTK_PROJECTS )
   MESSAGE( "VTK conversion library requires the VTK "   )
   MESSAGE( "library. Disabling VTK conversion library." )
   SET ( ENABLE_COMMON_VTK_CONVERSION FALSE )
ENDIF( LIB_BUILD_COMMON_VTK_CONVERSION AND NOT BUILD_VTK_PROJECTS )


IF( LIB_BUILD_COMMON_VTK_CONVERSION AND NOT BUILD_BOOST_PROJECTS )
   MESSAGE( "VTK conversion library requires the boost "   )
   MESSAGE( "library. Disabling VTK conversion library." )
   SET ( ENABLE_COMMON_VTK_CONVERSION FALSE )
ENDIF( LIB_BUILD_COMMON_VTK_CONVERSION AND NOT BUILD_BOOST_PROJECTS )
###########################################################################################



###########################################################################################
# Say where our files are
SET( COMMON_VTK_CONVERSION_INCLUDE_DIR   "${COMMON_VTK_CONVERSION_DIR}/include"   )
SET( COMMON_VTK_CONVERSION_TEMPLATES_DIR "${COMMON_VTK_CONVERSION_DIR}/templates" )
SET( COMMON_VTK_CONVERSION_SOURCE_DIR    "${COMMON_VTK_CONVERSION_DIR}/source"    )
SET( COMMON_VTK_CONVERSION_TEST_DIR      "${COMMON_VTK_CONVERSION_DIR}/test"      )

SET( COMMON_INCLUDE_DIRS
  ${COMMON_INCLUDE_DIRS}
  "${COMMON_VTK_CONVERSION_INCLUDE_DIR}"
  "${COMMON_VTK_CONVERSION_TEMPLATES_DIR}"
)
###########################################################################################



###########################################################################################
# File locations
SET( COMMON_VTK_CONVERSION_FILES
  "${COMMON_VTK_CONVERSION_INCLUDE_DIR}/ccipdITKImageToVTKImage.h"
  "${COMMON_VTK_CONVERSION_TEMPLATES_DIR}/ccipdITKImageToVTKImage.hxx"
  "${COMMON_VTK_CONVERSION_INCLUDE_DIR}/ccipdITKLandmarksToVTKPoints.h"
  "${COMMON_VTK_CONVERSION_INCLUDE_DIR}/ccipdITKLandmarksToVTKPolyData.h"
  "${COMMON_VTK_CONVERSION_INCLUDE_DIR}/ccipdITKMeshToVTKUnstructuredGrid.h"
  "${COMMON_VTK_CONVERSION_INCLUDE_DIR}/ccipdVTKPointsToITKLandmarks.h"
  "${COMMON_VTK_CONVERSION_INCLUDE_DIR}/ccipdVTKUnstructuredGridToITKMesh.h"
  "${COMMON_VTK_CONVERSION_SOURCE_DIR}/ccipdITKImageToVTKImage.cxx"
  "${COMMON_VTK_CONVERSION_SOURCE_DIR}/ccipdITKLandmarksToVTKPoints.cxx"
  "${COMMON_VTK_CONVERSION_SOURCE_DIR}/ccipdITKLandmarksToVTKPolyData.cxx"
  "${COMMON_VTK_CONVERSION_SOURCE_DIR}/ccipdITKMeshToVTKUnstructuredGrid.cxx"
  "${COMMON_VTK_CONVERSION_SOURCE_DIR}/ccipdVTKPointsToITKLandmarks.cxx"
  "${COMMON_VTK_CONVERSION_SOURCE_DIR}/ccipdVTKUnstructuredGridToITKMesh.cxx"
)    
###########################################################################################



###########################################################################################
# Create the libraries
IF( ENABLE_COMMON_VTK_CONVERSION )
  OPTION( SHARED_COMMON_VTK_CONVERSION
    "Build VTKConversion as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF( SHARED_COMMON_VTK_CONVERSION )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_COMMON_VTK_CONVERSION )
  ADD_LIBRARY( ccipdCommonVTKConversion ${LIB_TYPE} ${COMMON_VTK_CONVERSION_FILES} )
ENDIF( ENABLE_COMMON_VTK_CONVERSION )
###########################################################################################



###########################################################################################
# Link the libraries
IF( ENABLE_COMMON_VTK_CONVERSION )
  SET( COMMON_VTK_CONVERSION_LIBS
    "ccipdCommonVTKConversion"
    ${VTK_LIBRARIES}
    ${ITK_LIBRARIES}
  )
  
  TARGET_LINK_LIBRARIES( ccipdCommonVTKConversion ${COMMON_VTK_CONVERSION_LIBS} )
ENDIF( ENABLE_COMMON_VTK_CONVERSION )
###########################################################################################



###########################################################################################
IF(ENABLE_COMMON_VTK_CONVERSION)
MESSAGE(STATUS "Building common VTK conversion library: " ${ENABLE_COMMON_VTK_CONVERSION} )
ELSE()
MESSAGE( "Building common VTK conversion library: " ${ENABLE_COMMON_VTK_CONVERSION} )
ENDIF()
###########################################################################################


