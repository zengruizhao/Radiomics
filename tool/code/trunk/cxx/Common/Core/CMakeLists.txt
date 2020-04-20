


###########################################################################################
# File locations
SET( COMMON_CORE_INCLUDE_DIR   "${COMMON_CORE_DIR}/include"   )
SET( COMMON_CORE_TEMPLATES_DIR "${COMMON_CORE_DIR}/templates" )
SET( COMMON_CORE_SOURCE_DIR    "${COMMON_CORE_DIR}/source"    )
SET( COMMON_CORE_TEST_DIR      "${COMMON_CORE_DIR}/test"      )

SET( COMMON_INCLUDE_DIRS
  ${COMMON_INCLUDE_DIRS}
  "${COMMON_CORE_INCLUDE_DIR}"
  "${COMMON_CORE_TEMPLATES_DIR}"
)
###########################################################################################



###########################################################################################
# Copy the ccipdCommonCoreOptions file to the binary tree
CONFIGURE_FILE( ${COMMON_CORE_INCLUDE_DIR}/ccipdCommonCoreOptions.h.in
                ${Common_BINARY_DIR}/ccipdCommonCoreOptions.h @ONLY  )
###########################################################################################



###########################################################################################
# Set up the files
SET( COMMON_CORE_FILES
  "${COMMON_CORE_INCLUDE_DIR}/ccipdCloneable.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdCommon.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdCommonGroups.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdConcatenateParameters.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdCore.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdDisableWarningsMacro.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdEnableWarningsMacro.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdEnableSharedFromThis.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdFileTypes.h" 
  "${COMMON_CORE_INCLUDE_DIR}/ccipdITKForwardDeclarations.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdLandmarkTypes.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdMacros.h"	
  "${COMMON_CORE_INCLUDE_DIR}/ccipdMeshTypes.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdMRITypes.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdRGBSliceTypes.h"  
  "${COMMON_CORE_INCLUDE_DIR}/ccipdSliceTypes.h"  
  "${COMMON_CORE_INCLUDE_DIR}/ccipdSplitParameters.h"  
  "${COMMON_CORE_INCLUDE_DIR}/ccipdTRUSTypes.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdUniqueValuesTypes.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdUnorderedMapSerialization.h" 
  "${COMMON_CORE_INCLUDE_DIR}/ccipdVectorIndexTypes.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdVolumeContainerToVolumeFeatures.h"
  "${COMMON_CORE_INCLUDE_DIR}/ccipdVolumeTypes.h"
  
  "${COMMON_CORE_TEMPLATES_DIR}/ccipdCloneable.hxx"
  "${COMMON_CORE_TEMPLATES_DIR}/ccipdConcatenateParameters.hxx"
  "${COMMON_CORE_TEMPLATES_DIR}/ccipdEnableSharedFromThis.hxx"
  
  "${COMMON_CORE_SOURCE_DIR}/ccipdConcatenateParameters.cxx"
  "${COMMON_CORE_SOURCE_DIR}/ccipdCore.cxx"
  "${COMMON_CORE_SOURCE_DIR}/ccipdSplitParameters.cxx"
)

IF( BUILD_VTK_PROJECTS )
 # Common files which depend on VTK
 SET( COMMON_CORE_FILES
   ${COMMON_CORE_FILES}
   "${COMMON_CORE_INCLUDE_DIR}/ccipdVTKForwardDeclarations.h"
   )   
ENDIF( BUILD_VTK_PROJECTS )

IF( BUILD_BOOST_PROJECTS )
 # Common files which depend on boost
  SET( COMMON_CORE_FILES
    ${COMMON_CORE_FILES}
    "${COMMON_CORE_INCLUDE_DIR}/ccipdCloneableFactory.h"
    "${COMMON_CORE_INCLUDE_DIR}/ccipdFactory.h"
    "${COMMON_CORE_TEMPLATES_DIR}/ccipdCloneableFactory.hxx"
    "${COMMON_CORE_TEMPLATES_DIR}/ccipdCopyable.hxx"
    "${COMMON_CORE_TEMPLATES_DIR}/ccipdFactory.hxx"
    "${COMMON_CORE_TEMPLATES_DIR}/ccipdSharedPointerSerialization.hxx"
  )
  
  IF( BUILD_EIGEN_PROJECTS )
   # Common files which depend on Eigen and Boost
    SET( COMMON_CORE_FILES
      ${COMMON_CORE_FILES}
      "${COMMON_CORE_INCLUDE_DIR}/ccipdMatrixSerialization.h"
      "${COMMON_CORE_TEMPLATES_DIR}/ccipdMatrixSerialization.hxx"
      "${COMMON_CORE_SOURCE_DIR}/ccipdMatrixSerialization.cxx"
    )
  ENDIF( BUILD_EIGEN_PROJECTS ) 
ENDIF( BUILD_BOOST_PROJECTS )
###########################################################################################



###########################################################################################
# Create the libraries
# Note that all libraries below should appear in
# "GenerateCommonConfig.cmake"
IF( ENABLE_COMMON_CORE )
  OPTION ( SHARED_COMMON_CORE
    "Build common core as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF ( SHARED_COMMON_CORE )
    SET ( LIB_TYPE SHARED )
  ELSE ()
    SET ( LIB_TYPE STATIC )
  ENDIF ( SHARED_COMMON_CORE )
  ADD_LIBRARY( ccipdCommonCore ${LIB_TYPE} ${COMMON_CORE_FILES} )
ENDIF( ENABLE_COMMON_CORE )
###########################################################################################



###########################################################################################
# Link the libraries
IF( ENABLE_COMMON_CORE )
  IF( BUILD_BOOST_PROJECTS )
    # The matrix serialization depends on boost
    SET( COMMON_CORE_LIBS
      "ccipdCommonCore"
      ${Boost_LIBRARIES}
    )  
  ELSE()
    # Shared ptr serialization is only a header
    SET( COMMON_CORE_LIBS
      "ccipdCommonCore"
    )  
  ENDIF()
  
  TARGET_LINK_LIBRARIES( ccipdCommonCore ${COMMON_CORE_LIBS} )  
ENDIF( ENABLE_COMMON_CORE )
###########################################################################################



###########################################################################################
IF(ENABLE_COMMON_CORE)
MESSAGE(STATUS "Building common core library: "  ${ENABLE_COMMON_CORE} )
ELSE()
MESSAGE( "Building common core library: "  ${ENABLE_COMMON_CORE} )
ENDIF()

###########################################################################################


