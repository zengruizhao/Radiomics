
###########################################################################################
PROJECT(Classification)
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
# This section allows us to "include" this program's build directory in other projects
# and hence we get to link to Classification in other projects!
SET( Classification_BINARY_DIR ${PROJECT_BINARY_DIR} )
SET( LIBRARY_OUTPUT_PATH ${Classification_BINARY_DIR} )

# Where will the "Use" file be located?
SET ( CLASSIFICATION_USE_FILE ${Classification_BINARY_DIR}/UseClassification.cmake)

# Where will our libraries be located?
SET ( CLASSIFICATION_LIBRARY_DIRS "\"${LIBRARY_OUTPUT_PATH}\"" )

CONFIGURE_FILE(
  ${Classification_SOURCE_DIR}/ClassificationConfig.cmake.in
  ${Classification_BINARY_DIR}/ClassificationConfig.cmake @ONLY )

# Copy the UseClassification.cmake file to the binary tree
CONFIGURE_FILE( ${Classification_SOURCE_DIR}/UseClassification.cmake.in
                ${Classification_BINARY_DIR}/UseClassification.cmake @ONLY )
###########################################################################################



###########################################################################################
OPTION( BUILD_TESTING                  "Build testing executables" ON )
OPTION( BUILD_EIGEN_PROJECTS "Uncheck this if you don't have eigen installed."  ON )

OPTION( LIB_BUILD_CLASSIFICATION_CORE        "Build the core classification library."       ON )
OPTION( LIB_BUILD_CLASSIFICATION_VALIDATION  "Build the classification Validation library." ON )


IF( LIB_BUILD_CLASSIFICATION_CORE )
  SET( ENABLE_CLASSIFICATION_CORE TRUE )
ELSE()
  SET( ENABLE_CLASSIFICATION_CORE FALSE )
ENDIF()

IF( LIB_BUILD_CLASSIFICATION_VALIDATION )
  SET( ENABLE_CLASSIFICATION_VALIDATION TRUE )
ELSE()
  SET( ENABLE_CLASSIFICATION_VALIDATION FALSE )
ENDIF()
###########################################################################################



###########################################################################################
# where to search for additional cmake modules?
LIST( APPEND CMAKE_MODULE_PATH ${Classification_BINARY_DIR} )
INCLUDE( UseClassification )
###########################################################################################



###########################################################################################


# This file will say whether or not we actually found the optional package
CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/ClassificationOptions.h.in" # this is the template to create a real .h file
  "${PROJECT_BINARY_DIR}/ClassificationOptions.h"    # this should be included in your project
) 
###########################################################################################



###########################################################################################
# Additional cmake modules
LIST( APPEND CMAKE_MODULE_PATH "${ccipdCMakeCode_DIR}" )
INCLUDE( ccipdCompilerOptions  )
INCLUDE( ccipdRemoveDuplicates )

INCLUDE( ccipdUseSimpleINI )

###########################################################################################

IF( BUILD_EIGEN_PROJECTS )
  INCLUDE( ccipdUseEigen )
ENDIF( BUILD_EIGEN_PROJECTS )



###########################################################################################
# Include each submodule
SET( CLASSIFICATION_CORE_DIR       "${PROJECT_SOURCE_DIR}/Core"       )
SET( CLASSIFICATION_VALIDATION_DIR "${PROJECT_SOURCE_DIR}/Validation" )


# Make sure it's cleared when we start
SET( CLASSIFICATION_INCLUDE_DIRS "" )

INCLUDE( "${CLASSIFICATION_CORE_DIR}/CMakeLists.txt"       )
INCLUDE( "${CLASSIFICATION_VALIDATION_DIR}/CMakeLists.txt" )
###########################################################################################


###########################################################################################
# Include all the appropriate directories in this project

SET( CLASSIFICATION_INCLUDE_DIRS
  ${CLASSIFICATION_INCLUDE_DIRS}
  "${Classification_BINARY_DIR}"
)

 
INCLUDE_DIRECTORIES( BEFORE ${CLASSIFICATION_INCLUDE_DIRS} )
###########################################################################################



###########################################################################################
# Update our files which are generated on the fly

# Now that the include directories have changed, regenerate the use file
CONFIGURE_FILE( ${Classification_SOURCE_DIR}/UseClassification.cmake.in
                ${Classification_BINARY_DIR}/UseClassification.cmake @ONLY )


# Configure our cmake "Config" file
CONFIGURE_FILE(
  ${Classification_SOURCE_DIR}/ClassificationConfig.cmake.in
  ${Classification_BINARY_DIR}/ClassificationConfig.cmake @ONLY )
INCLUDE( UseClassification )
###########################################################################################

