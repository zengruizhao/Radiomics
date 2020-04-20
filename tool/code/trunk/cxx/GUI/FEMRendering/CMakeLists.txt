


###########################################################################################

IF( ENABLE_GUI_FEM_RENDERING AND NOT ENABLE_GUI_CORE )
    MESSAGE( "GUI FEM Rendering library requires " )
	MESSAGE( "the GUI core library. Disabling "    )
    MESSAGE( "the GUI FEM Rendering library."      )
    SET( ENABLE_GUI_FEM_RENDERING FALSE )
ENDIF()

IF( ENABLE_GUI_FEM_RENDERING AND NOT ENABLE_REGISTRATION_FEM )
    MESSAGE( "GUI FEM Rendering library requires the " )
	MESSAGE( "registration FEM library. Disabling "    )
    MESSAGE( "the GUI FEM Rendering library."          )
    SET( ENABLE_GUI_FEM_RENDERING FALSE )
ENDIF()
###########################################################################################



###########################################################################################
# Say where our files are
SET( GUI_FEM_RENDERING_INCLUDE_DIR "${GUI_FEM_RENDERING_DIR}/include" )
SET( GUI_FEM_RENDERING_SOURCE_DIR  "${GUI_FEM_RENDERING_DIR}/source"  )
SET( GUI_FEM_RENDERING_TEST_DIR    "${GUI_FEM_RENDERING_DIR}/test"    )
SET( GUI_FEM_RENDERING_UI_DIR      "${GUI_FEM_RENDERING_DIR}/ui"      )

SET( GUI_INCLUDE_DIRS
  ${GUI_INCLUDE_DIRS}
  "${GUI_FEM_RENDERING_INCLUDE_DIR}"
  )
###########################################################################################



###########################################################################################
# List our files
SET( TEST_GUI_FEM_RENDERING_FILES
  "${GUI_FEM_RENDERING_TEST_DIR}/TestFEMRendering.cxx" )
SET( GUI_FEM_RENDERING_FILES
  "${GUI_FEM_RENDERING_INCLUDE_DIR}/ccipdFEMRegistrationGUIObserver.h"
  "${GUI_FEM_RENDERING_INCLUDE_DIR}/ccipdFEMRenderer.h"
  "${GUI_FEM_RENDERING_INCLUDE_DIR}/ccipdFEMToActor.h"
  "${GUI_FEM_RENDERING_SOURCE_DIR}/ccipdFEMRegistrationGUIObserver.cxx"
  "${GUI_FEM_RENDERING_SOURCE_DIR}/ccipdFEMRenderer.cxx"
  "${GUI_FEM_RENDERING_SOURCE_DIR}/ccipdFEMToActor.cxx"
)

# These are Qt specific
IF( BUILD_QT_PROJECTS )
  SET( GUI_FEM_RENDERING_FILES
    ${GUI_FEM_RENDERING_FILES}
    "${GUI_FEM_RENDERING_INCLUDE_DIR}/ccipdFEMMaterialsDialog.h"
    "${GUI_FEM_RENDERING_INCLUDE_DIR}/ccipdFEMTransformDialog.h"
    "${GUI_FEM_RENDERING_INCLUDE_DIR}/ccipdFEMTransformOptionsGUI.h"
    "${GUI_FEM_RENDERING_SOURCE_DIR}/ccipdFEMMaterialsDialog.cxx"
    "${GUI_FEM_RENDERING_SOURCE_DIR}/ccipdFEMTransformDialog.cxx"
    "${GUI_FEM_RENDERING_SOURCE_DIR}/ccipdFEMTransformOptionsGUI.cxx"
  )  
  SET( GUI_FEM_RENDERING_UI_FILES
    "${GUI_FEM_RENDERING_UI_DIR}/ccipdFEMMaterialsDialog.ui"
    "${GUI_FEM_RENDERING_UI_DIR}/ccipdFEMTransformDialog.ui"
  )
  SET( GUI_FEM_RENDERING_MOC_FILES
    "${GUI_FEM_RENDERING_INCLUDE_DIR}/ccipdFEMMaterialsDialog.h"
    "${GUI_FEM_RENDERING_INCLUDE_DIR}/ccipdFEMTransformDialog.h"
  )
