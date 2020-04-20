


###########################################################################################
PROJECT( DimensionalityReduction )
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
# Configure DimensionalityReductionConfig.cmake for the build tree.

# This section allows us to "include" this program's build directory in other projects
# and hence we get to link to DimensionalityReduction in other projects!
SET( DimensionalityReduction_BINARY_DIR ${PROJECT_BINARY_DIR} )
SET( LIBRARY_OUTPUT_PATH ${DimensionalityReduction_BINARY_DIR} )

# Where will the "Use" file be located?
SET( DIMENSIONALITY_REDUCTION_USE_FILE
  "${DimensionalityReduction_BINARY_DIR}/UseDimensionalityReduction.cmake" )
SET( DIMENSIONALITY_REDUCTION_LIBRARY_DIRS "\"${LIBRARY_OUTPUT_PATH}\"" )
  
CONFIGURE_FILE( ${DimensionalityReduction_SOURCE_DIR}/DimensionalityReductionConfig.cmake.in
                ${DimensionalityReduction_BINARY_DIR}/DimensionalityReductionConfig.cmake @ONLY )

CONFIGURE_FILE(${DimensionalityReduction_SOURCE_DIR}/UseDimensionalityReduction.cmake.in
               ${DimensionalityReduction_BINARY_DIR}/UseDimensionalityReduction.cmake @ONLY )
###########################################################################################




###########################################################################################
OPTION( BUILD_EIGEN_PROJECTS "Uncheck this if you don't have eigen installed." OFF )
OPTION( BUILD_BOOST_PROJECTS "Uncheck this if you don't have boost installed." OFF )

OPTION( LIB_BUILD_DIMENSIONALITY_REDUCTION_CORE
  "Build the core dimensionality reduction library." ON )
OPTION( LIB_BUILD_DIMENSIONALITY_REDUCTION_SIM
  "Build the SIM dimensionality reduction library." ON )
  
IF ( LIB_BUILD_DIMENSIONALITY_REDUCTION_CORE )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_CORE TRUE )
ELSE()
  SET( ENABLE_DIMENSIONALITY_REDUCTION_CORE FALSE )
ENDIF()  

IF ( LIB_BUILD_DIMENSIONALITY_REDUCTION_SIM )
  SET( ENABLE_DIMENSIONALITY_REDUCTION_SIM TRUE )
ELSE()
  SET( ENABLE_DIMENSIONALITY_REDUCTION_SIM FALSE )
ENDIF()  
###########################################################################################



###########################################################################################
LIST( APPEND CMAKE_MODULE_PATH "${DimensionalityReduction_BINARY_DIR}" )
INCLUDE( UseDimensionalityReduction )
###########################################################################################



###########################################################################################
# Additional cmake modules

LIST( APPEND CMAKE_MODULE_PATH "${ccipdCMakeCode_DIR}" )
INCLUDE( ccipdCompilerOptions )
INCLUDE( ccipdUseGDCM         )

# The dimensionality reduction library requires the Eigen external library.
IF( BUILD_EIGEN_PROJECTS )
  INCLUDE( ccipdUseEigen )
ENDIF( BUILD_EIGEN_PROJECTS )

# The dimensionality reduction library requires the Boost external library.
IF( BUILD_BOOST_PROJECTS )
  INCLUDE( ccipdUseBoost )
ENDIF( BUILD_BOOST_PROJECTS )
###########################################################################################



###########################################################################################
# this file will say whether or not we actually found the optional packages
CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/ccipdDimensionalityReductionOptions.h.in" # this is the template to create a real .h file
  "${PROJECT_BINARY_DIR}/ccipdDimensionalityReductionOptions.h"    # this should be included in your project
) 
###########################################################################################



###########################################################################################

SET( DIMENSIONALITY_REDUCTION_CORE_DIR "${PROJECT_SOURCE_DIR}/Core" )

SET( DIMENSIONALITY_REDUCTION_SIM_DIR "${PROJECT_SOURCE_DIR}/SIM" )

# Make sure it's cleared when we start
SET( DIMENSIONALITY_REDUCTION_INCLUDE_DIRS "" )

INCLUDE( "${DIMENSIONALITY_REDUCTION_CORE_DIR}/CMakeLists.txt" )
INCLUDE( "${DIMENSIONALITY_REDUCTION_SIM_DIR}/CMakeLists.txt"  )
###########################################################################################



###########################################################################################
# Include all the appropriate directories in this project

SET( DIMENSIONALITY_REDUCTION_INCLUDE_DIRS
  ${DIMENSIONALITY_REDUCTION_INCLUDE_DIRS}
  "${DIMENSIONALITY_REDUCTION_INCLUDE_DIRS}"
  "${DimensionalityReduction_BINARY_DIR}"
)

INCLUDE_DIRECTORIES( BEFORE ${DIMENSIONALITY_REDUCTION_INCLUDE_DIRS} )
###########################################################################################



###########################################################################################
# Update our files which are generated on the fly

# Now that the include directories have changed, regenerate the use file
CONFIGURE_FILE ( ${DimensionalityReduction_SOURCE_DIR}/UseDimensionalityReduction.cmake.in
                 ${DimensionalityReduction_BINARY_DIR}/UseDimensionalityReduction.cmake @ONLY )

CONFIGURE_FILE( ${DimensionalityReduction_SOURCE_DIR}/DimensionalityReductionConfig.cmake.in
                ${DimensionalityReduction_BINARY_DIR}/DimensionalityReductionConfig.cmake @ONLY )
###########################################################################################



