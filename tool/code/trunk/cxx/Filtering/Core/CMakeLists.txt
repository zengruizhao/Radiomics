
IF(ENABLE_FILTERING_CORE)
	#IF(NOT ENABLE_COMMON_VTK_CONVERSION )
	#	MESSAGE( "GUI core requires building VTK Conversion.")
	#	SET( ENABLE_GUI_CORE FALSE )
	#ENDIF()
ENDIF()

IF(NOT ENABLE_FILTERING_CORE)
	MESSAGE( "Disabling Filtering Core Library.")
	RETURN()
ENDIF()

###########################################################################################
# Say where our files are
SET( FILTERING_CORE_INCLUDE_DIR   "${FILTERING_CORE_DIR}/include"   )
SET( FILTERING_CORE_TEMPLATES_DIR "${FILTERING_CORE_DIR}/templates" )
SET( FILTERING_CORE_SOURCE_DIR    "${FILTERING_CORE_DIR}/source"    )
SET( FILTERING_CORE_TEST_DIR      "${FILTERING_CORE_DIR}/test"      )
SET( FILTERING_CORE_PROJECTS_DIR  "${FILTERING_CORE_DIR}/projects"  )

SET( FILTERING_INCLUDE_DIRS  ${FILTERING_INCLUDE_DIRS}
							"${FILTERING_CORE_INCLUDE_DIR}"
							"${FILTERING_CORE_TEMPLATES_DIR}" )
###########################################################################################




###########################################################################################
SET( FILTERING_CORE_FILES
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickFilter.h"                   
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickEnergyFilter.h"             
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickInertiaFilter.h"            
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickDifferenceEntropyFilter.h"  
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickSumEntropyFilter.h"         
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickIMC1Filter.h"               
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickIMC2Filter.h"               
  "${FILTERING_CORE_INCLUDE_DIR}/itkCorrelationFilter.h"                
  "${FILTERING_CORE_INCLUDE_DIR}/itkDCEFeaturesFilter.h"                
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickClusterShadeFilter.h"       
  "${FILTERING_CORE_INCLUDE_DIR}/itkHaralickInverseDifferenceMomentFilter.h"  
  "${FILTERING_CORE_INCLUDE_DIR}/itkGaborFilter.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkImageFilterMultipleInputs.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkInterpolateMultipleImagesFunction.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkLinearInterpolateMultipleImagesFunction.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkSelectiveInterpolateMultipleImagesFunction.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkNearestNeighborInterpolateMultipleImagesFunction.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkResampleMultipleImagesFilter.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkRangeImageFilter.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkMeanHistogram.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkRayleighHistogram.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkNakagamiHistogram.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkMeanVectorImageFilter.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkPercentileVectorImageFilter.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkSkewImageFilter.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkStandardDeviationImageFilter.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkVectorImageGaussianModelEstimator.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkVectorImageToProbabilityImage.h"
  "${FILTERING_CORE_INCLUDE_DIR}/itkVectorImageToVectorImageFilter.h"
  "${FILTERING_CORE_INCLUDE_DIR}/ccipdCalculateProbabilityImage.h"
  "${FILTERING_CORE_INCLUDE_DIR}/ccipdFeatureTypes.h"
  "${FILTERING_CORE_INCLUDE_DIR}/ccipdFilteringGroups.h"
  "${FILTERING_CORE_INCLUDE_DIR}/ccipdHistogram.h"
  "${FILTERING_CORE_INCLUDE_DIR}/ccipdCreateGaborKernel.h"
  "${FILTERING_CORE_INCLUDE_DIR}/ccipdHistogramTypes.h"
  "${FILTERING_CORE_INCLUDE_DIR}/ccipdStatistics.h"
  "${FILTERING_CORE_INCLUDE_DIR}/ccipdVolumeFeaturesToGaussianModels.h"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickEnergyFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickInertiaFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickDifferenceEntropyFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickSumEntropyFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickIMC1Filter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickIMC2Filter.hxx"			  
  "${FILTERING_CORE_TEMPLATES_DIR}/itkCorrelationFilter.hxx"  
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickClusterShadeFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkHaralickInverseDifferenceMomentFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkDCEFeaturesFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkGaborFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkImageFilterMultipleInputs.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkInterpolateMultipleImagesFunction.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkLinearInterpolateMultipleImagesFunction.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkSelectiveInterpolateMultipleImagesFunction.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkNearestNeighborInterpolateMultipleImagesFunction.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkResampleMultipleImagesFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkRangeImageFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkMeanHistogram.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkRayleighHistogram.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkNakagamiHistogram.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkMeanVectorImageFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkPercentileVectorImageFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkSkewImageFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkStandardDeviationImageFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkVectorImageGaussianModelEstimator.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkVectorImageToProbabilityImage.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/itkVectorImageToVectorImageFilter.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/ccipdCreateGaborKernel.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/ccipdHistogram.hxx"
  "${FILTERING_CORE_TEMPLATES_DIR}/ccipdStatistics.hxx"
  "${FILTERING_CORE_SOURCE_DIR}/ccipdCalculateProbabilityImage.cxx"
  "${FILTERING_CORE_SOURCE_DIR}/ccipdHistogram.cxx"
  "${FILTERING_CORE_SOURCE_DIR}/ccipdStatistics.cxx"
  "${FILTERING_CORE_SOURCE_DIR}/ccipdVolumeFeaturesToGaussianModels.cxx"
)

