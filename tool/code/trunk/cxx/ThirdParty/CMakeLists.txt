


###########################################################################################
PROJECT( ThirdParty )
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

# Additional cmake modules
LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeCode" )

INCLUDE( ccipdCompilerOptions  )
INCLUDE( ccipdRemoveDuplicates )


###########################################################################################
# this file will say whether or not we actually found the optional package
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
###########################################################################################

###########################################################################################
# Say where our files are
SET( THIRD_PARTY_MEDIAL_AXIS_DIR  "${PROJECT_SOURCE_DIR}"  )
###########################################################################################

###########################################################################################
# Say where our files are
ADD_SUBDIRECTORY( ${THIRD_PARTY_MEDIAL_AXIS_DIR}/MedialCurveImageFilter )
###########################################################################################