PROJECT( Filtering )
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
# and hence we get to link to Filtering in other projects!
SET( Filtering_BINARY_DIR ${PROJECT_BINARY_DIR} )
SET( LIBRARY_OUTPUT_PATH ${Filtering_BINARY_DIR} )

# Where will the "Use" file be located?
SET( FILTERING_USE_FILE "${Filtering_BINARY_DIR}/UseFiltering.cmake" )
SET( FILTERING_LIBRARY_DIRS "\"${LIBRARY_OUTPUT_PATH}\"" )

CONFIGURE_FILE( ${Filtering_SOURCE_DIR}/FilteringConfig.cmake.in
                ${Filtering_BINARY_DIR}/FilteringConfig.cmake @ONLY )
  
# Copy the UseFiltering.cmake file to the binary tree
CONFIGURE_FILE(${Filtering_SOURCE_DIR}/UseFiltering.cmake.in
               ${Filtering_BINARY_DIR}/UseFiltering.cmake @ONLY IMMEDIATE)
               
###########################################################################################



###########################################################################################
OPTION( BUILD_BOOST_PROJECTS "Uncheck this if you don't have boost installed."          ON )
OPTION( BUILD_TESTING        "Build testing executables"                                ON )

OPTION( LIB_BUILD_FILTERING_CORE "Build the core filtering library."                    ON )
OPTION( LIB_BUILD_FILTERING_FEATURE_COLLECTION "Build the feature collection library."  ON )

OPTION( LIB_BUILD_FILTERING_DCE  "Build the DCE  filtering library."                    ON )

IF( LIB_BUILD_FILTERING_CORE )
  SET( ENABLE_FILTERING_CORE TRUE )
ELSE()
  SET( ENABLE_FILTERING_CORE FALSE )
ENDIF()

IF( LIB_BUILD_FILTERING_FEATURE_COLLECTION )
  SET( ENABLE_FILTERING_FEATURE_COLLECTION TRUE )
ELSE()
  SET( ENABLE_FILTERING_FEATURE_COLLECTION FALSE )
ENDIF()



IF( LIB_BUILD_FILTERING_DCE )
  SET( ENABLE_FILTERING_DCE TRUE )
ELSE()
  SET( ENABLE_FILTERING_DCE FALSE )
ENDIF()

###########################################################################################



###########################################################################################
# Additional cmake modules

LIST( APPEND CMAKE_MODULE_PATH "${Filtering_BINARY_DIR}" )
INCLUDE( UseFiltering )

LIST( APPEND CMAKE_MODULE_PATH "${ccipdCMakeCode_DIR}" )
INCLUDE( ccipdCompilerOptions  )
INCLUDE( ccipdRemoveDuplicates )
INCLUDE( ccipdUseGDCM         )

IF( BUILD_BOOST_PROJECTS )
  INCLUDE( ccipdUseBoost )
ENDIF( BUILD_BOOST_PROJECTS )

###########################################################################################



###########################################################################################
# Include the submodule cmake files

SET( FILTERING_CORE_DIR               "${PROJECT_SOURCE_DIR}/Core"              )
SET( FILTERING_DCE_DIR               "${PROJECT_SOURCE_DIR}/DCE"                )
SET( FILTERING_FEATURE_COLLECTION_DIR "${PROJECT_SOURCE_DIR}/FeatureCollection" )

# Make sure it's cleared when we start
SET( FILTERING_INCLUDE_DIRS "" )

INCLUDE( "${FILTERING_CORE_DIR}/CMakeLists.txt"               )
INCLUDE( "${FILTERING_DCE_DIR}/CMakeLists.txt"               )
INCLUDE( "${FILTERING_FEATURE_COLLECTION_DIR}/CMakeLists.txt" )

###########################################################################################



###########################################################################################
# Include all the appropriate directories in this project

SET( FILTERING_INCLUDE_DIRS
  ${FILTERING_INCLUDE_DIRS}
  "${Filtering_BINARY_DIR}"
)

INCLUDE_DIRECTORIES( BEFORE ${FILTERING_INCLUDE_DIRS} )
###########################################################################################



###########################################################################################

# Now that the include directories have changed, regenerate the use file
CONFIGURE_FILE ( ${Filtering_SOURCE_DIR}/UseFiltering.cmake.in
                 ${Filtering_BINARY_DIR}/UseFiltering.cmake @ONLY )

CONFIGURE_FILE( ${Filtering_SOURCE_DIR}/FilteringConfig.cmake.in
                ${Filtering_BINARY_DIR}/FilteringConfig.cmake @ONLY )

###########################################################################################


