
IF(ENABLE_DATA_EVALUATION) 
	IF(NOT ENABLE_DATA_CORE ) 
		MESSAGE( "Data Evaluation requires building Data Core.")
		SET ( ENABLE_DATA_EVALUATION FALSE )
	ENDIF()
	
#	IF(NOT ENABLE_SEGMENTATION_CORE )
#		MESSAGE( "Data Evaluation requires building Segmentation Core.")
#		SET ( ENABLE_DATA_EVALUATION FALSE )
#	ENDIF()
ENDIF()

IF(NOT ENABLE_DATA_EVALUATION)
	MESSAGE( "Disabling Data Evaluation library." )
	RETURN()
ENDIF()



###########################################################################################
# Say where our files are
SET( DATA_EVALUATION_INCLUDE_DIR   "${DATA_EVALUATION_DIR}/include"   )
SET( DATA_EVALUATION_SOURCE_DIR    "${DATA_EVALUATION_DIR}/source"    )
SET( DATA_EVALUATION_TEMPLATE_DIR  "${DATA_EVALUATION_DIR}/templates" )
SET( DATA_EVALUATION_TEST_DIR      "${DATA_EVALUATION_DIR}/test"      )

SET( DATA_INCLUDE_DIRS   ${DATA_INCLUDE_DIRS}
						"${DATA_EVALUATION_INCLUDE_DIR}"
						"${DATA_EVALUATION_TEMPLATE_DIR}" 
						"${DATA_CORE_INCLUDE_DIR}" 
	)
###########################################################################################



###########################################################################################
SET( DATA_EVALUATION_FILES
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateCenterOfMassDistance.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateDICE.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateHausdorffDistance.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateLandmarkRMSE.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateMAD.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateMedialAxesDistance.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateMedialAxesRMSE.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateSimpleMedialAxesDistance.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateSimpleMedialAxesRMSE.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateVolume.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateWarpFieldDifference.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdCalculateWarpFieldSSD.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdMaskToDistanceImage.h"  
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdPrepareMaskForEvaluation.h" 
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdSurfaceToDistanceImage.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdQuantitativeSegmentationResults.h" 
  "${DATA_EVALUATION_INCLUDE_DIR}/itkMeanAbsoluteDistanceImageFilter.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/itkMaskStandardDeviationImageFilter.h"
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdConfusionMatrix.h"  
  "${DATA_EVALUATION_INCLUDE_DIR}/ccipdStatisticalPerformanceMeasures.h"
  
  "${DATA_EVALUATION_TEMPLATE_DIR}/ccipdMaskToDistanceImage.hxx"
  "${DATA_EVALUATION_TEMPLATE_DIR}/ccipdCalculateVolume.hxx"
  "${DATA_EVALUATION_TEMPLATE_DIR}/ccipdSurfaceToDistanceImage.hxx"
  "${DATA_EVALUATION_TEMPLATE_DIR}/itkMeanAbsoluteDistanceImageFilter.hxx"
  "${DATA_EVALUATION_TEMPLATE_DIR}/itkMaskStandardDeviationImageFilter.hxx"
  "${DATA_EVALUATION_TEMPLATE_DIR}/ccipdConfusionMatrix.hxx"
  "${DATA_EVALUATION_TEMPLATE_DIR}/ccipdStatisticalPerformanceMeasures.hxx"
  
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateCenterOfMassDistance.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdMaskToDistanceImage.cxx"  
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateDICE.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateHausdorffDistance.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateLandmarkRMSE.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateMAD.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateMedialAxesDistance.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateMedialAxesRMSE.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateSimpleMedialAxesDistance.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateSimpleMedialAxesRMSE.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateVolume.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateWarpFieldDifference.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdCalculateWarpFieldSSD.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdPrepareMaskForEvaluation.cxx"
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdQuantitativeSegmentationResults.cxx" 
  "${DATA_EVALUATION_SOURCE_DIR}/ccipdStatisticalPerformanceMeasures.cxx" 
)
SET( TEST_DICE_FILES
  "${DATA_EVALUATION_TEST_DIR}/TestDice.cxx"
)
SET( TEST_SLICE_VOLUME_CONVERSION_FILES
  "${DATA_EVALUATION_TEST_DIR}/TestSliceVolumeConversion.cxx"
)
SET( TEST_WARP_FIELD_STANDARD_DEVIATION_FILES
  "${DATA_EVALUATION_TEST_DIR}/TestWarpFieldStandardDeviation.cxx"
)
SET( TEST_MAD_FILES
  "${DATA_EVALUATION_TEST_DIR}/TestMAD.cxx"
)
SET( TEST_MEDIAL_AXIS_FILES_SIMPLE
  "${DATA_EVALUATION_TEST_DIR}/TestMedialAxis.cxx"
)
SET( TEST_SSD_FILES
  "${DATA_EVALUATION_TEST_DIR}/TestSSD.cxx"
)
SET( TEST_MASK_TO_DISTANCE_FILES
  "${DATA_EVALUATION_TEST_DIR}/TestMaskToDistanceImage.cxx"
)

SET( TEST_CONFUSION_MATRIX_FILES          "${DATA_EVALUATION_TEST_DIR}/TestConfusionMatrix.cxx" )
SET( TEST_STAT_PERFORMANCE_MEASURES_FILES "${DATA_EVALUATION_TEST_DIR}/TestStatisticalPerformanceMeasures.cxx" )
###########################################################################################



