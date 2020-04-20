


#######################testing####################################################################
PROJECT( Segmentation )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
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
# Additional cmake modules
SET( Segmentation_BINARY_DIR ${PROJECT_BINARY_DIR} )
SET( LIBRARY_OUTPUT_PATH ${Segmentation_BINARY_DIR} )

# Where will the "Use" file be located?
SET( SEGMENTATION_USE_FILE "${Segmentation_BINARY_DIR}/UseSegmentation.cmake" )
SET( SEGMENTATION_LIBRARY_DIRS "\"${LIBRARY_OUTPUT_PATH}\"" )

CONFIGURE_FILE( ${Segmentation_SOURCE_DIR}/SegmentationConfig.cmake.in
                ${Segmentation_BINARY_DIR}/SegmentationConfig.cmake @ONLY )
  
# Copy the UseSegmentation.cmake file to the binary tree
CONFIGURE_FILE(${Segmentation_SOURCE_DIR}/UseSegmentation.cmake.in
               ${Segmentation_BINARY_DIR}/UseSegmentation.cmake @ONLY IMMEDIATE )

###########################################################################################



###########################################################################################
OPTION( BUILD_BOOST_PROJECTS "Uncheck this if you don't have boost installed." OFF )
OPTION( BUILD_EIGEN_PROJECTS "Uncheck this if you don't have eigen installed." OFF )
OPTION( BUILD_TESTING        "Build testing executables"                       ON  )

OPTION( LIB_BUILD_SEGMENTATION_CORE "Build the core segmentation library."     ON )
OPTION( LIB_BUILD_SEGMENTATION_MFA "Build the MFA segmentation lbiraries"      ON  )

IF( LIB_BUILD_SEGMENTATION_CORE )
  SET( ENABLE_SEGMENTATION_CORE TRUE )
ELSE()
  SET( ENABLE_SEGMENTATION_CORE FALSE )
ENDIF()

IF( LIB_BUILD_SEGMENTATION_MFA )
  SET( ENABLE_SEGMENTATION_MFA TRUE )
ELSE()
  SET( ENABLE_SEGMENTATION_MFA FALSE )
ENDIF()

###########################################################################################



###########################################################################################
LIST( APPEND CMAKE_MODULE_PATH "${Segmentation_BINARY_DIR}" )
INCLUDE( UseSegmentation )

LIST( APPEND CMAKE_MODULE_PATH "${ccipdCMakeCode_DIR}" )
INCLUDE( ccipdCompilerOptions  )
INCLUDE( ccipdRemoveDuplicates )
INCLUDE( ccipdUseGDCM          )
###########################################################################################



###########################################################################################
# The MFA library requires the Eigen external library.
IF ( BUILD_EIGEN_PROJECTS )
  INCLUDE( ccipdUseEigen )
ENDIF ( BUILD_EIGEN_PROJECTS )
###########################################################################################



###########################################################################################
# we will optionally use the ItkVtkGlue package for displaying progress
INCLUDE( "CMakeGlue.txt" )
###########################################################################################



###########################################################################################
# this file will say whether or not we actually found the optional packages
CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/ccipdSegmentationOptions.h.in" # this is the template to create a real .h file
  "${PROJECT_BINARY_DIR}/ccipdSegmentationOptions.h"    # this should be included in your project
) 
###########################################################################################



###########################################################################################
# Include the submodules

SET( SEGMENTATION_CORE_DIR "${PROJECT_SOURCE_DIR}/Core" )
SET( SEGMENTATION_MFA_DIR "${PROJECT_SOURCE_DIR}/MFA"   )

# Make sure it's cleared when we start
SET( SEGMENTATION_INCLUDE_DIRS "" )

INCLUDE( "${SEGMENTATION_CORE_DIR}/CMakeLists.txt" )
INCLUDE( "${SEGMENTATION_MFA_DIR}/CMakeLists.txt"  )

###########################################################################################



###########################################################################################
# Include all the appropriate directories in this project

SET( SEGMENTATION_INCLUDE_DIRS
  ${SEGMENTATION_INCLUDE_DIRS}
  "${SEGMENTATION_INCLUDE_DIRS}"
  "${Segmentation_BINARY_DIR}"
)

INCLUDE_DIRECTORIES( BEFORE ${SEGMENTATION_INCLUDE_DIRS} )
###########################################################################################


###########################################################################################

# Now that the include directories have changed, regenerate the use file
CONFIGURE_FILE ( ${Segmentation_SOURCE_DIR}/UseSegmentation.cmake.in
                 ${Segmentation_BINARY_DIR}/UseSegmentation.cmake @ONLY )

CONFIGURE_FILE( ${Segmentation_SOURCE_DIR}/SegmentationConfig.cmake.in
                ${Segmentation_BINARY_DIR}/SegmentationConfig.cmake @ONLY )

###########################################################################################


