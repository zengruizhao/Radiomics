


##########################################################################################
# Dependencies

# We have to set this again in case the
# segmentation config file switched it back to false.
IF( LIB_BUILD_SEGMENTATION_CORE )
  SET( ENABLE_SEGMENTATION_CORE TRUE )
ELSE()
  SET( ENABLE_SEGMENTATION_CORE FALSE )
ENDIF()

IF( ENABLE_SEGMENTATION_CORE AND NOT BUILD_EIGEN_PROJECTS )
  MESSAGE( "Segmentation core library requires the " )
  MESSAGE( " eigen library. Disabling "              )
  MESSAGE( "segmentation core library."              )
  SET( ENABLE_SEGMENTATION_CORE FALSE )
ENDIF()

IF ( ENABLE_SEGMENTATION_CORE AND NOT ENABLE_REGISTRATION_IMAGES )
  MESSAGE( "Segmentation core library requires the " )
  MESSAGE( "registration images library. Disabling " )
  MESSAGE( "segmentation core library."              )
  SET( ENABLE_SEGMENTATION_CORE FALSE )
ENDIF()

IF ( ENABLE_SEGMENTATION_CORE AND NOT ENABLE_REGISTRATION_CORE )
  MESSAGE( "Segmentation core library requires the "  )
  MESSAGE( "registration core library. Disabling "    )
  MESSAGE( "segmentation core library."               )
  SET( ENABLE_SEGMENTATION_CORE FALSE )
ENDIF()  
###########################################################################################



###########################################################################################
# Say where our files are
SET( SEGMENTATION_CORE_INCLUDE_DIR   "${SEGMENTATION_CORE_DIR}/include"   )
SET( SEGMENTATION_CORE_TEMPLATES_DIR "${SEGMENTATION_CORE_DIR}/templates" )
SET( SEGMENTATION_CORE_SOURCE_DIR    "${SEGMENTATION_CORE_DIR}/source"    )
SET( SEGMENTATION_CORE_TEST_DIR      "${SEGMENTATION_CORE_DIR}/test"      )
SET( SEGMENTATION_CORE_PROJECTS_DIR  "${SEGMENTATION_CORE_DIR}/projects"  )

SET( SEGMENTATION_INCLUDE_DIRS
  ${SEGMENTATION_INCLUDE_DIRS}
  "${SEGMENTATION_CORE_INCLUDE_DIR}"
  "${SEGMENTATION_CORE_TEMPLATES_DIR}" )
###########################################################################################



