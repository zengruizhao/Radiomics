###########################################################################################
# Say where our files are
SET( FILTERING_DCE_INCLUDE_DIR   "${FILTERING_DCE_DIR}/include"   )
SET( FILTERING_DCE_TEMPLATES_DIR "${FILTERING_DCE_DIR}/templates" )
# This is a fully templated, so no need for source folder
#SET( FILTERING_DCE_SOURCE_DIR    "${FILTERING_DCE_DIR}/source"    )
SET( FILTERING_DCE_TEST_DIR      "${FILTERING_DCE_DIR}/test"      )
SET( FILTERING_DCE_PROJECTS_DIR  "${FILTERING_DCE_DIR}/projects"  )

SET( FILTERING_INCLUDE_DIRS
  ${FILTERING_INCLUDE_DIRS}
  "${FILTERING_DCE_INCLUDE_DIR}"
  "${FILTERING_DCE_TEMPLATES_DIR}" )
###########################################################################################

###########################################################################################
SET( FILTERING_DCE_FILES
         "${FILTERING_DCE_INCLUDE_DIR}/itkDCEMaxFilter.h"      "${FILTERING_DCE_TEMPLATES_DIR}/itkDCEMaxFilter.hxx"
		 "${FILTERING_DCE_INCLUDE_DIR}/itkDCEMeanFilter.h"     "${FILTERING_DCE_TEMPLATES_DIR}/itkDCEMeanFilter.hxx"
		 "${FILTERING_DCE_INCLUDE_DIR}/itkDCEMedianFilter.h"   "${FILTERING_DCE_TEMPLATES_DIR}/itkDCEMedianFilter.hxx"
		 "${FILTERING_DCE_INCLUDE_DIR}/itkDCEMinFilter.h"      "${FILTERING_DCE_TEMPLATES_DIR}/itkDCEMinFilter.hxx"
		 "${FILTERING_DCE_INCLUDE_DIR}/itkDCEStdDevFilter.h"   "${FILTERING_DCE_TEMPLATES_DIR}/itkDCEStdDevFilter.hxx"
		 "${FILTERING_DCE_INCLUDE_DIR}/itkDCEVarianceFilter.h" "${FILTERING_DCE_TEMPLATES_DIR}/itkDCEVarianceFilter.hxx"
		 "${FILTERING_DCE_INCLUDE_DIR}/itkDCESumFilter.h"      "${FILTERING_DCE_TEMPLATES_DIR}/itkDCESumFilter.hxx"
		 
)

SET( TEST_DCE_MAX_FILTERING_FILES      "${FILTERING_DCE_TEST_DIR}/TestDCEMaxFilter.cxx"     )
SET( TEST_DCE_MEAN_FILTERING_FILES     "${FILTERING_DCE_TEST_DIR}/TestDCEMeanFilter.cxx"    )
SET( TEST_DCE_MEDIAN_FILTERING_FILES   "${FILTERING_DCE_TEST_DIR}/TestDCEMedianFilter.cxx"  )
SET( TEST_DCE_MIN_FILTERING_FILES      "${FILTERING_DCE_TEST_DIR}/TestDCEMinFilter.cxx"     )
SET( TEST_DCE_STDDEV_FILTERING_FILES   "${FILTERING_DCE_TEST_DIR}/TestDCEStdDevFilter.cxx"  )
SET( TEST_DCE_VARIANCE_FILTERING_FILES "${FILTERING_DCE_TEST_DIR}/TestDCEVarianceFilter.cxx")
SET( TEST_DCE_SUM_FILTERING_FILES      "${FILTERING_DCE_TEST_DIR}/TestDCESumFilter.cxx"     )

#SET( COMPARE_FILTER_FILES
#  "${FILTERING_CORE_PROJECTS_DIR}/ccipdCompareFilterTypes.cxx"
#)

###########################################################################################



###########################################################################################
# Create our libraries		
IF( ENABLE_FILTERING_DCE )
  OPTION( SHARED_FILTERING_DCE "Build Filtering DCE as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF( SHARED_FILTERING_DCE )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_FILTERING_DCE )  
  #ADD_LIBRARY( ccipdFilteringDCE ${LIB_TYPE} ${FILTERING_DCE_FILES} )
  #SET( FILTERING_DCE_LIBS "ccipdFilteringDCE" ${ITK_LIBRARIES} )
  #CCIPD_REMOVE_DUPLICATE_LIBS( FILTERING_DCE_LIBS ${FILTERING_DCE_LIBS} )
  #TARGET_LINK_LIBRARIES( ccipdFilteringDCE ${FILTERING_DCE_LIBS} )
  
  
  
  
  
  
  # Create our test programs
IF(BUILD_TESTING )

  ADD_EXECUTABLE( TestDCEMaxFilter ${TEST_DCE_MAX_FILTERING_FILES} )
  TARGET_LINK_LIBRARIES( TestDCEMaxFilter         ${ITK_LIBRARIES} )
  
  ADD_EXECUTABLE( TestDCEMeanFilter ${TEST_DCE_MEAN_FILTERING_FILES} )
  TARGET_LINK_LIBRARIES( TestDCEMeanFilter         ${ITK_LIBRARIES} )
  
  ADD_EXECUTABLE( TestDCEMedianFilter ${TEST_DCE_MEDIAN_FILTERING_FILES} )
  TARGET_LINK_LIBRARIES( TestDCEMedianFilter         ${ITK_LIBRARIES} )
  
  ADD_EXECUTABLE( TestDCEMinFilter ${TEST_DCE_MIN_FILTERING_FILES} )
  TARGET_LINK_LIBRARIES( TestDCEMinFilter         ${ITK_LIBRARIES} )
  
  ADD_EXECUTABLE( TestDCEStdDevFilter ${TEST_DCE_STDDEV_FILTERING_FILES} )
  TARGET_LINK_LIBRARIES( TestDCEStdDevFilter         ${ITK_LIBRARIES} )
  
  ADD_EXECUTABLE( TestDCEVarianceFilter ${TEST_DCE_VARIANCE_FILTERING_FILES} )
  TARGET_LINK_LIBRARIES( TestDCEVarianceFilter         ${ITK_LIBRARIES} )
  
  ADD_EXECUTABLE( TestDCESumFilter ${TEST_DCE_SUM_FILTERING_FILES} )
  TARGET_LINK_LIBRARIES( TestDCESumFilter         ${ITK_LIBRARIES} )

ENDIF(BUILD_TESTING )
  
  
  
  
  
ENDIF( ENABLE_FILTERING_DCE )
###########################################################################################


IF(ENABLE_FILTERING_DCE)
MESSAGE(STATUS "Building Filtering DCE library: " ${ENABLE_FILTERING_DCE} )
ELSE()
MESSAGE( "Building Filtering DCE library: " ${ENABLE_FILTERING_DCE} )
ENDIF()

###########################################################################################


