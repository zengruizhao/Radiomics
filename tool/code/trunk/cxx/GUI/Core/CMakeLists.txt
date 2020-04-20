
IF(ENABLE_GUI_CORE)
	IF(NOT ENABLE_COMMON_VTK_CONVERSION )
		MESSAGE( "GUI core requires building VTK Conversion.")
		SET( ENABLE_GUI_CORE FALSE )
	ENDIF()
	
	IF(NOT ENABLE_FILTERING_CORE )
		MESSAGE( "GUI core requires building Filtering Core.")
		SET( ENABLE_GUI_CORE FALSE )
	ENDIF()

	IF(NOT ENABLE_SEGMENTATION_CORE )
		MESSAGE( "GUI core requires building Segmentation Core.")
		SET( ENABLE_GUI_CORE FALSE )
	ENDIF()

	IF(NOT ENABLE_DATA_PATIENTS )
		MESSAGE( "GUI core required building Data Patients.")
		SET( ENABLE_GUI_CORE FALSE )
	ENDIF()
ENDIF()

IF(NOT ENABLE_GUI_CORE)
	MESSAGE( "Disabling GUI Core Library.")
	RETURN()
ENDIF()

###########################################################################################
# check for dependencies of some test programs
SET( ENABLE_QT_VTK_HELLO           FALSE )
SET( ENABLE_QT_UI                  FALSE )
SET( ENABLE_STATUS_MESSAGE_HANDLER FALSE )
IF(BUILD_TESTING AND BUILD_QT_PROJECTS)
    IF( BUILD_VTK_PROJECTS )
		SET( ENABLE_QT_VTK_HELLO TRUE )
		SET( ENABLE_QT_UI        TRUE )
    ENDIF( BUILD_VTK_PROJECTS )
		SET( ENABLE_STATUS_MESSAGE_HANDLER TRUE )
ENDIF(BUILD_TESTING AND BUILD_QT_PROJECTS)
###########################################################################################



###########################################################################################
# Say where our files are
SET( GUI_CORE_INCLUDE_DIR   "${GUI_CORE_DIR}/include"   )
SET( GUI_CORE_TEMPLATES_DIR "${GUI_CORE_DIR}/templates" )
SET( GUI_CORE_SOURCE_DIR    "${GUI_CORE_DIR}/source"    )
SET( GUI_CORE_TEST_DIR      "${GUI_CORE_DIR}/test"      )
SET( GUI_CORE_UI_DIR        "${GUI_CORE_DIR}/ui"        )

SET( GUI_INCLUDE_DIRS    ${GUI_INCLUDE_DIRS}
						"${GUI_CORE_INCLUDE_DIR}"
						${FILTERING_INCLUDE_DIRS}
						"${GUI_CORE_TEMPLATES_DIR}" 
	)
###########################################################################################




