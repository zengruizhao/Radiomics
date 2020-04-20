


###########################################################################################

IF( ENABLE_GUI_RENDER_MFA AND NOT ENABLE_GUI_CORE )
  MESSAGE( "GUI MFA Rendering library requires " )
  MESSAGE( "the GUI core library. Disabling "    )
  MESSAGE( "the GUI FEM Rendering library."      )
  SET( ENABLE_GUI_RENDER_MFA FALSE )
ENDIF()

IF( ENABLE_GUI_RENDER_MFA AND NOT ENABLE_SEGMENTATION_MFA )
  MESSAGE( "GUI FEM Rendering library requires the " )
  MESSAGE( "segmentation MFA library. Disabling "    )
  MESSAGE( "the GUI FEM Rendering library."          )
  SET( ENABLE_GUI_RENDER_MFA FALSE )
ENDIF()

IF( ENABLE_GUI_RENDER_MFA AND NOT BUILD_QT_PROJECTS )
  MESSAGE( "GUI MFA Rendering library requires " )
  MESSAGE( "the QT library. Disabling "          )
  MESSAGE( "the GUI FEM Rendering library."      )
  SET( ENABLE_GUI_RENDER_MFA FALSE )
ENDIF()

###########################################################################################



###########################################################################################
# Say where our files are
SET( GUI_RENDER_MFA_INCLUDE_DIR "${GUI_RENDER_MFA_DIR}/include" )
SET( GUI_RENDER_MFA_SOURCE_DIR  "${GUI_RENDER_MFA_DIR}/source"  )
SET( GUI_RENDER_MFA_TEST_DIR    "${GUI_RENDER_MFA_DIR}/test"    )
SET( GUI_RENDER_MFA_UI_DIR      "${GUI_RENDER_MFA_DIR}/ui"      )

SET( GUI_INCLUDE_DIRS
  ${GUI_INCLUDE_DIRS}
  "${GUI_RENDER_MFA_INCLUDE_DIR}"
  )
###########################################################################################



###########################################################################################
# List our files
SET( TEST_GUI_RENDER_MFA_FILES
  "${GUI_RENDER_MFA_TEST_DIR}/TestMFARendering.cxx" )

SET( GUI_RENDER_MFA_FILES
)

# These are Qt specific
IF( BUILD_QT_PROJECTS )
  SET( GUI_RENDER_MFA_FILES
    ${GUI_RENDER_MFA_FILES}
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFAAlgorithmTableWidget.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFAAlgorithmTableWidgetAdd.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFAAlgorithmTableWidgetGet.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFAAlgorithmTableWidgetSet.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFAInterpolatorDelegate.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFARegularizerTypeDelegate.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFARenderingRefresh.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFASegmenterOptionsGUI.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFASegmenterOptionsDialog.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFATransformDelegate.h"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFAAlgorithmTableWidget.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFAAlgorithmTableWidgetAdd.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFAAlgorithmTableWidgetGet.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFAAlgorithmTableWidgetSet.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFAInterpolatorDelegate.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFAModelLoadGUI.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFARegularizerTypeDelegate.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFARenderingRefresh.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFASegmenterLoadGUI.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFASegmenterOptionsGUI.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFASegmenterOptionsDialog.cxx"
    "${GUI_RENDER_MFA_SOURCE_DIR}/ccipdMFATransformDelegate.cxx"
  )  
  SET( GUI_RENDER_MFA_UI_FILES
    "${GUI_RENDER_MFA_UI_DIR}/ccipdMFASegmenterOptionsDialog.ui"
  )
  SET( GUI_RENDER_MFA_MOC_FILES
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFAAlgorithmTableWidget.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFAInterpolatorDelegate.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFARegularizerTypeDelegate.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFASegmenterOptionsDialog.h"
    "${GUI_RENDER_MFA_INCLUDE_DIR}/ccipdMFATransformDelegate.h"
  )
