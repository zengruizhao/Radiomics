# Check if required libraries are enabled
IF(ENABLE_CLASSIFICATION_VALIDATION)

IF(NOT ENABLE_CLASSIFICATION_CORE)
  MESSAGE( "Classification Validation requires Classification Core.")
  SET( ENABLE_CLASSIFICATION_VALIDATION FALSE )
ENDIF()

IF(NOT ENABLE_COMMON_CORE)
  MESSAGE("Classification Validation requires Common Core.")
  SET(ENABLE_CLASSIFICATION_VALIDATION FALSE )
ENDIF()

IF(NOT ENABLE_COMMON_MATH)
  MESSAGE("Classification Validation requires Common Math.")
  SET(ENABLE_CLASSIFICATION_VALIDATION FALSE )
ENDIF()

IF(NOT ENABLE_COMMON_IMAGES)
  MESSAGE("Classification Validation requires Common Images.")
  SET(ENABLE_CLASSIFICATION_VALIDATION FALSE )
ENDIF()


IF(NOT ENABLE_DATA_CORE)
  MESSAGE("Classification Validation requires Data Core.")
  SET(ENABLE_CLASSIFICATION_VALIDATION FALSE )
ENDIF()

IF(NOT ENABLE_DATA_VALIDATION)
  MESSAGE("Classification Validation requires Data Validation.")
  SET(ENABLE_CLASSIFICATION_VALIDATION FALSE)
ENDIF()

IF(NOT ENABLE_PREPROCESSING_CORE)
  MESSAGE( "Classification Validation requires Preprocessing Core.")
  SET(ENABLE_CLASSIFICATION_VALIDATION FALSE )
ENDIF()

IF(NOT ENABLE_FILTERING_FEATURE_COLLECTION)
  MESSAGE( "Classification Validation requires Filtering Feature Collection.")
  SET(ENABLE_CLASSIFICATION_VALIDATION FALSE )
ENDIF()




ENDIF()

IF(NOT ENABLE_CLASSIFICATION_VALIDATION)
	MESSAGE("Disabling Classification Validation library.")
	RETURN()
ENDIF()



###########################################################################################



###########################################################################################
# Set the location of our files
SET( CLASSIFICATION_VALIDATION_INCLUDE_DIR   "${CLASSIFICATION_VALIDATION_DIR}/include"   )
SET( CLASSIFICATION_VALIDATION_TEMPLATES_DIR "${CLASSIFICATION_VALIDATION_DIR}/templates" )
SET( CLASSIFICATION_VALIDATION_SOURCE_DIR    "${CLASSIFICATION_VALIDATION_DIR}/source"    )
SET( CLASSIFICATION_VALIDATION_TEST_DIR      "${CLASSIFICATION_VALIDATION_DIR}/test"      )
SET( CLASSIFICATION_VALIDATION_PROJECTS_DIR  "${CLASSIFICATION_VALIDATION_DIR}/projects"  )


SET ( CLASSIFICATION_INCLUDE_DIRS       ${CLASSIFICATION_INCLUDE_DIRS}
									   "${CLASSIFICATION_VALIDATION_INCLUDE_DIR}" 
									   "${CLASSIFICATION_VALIDATION_TEMPLATES_DIR}" 
									   "${DATA_VALIDATION_INCLUDE_DIR}"
									   "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}"
    )
###########################################################################################



###########################################################################################
# List our files
SET( CLASSIFICATION_VALIDATION_FILES
  "${CLASSIFICATION_VALIDATION_INCLUDE_DIR}/ccipdPrepareClassificationData.h"
#  "${CLASSIFICATION_VALIDATION_TEMPLATES_DIR}/*.hxx"
  "${CLASSIFICATION_VALIDATION_SOURCE_DIR}/ccipdPrepareClassificationData.cxx"
)

# Tests
#  SET(TEST_LDA_CLASSIFICATION_FILES "${CLASSIFICATION_CORE_TEST_DIR}/TestLDAClassification.cxx")
#  SET(TEST_QDA_CLASSIFICATION_FILES "${CLASSIFICATION_CORE_TEST_DIR}/TestQDAClassification.cxx")
 
# Projects
SET(GET_OPTIMAL_FEATURE_SET_FILES "${CLASSIFICATION_VALIDATION_PROJECTS_DIR}/ccipdGetOptimalFeatureSet.cxx")

###########################################################################################
# Add our libraries
IF( ENABLE_CLASSIFICATION_VALIDATION )	

  OPTION( SHARED_CLASSIFICATION_VALIDATION "Build Classification Validation as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
    
  IF( SHARED_CLASSIFICATION_VALIDATION )
     SET( LIB_TYPE SHARED )
  ELSE()
     SET( LIB_TYPE STATIC )
  ENDIF( SHARED_CLASSIFICATION_VALIDATION )	  

  ADD_LIBRARY( ccipdClassificationValidation ${LIB_TYPE} ${CLASSIFICATION_VALIDATION_FILES} )	 

  # link any extra libraries (itk, vtk, etc.) we need to link
  SET( CLASSIFICATION_VALIDATION_LIBS
    "ccipdClassificationValidation"
	${CLASSIFICATION_CORE_LIBS}
	
	${COMMON_CORE_LIBS}
	${COMMON_MATH_LIBS}
	${COMMON_IMAGE_LIBS}
	
    
	${DATA_CORE_LIBS}
	${DATA_VALIDATION_LIBS}
	
	${PREPROCESSING_CORE_LIBS}
	${SEGMENTATION_CORE_LIBS}
	${REGISTRATION_CORE_LIBS}
  )
  CCIPD_REMOVE_DUPLICATE_LIBS( CLASSIFICATION_VALIDATION_LIBS ${CLASSIFICATION_VALIDATION_LIBS} )

  TARGET_LINK_LIBRARIES( ccipdClassificationValidation ${CLASSIFICATION_VALIDATION_LIBS} )
  
ENDIF( ENABLE_CLASSIFICATION_VALIDATION )

###########################################################################################
# Add our test programs if build_testing is true
IF( BUILD_TESTING AND ENABLE_CLASSIFICATION_VALIDATION)
  
  #ADD_EXECUTABLE( TestLDAClassification ${TEST_LDA_CLASSIFICATION_FILES}   )
  #TARGET_LINK_LIBRARIES( TestLDAClassification ${CLASSIFICATION_CORE_LIBS} )  
  
  #ADD_EXECUTABLE( TestQDAClassification ${TEST_QDA_CLASSIFICATION_FILES})
  #TARGET_LINK_LIBRARIES( TestQDAClassification ${CLASSIFICATION_CORE_LIBS} )

ENDIF( BUILD_TESTING AND ENABLE_CLASSIFICATION_VALIDATION )
###########################################################################################

  ADD_EXECUTABLE( GetOptimalFeatureSet ${GET_OPTIMAL_FEATURE_SET_FILES}   )
  TARGET_LINK_LIBRARIES( GetOptimalFeatureSet  ${CLASSIFICATION_VALIDATION_LIBS} ) 


###########################################################################################
IF(ENABLE_CLASSIFICATION_VALIDATION)
MESSAGE(STATUS "Building classification Validation library: " ${ENABLE_CLASSIFICATION_VALIDATION} )

ELSE()
MESSAGE("Building classification Validation library: " ${ENABLE_CLASSIFICATION_VALIDATION} )
ENDIF()
###########################################################################################


