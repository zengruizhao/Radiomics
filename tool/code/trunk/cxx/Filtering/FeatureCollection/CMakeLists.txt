
IF(ENABLE_FILTERING_FEATURE_COLLECTION)

	IF(NOT ENABLE_COMMON_IMAGES )
		MESSAGE( "Filtering Feature Collection requires building Common Images." )
		SET( ENABLE_FILTERING_FEATURE_COLLECTION FALSE )
	ENDIF()

	IF(NOT ENABLE_COMMON_HASH )
		MESSAGE( "Filtering Feature Collection requires building Common Hash." )
		SET( ENABLE_FILTERING_FEATURE_COLLECTION FALSE )
	ENDIF()

#	IF(NOT ENABLE_DATA_CORE )
#		MESSAGE( "Filtering Feature Collection requires building Data Core." )
#		SET( ENABLE_FILTERING_FEATURE_COLLECTION FALSE )
#	ENDIF()

#	IF (NOT ENABLE_REGISTRATION_IMAGES )
#		MESSAGE( "Filtering Feature Collection requires building Registration Images." )
#		SET( ENABLE_FILTERING_FEATURE_COLLECTION FALSE )
#	ENDIF ()

	IF (NOT BUILD_BOOST_PROJECTS )
		MESSAGE( "Filtering Feature Collection requires building Boost." )
		SET( ENABLE_FILTERING_FEATURE_COLLECTION FALSE )
	ENDIF ()

ENDIF()

IF(NOT ENABLE_FILTERING_FEATURE_COLLECTION)
	MESSAGE( "Disabling Filtering Feature Collection library." )
	RETURN()
ENDIF()


###########################################################################################



###########################################################################################
# Say where our files are
SET( FILTERING_FEATURE_COLLECTION_INCLUDE_DIR
  "${FILTERING_FEATURE_COLLECTION_DIR}/include" )
SET( FILTERING_FEATURE_COLLECTION_TEMPLATES_DIR
  "${FILTERING_FEATURE_COLLECTION_DIR}/templates" )
SET( FILTERING_FEATURE_COLLECTION_SOURCE_DIR
  "${FILTERING_FEATURE_COLLECTION_DIR}/source" )
SET( FILTERING_FEATURE_COLLECTION_TEST_DIR
  "${FILTERING_FEATURE_COLLECTION_DIR}/test"  )
  
SET( FILTERING_INCLUDE_DIRS
  ${FILTERING_INCLUDE_DIRS}
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}"
  "${FILTERING_FEATURE_COLLECTION_TEMPLATES_DIR}"
  )
###########################################################################################



