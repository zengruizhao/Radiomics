


###########################################################################################

IF( ENABLE_PREPROCESSING_CORE AND NOT ENABLE_REGISTRATION_IMAGES )
  MESSAGE( "Preprocessing core library requires "   )
  MESSAGE( "the registration images library. "        )
  MESSAGE( "Disabling preprocessing core library."  )
  SET( ENABLE_PREPROCESSING_CORE FALSE )
ENDIF()

IF( ENABLE_PREPROCESSING_CORE AND NOT ENABLE_REGISTRATION_TRANSFORMATIONS )
  MESSAGE( "Preprocessing core library requires "     )
  MESSAGE( "the registration transformations library. " )
  MESSAGE( "Disabling preprocessing core library."    )
  SET( ENABLE_PREPROCESSING_CORE FALSE )
ENDIF()
###########################################################################################



###########################################################################################
# Set the location of our files
SET( PREPROCESSING_CORE_INCLUDE_DIR   "${PREPROCESSING_CORE_DIR}/include"   )
SET( PREPROCESSING_CORE_TEMPLATES_DIR "${PREPROCESSING_CORE_DIR}/templates" )
SET( PREPROCESSING_CORE_SOURCE_DIR    "${PREPROCESSING_CORE_DIR}/source"    )
SET( PREPROCESSING_CORE_TEST_DIR      "${PREPROCESSING_CORE_DIR}/test"      )

SET ( PREPROCESSING_INCLUDE_DIRS
  ${PREPROCESSING_INCLUDE_DIRS}
  "${PREPROCESSING_CORE_INCLUDE_DIR}"
  "${PREPROCESSING_CORE_TEMPLATES_DIR}" 
  )
###########################################################################################



###########################################################################################
# List our files
SET( PREPROCESSING_CORE_FILES
  "${PREPROCESSING_CORE_INCLUDE_DIR}/itkCohenBiasCorrectionFilter.h"
  "${PREPROCESSING_CORE_INCLUDE_DIR}/ccipdCohenMRIBiasCorrection.h"
  "${PREPROCESSING_CORE_INCLUDE_DIR}/ccipdN4MRIBiasCorrection.h"
  "${PREPROCESSING_CORE_INCLUDE_DIR}/ccipdPreprocessVolume.h"
  "${PREPROCESSING_CORE_INCLUDE_DIR}/ccipdPreprocessMRI.h"
  "${PREPROCESSING_CORE_INCLUDE_DIR}/ccipdPreprocessGIPL.h" 
  "${PREPROCESSING_CORE_INCLUDE_DIR}/ccipdPreprocessEYE.h"
  "${PREPROCESSING_CORE_INCLUDE_DIR}/ccipdPreprocessT2.h"
  "${PREPROCESSING_CORE_INCLUDE_DIR}/ccipdPreprocessADC.h"
  
  "${PREPROCESSING_CORE_TEMPLATES_DIR}/itkCohenBiasCorrectionFilter.hxx"
  "${PREPROCESSING_CORE_TEMPLATES_DIR}/ccipdPreprocessADC.hxx"
  "${PREPROCESSING_CORE_TEMPLATES_DIR}/ccipdPreprocessT2.hxx"
  
  "${PREPROCESSING_CORE_SOURCE_DIR}/ccipdCohenMRIBiasCorrection.cxx"
  "${PREPROCESSING_CORE_SOURCE_DIR}/ccipdN4MRIBiasCorrection.cxx"
  "${PREPROCESSING_CORE_SOURCE_DIR}/ccipdPreprocessVolume.cxx"
  "${PREPROCESSING_CORE_SOURCE_DIR}/ccipdPreprocessMRI.cxx"
  "${PREPROCESSING_CORE_SOURCE_DIR}/ccipdPreprocessGIPL.cxx"
  "${PREPROCESSING_CORE_SOURCE_DIR}/ccipdPreprocessEYE.cxx"
  "${PREPROCESSING_CORE_SOURCE_DIR}/ccipdPreprocessADC.cxx"
  
  
)

IF( BUILD_TESTING )

  SET( TEST_COHEN_BIAS_CORRECTION_FILES
    "${PREPROCESSING_CORE_TEST_DIR}/TestCohenBiasCorrectionFilter.cxx"
  )

  SET( TEST_N4_BIAS_CORRECTION_FILES
    "${PREPROCESSING_CORE_TEST_DIR}/TestN4BiasCorrectionFilter.cxx"
  )

  SET( TEST_MRI_PREPROCESSING_FILES
    "${PREPROCESSING_CORE_TEST_DIR}/TestMRIPreprocessing.cxx"
  )    

  SET( TEST_TRUS_PREPROCESSING_FILES
    "${PREPROCESSING_CORE_TEST_DIR}/TestTRUSPreprocessing.cxx"
  )
  