###########################################################################################
# List our files
SET( GUI_CORE_FILES
  "${GUI_CORE_INCLUDE_DIR}/ccipdAnimateWarpField.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdBlendImages.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdBoundingBoxToActor.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdCopySliderParameters.h"

  "${GUI_CORE_INCLUDE_DIR}/ccipdDisplayStatusMessage.h"  
  
  
  "${GUI_CORE_INCLUDE_DIR}/ccipdGUIGroups.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdImageToProp.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdIsosurfaceRendering.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdLandmarksToActor.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdLandmarkToCursor.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdMeshToActor.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdMouseInteractorStylePointPicker.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdPickPointsOnImage.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdPolyDataToArrows.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdPolyDataToSpheres.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdRegistrationGUIObserver.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdRenderPointPicker.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdUpdateVTKImageBlinds.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdUpdateVTKImageProp.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdUpdateVTKImageSlice.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdUpdateVTKImageWindowLevel.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdUpdateVTKPropTransparency.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdUpdateVTKPropColor.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdRenderITKImage.h"
  "${GUI_CORE_INCLUDE_DIR}/ccipdVTKUtils.h"
  "${GUI_CORE_TEMPLATES_DIR}/ccipdVTKUtils.hxx"
  "${GUI_CORE_TEMPLATES_DIR}/ccipdUpdateVTKImageProp.hxx"
  "${GUI_CORE_TEMPLATES_DIR}/ccipdRenderITKImage.hxx"
  "${GUI_CORE_TEMPLATES_DIR}/ccipdBlendImages.hxx"
  "${GUI_CORE_TEMPLATES_DIR}/ccipdImageToProp.hxx"
  "${GUI_CORE_TEMPLATES_DIR}/ccipdPickPointsOnImage.hxx"
  "${GUI_CORE_TEMPLATES_DIR}/ccipdRenderPointPicker.hxx"
  "${GUI_CORE_TEMPLATES_DIR}/ccipdUpdateVTKImageBlinds.hxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdAnimateWarpField.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdBlendImages.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdBoundingBoxToActor.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdCopySliderParameters.cxx"
  
   "${GUI_CORE_SOURCE_DIR}/ccipdDisplayStatusMessage.cxx" 
  
  "${GUI_CORE_SOURCE_DIR}/ccipdImageToProp.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdIsosurfaceRendering.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdLandmarksToActor.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdLandmarkToCursor.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdMeshToActor.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdMouseInteractorStylePointPicker.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdPickPointsOnImage.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdPolyDataToArrows.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdPolyDataToSpheres.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdRegistrationGUIObserver.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdRenderPointPicker.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdUpdateVTKImageBlinds.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdUpdateVTKImageProp.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdUpdateVTKImageSlice.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdUpdateVTKImageWindowLevel.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdUpdateVTKPropTransparency.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdUpdateVTKPropColor.cxx"
  "${GUI_CORE_SOURCE_DIR}/ccipdVTKUtils.cxx"
)
  
IF( BUILD_QT_PROJECTS )
  SET( GUI_CORE_FILES
    ${GUI_CORE_FILES}
    "${GUI_CORE_INCLUDE_DIR}/ccipdCheckBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdComboBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdGradientDescentDialog.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdGradientDescentOptionsGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdLoadGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdLoadRGBSliceGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdLoadVolumeGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdParticleSwarmDialog.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdParticleSwarmOptionsGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdPercentSpinBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdPickPointsDialog.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdPickPointsOnVolume.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSaveGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSaveImageGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSaveLandmarksGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSaveLinearTransformGUI.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSetDoubleSpinBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSetIntSpinBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSpinBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdUpdateWindowLevelSliders.h"
	"${GUI_CORE_INCLUDE_DIR}/ccipdUpdateSlicesSliders.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdUpdateTransparencySlider.h"
    
	"${GUI_CORE_TEMPLATES_DIR}/ccipdLoadGUI.hxx"
    "${GUI_CORE_TEMPLATES_DIR}/ccipdSaveImageGUI.hxx"
    "${GUI_CORE_TEMPLATES_DIR}/ccipdSpinBoxDelegate.hxx"
    
	"${GUI_CORE_SOURCE_DIR}/ccipdCheckBoxDelegate.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdComboBoxDelegate.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdDoubleSpinBoxDelegate.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdGradientDescentDialog.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdGradientDescentOptionsGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdIntSpinBoxDelegate.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdLoadPatientGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdLoadRGBSliceGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdLoadVolumeGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdLoadStudyGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdParticleSwarmDialog.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdParticleSwarmOptionsGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdPercentSpinBoxDelegate.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdPickPointsDialog.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdPickPointsOnVolume.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdSaveGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdSaveLandmarksGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdSaveLinearTransformGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdSaveRGBSliceGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdSaveImageGUI.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdSetDoubleSpinBoxDelegate.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdSetIntSpinBoxDelegate.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdStatusMessageHandler.cxx"
    "${GUI_CORE_SOURCE_DIR}/ccipdUpdateWindowLevelSliders.cxx"
	"${GUI_CORE_SOURCE_DIR}/ccipdUpdateSlicesSliders.cxx"
	"${GUI_CORE_SOURCE_DIR}/ccipdUpdateTransparencySlider.cxx"
  )
  SET( GUI_CORE_UI_FILES
    "${GUI_CORE_UI_DIR}/ccipdGradientDescentDialog.ui"
    "${GUI_CORE_UI_DIR}/ccipdParticleSwarmDialog.ui"
    "${GUI_CORE_UI_DIR}/ccipdPickPointsDialog.ui"
  )
  SET( GUI_CORE_MOC_FILES
  	"${GUI_CORE_INCLUDE_DIR}/ccipdStatusMessageHandler.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdCheckBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdComboBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdGradientDescentDialog.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdParticleSwarmDialog.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdPercentSpinBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdPickPointsDialog.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSetDoubleSpinBoxDelegate.h"
    "${GUI_CORE_INCLUDE_DIR}/ccipdSetIntSpinBoxDelegate.h"
  )
  
  SET( TEST_POINT_PICKER_3D_FILES "${GUI_CORE_TEST_DIR}/TestPointPicker3D.cxx"               )

  SET( TEST_STATUS_MESSAGE_HANDLER_FILES "${GUI_CORE_TEST_DIR}/TestStatusMessageHandler.cxx" )

  
