


###########################################################################################

IF( ENABLE_GUI_PROSTALIGN AND NOT BUILD_QT_PROJECTS )
  MESSAGE( "Prostalign requires Qt. " )
  MESSAGE( "Disabling prostalign."      )
  SET( ENABLE_GUI_PROSTALIGN FALSE )
ENDIF()

# Note that FEMRendering requires GUI, so that dependency is already met.
IF( ENABLE_GUI_PROSTALIGN AND NOT ENABLE_GUI_FEM_RENDERING )
  MESSAGE( "Prostalign requires the GUI FEM "         )
  MESSAGE( "rendering library. Disabling prostalign." )
  SET( ENABLE_GUI_PROSTALIGN FALSE )
ENDIF( ENABLE_GUI_PROSTALIGN AND NOT ENABLE_GUI_FEM_RENDERING )

###########################################################################################



###########################################################################################
# Include the submodules
SET( GUI_PROSTALIGN_INCLUDE_DIR  "${GUI_PROSTALIGN_DIR}/include"  )
SET( GUI_PROSTALIGN_SOURCE_DIR   "${GUI_PROSTALIGN_DIR}/source"   )
SET( GUI_PROSTALIGN_PROJECTS_DIR "${GUI_PROSTALIGN_DIR}/projects" )
SET( GUI_PROSTALIGN_UI_DIR       "${GUI_PROSTALIGN_DIR}/ui"       )

SET( GUI_INCLUDE_DIRS
  ${GUI_INCLUDE_DIRS}
  "${GUI_PROSTALIGN_INCLUDE_DIR}" )
###########################################################################################



###########################################################################################
# List our files
SET( GUI_PROSTALIGN_FILES
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignChooser.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignFEM.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignResults.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignResultsOptions.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignTPS.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignTPSOptions.h"
  "${GUI_PROSTALIGN_SOURCE_DIR}/ccipdProstalignChooser.cxx"
  "${GUI_PROSTALIGN_SOURCE_DIR}/ccipdProstalignFEM.cxx"
  "${GUI_PROSTALIGN_SOURCE_DIR}/ccipdProstalignResults.cxx"
  "${GUI_PROSTALIGN_SOURCE_DIR}/ccipdProstalignResultsOptions.cxx"
  "${GUI_PROSTALIGN_SOURCE_DIR}/ccipdProstalignTPS.cxx"
  "${GUI_PROSTALIGN_SOURCE_DIR}/ccipdProstalignTPSOptions.cxx"
  "${GUI_PROSTALIGN_PROJECTS_DIR}/ccipdProstalign.cxx"
)
SET( GUI_PROSTALIGN_MOC_FILES
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignChooser.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignFEM.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignTPS.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignTPSOptions.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignResults.h"
  "${GUI_PROSTALIGN_INCLUDE_DIR}/ccipdProstalignResultsOptions.h"
)
SET( GUI_PROSTALIGN_UI_FILES
  "${GUI_PROSTALIGN_UI_DIR}/ccipdProstalignChooser.ui"
  "${GUI_PROSTALIGN_UI_DIR}/ccipdProstalignFEM.ui"
  "${GUI_PROSTALIGN_UI_DIR}/ccipdProstalignResults.ui"
  "${GUI_PROSTALIGN_UI_DIR}/ccipdProstalignResultsOptions.ui"
  "${GUI_PROSTALIGN_UI_DIR}/ccipdProstalignTPS.ui"
  "${GUI_PROSTALIGN_UI_DIR}/ccipdProstalignTPSOptions.ui"
)
###########################################################################################



###########################################################################################
# This is needed for Qt to configure our C++ code.
# http://bit.ly/IL0yRT 
IF( ENABLE_GUI_PROSTALIGN AND BUILD_QT_PROJECTS )
  
	IF(Qt5Widgets_FOUND)
		QT5_WRAP_UI ( GUI_PROSTALIGN_UI_RESULT  ${GUI_PROSTALIGN_UI_FILES}  )
		QT5_WRAP_CPP( GUI_PROSTALIGN_MOC_RESULT ${GUI_PROSTALIGN_MOC_FILES} )
	ELSE(Qt5Widgets_FOUND)
		QT4_WRAP_UI ( GUI_PROSTALIGN_UI_RESULT  ${GUI_PROSTALIGN_UI_FILES}  )
		QT4_WRAP_CPP( GUI_PROSTALIGN_MOC_RESULT ${GUI_PROSTALIGN_MOC_FILES} )
	ENDIF(Qt5Widgets_FOUND)	
	
  SOURCE_GROUP( "Resources" FILES ${GUI_PROSTALIGN_UI_FILES} )
  SOURCE_GROUP( "Generated" FILES
    ${GUI_PROSTALIGN_UI_RESULT}
    ${GUI_PROSTALIGN_MOC_RESULT}
  )
ENDIF()
###########################################################################################



###########################################################################################
# Create our test programs
IF( ENABLE_GUI_PROSTALIGN )
  ADD_EXECUTABLE( ProstalignMain
    ${GUI_PROSTALIGN_FILES}
    ${GUI_PROSTALIGN_UI_RESULT}
    ${GUI_PROSTALIGN_MOC_RESULT}
  )
ENDIF( ENABLE_GUI_PROSTALIGN )
###########################################################################################



###########################################################################################
# Link our libraries
IF( ENABLE_GUI_PROSTALIGN )

  SET( PROSTALIGN_MAIN_LIBS
    ${GUI_FEM_RENDERING_LIBS} )
    
  CCIPD_REMOVE_DUPLICATE_LIBS( PROSTALIGN_MAIN_LIBS ${PROSTALIGN_MAIN_LIBS} )

  TARGET_LINK_LIBRARIES( ProstalignMain ${PROSTALIGN_MAIN_LIBS} )
  
ENDIF( ENABLE_GUI_PROSTALIGN )
###########################################################################################



###########################################################################################
IF(ENABLE_GUI_PROSTALIGN)
MESSAGE (STATUS "Building Prostalign: " ${ENABLE_GUI_PROSTALIGN} )
ELSE()
MESSAGE ( "Building Prostalign: " ${ENABLE_GUI_PROSTALIGN} )
ENDIF()
###########################################################################################

