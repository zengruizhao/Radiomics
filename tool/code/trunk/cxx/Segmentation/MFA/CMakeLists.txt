


###########################################################################################
# Dependencies
  
IF( ENABLE_SEGMENTATION_MFA AND NOT ENABLE_COMMON_CORE )
  MESSAGE( "Segmentation MFA library depends on " )
  MESSAGE( "common core library. "                )
  MESSAGE( "Disabling segmentation MFA library. " )
  SET( ENABLE_SEGMENTATION_MFA FALSE )
ENDIF()

IF( ENABLE_SEGMENTATION_MFA AND NOT ENABLE_REGISTRATION_ALGORITHMS )
  MESSAGE( "Segmentation MFA library depends on " )
  MESSAGE( "registration algorithms library. "    )
  MESSAGE( "Disabling segmentation MFA library. " )
  SET( ENABLE_SEGMENTATION_MFA FALSE )
ENDIF()

IF( ENABLE_SEGMENTATION_MFA AND NOT ENABLE_SEGMENTATION_CORE )
  MESSAGE( "Segmentation MFA library depends on " )
  MESSAGE( "segmentation core library. "          )
  MESSAGE( "Disabling segmentation MFA library. " )
  SET( ENABLE_SEGMENTATION_MFA FALSE )
ENDIF()

IF( ENABLE_SEGMENTATION_MFA AND NOT ENABLE_PREPROCESSING_CORE )
  MESSAGE( "Segmentation MFA library depends on " )
  MESSAGE( "preprocessing core library. "       )
  MESSAGE( "Disabling segmentation MFA library. " )
  SET( ENABLE_SEGMENTATION_MFA FALSE )
ENDIF()
    
IF( ENABLE_SEGMENTATION_MFA AND NOT ENABLE_DIMENSIONALITY_REDUCTION_SIM )
  MESSAGE( "Segmentation MFA library depends on "    )
  MESSAGE( "dimensionality reduction SIM library. "  )
  MESSAGE( "Disabling segmentation MFA library. "    )
  SET( ENABLE_SEGMENTATION_MFA FALSE )
ENDIF()
###########################################################################################



###########################################################################################
# Say where our files are
SET( SEGMENTATION_MFA_INCLUDE_DIR   "${SEGMENTATION_MFA_DIR}/include"   )
SET( SEGMENTATION_MFA_TEMPLATES_DIR "${SEGMENTATION_MFA_DIR}/templates" )
SET( SEGMENTATION_MFA_SOURCE_DIR    "${SEGMENTATION_MFA_DIR}/source"    )
SET( SEGMENTATION_MFA_TEST_DIR      "${SEGMENTATION_MFA_DIR}/test"      )
SET( SEGMENTATION_MFA_PROJECTS_DIR  "${SEGMENTATION_MFA_DIR}/projects"  )

SET( SEGMENTATION_INCLUDE_DIRS
   ${SEGMENTATION_INCLUDE_DIRS}
  "${SEGMENTATION_MFA_INCLUDE_DIR}"
  "${SEGMENTATION_MFA_TEMPLATES_DIR}"
)
###########################################################################################