ENDIF( BUILD_QT_PROJECTS )

SET( TEST_COMPARE_HISTOGRAMS_FILES
  "${GUI_CORE_TEST_DIR}/TestCompareHistograms.cxx" 
)

SET( TEST_LANDMARK_RENDERING_FILES
  "${GUI_CORE_TEST_DIR}/TestLandmarkRendering.cxx" 
)
SET( TEST_TRIANGULATION_RENDERING_FILES
  "${GUI_CORE_TEST_DIR}/TestTriangulationRendering.cxx" 
)
SET( TEST_LEVELSET_RENDERING_FILES
  "${GUI_CORE_TEST_DIR}/TestLevelsetRendering.cxx" 
)
SET( TEST_POINT_PICKER_2D_FILES
  "${GUI_CORE_TEST_DIR}/TestPointPicker2D.cxx" 
)

SET( TEST_QT_VTK_HELLO_FILES
  "${GUI_CORE_TEST_DIR}/TestQtVtkHelloWorld.cxx"
)
SET( TEST_QT_UI_FILES
  "${GUI_CORE_TEST_DIR}/TestQtUI.h"
  "${GUI_CORE_TEST_DIR}/TestQtUI.cxx"
)
SET( TEST_QT_UI_UI_FILES
  "${GUI_CORE_UI_DIR}/TestQtUI.ui"
)
SET( TEST_QT_UI_MOC_FILES
  "${GUI_CORE_TEST_DIR}/TestQtUI.h"
)
###########################################################################################



###########################################################################################

IF( BUILD_QT_PROJECTS )

  IF(Qt5Widgets_FOUND)
	QT5_WRAP_UI ( TEST_QT_UI_UI_RESULT  ${TEST_QT_UI_UI_FILES}  )
	QT5_WRAP_CPP( TEST_QT_UI_MOC_RESULT ${TEST_QT_UI_MOC_FILES} )
  ELSE(Qt5Widgets_FOUND)
    QT4_WRAP_UI ( TEST_QT_UI_UI_RESULT  ${TEST_QT_UI_UI_FILES}  )
    QT4_WRAP_CPP( TEST_QT_UI_MOC_RESULT ${TEST_QT_UI_MOC_FILES} )
  ENDIF(Qt5Widgets_FOUND)
  
  
  SET( TEST_QT_UI_FILES 
    ${TEST_QT_UI_FILES}
    ${TEST_QT_UI_UI_RESULT}
    ${TEST_QT_UI_MOC_RESULT}
  )
  
	IF(Qt5Widgets_FOUND)
		QT5_WRAP_UI ( GUI_CORE_UI_RESULT  ${GUI_CORE_UI_FILES} )
		QT5_WRAP_CPP( GUI_CORE_MOC_RESULT ${GUI_CORE_MOC_FILES})
	ELSE(Qt5Widgets_FOUND)
        QT4_WRAP_UI ( GUI_CORE_UI_RESULT  ${GUI_CORE_UI_FILES} )
		QT4_WRAP_CPP( GUI_CORE_MOC_RESULT ${GUI_CORE_MOC_FILES})
	ENDIF(Qt5Widgets_FOUND)
  

  SET( GUI_CORE_FILES
    ${GUI_CORE_FILES}
    ${GUI_CORE_UI_RESULT}
    ${GUI_CORE_MOC_RESULT} )
  
  SOURCE_GROUP( "Resources" FILES
    ${TEST_QT_UI_UI_FILES}
    ${GUI_CORE_UI_FILES}
  )
  SOURCE_GROUP( "Generated" FILES
    ${TEST_QT_UI_UI_RESULT}
    ${TEST_QT_UI_MOC_RESULT}
    ${GUI_CORE_UI_RESULT}
    ${GUI_CORE_MOC_RESULT}
  )
