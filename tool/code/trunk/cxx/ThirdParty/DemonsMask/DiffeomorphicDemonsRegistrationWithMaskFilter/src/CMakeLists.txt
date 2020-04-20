cmake_minimum_required(VERSION 2.6)
cmake_policy(VERSION 2.6)

FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

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