###########################################################################################
# Configure some registration algorithms options
CONFIGURE_FILE(
  ${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdFilteringFeatureCollectionOptions.h.in
  ${Filtering_BINARY_DIR}/ccipdFilteringFeatureCollectionOptions.h @ONLY )
###########################################################################################



###########################################################################################
SET( FILTERING_FEATURE_COLLECTION_FILES
  "${Filtering_BINARY_DIR}/ccipdFilteringFeatureCollectionOptions.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdFeatureCollectionTypes.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdConcatenateFeatureImages.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdFeatureCollection.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdFeatureID.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdFeaturesFromTextures.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureCollectionParser.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureFeatureExtractor.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureFeatureFirstOrderParameters.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureFeatureGaborParameters.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureFeatureHaralickParameters.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureFeatureSobelParameters.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureFeatureParser.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureFeatureGetParametersByVariant.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTextureFeatureMapParametersType.h"
  "${FILTERING_FEATURE_COLLECTION_INCLUDE_DIR}/ccipdTexturesFromFeatures.h"
  "${FILTERING_FEATURE_COLLECTION_TEMPLATES_DIR}/ccipdTextureCollectionParser.hxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdConcatenateFeatureImages.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdFeatureCollection.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdFeatureID.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdFeaturesFromTextures.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTextureCollectionParser.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTextureFeatureExtractor.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTextureFeatureFirstOrderParameters.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTextureFeatureGaborParameters.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTextureFeatureHaralickParameters.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTextureFeatureGetParametersByVariant.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTextureFeatureSobelParameters.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTextureFeatureParser.cxx"
  "${FILTERING_FEATURE_COLLECTION_SOURCE_DIR}/ccipdTexturesFromFeatures.cxx"
)
SET( TEST_GAUSSIANMODEL_FILTERING_FILES
  "${FILTERING_FEATURE_COLLECTION_TEST_DIR}/TestGaussianModelImage.cxx"
)
SET( TEST_EXTRACT_FEATURES_FILES
  "${FILTERING_FEATURE_COLLECTION_TEST_DIR}/TestExtractTextureFeatures.cxx"
)
###########################################################################################



###########################################################################################
# Create our libraries		
IF( ENABLE_FILTERING_FEATURE_COLLECTION )
  OPTION( SHARED_FILTERING_FEATURE_COLLECTION
    "Build FeatureCollection as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF( SHARED_FILTERING_FEATURE_COLLECTION )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_FILTERING_FEATURE_COLLECTION )  
  ADD_LIBRARY( ccipdFilteringFeatureCollection
    ${LIB_TYPE} ${FILTERING_FEATURE_COLLECTION_FILES} )
ENDIF( ENABLE_FILTERING_FEATURE_COLLECTION )
###########################################################################################



###########################################################################################
IF ( BUILD_TESTING AND ENABLE_FILTERING_FEATURE_COLLECTION )
  # Create our test programs
  ADD_EXECUTABLE( TestGaussianModelImage ${TEST_GAUSSIANMODEL_FILTERING_FILES} )
  ADD_EXECUTABLE( TestExtractTextureFeatures    ${TEST_EXTRACT_FEATURES_FILES} )  
ENDIF()
###########################################################################################



###########################################################################################
IF( ENABLE_FILTERING_FEATURE_COLLECTION )

  SET( FILTERING_FEATURE_COLLECTION_LIBS
    "ccipdFilteringFeatureCollection"
    ${COMMON_HASH_LIBS}
    ${COMMON_IMAGES_LIBS}
	  ${REGISTRATION_IMAGES_LIBS}
    ${DATA_CORE_LIBS}
  )

  CCIPD_REMOVE_DUPLICATE_LIBS( FILTERING_FEATURE_COLLECTION_LIBS ${FILTERING_FEATURE_COLLECTION_LIBS} )
  
  TARGET_LINK_LIBRARIES( ccipdFilteringFeatureCollection
    ${FILTERING_FEATURE_COLLECTION_LIBS} )

ENDIF( ENABLE_FILTERING_FEATURE_COLLECTION )
  
IF( ENABLE_FILTERING_FEATURE_COLLECTION AND BUILD_TESTING )

  IF( ENABLE_DATA_CORE AND ENABLE_FILTERING_CORE )  
    IF( ENABLE_REGISTRATION_IMAGES )
      TARGET_LINK_LIBRARIES( TestGaussianModelImage                     
        ${FILTERING_CORE_LIBS}
        ${FILTERING_FEATURE_COLLECTION_LIBS} )
    ENDIF( ENABLE_REGISTRATION_IMAGES )
	
	TARGET_LINK_LIBRARIES( TestExtractTextureFeatures
    ${FILTERING_CORE_LIBS}
    ${FILTERING_FEATURE_COLLECTION_LIBS} )
  ENDIF( ENABLE_DATA_CORE AND ENABLE_FILTERING_CORE )
  
ENDIF( ENABLE_FILTERING_FEATURE_COLLECTION AND BUILD_TESTING )
###########################################################################################



###########################################################################################

MESSAGE(STATUS "Building FeatureCollection library: " ${ENABLE_FILTERING_FEATURE_COLLECTION} )  


###########################################################################################