ENDIF( BUILD_QT_PROJECTS )
###########################################################################################



###########################################################################################
# Create our libraries
IF( ENABLE_GUI_CORE )
  OPTION ( SHARED_GUI_CORE
    "Build GUI as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF( SHARED_GUI_CORE )
    SET ( LIB_TYPE SHARED )
  ELSE()
    SET ( LIB_TYPE STATIC )
  ENDIF( SHARED_GUI_CORE )  
  ADD_LIBRARY( ccipdGUICore ${LIB_TYPE} ${GUI_CORE_FILES} )
ENDIF( ENABLE_GUI_CORE )
###########################################################################################



###########################################################################################
# Create our test programs
IF( BUILD_TESTING AND ENABLE_GUI_CORE )

  IF( ENABLE_DATA_CORE AND ENABLE_BIAS_CORRECTION_CORE )
    ADD_EXECUTABLE( TestPointPicker2D ${TEST_POINT_PICKER_2D_FILES} )
    
    IF(BUILD_QT_PROJECTS)
		ADD_EXECUTABLE( TestPointPicker3D ${TEST_POINT_PICKER_3D_FILES} )
    ENDIF()

  ENDIF()
  
  IF( ENABLE_SEGMENTATION_CORE )
    ADD_EXECUTABLE( TestLevelsetRendering      ${TEST_LEVELSET_RENDERING_FILES}      )
    IF ( ENABLE_DATA_CORE )  
      ADD_EXECUTABLE( TestLandmarkRendering      ${TEST_LANDMARK_RENDERING_FILES}      )
      ADD_EXECUTABLE( TestTriangulationRendering ${TEST_TRIANGULATION_RENDERING_FILES} )
    ENDIF (ENABLE_DATA_CORE )
  ENDIF( ENABLE_SEGMENTATION_CORE )
  
  IF( ENABLE_QT_VTK_HELLO )
    ADD_EXECUTABLE( TestQtVtkHelloWorld ${TEST_QT_VTK_HELLO_FILES} )
  ENDIF( ENABLE_QT_VTK_HELLO )

  IF( ENABLE_QT_UI )
    ADD_EXECUTABLE( TestQtUI ${TEST_QT_UI_FILES} )
  ENDIF( ENABLE_QT_UI )
  
	IF( ENABLE_STATUS_MESSAGE_HANDLER )
		ADD_EXECUTABLE( TestQtStatusMessageHandler ${TEST_STATUS_MESSAGE_HANDLER_FILES} )
	ENDIF( ENABLE_STATUS_MESSAGE_HANDLER )

  
ENDIF( BUILD_TESTING AND ENABLE_GUI_CORE )
###########################################################################################

	  IF( BUILD_TESTING AND BUILD_VTK_PROJECTS)
		ADD_EXECUTABLE( TestCompareHistograms ${TEST_COMPARE_HISTOGRAMS_FILES} )
	  ENDIF( BUILD_TESTING AND BUILD_VTK_PROJECTS)



###########################################################################################
# Link our libraries
IF( ENABLE_GUI_CORE )

  SET ( GUI_CORE_LIBS
    "ccipdGUICore"
    ${SEGMENTATION_CORE_LIBS}
	${FILTERING_CORE_LIBS}
    ${DATA_PATIENTS_LIBS}
    ${COMMON_VTK_CONVERSION_LIBS}
  )

  CCIPD_REMOVE_DUPLICATE_LIBS( GUI_CORE_LIBS ${GUI_CORE_LIBS} )

  TARGET_LINK_LIBRARIES( ccipdGUICore ${GUI_CORE_LIBS} )

  IF( BUILD_TESTING )
    IF( ENABLE_SEGMENTATION_CORE )
      TARGET_LINK_LIBRARIES( TestLevelsetRendering
         ${GUI_CORE_LIBS} )
      IF( ENABLE_DATA_CORE )
        TARGET_LINK_LIBRARIES( TestTriangulationRendering
           ${COMMON_VTK_CONVERSION_LIBS} ${SEGMENTATION_CORE_LIBS} ${DATA_CORE_LIBS} )
        TARGET_LINK_LIBRARIES( TestLandmarkRendering
           ${COMMON_VTK_CONVERSION_LIBS} ${SEGMENTATION_CORE_LIBS} ${DATA_CORE_LIBS} )   
      ENDIF( ENABLE_DATA_CORE )
    ENDIF()
    
    IF( ENABLE_QT_VTK_HELLO )
      TARGET_LINK_LIBRARIES( TestQtVtkHelloWorld ${QT_LIBRARIES} ${VTK_LIBRARIES} )
    ENDIF( ENABLE_QT_VTK_HELLO )
    
    IF( ENABLE_QT_UI )
      TARGET_LINK_LIBRARIES( TestQtUI ${QT_LIBRARIES} ${VTK_LIBRARIES} )
	    
    ENDIF( ENABLE_QT_UI )
    
	IF(ENABLE_STATUS_MESSAGE_HANDLER)
		TARGET_LINK_LIBRARIES( TestQtStatusMessageHandler ${GUI_CORE_LIBS} ${QT_LIBRARIES})
	ENDIF(ENABLE_STATUS_MESSAGE_HANDLER)
	
  ENDIF( BUILD_TESTING )
  
  IF( BUILD_TESTING )
    IF( ENABLE_DATA_CORE AND ENABLE_BIAS_CORRECTION_CORE )
      TARGET_LINK_LIBRARIES( TestPointPicker2D
         ${VTK_LIBRARIES}
         ${DATA_CORE_LIBS}
         ${GUI_CORE_LIBS}
         ${BIAS_CORRECTION_CORE_LIBS} )
         
      IF( BUILD_QT_PROJECTS )
        TARGET_LINK_LIBRARIES( TestPointPicker3D
           ${VTK_LIBRARIES}
           ${SEGMENTATION_CORE_LIBS}
           ${GUI_CORE_LIBS}
           ${BIAS_CORRECTION_CORE_LIBS}
           ${QT_LIBRARIES} )
      
      ENDIF( BUILD_QT_PROJECTS )
      
    ENDIF( ENABLE_DATA_CORE AND ENABLE_BIAS_CORRECTION_CORE )
	
	IF(BUILD_VTK_PROJECTS)
		TARGET_LINK_LIBRARIES( TestCompareHistograms ${GUI_CORE_LIBS} ${VTK_LIBRARIES} ${ITK_LIBRARIES})
	ENDIF(BUILD_VTK_PROJECTS)

  ENDIF( BUILD_TESTING )

ENDIF( ENABLE_GUI_CORE )

###########################################################################################

###########################################################################################
IF(ENABLE_GUI_CORE)
MESSAGE (STATUS "Building GUI core library: " ${ENABLE_GUI_CORE} )
ELSE()
MESSAGE ( "Building GUI core library: " ${ENABLE_GUI_CORE} )
ENDIF()
###########################################################################################
