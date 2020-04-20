###########################################################################################			
# Check if required libraries are enabled
IF( ENABLE_CLASSIFICATION_CORE AND NOT ENABLE_COMMON_MATH )
  MESSAGE( "Classification core library requires "   )
  MESSAGE( "the Common Math library. "       )
  MESSAGE( "Disabling classification core library."  )
  SET( ENABLE_CLASSIFICATION_CORE FALSE )
ENDIF()

###########################################################################################



###########################################################################################
# Set the location of our files
SET( CLASSIFICATION_CORE_INCLUDE_DIR   "${CLASSIFICATION_CORE_DIR}/include"   )
SET( CLASSIFICATION_CORE_TEMPLATES_DIR "${CLASSIFICATION_CORE_DIR}/templates" )
SET( CLASSIFICATION_CORE_SOURCE_DIR    "${CLASSIFICATION_CORE_DIR}/source"    )
SET( CLASSIFICATION_CORE_TEST_DIR      "${CLASSIFICATION_CORE_DIR}/test"      )
SET( CLASSIFICATION_CORE_PROJECTS_DIR  "${CLASSIFICATION_CORE_DIR}/projects"  )
SET ( CLASSIFICATION_INCLUDE_DIRS       ${CLASSIFICATION_INCLUDE_DIRS}
                                       "${CLASSIFICATION_CORE_INCLUDE_DIR}"
                                       "${CLASSIFICATION_CORE_TEMPLATES_DIR}" 
    )
###########################################################################################



###########################################################################################
# List our files
SET( CLASSIFICATION_CORE_FILES
  "${CLASSIFICATION_CORE_INCLUDE_DIR}/ccipdClassifierBase.h"
  "${CLASSIFICATION_CORE_INCLUDE_DIR}/ccipdDAClassifier.h"
  "${CLASSIFICATION_CORE_INCLUDE_DIR}/ccipdLDAClassifier.h"
  "${CLASSIFICATION_CORE_INCLUDE_DIR}/ccipdProbabilisticClassifier.h"  
  "${CLASSIFICATION_CORE_INCLUDE_DIR}/ccipdQDAClassifier.h"
  "${CLASSIFICATION_CORE_INCLUDE_DIR}/ccipdTrainableClassifier.h"
#  "${CLASSIFICATION_CORE_TEMPLATES_DIR}/*.hxx"
  "${CLASSIFICATION_CORE_SOURCE_DIR}/ccipdProbabilisticClassifier.cxx"
  "${CLASSIFICATION_CORE_SOURCE_DIR}/ccipdLDAClassifier.cxx"
  "${CLASSIFICATION_CORE_SOURCE_DIR}/ccipdQDAClassifier.cxx"
)



  SET(TEST_LDA_CLASSIFICATION_FILES "${CLASSIFICATION_CORE_TEST_DIR}/TestLDAClassification.cxx")

  SET(TEST_QDA_CLASSIFICATION_FILES "${CLASSIFICATION_CORE_TEST_DIR}/TestQDAClassification.cxx")

###########################################################################################
###########################################################################################
# Add our libraries
IF( ENABLE_CLASSIFICATION_CORE )	

  OPTION( SHARED_CLASSIFICATION_CORE "Build Classification as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
    
  IF( SHARED_CLASSIFICATION_CORE )
     SET( LIB_TYPE SHARED )
  ELSE()
     SET( LIB_TYPE STATIC )
  ENDIF( SHARED_CLASSIFICATION_CORE )	  

  ADD_LIBRARY( ccipdClassificationCore ${LIB_TYPE} ${CLASSIFICATION_CORE_FILES} )	 

  # link any extra libraries (itk, vtk, etc.) we need to link
  SET( CLASSIFICATION_CORE_LIBS
    "ccipdClassificationCore"
    ${COMMON_MATH_LIBS}
  )
  CCIPD_REMOVE_DUPLICATE_LIBS( CLASSIFICATION_CORE_LIBS ${CLASSIFICATION_CORE_LIBS} )

  TARGET_LINK_LIBRARIES( ccipdClassificationCore ${CLASSIFICATION_CORE_LIBS} )
  
ENDIF( ENABLE_CLASSIFICATION_CORE )

###########################################################################################
# Add our test programs if build_testing is true
IF( BUILD_TESTING AND ENABLE_CLASSIFICATION_CORE)
  
  ADD_EXECUTABLE( TestLDAClassification ${TEST_LDA_CLASSIFICATION_FILES}   )
  TARGET_LINK_LIBRARIES( TestLDAClassification ${CLASSIFICATION_CORE_LIBS} )  
  
  ADD_EXECUTABLE( TestQDAClassification ${TEST_QDA_CLASSIFICATION_FILES})
  TARGET_LINK_LIBRARIES( TestQDAClassification ${CLASSIFICATION_CORE_LIBS} )

ENDIF( BUILD_TESTING AND ENABLE_CLASSIFICATION_CORE )
###########################################################################################



###########################################################################################
IF(ENABLE_CLASSIFICATION_CORE)
	MESSAGE(STATUS "Building classification core library: " ${ENABLE_CLASSIFICATION_CORE} )
ELSE()
	MESSAGE("Building classification core library: " ${ENABLE_CLASSIFICATION_CORE} )
ENDIF()
###########################################################################################