SET( TEST_HISTOGRAM_FILES
  "${FILTERING_CORE_TEST_DIR}/TestHistogram.cxx"
)

SET( TEST_HARALICK_FILTERING_FILES
  "${FILTERING_CORE_TEST_DIR}/TestHaralickFilter.cxx"
)
SET( TEST_HARALICK2_FILTERING_FILES
  "${FILTERING_CORE_TEST_DIR}/TestHaralickFilter2.cxx"
)

SET( TEST_HARALICK3D_FILTERING_FILES
  "${FILTERING_CORE_TEST_DIR}/TestHaralickFilter3D.cxx"
)

SET( TEST_GABOR_FILTERING_FILES
  "${FILTERING_CORE_TEST_DIR}/TestGaborFilter.cxx"
)

SET( TEST_FIXEDWINDOW_FILES
  "${FILTERING_CORE_TEST_DIR}/TestFixedWindow.cxx"
)
SET( TEST_MOVINGWINDOW_FILES
  "${FILTERING_CORE_TEST_DIR}/TestMovingWindow.cxx"
)
SET (TEST_CONVOLUTIONFILTER_FILES
  "${FILTERING_CORE_TEST_DIR}/TestConvolutionFilter.cxx"  
)
SET( TEST_DISCRETEGAUSSIANDISPLACEMENTFIELDFILTER_FILTERING_FILES
  "${FILTERING_CORE_TEST_DIR}/TestDiscreteGaussianDisplacementFieldFilter.cxx"
)
SET( CALCULATE_GRADIENT_FILES
  "${FILTERING_CORE_TEST_DIR}/TestGradientRecursiveGaussianImageFilter.cxx"
)
SET( TEST_CUT_OUT_FILES
  "${FILTERING_CORE_TEST_DIR}/TestCutOut.cxx"
)
SET( COMPARE_FILTER_FILES
  "${FILTERING_CORE_PROJECTS_DIR}/ccipdCompareFilterTypes.cxx"
)
SET (TEST_MULTIPLYFILTER_FILES
  "${FILTERING_CORE_TEST_DIR}/TestMultiplyFilter.cxx"  
)
SET (TEST_COMBINEIMAGES2VECTOR_FILES
  "${FILTERING_CORE_TEST_DIR}/TestCombineImages2Vector.cxx"  
)
SET (TEST_COMBINE3DDATAINTO4DDATA_FILES
  "${FILTERING_CORE_TEST_DIR}/TestCombine3DDataInto4DData.cxx"  
)
###########################################################################################