###########################################################################################
# Create our libraries		
IF( ENABLE_DATA_EVALUATION )
  OPTION ( SHARED_DATA_EVALUATION
    "Build Data evaluation as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF ( SHARED_DATA_EVALUATION )
    SET ( LIB_TYPE SHARED )
  ELSE ()
    SET ( LIB_TYPE STATIC )
  ENDIF ( SHARED_DATA_EVALUATION )  
  ADD_LIBRARY( ccipdDataEvaluation ${LIB_TYPE} ${DATA_EVALUATION_FILES} )
ENDIF( ENABLE_DATA_EVALUATION )
###########################################################################################



###########################################################################################
IF( BUILD_TESTING AND ENABLE_DATA_EVALUATION )
  # Create our test programs

  ADD_EXECUTABLE( TestDice                             ${TEST_DICE_FILES}                          )
  ADD_EXECUTABLE( TestSliceVolumeConversion            ${TEST_SLICE_VOLUME_CONVERSION_FILES}       )
  ADD_EXECUTABLE( TestWarpFieldStandardDeviation       ${TEST_WARP_FIELD_STANDARD_DEVIATION_FILES} )
  ADD_EXECUTABLE( TestMAD                              ${TEST_MAD_FILES}                           )
  ADD_EXECUTABLE( TestConfusionMatrix                  ${TEST_CONFUSION_MATRIX_FILES}     )  
  ADD_EXECUTABLE( TestStatisticalPerformanceMeasures   ${TEST_STAT_PERFORMANCE_MEASURES_FILES}     )
  
  
  IF ( ENABLE_DATA_CORE )
    ADD_EXECUTABLE( TestMedialAxisSimple      ${TEST_MEDIAL_AXIS_FILES_SIMPLE}   )
    ADD_EXECUTABLE( TestMaskToDistance        ${TEST_MASK_TO_DISTANCE_FILES}     )
    
    IF ( ENABLE_REGISTRATION_TRANSFORMATIONS )  
      ADD_EXECUTABLE( TestSSD                   ${TEST_SSD_FILES} )
    ENDIF ( ENABLE_REGISTRATION_TRANSFORMATIONS )  
    
  ENDIF ( ENABLE_DATA_CORE )
  
ENDIF( BUILD_TESTING AND ENABLE_DATA_EVALUATION )
###########################################################################################


###########################################################################################
# Link the libraries
IF( ENABLE_DATA_EVALUATION )

  SET( DATA_EVALUATION_LIBS
    "ccipdDataEvaluation"
    ${SEGMENTATION_CORE_LIBS}
    ${ITK_LIBRARIES}
  )
  
  CCIPD_REMOVE_DUPLICATE_LIBS( DATA_EVALUATION_LIBS ${DATA_EVALUATION_LIBS} )
  
  TARGET_LINK_LIBRARIES( ccipdDataEvaluation
    ${DATA_EVALUATION_LIBS} )
    
ENDIF( ENABLE_DATA_EVALUATION )

IF( ENABLE_DATA_EVALUATION AND BUILD_TESTING )
  TARGET_LINK_LIBRARIES( TestDice                           ${DATA_EVALUATION_LIBS} )
  TARGET_LINK_LIBRARIES( TestSliceVolumeConversion          ${DATA_CORE_LIBS} ${DATA_EVALUATION_LIBS} ${COMMON_IMAGES_LIBS} )
  TARGET_LINK_LIBRARIES( TestWarpFieldStandardDeviation     ${DATA_CORE_LIBS} ${DATA_EVALUATION_LIBS} ${REGISTRATION_IMAGES_LIBS})
  TARGET_LINK_LIBRARIES( TestMAD                            ${DATA_EVALUATION_LIBS} )
  TARGET_LINK_LIBRARIES( TestConfusionMatrix                ${DATA_EVALUATION_LIBS} ) 
  TARGET_LINK_LIBRARIES( TestStatisticalPerformanceMeasures ${DATA_EVALUATION_LIBS} ) 

 
  IF ( ENABLE_DATA_CORE )
    TARGET_LINK_LIBRARIES( TestMedialAxisSimple     ${DATA_EVALUATION_LIBS} ${DATA_CORE_LIBS} )
       
    IF ( ENABLE_REGISTRATION_TRANSFORMATIONS )
      TARGET_LINK_LIBRARIES( TestMaskToDistance       
        ${DATA_EVALUATION_LIBS} 
        ${DATA_CORE_LIBS} 
        ${REGISTRATION_TRANSFORMATIONS_LIBS})

      TARGET_LINK_LIBRARIES( TestSSD
	      ${REGISTRATION_TRANSFORMATIONS_LIBS}
	      ${DATA_CORE_LIBS} 
        ${DATA_EVALUATION_LIBS} )
    ENDIF ( ENABLE_REGISTRATION_TRANSFORMATIONS )
    
  ENDIF( ENABLE_DATA_CORE )
  
ENDIF( ENABLE_DATA_EVALUATION AND BUILD_TESTING )
###########################################################################################



###########################################################################################
# Tell the user which libraries are being built.

MESSAGE(STATUS "Building Data Evaluation: "  ${ENABLE_DATA_EVALUATION} )

###########################################################################################
