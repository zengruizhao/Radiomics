
IF(ENABLE_COMMON_IMAGES)

ENDIF()

IF(NOT ENABLE_COMMON_IMAGES)
	MESSAGE("Disabling Common Images library.")
	RETURN()
ENDIF()

IF(NOT ENABLE_DATA_PATIENTS)
  MESSAGE( "The extract labels executables "       )
  MESSAGE( "require the data patients library. "   )
  MESSAGE( "Disabling extract labels executables." )
  SET( ENABLE_EXTRACT_LABELS FALSE )
ELSE()
  SET( ENABLE_EXTRACT_LABELS TRUE )
ENDIF()
###########################################################################################



###########################################################################################
# Say where our files are
SET( COMMON_IMAGES_INCLUDE_DIR   "${COMMON_IMAGES_DIR}/include"   )
SET( COMMON_IMAGES_TEMPLATES_DIR "${COMMON_IMAGES_DIR}/templates" )
SET( COMMON_IMAGES_SOURCE_DIR    "${COMMON_IMAGES_DIR}/source"    )
SET( COMMON_IMAGES_TEST_DIR      "${COMMON_IMAGES_DIR}/test"      )
SET( COMMON_IMAGES_PROJECTS_DIR  "${COMMON_IMAGES_DIR}/projects"  )
SET( COMMON_IMAGES_LUTS_DIR      "${COMMON_IMAGES_DIR}/LookUpTables"  )


SET( COMMON_INCLUDE_DIRS
  ${COMMON_INCLUDE_DIRS}
  "${COMMON_IMAGES_INCLUDE_DIR}"
  "${FILTERING_COMMON_INCLUDE_DIR}"
  "${COMMON_IMAGES_TEMPLATES_DIR}"
)
###########################################################################################




###########################################################################################
# Set up the files
SET( COMMON_IMAGES_FILES
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdAddMissingSlicesToMask.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdBoundingBoxCoordinates.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdBoundingBoxCreation.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdConstVolume.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdCreateBinaryMask.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdCropImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdDuplicateImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdDuplicateImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdExtractFeatureImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdExtractUniqueValues.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdGetChannelByIndex.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdGetDiagonalDistance.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdGetLargestContiguousRegion.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdIntensityStandardization.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdStandardizeImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdLoadImage.h" 
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdWriteImage.h"   
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdMaskImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdMaskPixelValue.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdProcessLabels.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdSampleImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdSliceVolumeConversion.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdSplitRegion.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdRgbToScalarImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdScalarToRgbImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdSetChannelByIndex.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdLookupTablesTypes.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdFlipImage.h"
  "${COMMON_IMAGES_INCLUDE_DIR}/ccipdInsertImage.h"

  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdBoundingBoxCoordinates.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdBoundingBoxCreation.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdCreateBinaryMask.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdCropImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdDilateImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdDuplicateImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdExtractFeatureImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdExtractUniqueValues.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdGetChannelByIndex.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdGetDiagonalDistance.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdGetLargestContiguousRegion.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdLoadImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdWriteImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdStandardizeImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdMaskImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdMaskPixelValue.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdSampleImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdSliceVolumeConversion.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdSplitRegion.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdRgbToScalarImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdScalarToRgbImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdSetChannelByIndex.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdIntensityStandardization.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdFlipImage.hxx"
  "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdInsertImage.hxx"
  
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdAddMissingSlicesToMask.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdBoundingBoxCoordinates.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdBoundingBoxCreation.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdConstVolume.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdCreateBinaryMask.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdCropImage.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdDilateImage.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdDuplicateImage.cxx"  
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdExtractFeatureImage.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdExtractUniqueValues.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdGetDiagonalDistance.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdGetLargestContiguousRegion.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdIntensityStandardization.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdStandardizeImage.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdMaskImage.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdMaskPixelValue.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdProcessLabels.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdSliceVolumeConversion.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdSplitRegion.cxx"
  "${COMMON_IMAGES_SOURCE_DIR}/ccipdVolumeContainerToVolumeFeatures.cxx"

  
  "${COMMON_IMAGES_LUTS_DIR}/ccipd_Jet.lut"
  
  
  )

