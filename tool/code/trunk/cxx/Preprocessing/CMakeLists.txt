


###########################################################################################
PROJECT(Preprocessing)
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
# and hence we get to link to Common in other projects!
SET( Preprocessing_BINARY_DIR ${PROJECT_BINARY_DIR} )
SET( LIBRARY_OUTPUT_PATH ${Preprocessing_BINARY_DIR} )

# Where will the "Use" file be located?
SET ( PREPROCESSING_USE_FILE ${Preprocessing_BINARY_DIR}/UsePreprocessing.cmake)

# Where will our libraries be located?
SET ( PREPROCESSING_LIBRARY_DIRS "\"${LIBRARY_OUTPUT_PATH}\"" )

CONFIGURE_FILE(
  ${Preprocessing_SOURCE_DIR}/PreprocessingConfig.cmake.in
  ${Preprocessing_BINARY_DIR}/PreprocessingConfig.cmake
  @ONLY )

# Copy the UseCommon.cmake file to the binary tree
CONFIGURE_FILE( ${Preprocessing_SOURCE_DIR}/UsePreprocessing.cmake.in
                ${Preprocessing_BINARY_DIR}/UsePreprocessing.cmake @ONLY )
###########################################################################################



###########################################################################################
OPTION( BUILD_TESTING                  "Build testing executables" ON )
OPTION( LIB_BUILD_PREPROCESSING_CORE "Build the core bias correction library." ON  )

IF( LIB_BUILD_PREPROCESSING_CORE )
  SET( ENABLE_PREPROCESSING_CORE TRUE )
ELSE()
  SET( ENABLE_PREPROCESSING_CORE FALSE )
ENDIF()
###########################################################################################



###########################################################################################
# where to search for additional cmake modules?
LIST( APPEND CMAKE_MODULE_PATH ${Preprocessing_BINARY_DIR} )
INCLUDE( UsePreprocessing )
###########################################################################################



###########################################################################################
# Additional cmake modules
LIST( APPEND CMAKE_MODULE_PATH "${ccipdCMakeCode_DIR}" )
INCLUDE( ccipdCompilerOptions  )
INCLUDE( ccipdRemoveDuplicates )
INCLUDE( ccipdUseGDCM          )
###########################################################################################



###########################################################################################
# Include each submodule

SET( PREPROCESSING_CORE_DIR "${PROJECT_SOURCE_DIR}/Core" )

# Make sure it's cleared when we start
SET( PREPROCESSING_INCLUDE_DIRS "" )

INCLUDE( "${PREPROCESSING_CORE_DIR}/CMakeLists.txt" )

###########################################################################################


###########################################################################################
# Include all the appropriate directories in this project

SET( PREPROCESSING_INCLUDE_DIRS
  ${PREPROCESSING_INCLUDE_DIRS}
  "${Preprocessing_BINARY_DIR}"
)

 
INCLUDE_DIRECTORIES( BEFORE ${PREPROCESSING_INCLUDE_DIRS} )
###########################################################################################



###########################################################################################
# Update our files which are generated on the fly

# Now that the include directories have changed, regenerate the use file
CONFIGURE_FILE( ${Preprocessing_SOURCE_DIR}/UsePreprocessing.cmake.in
                ${Preprocessing_BINARY_DIR}/UsePreprocessing.cmake @ONLY )


# Configure our cmake "Config" file
CONFIGURE_FILE(
  ${Preprocessing_SOURCE_DIR}/PreprocessingConfig.cmake.in
  ${Preprocessing_BINARY_DIR}/PreprocessingConfig.cmake
  @ONLY )

###########################################################################################

