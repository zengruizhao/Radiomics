PROJECT(MedialCurve)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
IF(COMMAND cmake_policy)
    CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)
# Eliminate a warning when building in Windows.
# This policy was introduced in CMake version 2.8.11.
# CMake version 2.8.11.2 warns when the policy is not set
# and uses OLD behavior.
if(POLICY CMP0020)
  CMAKE_POLICY(SET CMP0020 NEW)
endif()
if(POLICY CMP0022)
  CMAKE_POLICY(SET CMP0022 NEW)
endif()
###########################################################################################



###########################################################################################

IF ( MODULE_BUILD_THIRDPARTY  )
	SET ( ENABLE_MEDIALCURVE TRUE )
ELSE()
	SET ( ENABLE_MEDIALCURVE FALSE )
	RETURN() # Exit if we shouldn't build.
ENDIF ( MODULE_BUILD_THIRDPARTY )
###########################################################################################



###########################################################################################

###########################################################################################



###########################################################################################
# Say where our files are
SET( MEDIAL_CURVE_INCLUDE_DIR  "${MedialCurve_SOURCE_DIR}/include"  )
SET( MEDIAL_CURVE_SRC_DIR      "${MedialCurve_SOURCE_DIR}/src"      )
SET( MEDIAL_CURVE_TEST_DIR     "${MedialCurve_SOURCE_DIR}/test"     )
###########################################################################################



###########################################################################################
SET( LIBRARY_OUTPUT_PATH ${MedialCurve_BINARY_DIR} )

# Where will the "Use" file be located?
SET ( MEDIAL_CURVE_USE_FILE ${MedialCurve_BINARY_DIR}/UseMedialCurve.cmake)

SET ( MEDIAL_CURVE_INCLUDE_DIRS
  "${MedialCurve_BINARY_DIR}"
  "${MEDIAL_CURVE_INCLUDE_DIR}"
  )
 
# Where will our libraries be located?
SET ( MEDIAL_CURVE_LIBRARY_DIRS
  ${LIBRARY_OUTPUT_PATH}
  )
  
# Where is our binary directory?
SET ( MEDIAL_CURVE_BINARY_DIR
  ${MedialCurve_BINARY_DIR}
  )

INCLUDE_DIRECTORIES( ${MEDIAL_CURVE_INCLUDE_DIRS} )

###########################################################################################



###########################################################################################

# Copy the UseMedialCurve.cmake file to the binary tree
CONFIGURE_FILE(${MedialCurve_SOURCE_DIR}/UseMedialCurve.cmake.in
               ${MedialCurve_BINARY_DIR}/UseMedialCurve.cmake @ONLY )

###########################################################################################



###########################################################################################
# Additional cmake modules
LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_BINARY_DIR}" )
INCLUDE( UseMedialCurve )

LIST( APPEND CMAKE_MODULE_PATH "${ccipdCMakeCode_DIR}" )
INCLUDE( ccipdCompilerOptions )
#INCLUDE( ccipdUseGDCM         )
###########################################################################################


###########################################################################################
INCLUDE(${MedialCurve_SOURCE_DIR}/IJMacros.txt)


#Change PROJECT_NAME to the name of your project
PROJECT(itkMedialCurveTest)

IF(WIN32)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_WARNINGS" CACHE STRING "Flags to get rid of warnings in VS 2005 and upper" FORCE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_WARNINGS" CACHE STRING "Flags to get rid of warnings in VS 2005 and upper" FORCE)
ENDIF(WIN32)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)
###########################################################################################




###########################################################################################
SET( MEDIAL_CURVE_FILES
	"${MEDIAL_CURVE_INCLUDE_DIR}/itkAverageOutwardFluxImageFilter.h"
	"${MEDIAL_CURVE_INCLUDE_DIR}/itkAverageOutwardFluxImageFilter.hxx"
	"${MEDIAL_CURVE_INCLUDE_DIR}/itkMedialCurveImageFilter.h"
	"${MEDIAL_CURVE_INCLUDE_DIR}/itkMedialCurveImageFilter.hxx"
	"${MEDIAL_CURVE_TEST_DIR}/ImageCompare.cxx"
	"${MEDIAL_CURVE_TEST_DIR}/itkMedialCurveTest.cpp"
	)
###########################################################################################



###########################################################################################

# Copy the UseMedialCurve.cmake file to the binary tree
CONFIGURE_FILE(${MedialCurve_SOURCE_DIR}/UseMedialCurve.cmake.in
               ${MedialCurve_BINARY_DIR}/UseMedialCurve.cmake @ONLY )

# Configure MedialCurveConfig.cmake for the build tree.
CONFIGURE_FILE(${MedialCurve_SOURCE_DIR}/MedialCurveConfig.cmake.in
               ${MedialCurve_BINARY_DIR}/MedialCurveConfig.cmake @ONLY )
###########################################################################################



###########################################################################################
# add our libraries
OPTION ( SHARED_THIRD_PARTY "Build ThirdParty as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
IF ( SHARED_THIRD_PARTY )
  SET ( LIB_TYPE SHARED )
ELSE ()
  SET ( LIB_TYPE STATIC )
ENDIF ( SHARED_THIRD_PARTY )
ADD_LIBRARY( MedialCurve ${LIB_TYPE} ${MEDIAL_CURVE_FILES} )
SET_TARGET_PROPERTIES( MedialCurve PROPERTIES LINKER_LANGUAGE C)
###########################################################################################



###########################################################################################


# Which libraries do we have?
SET ( MEDIAL_CURVE_LIBS
  "MedialCurve"
  ${ITK_LIBRARIES} 
)

TARGET_LINK_LIBRARIES( MedialCurve ${MEDIAL_CURVE_LIBS} )
###########################################################################################



###########################################################################################
IF ( BUILD_TESTING )
	ADD_EXECUTABLE( ImageCompare ${MEDIAL_CURVE_TEST_DIR}/ImageCompare.cxx )
	ADD_EXECUTABLE( itkMedialCurveTest ${MEDIAL_CURVE_TEST_DIR}/itkMedialCurveTest.cpp )

	
	TARGET_LINK_LIBRARIES( ImageCompare ${MEDIAL_CURVE_LIBS} )
	TARGET_LINK_LIBRARIES( itkMedialCurveTest ${MEDIAL_CURVE_LIBS} )

ENDIF ( BUILD_TESTING )
###########################################################################################