ENDIF( BUILD_QT_PROJECTS )
###########################################################################################



###########################################################################################
# This is needed for Qt to configure our C++ code.
# http://bit.ly/IL0yRT 
IF( ENABLE_GUI_RENDER_MFA AND BUILD_QT_PROJECTS )

	IF(Qt5Widgets_FOUND)
		QT5_WRAP_UI ( GUI_RENDER_MFA_UI_RESULT  ${GUI_RENDER_MFA_UI_FILES}  )
		QT5_WRAP_CPP( GUI_RENDER_MFA_MOC_RESULT ${GUI_RENDER_MFA_MOC_FILES} )
	ELSE(Qt5Widgets_FOUND)
		QT4_WRAP_UI ( GUI_RENDER_MFA_UI_RESULT  ${GUI_RENDER_MFA_UI_FILES}  )
		QT4_WRAP_CPP( GUI_RENDER_MFA_MOC_RESULT ${GUI_RENDER_MFA_MOC_FILES} )  
	ENDIF(Qt5Widgets_FOUND)	
	
  SET( GUI_RENDER_MFA_FILES
    ${GUI_RENDER_MFA_FILES} 
    ${GUI_RENDER_MFA_UI_RESULT}
    ${GUI_RENDER_MFA_MOC_RESULT}
  )
    
  SOURCE_GROUP( "Resources" FILES ${GUI_RENDER_MFA_UI_FILES} )
  SOURCE_GROUP( "Generated" FILES
    ${GUI_RENDER_MFA_UI_RESULT}
    ${GUI_RENDER_MFA_MOC_RESULT}
  )
ENDIF()
###########################################################################################



###########################################################################################
# Create our libraries
IF( ENABLE_GUI_RENDER_MFA )
  OPTION( SHARED_GUI_RENDER_MFA
    "Build MFA GUI as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF( SHARED_GUI_RENDER_MFA )
    SET ( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_GUI_RENDER_MFA )  
  ADD_LIBRARY( ccipdGUIRenderMFA ${LIB_TYPE} ${GUI_RENDER_MFA_FILES} )
ENDIF( ENABLE_GUI_RENDER_MFA )
###########################################################################################



###########################################################################################
# Create our test programs
IF( ENABLE_GUI_RENDER_MFA AND BUILD_TESTING )
  ADD_EXECUTABLE( TestRenderMFA ${TEST_GUI_RENDER_MFA_FILES} )
ENDIF( ENABLE_GUI_RENDER_MFA AND BUILD_TESTING )
###########################################################################################



###########################################################################################
# Link our libraries

IF( ENABLE_GUI_RENDER_MFA )

  SET( GUI_RENDER_MFA_LIBS
    "ccipdGUIRenderMFA"
    ${SEGMENTATION_MFA_LIBS}
    ${GUI_CORE_LIBS}
  )

  CCIPD_REMOVE_DUPLICATE_LIBS( GUI_RENDER_MFA_LIBS ${GUI_RENDER_MFA_LIBS} )

  TARGET_LINK_LIBRARIES( ccipdGUIRenderMFA ${GUI_RENDER_MFA_LIBS} )
  
ENDIF( ENABLE_GUI_RENDER_MFA )

IF( ENABLE_GUI_RENDER_MFA AND BUILD_TESTING )
  TARGET_LINK_LIBRARIES( TestRenderMFA ${GUI_RENDER_MFA_LIBS} )
ENDIF( ENABLE_GUI_RENDER_MFA AND BUILD_TESTING )
###########################################################################################



###########################################################################################
IF(ENABLE_GUI_RENDER_MFA)
MESSAGE (STATUS "Building MFA rendering library: " ${ENABLE_GUI_RENDER_MFA} )
ELSE()
MESSAGE ( "Building MFA rendering library: " ${ENABLE_GUI_RENDER_MFA} )
ENDIF()
###########################################################################################