IF( BUILD_BOOST_PROJECTS )
  SET( COMMON_IMAGES_FILES
    ${COMMON_IMAGES_FILES}  
    "${COMMON_IMAGES_INCLUDE_DIR}/ccipdRegionCube.h"
    "${COMMON_IMAGES_INCLUDE_DIR}/ccipdRegionShape.h"
    "${COMMON_IMAGES_INCLUDE_DIR}/ccipdRegionSphere.h"
    "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdRegionCube.hxx"
    "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdRegionShape.hxx"
    "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdRegionSphere.hxx"
    "${COMMON_IMAGES_SOURCE_DIR}/ccipdRegionCube.cxx"
    "${COMMON_IMAGES_SOURCE_DIR}/ccipdRegionShape.cxx"
    "${COMMON_IMAGES_SOURCE_DIR}/ccipdRegionSphere.cxx"
  )
ENDIF()

IF( ENABLE_COMMON_MATH )
  SET( COMMON_IMAGES_FILES
    ${COMMON_IMAGES_FILES}
    "${COMMON_IMAGES_INCLUDE_DIR}/ccipdBoundingBoxRandomization.h"
    "${COMMON_IMAGES_TEMPLATES_DIR}/ccipdBoundingBoxRandomization.hxx"
    "${COMMON_IMAGES_SOURCE_DIR}/ccipdBoundingBoxRandomization.cxx"
  )
ENDIF( ENABLE_COMMON_MATH )

IF( ENABLE_MEDIALCURVE )
 # Common files which depend on MedialCurve
 SET( COMMON_IMAGES_FILES
    ${COMMON_IMAGES_FILES}
    "${COMMON_IMAGES_INCLUDE_DIR}/ccipdExtractMedialAxis.h"
    "${COMMON_IMAGES_SOURCE_DIR}/ccipdExtractMedialAxis.cxx"
  )
ENDIF( ENABLE_MEDIALCURVE )

# Testing files
SET( TEST_INTENSITY_STANDARDIZATION_FILES
  "${COMMON_IMAGES_TEST_DIR}/TestIntensityStandardization.cxx"
)
SET( TEST_STANDARDIZE_IMAGE_FILES
  "${COMMON_IMAGES_TEST_DIR}/TestStandardizeImage.cxx"
)
SET( TEST_CROPPING_FILES
  "${COMMON_IMAGES_TEST_DIR}/TestCropping.cxx"
)
SET( TEST_EXTRACT_LABELS_FILES
  "${COMMON_IMAGES_TEST_DIR}/TestExtractLabels.cxx"
)
SET( TEST_MERGE_LABELS_FILES
  "${COMMON_IMAGES_TEST_DIR}/TestMergeLabels.cxx"
)
SET( TEST_READ_UNKNOWN_IMAGE_TYPE_FILES
  "${COMMON_IMAGES_TEST_DIR}/TestReadUnknownImageType.cxx"
)
SET( TEST_LARGEST_CONTIGUOUS_REGION_FILES
  "${COMMON_IMAGES_TEST_DIR}/TestLargestContiguousRegion.cxx"
)
SET( EXTRACT_LABELS_FILES
  "${COMMON_IMAGES_PROJECTS_DIR}/ccipdExtractLabels.cxx"
)
SET( EXTRACT_MANY_LABELS_FILES
  "${COMMON_IMAGES_PROJECTS_DIR}/ccipdExtractManyLabels.cxx"
)
###########################################################################################



###########################################################################################
# Create the libraries
IF( ENABLE_COMMON_IMAGES )
  OPTION( SHARED_COMMON_IMAGES
    "Build common images as a shared (TRUE/checked) or static (FALSE/unchecked) library" OFF )
  IF( SHARED_COMMON_IMAGES )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_COMMON_IMAGES )
  ADD_LIBRARY( ccipdCommonImages ${LIB_TYPE} ${COMMON_IMAGES_FILES} )
ENDIF( ENABLE_COMMON_IMAGES )
###########################################################################################