ENDIF( BUILD_TESTING )
###########################################################################################



###########################################################################################
# add our libraries
IF( ENABLE_PREPROCESSING_CORE )
  OPTION( SHARED_PREPROCESSING_CORE
    "Build BiasCorrection as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF( SHARED_PREPROCESSING_CORE )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_PREPROCESSING_CORE )
  ADD_LIBRARY( ccipdPreprocessingCore ${LIB_TYPE} ${PREPROCESSING_CORE_FILES} )
ENDIF( ENABLE_PREPROCESSING_CORE )
###########################################################################################



###########################################################################################
# add our test programs if build_testing is true
IF( BUILD_TESTING AND ENABLE_PREPROCESSING_CORE AND ENABLE_DATA_CORE )

  ADD_EXECUTABLE( TestCohenBiasCorrection ${TEST_COHEN_BIAS_CORRECTION_FILES} )
  ADD_EXECUTABLE( TestMRIPreprocessing    ${TEST_MRI_PREPROCESSING_FILES}     )
  
  IF( ENABLE_REGISTRATION_CORE ) 
	ADD_EXECUTABLE( TestN4BiasCorrection  ${TEST_N4_BIAS_CORRECTION_FILES} )
  ENDIF( ENABLE_REGISTRATION_CORE ) 
  
  IF(  ENABLE_DATA_EVALUATION AND ENABLE_DATA_PATIENTS  )	
    ADD_EXECUTABLE( TestTRUSPreprocessing ${TEST_TRUS_PREPROCESSING_FILES} )
  ENDIF(  ENABLE_DATA_EVALUATION AND ENABLE_DATA_PATIENTS  )
	
ENDIF( BUILD_TESTING AND ENABLE_PREPROCESSING_CORE AND ENABLE_DATA_CORE )
###########################################################################################



###########################################################################################
# link any extra libraries (itk, vtk, etc.) we need to link
IF( ENABLE_PREPROCESSING_CORE )

  SET( PREPROCESSING_CORE_LIBS
    "ccipdPreprocessingCore"
    ${REGISTRATION_IMAGES_LIBS}
    ${REGISTRATION_TRANSFORMATIONS_LIBS}
  )

  CCIPD_REMOVE_DUPLICATE_LIBS( PREPROCESSING_CORE_LIBS ${PREPROCESSING_CORE_LIBS} )

  TARGET_LINK_LIBRARIES( ccipdPreprocessingCore ${PREPROCESSING_CORE_LIBS} )
ENDIF( ENABLE_PREPROCESSING_CORE )
  
IF( BUILD_TESTING AND ENABLE_PREPROCESSING_CORE )
  IF( ENABLE_DATA_CORE )
    TARGET_LINK_LIBRARIES( TestCohenBiasCorrection
      ${DATA_CORE_LIBS} ${PREPROCESSING_CORE_LIBS} )  
    TARGET_LINK_LIBRARIES( TestMRIPreprocessing
      ${DATA_CORE_LIBS} ${PREPROCESSING_CORE_LIBS} )
	  
    IF( ENABLE_REGISTRATION_CORE )    
	  TARGET_LINK_LIBRARIES( TestN4BiasCorrection
        ${PREPROCESSING_CORE_LIBS}
        ${REGISTRATION_CORE_LIBS}
        ${DATA_CORE_LIBS}
        ${FILTERING_CORE_LIBS}
       )
    ENDIF( ENABLE_REGISTRATION_CORE )
	  
	IF( ENABLE_DATA_EVALUATION AND ENABLE_DATA_PATIENTS )	  
      TARGET_LINK_LIBRARIES( TestTRUSPreprocessing
        ${PREPROCESSING_CORE_LIBS}
        ${DATA_PATIENTS_LIBS}
        ${DATA_EVALUATION_LIBS} 
      )
	ENDIF( ENABLE_DATA_EVALUATION AND ENABLE_DATA_PATIENTS )	
 	
  ENDIF( ENABLE_DATA_CORE )	
ENDIF( BUILD_TESTING AND ENABLE_PREPROCESSING_CORE )
###########################################################################################



###########################################################################################
IF(ENABLE_PREPROCESSING_CORE)
MESSAGE(STATUS "Building preprocessing core library: " ${ENABLE_PREPROCESSING_CORE} )
ELSE()
MESSAGE(       "Building preprocessing core library: " ${ENABLE_PREPROCESSING_CORE} )
ENDIF()
###########################################################################################