ENDIF( BUILD_QT_PROJECTS )
###########################################################################################



###########################################################################################
# This is needed for Qt to configure our C++ code.
# http://bit.ly/IL0yRT 
IF( ENABLE_GUI_FEM_RENDERING AND BUILD_QT_PROJECTS )
  
	IF(Qt5Widgets_FOUND)
		QT5_WRAP_UI ( GUI_FEM_RENDERING_UI_RESULT ${GUI_FEM_RENDERING_UI_FILES}  )
		QT5_WRAP_CPP( GUI_FEM_RENDERING_MOC_RESULT ${GUI_FEM_RENDERING_MOC_FILES})
	ELSE(Qt5Widgets_FOUND)
		QT4_WRAP_UI ( GUI_FEM_RENDERING_UI_RESULT ${GUI_FEM_RENDERING_UI_FILES}  )
		QT4_WRAP_CPP( GUI_FEM_RENDERING_MOC_RESULT ${GUI_FEM_RENDERING_MOC_FILES})  
	ENDIF(Qt5Widgets_FOUND)

  SOURCE_GROUP( "Resources" FILES ${GUI_FEM_RENDERING_UI_FILES} )
  SOURCE_GROUP( "Generated" FILES
    ${GUI_FEM_RENDERING_UI_RESULT}
    ${GUI_FEM_RENDERING_MOC_RESULT}
  )
ENDIF()
###########################################################################################



###########################################################################################
# Create our libraries
IF( ENABLE_GUI_FEM_RENDERING )
  OPTION( SHARED_GUI_FEM_RENDERING
    "Build GUI as a shared (TRUE/checked) or static (FALSE/unchecked)" OFF )
  IF( SHARED_GUI_FEM_RENDERING )
    SET ( LIB_TYPE SHARED )
  ELSE()
    SET( LIB_TYPE STATIC )
  ENDIF( SHARED_GUI_FEM_RENDERING )  
  ADD_LIBRARY( ccipdGUIFEMRendering ${LIB_TYPE}
    ${GUI_FEM_RENDERING_FILES}
    ${GUI_FEM_RENDERING_UI_RESULT}
    ${GUI_FEM_RENDERING_MOC_RESULT} )
ENDIF( ENABLE_GUI_FEM_RENDERING )
###########################################################################################



###########################################################################################
# Create our test programs
IF( ENABLE_GUI_FEM_RENDERING AND BUILD_TESTING )
  ADD_EXECUTABLE( TestFEMRendering ${TEST_GUI_FEM_RENDERING_FILES} )
ENDIF( ENABLE_GUI_FEM_RENDERING AND BUILD_TESTING )
###########################################################################################



###########################################################################################
# Link our libraries

IF( ENABLE_GUI_FEM_RENDERING )

  SET( GUI_FEM_RENDERING_LIBS
    "ccipdGUIFEMRendering"
    ${REGISTRATION_FEM_LIBS}
    ${GUI_CORE_LIBS}
  )

  CCIPD_REMOVE_DUPLICATE_LIBS( GUI_FEM_RENDERING_LIBS ${GUI_FEM_RENDERING_LIBS} )

  TARGET_LINK_LIBRARIES( ccipdGUIFEMRendering ${GUI_FEM_RENDERING_LIBS} )
ENDIF( ENABLE_GUI_FEM_RENDERING )

IF( ENABLE_GUI_FEM_RENDERING AND BUILD_TESTING )
  TARGET_LINK_LIBRARIES( TestFEMRendering ${GUI_FEM_RENDERING_LIBS} )
ENDIF( ENABLE_GUI_FEM_RENDERING AND BUILD_TESTING )
###########################################################################################



###########################################################################################
IF(ENABLE_GUI_FEM_RENDERING)
MESSAGE (STATUS "Building FEM Rendering library: " ${ENABLE_GUI_FEM_RENDERING} )
ELSE()
MESSAGE ( "Building FEM Rendering library: " ${ENABLE_GUI_FEM_RENDERING} )
ENDIF()
###########################################################################################