###########################################################################################
IF( BUILD_TESTING AND ENABLE_COMMON_IMAGES )
  # Create test programs
  ADD_EXECUTABLE( TestReadUnknownImageType ${TEST_READ_UNKNOWN_IMAGE_TYPE_FILES} )
  
  IF( ENABLE_DATA_CORE )
    ADD_EXECUTABLE( TestCropping ${TEST_CROPPING_FILES} )
    ADD_EXECUTABLE( TestLargestContiguousRegion ${TEST_LARGEST_CONTIGUOUS_REGION_FILES} )
    
    IF( ENABLE_EXTRACT_LABELS )
      ADD_EXECUTABLE( TestExtractLabels ${TEST_EXTRACT_LABELS_FILES} )
      ADD_EXECUTABLE( TestMergeLabels ${TEST_MERGE_LABELS_FILES} )
      ADD_EXECUTABLE( ExtractLabels ${EXTRACT_LABELS_FILES} )  
      ADD_EXECUTABLE( ExtractManyLabels ${EXTRACT_MANY_LABELS_FILES} )  
    ENDIF( ENABLE_EXTRACT_LABELS )
 
    IF( BUILD_BOOST_PROJECTS )
      ADD_EXECUTABLE( TestIntensityStandardization ${TEST_INTENSITY_STANDARDIZATION_FILES} )
    ENDIF( BUILD_BOOST_PROJECTS )
  
  ENDIF( ENABLE_DATA_CORE )
  
  
  IF(ENABLE_FILTERING_CORE)
	  ADD_EXECUTABLE( TestStandardizeImage ${TEST_STANDARDIZE_IMAGE_FILES} )
	ENDIF()
  
    
ENDIF( BUILD_TESTING AND ENABLE_COMMON_IMAGES )
###########################################################################################



###########################################################################################
# Link the libraries
IF( ENABLE_COMMON_IMAGES )
  SET( COMMON_IMAGES_LIBS
    "ccipdCommonImages"
    ${ITK_LIBRARIES}
  )

  IF( ENABLE_COMMON_MATH )
    SET( COMMON_IMAGES_LIBS
      ${COMMON_IMAGES_LIBS}
      ${COMMON_MATH_LIBS}
    )
  ENDIF( ENABLE_COMMON_MATH )
  
  CCIPD_REMOVE_DUPLICATE_LIBS( COMMON_IMAGES_LIBS ${COMMON_IMAGES_LIBS} )

  TARGET_LINK_LIBRARIES( ccipdCommonImages ${COMMON_IMAGES_LIBS} )  
ENDIF( ENABLE_COMMON_IMAGES )

IF( BUILD_TESTING AND ENABLE_COMMON_IMAGES )
  TARGET_LINK_LIBRARIES( TestReadUnknownImageType
    ${ITK_LIBRARIES} )
  
  IF( ENABLE_DATA_CORE )  
    TARGET_LINK_LIBRARIES( TestCropping
      ${DATA_CORE_LIBS}
      ${COMMON_IMAGES_LIBS} )
      
    TARGET_LINK_LIBRARIES( TestLargestContiguousRegion
      ${COMMON_IMAGES_LIBS}
      ${DATA_CORE_LIBS} )

    IF( ENABLE_EXTRACT_LABELS )
      TARGET_LINK_LIBRARIES( TestExtractLabels
        ${DATA_CORE_LIBS}
        ${COMMON_IMAGES_LIBS} )
      TARGET_LINK_LIBRARIES( TestMergeLabels
        ${DATA_CORE_LIBS}
        ${COMMON_IMAGES_LIBS} )
      TARGET_LINK_LIBRARIES( ExtractLabels
        ${DATA_CORE_LIBS}
        ${COMMON_IMAGES_LIBS}
        ${FILTERING_CORE_LIBS}
        ${DATA_PATIENTS_LIBS} )  
      TARGET_LINK_LIBRARIES( ExtractManyLabels
        ${DATA_CORE_LIBS}
        ${COMMON_IMAGES_LIBS}
        ${FILTERING_CORE_LIBS} )  
    ENDIF()

    IF( BUILD_BOOST_PROJECTS )
      TARGET_LINK_LIBRARIES( TestIntensityStandardization
        ${DATA_CORE_LIBS}
        ${COMMON_IMAGES_LIBS}
        ${Boost_LIBRARIES} )
    ENDIF( BUILD_BOOST_PROJECTS )

  ENDIF( ENABLE_DATA_CORE )
  
    
	IF(ENABLE_FILTERING_CORE)
		TARGET_LINK_LIBRARIES( TestStandardizeImage ${COMMON_IMAGES_LIBS} )
	ENDIF()
ENDIF( BUILD_TESTING AND ENABLE_COMMON_IMAGES )
###########################################################################################



###########################################################################################
IF(ENABLE_COMMON_IMAGES)
MESSAGE(STATUS "Building common images library: " ${ENABLE_COMMON_IMAGES} )
ELSE()
MESSAGE( "Building common images library: " ${ENABLE_COMMON_IMAGES} )
ENDIF()
###########################################################################################