###########################################################################################
# List our files
SET( SEGMENTATION_CORE_FILES
  "${SEGMENTATION_CORE_INCLUDE_DIR}/itkMaskToLevelsetFilter.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdClosestPoint.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdClosestPoints.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdExtractSubMask.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdLevelsets.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdLevelsetToSurface.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdMaskToLandmarks.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdMaskToLevelset.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdNormalDirectionCalculator.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdSegmentationGroups.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdSegmentationTypes.h"
  "${SEGMENTATION_CORE_INCLUDE_DIR}/ccipdSegmenter.h"
  "${SEGMENTATION_CORE_TEMPLATES_DIR}/itkMaskToLevelsetFilter.hxx"
  "${SEGMENTATION_CORE_TEMPLATES_DIR}/ccipdClosestPoint.hxx"
  "${SEGMENTATION_CORE_TEMPLATES_DIR}/ccipdClosestPoints.hxx"
  "${SEGMENTATION_CORE_TEMPLATES_DIR}/ccipdLevelsetToSurface.hxx"
  "${SEGMENTATION_CORE_TEMPLATES_DIR}/ccipdMaskToLevelset.hxx"
  "${SEGMENTATION_CORE_TEMPLATES_DIR}/ccipdNormalDirectionCalculator.hxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdClosestPoint.cxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdClosestPoints.cxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdExtractSubMask.cxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdLevelsets.cxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdLevelsetToSurface.cxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdMaskToLandmarks.cxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdMaskToLevelset.cxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdNormalDirectionCalculator.cxx"
  "${SEGMENTATION_CORE_SOURCE_DIR}/ccipdSegmenter.cxx"
)
SET( TEST_LEVELSET_NORMALIZATION_FILES
  "${SEGMENTATION_CORE_TEST_DIR}/TestLevelsetNormalization.cxx"
)
SET( TEST_COMPUTE_LEVELSET_FILES
  "${SEGMENTATION_CORE_TEST_DIR}/TestComputeLevelset.cxx"
)
SET( TEST_ENLARGE_MASKS_FILES
  "${SEGMENTATION_CORE_TEST_DIR}/TestEnlargeMasks.cxx"
)
SET( TEST_MIDGLAND_ACCURACY_FILES
  "${SEGMENTATION_CORE_TEST_DIR}/TestMidglandAccuracy.cxx"
)
SET( TEST_MEDIAL_AXIS_FILES
  "${SEGMENTATION_CORE_TEST_DIR}/TestExtractMedialAxis.cxx"
)
SET( TEST_CLOSEST_POINTS_FILES
  "${SEGMENTATION_CORE_TEST_DIR}/TestClosestPoints.cxx"
)
SET( TEST_LARGEST_CONTIGUOUS_REGION_FILES
  "${SEGMENTATION_CORE_TEST_DIR}/TestLargestContiguousRegion.cxx"
)
SET( TEST_MASK_SURFACE_DISPLACEMENT_FILES
  "${SEGMENTATION_CORE_TEST_DIR}/TestMaskSurfaceDisplacement.cxx"
)
SET( ADD_LEVELSET_IN_MULTI_LABEL_FILES
  "${SEGMENTATION_CORE_PROJECTS_DIR}/ccipdAddLevelsetsInMultiLabelFile.cxx"
)
###########################################################################################


  
###########################################################################################
# Create our libraries
IF( ENABLE_SEGMENTATION_CORE )
  OPTION ( SHARED_SEGMENTATION_CORE
    "Build Segmentation as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF ( SHARED_SEGMENTATION_CORE )
    SET ( LIB_TYPE SHARED )
  ELSE ()
    SET ( LIB_TYPE STATIC )
  ENDIF ( SHARED_SEGMENTATION_CORE )
  ADD_LIBRARY( ccipdSegmentationCore ${LIB_TYPE} ${SEGMENTATION_CORE_FILES} )
ENDIF( ENABLE_SEGMENTATION_CORE )
###########################################################################################



###########################################################################################
IF( BUILD_TESTING AND ENABLE_SEGMENTATION_CORE )
  # Create our test programs

  ADD_EXECUTABLE( TestLevelsetNormalization   ${TEST_LEVELSET_NORMALIZATION_FILES}    )
  ADD_EXECUTABLE( TestComputeLevelset         ${TEST_COMPUTE_LEVELSET_FILES}          )
  ADD_EXECUTABLE( TestEnlargeMasks            ${TEST_ENLARGE_MASKS_FILES}             )
  ADD_EXECUTABLE( TestClosestPoints           ${TEST_CLOSEST_POINTS_FILES}            )
  ADD_EXECUTABLE( TestMaskSurfaceDisplacement ${TEST_MASK_SURFACE_DISPLACEMENT_FILES} )
  
  IF ( ENABLE_DATA_EVALUATION ) 
    ADD_EXECUTABLE( TestMidglandAccuracy        ${TEST_MIDGLAND_ACCURACY_FILES}      )
  ENDIF ( ENABLE_DATA_EVALUATION )
  
  IF ( ENABLE_MEDIALCURVE AND ENABLE_MEDIALCURVE_PROJECT AND ENABLE_DATA_CORE )
    ADD_EXECUTABLE( TestMedialAxis     ${TEST_MEDIAL_AXIS_FILES} )
  ENDIF ( ENABLE_MEDIALCURVE AND ENABLE_MEDIALCURVE_PROJECT AND ENABLE_DATA_CORE )

  ADD_EXECUTABLE( AddLevelsetsInMultiLabelfile ${ADD_LEVELSET_IN_MULTI_LABEL_FILES} )  
  
ENDIF( BUILD_TESTING AND ENABLE_SEGMENTATION_CORE )
###########################################################################################




###########################################################################################
# Link our libraries
IF( ENABLE_SEGMENTATION_CORE )

  SET( SEGMENTATION_CORE_LIBS
    "ccipdSegmentationCore"
    ${REGISTRATION_IMAGES_LIBS}
    ${REGISTRATION_CORE_LIBS}
    ${COMMON_IMAGES_LIBS}
    ${FILTERING_CORE_LIBS}
  )
  CCIPD_REMOVE_DUPLICATE_LIBS( SEGMENTATION_CORE_LIBS ${SEGMENTATION_CORE_LIBS} )
  TARGET_LINK_LIBRARIES( ccipdSegmentationCore ${SEGMENTATION_CORE_LIBS} )
  
ENDIF( ENABLE_SEGMENTATION_CORE )

IF ( BUILD_TESTING AND ENABLE_SEGMENTATION_CORE )

  TARGET_LINK_LIBRARIES( TestLevelsetNormalization   ${SEGMENTATION_CORE_LIBS} )
  TARGET_LINK_LIBRARIES( TestComputeLevelset         
    ${SEGMENTATION_CORE_LIBS} 
    ${DATA_CORE_LIBS} )
  TARGET_LINK_LIBRARIES( TestEnlargeMasks            ${SEGMENTATION_CORE_LIBS} )
  TARGET_LINK_LIBRARIES( TestMaskSurfaceDisplacement ${SEGMENTATION_CORE_LIBS} ${DATA_CORE_LIBS} )
  
  IF( ENABLE_GUI_CORE )
    TARGET_LINK_LIBRARIES( TestClosestPoints ${GUI_CORE_LIBS} ${SEGMENTATION_CORE_LIBS} ${DATA_CORE_LIBS} )
  ELSE()
    TARGET_LINK_LIBRARIES( TestClosestPoints ${SEGMENTATION_CORE_LIBS} ${DATA_CORE_LIBS} )
  ENDIF( ENABLE_GUI_CORE )
    
  IF ( ENABLE_DATA_EVALUATION )
  TARGET_LINK_LIBRARIES( TestMidglandAccuracy      ${SEGMENTATION_CORE_LIBS} ${DATA_EVALUATION_LIBS} )
  ENDIF ( ENABLE_DATA_EVALUATION )
  
  IF ( ENABLE_MEDIALCURVE AND ENABLE_MEDIALCURVE_PROJECT AND ENABLE_DATA_CORE  )
    TARGET_LINK_LIBRARIES( TestMedialAxis
      ${SEGMENTATION_CORE_LIBS} ${DATA_CORE_LIBS} )
  ENDIF ( ENABLE_MEDIALCURVE AND ENABLE_MEDIALCURVE_PROJECT AND ENABLE_DATA_CORE )
   
  TARGET_LINK_LIBRARIES( AddLevelsetsInMultiLabelfile
    ${DATA_CORE_LIBS}
    ${SEGMENTATION_CORE_LIBS} )  

ENDIF ( BUILD_TESTING AND ENABLE_SEGMENTATION_CORE )
###########################################################################################



###########################################################################################
IF(ENABLE_SEGMENTATION_CORE)
MESSAGE(STATUS "Building segmentation core library: " ${ENABLE_SEGMENTATION_CORE} )
ELSE()
MESSAGE( "Building segmentation core library: " ${ENABLE_SEGMENTATION_CORE} )
ENDIF()
###########################################################################################