###########################################################################################
# List our files
SET( SEGMENTATION_MFA_FILES
  "${SEGMENTATION_MFA_INCLUDE_DIR}/itkMFAReconstructionFilter.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/itkMFAReconstructionMetric.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/itkMFARegularizedKnownValues.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFABoundingBoxRandomizer.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFABoundingBoxTraining.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFADrivingFeaturePreparer.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFADrivingLevelsetObjects.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFADrivingLevelsetExtractor.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFADrivingTextureEnums.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFADrivingTextureExtractor.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAExtractorHasFeature.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAFeaturesFromLevelsets.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAForwardDeclarations.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAGetReconstruction.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAInvalidStudies.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFALevelsetExtractor.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFALevelsetExtractorClone.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFALevelsetsFromFeatures.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAMaskAligner.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAModel.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAPrepareFilter.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAPruneDrivingFeatures.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFAReconstructionBase.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFASegmentAndSave.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFASegmenter.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFASegmenterCorrection.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFATextureExtractor.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFATextureExtractorClone.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFATransformedReconstruction.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFATypes.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdRegisterMFAAlgorithm.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdRegisterMFABase.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdRegisterMFAImages.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdRegisterMFAMetric.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdRegisterMFARegularizedMetric.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdRegisterMFAStudyOutput.h"
  "${SEGMENTATION_MFA_TEMPLATES_DIR}/itkMFAReconstructionFilter.hxx"
  "${SEGMENTATION_MFA_TEMPLATES_DIR}/itkMFAReconstructionMetric.hxx"
  "${SEGMENTATION_MFA_TEMPLATES_DIR}/itkMFARegularizedKnownValues.hxx"
  "${SEGMENTATION_MFA_TEMPLATES_DIR}/ccipdMFAReconstructionBase.hxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/itkMFAReconstructionMetric.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/itkMFAReconstructionFilter.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/itkMFARegularizedKnownValues.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFABoundingBoxRandomizer.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFABoundingBoxTraining.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFADrivingFeaturePreparer.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFADrivingLevelsetObjects.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFADrivingLevelsetExtractor.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFADrivingTextureEnums.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFADrivingTextureExtractor.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAExtractorHasFeature.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAFeaturesFromLevelsets.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAGetReconstruction.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAInvalidStudies.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFALevelsetExtractor.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFALevelsetExtractorClone.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFALevelsetsFromFeatures.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAMaskAligner.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAModel.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAModelFactory.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAModelSerializable.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAPrepareFilter.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAPruneDrivingFeatures.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFAReconstructionBase.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFASegmentAndSave.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFASegmenter.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFASegmenterCorrection.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFASegmenterCorrectionFactory.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFASegmenterFactory.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFASegmenterSerializable.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFATextureExtractor.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFATextureExtractorClone.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFATransformedReconstruction.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdRegisterMFAAlgorithm.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdRegisterMFAImages.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdRegisterMFAMetric.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdRegisterMFARegularizedMetric.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdRegisterMFAStudyOutput.cxx"
  )
SET( MFA_CROSS_VALIDATION_FILES
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFACrossValidationFileNames.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFACrossValidationSegmenting.h"
  "${SEGMENTATION_MFA_INCLUDE_DIR}/ccipdMFACrossValidationTraining.h"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFACrossValidationFileNames.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFACrossValidationSegmenting.cxx"
  "${SEGMENTATION_MFA_SOURCE_DIR}/ccipdMFACrossValidationTraining.cxx"
  "${SEGMENTATION_MFA_PROJECTS_DIR}/ccipdMFACrossValidation.cxx"
  )

SET( TEST_MFA_FEATURES_FILES
  "${SEGMENTATION_MFA_TEST_DIR}/TestMFAFeatures.cxx"
)
SET( TEST_MFA_SAVING_LOADING_FILES
  "${SEGMENTATION_MFA_TEST_DIR}/TestMFASavingLoading.cxx"
)

