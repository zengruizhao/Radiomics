


###########################################################################################
PROJECT( Common )
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
SET( Common_BINARY_DIR ${PROJECT_BINARY_DIR} )
SET( LIBRARY_OUTPUT_PATH ${Common_BINARY_DIR} )

# Configure CommonConfig.cmake for the build tree.
SET( COMMON_LIBRARY_DIRS "\"${LIBRARY_OUTPUT_PATH}\"" )
SET( COMMON_USE_FILE     ${Common_BINARY_DIR}/UseCommon.cmake)

CONFIGURE_FILE( ${Common_SOURCE_DIR}/CommonConfig.cmake.in
                ${Common_BINARY_DIR}/CommonConfig.cmake @ONLY )

# Copy the UseCommon.cmake file to the binary tree
CONFIGURE_FILE( ${Common_SOURCE_DIR}/UseCommon.cmake.in
                ${Common_BINARY_DIR}/UseCommon.cmake @ONLY )
###########################################################################################

				
				
###########################################################################################
OPTION( BUILD_BOOST_PROJECTS "Uncheck this if you don't have boost installed." ON )
OPTION( BUILD_EIGEN_PROJECTS "Uncheck this if you don't have eigen installed."  ON )
OPTION( BUILD_MEDIALCURVE_PROJECTS "Uncheck this if you don't have medialcurve compiled." ON )
OPTION( BUILD_VTK_PROJECTS "Build projects which depend on VTK" ON )
OPTION( BUILD_TESTING "Build testing executables" ON )

OPTION( LIB_BUILD_COMMON_CORE "Build the ccipd common core library." ON )
OPTION( LIB_BUILD_COMMON_LANDMARKS "Build the ccipd common landmarks library." ON )
OPTION( LIB_BUILD_COMMON_IMAGES "Build the ccipd common images library." ON )
OPTION( LIB_BUILD_COMMON_HASH "Build the ccipd common hash library."      ON )
OPTION( LIB_BUILD_COMMON_MATH "Build the CCIPD Math library" ON )
OPTION( LIB_BUILD_COMMON_VTK_CONVERSION "Build the VTK Conversion library" ON )

IF( LIB_BUILD_COMMON_CORE )
  SET( ENABLE_COMMON_CORE TRUE )
ELSE()
  SET( ENABLE_COMMON_CORE FALSE )
ENDIF()

IF( LIB_BUILD_COMMON_HASH )
  SET( ENABLE_COMMON_HASH TRUE )
ELSE()
  SET( ENABLE_COMMON_HASH FALSE )
ENDIF()

IF( LIB_BUILD_COMMON_LANDMARKS )
  SET( ENABLE_COMMON_LANDMARKS TRUE )
ELSE()
  SET( ENABLE_COMMON_LANDMARKS FALSE )
ENDIF()

IF( LIB_BUILD_COMMON_IMAGES )
  SET( ENABLE_COMMON_IMAGES TRUE )
ELSE()
  SET( ENABLE_COMMON_IMAGES FALSE )
ENDIF()

IF( LIB_BUILD_COMMON_VTK_CONVERSION )
  SET( ENABLE_COMMON_VTK_CONVERSION TRUE )
ELSE()
  SET( ENABLE_COMMON_VTK_CONVERSION FALSE )
ENDIF()

IF( LIB_BUILD_COMMON_MATH )
  SET( ENABLE_COMMON_MATH TRUE )
ELSE()
  SET( ENABLE_COMMON_MATH FALSE )
ENDIF()

###########################################################################################



###########################################################################################
# where to search for additional cmake modules?
LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_BINARY_DIR} )
INCLUDE( UseCommon )

# include some additional modules
LIST( APPEND CMAKE_MODULE_PATH "${ccipdCMakeCode_DIR}" )
INCLUDE( ccipdCompilerOptions  )
INCLUDE( ccipdRemoveDuplicates )
INCLUDE( ccipdUseGDCM          )

IF ( BUILD_BOOST_PROJECTS)
  INCLUDE( ccipdUseBoost )
ENDIF( BUILD_BOOST_PROJECTS)

IF( BUILD_EIGEN_PROJECTS )
  INCLUDE( ccipdUseEigen )
ENDIF( BUILD_EIGEN_PROJECTS )
###########################################################################################



###########################################################################################
# Include each submodule

SET( COMMON_CORE_DIR           "${PROJECT_SOURCE_DIR}/Core"          )
SET( COMMON_HASH_DIR           "${PROJECT_SOURCE_DIR}/Hash"          )
SET( COMMON_IMAGES_DIR         "${PROJECT_SOURCE_DIR}/Images"        )
SET( COMMON_LANDMARKS_DIR      "${PROJECT_SOURCE_DIR}/Landmarks"     )
SET( COMMON_MATH_DIR           "${PROJECT_SOURCE_DIR}/Math"          )
SET( COMMON_VTK_CONVERSION_DIR "${PROJECT_SOURCE_DIR}/VTKConversion" )

# Make sure it's cleared when we start
SET( COMMON_INCLUDE_DIRS "" )

INCLUDE( "${COMMON_CORE_DIR}/CMakeLists.txt"           )
INCLUDE( "${COMMON_HASH_DIR}/CMakeLists.txt"           )
INCLUDE( "${COMMON_MATH_DIR}/CMakeLists.txt"           )
INCLUDE( "${COMMON_IMAGES_DIR}/CMakeLists.txt"         )
INCLUDE( "${COMMON_LANDMARKS_DIR}/CMakeLists.txt"      )
INCLUDE( "${COMMON_VTK_CONVERSION_DIR}/CMakeLists.txt" )

###########################################################################################



###########################################################################################
# Include all the appropriate directories in this project

SET( COMMON_INCLUDE_DIRS
  ${COMMON_INCLUDE_DIRS}
  "${Common_BINARY_DIR}"
)

INCLUDE_DIRECTORIES( BEFORE ${COMMON_INCLUDE_DIRS} )
###########################################################################################



###########################################################################################
# Update our files which are generated on the fly

# Now that the include directories have changed, regenerate the use file
CONFIGURE_FILE( ${Common_SOURCE_DIR}/UseCommon.cmake.in
                ${Common_BINARY_DIR}/UseCommon.cmake @ONLY )

CONFIGURE_FILE( ${Common_SOURCE_DIR}/CommonConfig.cmake.in
                ${Common_BINARY_DIR}/CommonConfig.cmake @ONLY )
###########################################################################################