###########################################################################################
# Create our libraries		
IF( ENABLE_FILTERING_CORE )
  OPTION( SHARED_FILTERING_CORE
    "Build Filtering as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF( SHARED_FILTERING_CORE )
    SET( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_FILTERING_CORE )  
  ADD_LIBRARY( ccipdFilteringCore ${LIB_TYPE} ${FILTERING_CORE_FILES} )
ENDIF( ENABLE_FILTERING_CORE )
###########################################################################################



###########################################################################################
IF( ENABLE_FILTERING_CORE AND BUILD_TESTING )
  # Create our test programs
  IF( ENABLE_DATA_CORE )
    ADD_EXECUTABLE( TestFixedWindow ${TEST_FIXEDWINDOW_FILES} )
    ADD_EXECUTABLE( TestMovingWindow ${TEST_MOVINGWINDOW_FILES} )
    ADD_EXECUTABLE( TestConvolutionFilter ${TEST_CONVOLUTIONFILTER_FILES} )  
    ADD_EXECUTABLE( TestMultiplyFilter ${TEST_MULTIPLYFILTER_FILES}  )
    ADD_EXECUTABLE( TestCutOut ${TEST_CUT_OUT_FILES} )
    ADD_EXECUTABLE( CompareFilterTypes ${COMPARE_FILTER_FILES} )
  ENDIF( ENABLE_DATA_CORE )
    
  ADD_EXECUTABLE( TestCombineImages2VectorFilter ${TEST_COMBINEIMAGES2VECTOR_FILES} )
  ADD_EXECUTABLE( TestCombine3DDataInto4DData ${TEST_COMBINE3DDATAINTO4DDATA_FILES} )
  ADD_EXECUTABLE( TestDiscreteGaussianDisplacementFieldFilter ${TEST_DISCRETEGAUSSIANDISPLACEMENTFIELDFILTER_FILTERING_FILES} )
  ADD_EXECUTABLE( TestGradientRecursiveGaussianImageFilter ${CALCULATE_GRADIENT_FILES} )
  ADD_EXECUTABLE( TestHaralickFilter ${TEST_HARALICK_FILTERING_FILES} )
  ADD_EXECUTABLE( TestHaralickFilter2 ${TEST_HARALICK2_FILTERING_FILES} )
  ADD_EXECUTABLE( TestHaralickFilter3D ${TEST_HARALICK3D_FILTERING_FILES} )
  ADD_EXECUTABLE( TestGaborFilter ${TEST_GABOR_FILTERING_FILES} )
  ADD_EXECUTABLE( TestHistogram ${TEST_HISTOGRAM_FILES} )
ENDIF( ENABLE_FILTERING_CORE AND BUILD_TESTING )
###########################################################################################



###########################################################################################
IF( ENABLE_FILTERING_CORE )

  SET( FILTERING_CORE_LIBS
    "ccipdFilteringCore"
    ${ITK_LIBRARIES}
  )
  CCIPD_REMOVE_DUPLICATE_LIBS( FILTERING_CORE_LIBS ${FILTERING_CORE_LIBS} )
  
  TARGET_LINK_LIBRARIES( ccipdFilteringCore ${FILTERING_CORE_LIBS} )
ENDIF( ENABLE_FILTERING_CORE )

IF( ENABLE_FILTERING_CORE AND BUILD_TESTING )

  IF( ENABLE_DATA_CORE )
  
	CCIPD_REMOVE_DUPLICATE_LIBS( DATA_CORE_LIBS ${DATA_CORE_LIBS} )
  
  
    TARGET_LINK_LIBRARIES( TestFixedWindow                           ${DATA_CORE_LIBS} )
    TARGET_LINK_LIBRARIES( TestMovingWindow                          ${DATA_CORE_LIBS} )
    TARGET_LINK_LIBRARIES( TestConvolutionFilter                     ${DATA_CORE_LIBS} )
    TARGET_LINK_LIBRARIES( TestMultiplyFilter                        ${DATA_CORE_LIBS} )
    TARGET_LINK_LIBRARIES( TestCutOut                                ${DATA_CORE_LIBS} )
    TARGET_LINK_LIBRARIES( CompareFilterTypes                        ${DATA_CORE_LIBS} )
  ENDIF( ENABLE_DATA_CORE )

  CCIPD_REMOVE_DUPLICATE_LIBS( ITK_LIBRARIES ${ITK_LIBRARIES} )
  
  TARGET_LINK_LIBRARIES( TestCombineImages2VectorFilter              
    ${ITK_LIBRARIES}
    ${DATA_CORE_LIBS}  )
  TARGET_LINK_LIBRARIES( TestCombine3DDataInto4DData              
    ${ITK_LIBRARIES}
    ${DATA_CORE_LIBS}  )
  TARGET_LINK_LIBRARIES( TestDiscreteGaussianDisplacementFieldFilter ${ITK_LIBRARIES}  )
  TARGET_LINK_LIBRARIES( TestGradientRecursiveGaussianImageFilter    ${ITK_LIBRARIES}  )
  TARGET_LINK_LIBRARIES( TestHaralickFilter                          ${ITK_LIBRARIES}  )
  TARGET_LINK_LIBRARIES( TestHaralickFilter2                         ${ITK_LIBRARIES}  )
  TARGET_LINK_LIBRARIES( TestHaralickFilter3D                        ${ITK_LIBRARIES}  )
  TARGET_LINK_LIBRARIES( TestGaborFilter                             ${ITK_LIBRARIES}  )
  TARGET_LINK_LIBRARIES( TestHistogram                               ${ITK_LIBRARIES}  )
ENDIF( ENABLE_FILTERING_CORE AND BUILD_TESTING)
###########################################################################################


MESSAGE(STATUS "Building Filtering core library: " ${ENABLE_FILTERING_CORE} )


###########################################################################################