SET( MFA_TRAIN_FILES
  "${SEGMENTATION_MFA_PROJECTS_DIR}/ccipdMFATrain.cxx"
)
SET( MFA_SEGMENT_FILES
  "${SEGMENTATION_MFA_PROJECTS_DIR}/ccipdMFASegment.cxx"
)
###########################################################################################


  
###########################################################################################
# Create our libraries
IF ( ENABLE_SEGMENTATION_MFA )
  OPTION ( SHARED_SEGMENTATION_MFA
    "Build MFA as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF ( SHARED_SEGMENTATION_MFA )
    SET ( LIB_TYPE SHARED )
  ELSE ()
    SET ( LIB_TYPE STATIC )
  ENDIF ( SHARED_SEGMENTATION_MFA )
  ADD_LIBRARY( ccipdSegmentationMFA ${LIB_TYPE} ${SEGMENTATION_MFA_FILES} )
ENDIF ( ENABLE_SEGMENTATION_MFA )
###########################################################################################



###########################################################################################
IF ( BUILD_TESTING AND ENABLE_SEGMENTATION_MFA )

  # Create our test programs
  IF( ENABLE_DATA_CORE AND  ENABLE_FILTERING_FEATURE_COLLECTION )
    ADD_EXECUTABLE( TestMFAFeatures           ${TEST_MFA_FEATURES_FILES}         )
  ENDIF( ENABLE_DATA_CORE AND  ENABLE_FILTERING_FEATURE_COLLECTION )
  
  ADD_EXECUTABLE( TestMFASavingLoading      ${TEST_MFA_SAVING_LOADING_FILES}     )

ENDIF ( BUILD_TESTING AND ENABLE_SEGMENTATION_MFA )
###########################################################################################



###########################################################################################
# Create our projects
IF ( ENABLE_SEGMENTATION_MFA )

  ADD_EXECUTABLE( MFATrain           ${MFA_TRAIN_FILES}   )
  
  IF( ENABLE_DATA_EVALUATION )
    ADD_EXECUTABLE( MFASegment         ${MFA_SEGMENT_FILES} )

    IF( ENABLE_DATA_VALIDATION )	
      ADD_EXECUTABLE( MFACrossValidation ${MFA_CROSS_VALIDATION_FILES} )
    ENDIF( ENABLE_DATA_VALIDATION )
  ENDIF( ENABLE_DATA_EVALUATION )
  
ENDIF( ENABLE_SEGMENTATION_MFA )
###########################################################################################



###########################################################################################
# Link our libraries
IF( ENABLE_SEGMENTATION_MFA )

  SET( SEGMENTATION_MFA_LIBS
    "ccipdSegmentationMFA"
    ${COMMON_CORE_LIBS}
    ${REGISTRATION_ALGORITHMS_LIBS}
    ${SEGMENTATION_CORE_LIBS}
    ${PREPROCESSING_CORE_LIBS}
    ${DIMENSIONALITY_REDUCTION_CORE_LIBS}
    ${DIMENSIONALITY_REDUCTION_SIM_LIBS}
  )
  CCIPD_REMOVE_DUPLICATE_LIBS( SEGMENTATION_MFA_LIBS ${SEGMENTATION_MFA_LIBS} )
  TARGET_LINK_LIBRARIES( ccipdSegmentationMFA ${SEGMENTATION_MFA_LIBS} )
  
  IF( BUILD_TESTING )
  
    IF( ENABLE_DATA_CORE AND  ENABLE_FILTERING_FEATURE_COLLECTION )
      TARGET_LINK_LIBRARIES( TestMFAFeatures
        ${FILTERING_FEATURE_COLLECTION_LIBS}
        ${PREPROCESSING_CORE_LIBS}
        ${DATA_CORE_LIBS} )
    ENDIF( ENABLE_DATA_CORE AND  ENABLE_FILTERING_FEATURE_COLLECTION )
	
    TARGET_LINK_LIBRARIES( TestMFASavingLoading ${ITK_LIBRARIES} )
  
  ENDIF( BUILD_TESTING )

  TARGET_LINK_LIBRARIES( MFATrain
    ${SEGMENTATION_MFA_LIBS} )
  
  IF( ENABLE_DATA_EVALUATION )	  
    TARGET_LINK_LIBRARIES( MFASegment
      ${SEGMENTATION_MFA_LIBS}
      ${DATA_EVALUATION_LIBS}    )
  
    IF( ENABLE_DATA_VALIDATION)	
      TARGET_LINK_LIBRARIES( MFACrossValidation
        ${SEGMENTATION_MFA_LIBS}
        ${DATA_VALIDATION_LIBS} 
        ${DATA_EVALUATION_LIBS} )
    ENDIF( ENABLE_DATA_VALIDATION )
  
  ENDIF( ENABLE_DATA_EVALUATION )	  
  
ENDIF( ENABLE_SEGMENTATION_MFA )
###########################################################################################



###########################################################################################
IF(ENABLE_SEGMENTATION_MFA)
MESSAGE(STATUS "Building MFA library: " ${ENABLE_SEGMENTATION_MFA}  )
ELSE()
MESSAGE( "Building MFA library: " ${ENABLE_SEGMENTATION_MFA}  )
ENDIF()
###########################################################################################

