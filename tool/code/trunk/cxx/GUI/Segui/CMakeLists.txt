


###########################################################################################

IF( ENABLE_GUI_SEGUI AND NOT ENABLE_GUI_RENDER_MFA )
  MESSAGE( "Segui program requires "               )
  MESSAGE( "the MFA rendering library. Disabling " )
  MESSAGE( "the Segui program."                    )
  SET( ENABLE_GUI_SEGUI FALSE )
ENDIF()

###########################################################################################



###########################################################################################
# Say where our files are
SET( GUI_SEGUI_INCLUDE_DIR "${GUI_SEGUI_DIR}/include" )
SET( GUI_SEGUI_SOURCE_DIR  "${GUI_SEGUI_DIR}/source"  )
SET( GUI_SEGUI_UI_DIR      "${GUI_SEGUI_DIR}/ui"      )

SET( GUI_INCLUDE_DIRS
  ${GUI_INCLUDE_DIRS}
  "${GUI_SEGUI_INCLUDE_DIR}"
  )
###########################################################################################



###########################################################################################
# List our files
SET( GUI_SEGUI_FILES
  "${GUI_SEGUI_INCLUDE_DIR}/ccipdSeguiWindow.h"
  "${GUI_SEGUI_SOURCE_DIR}/ccipdSeguiWindow.cxx"
)

# These are Qt specific
IF( BUILD_QT_PROJECTS )
  SET( GUI_SEGUI_FILES
    ${GUI_SEGUI_FILES}
    "${GUI_SEGUI_INCLUDE_DIR}/ccipdSeguiWindow.h"
    "${GUI_SEGUI_SOURCE_DIR}/ccipdSeguiWindow.cxx"
    "${GUI_SEGUI_SOURCE_DIR}/ccipdSeguiMain.cxx"
  )  
  SET( GUI_SEGUI_UI_FILES
    "${GUI_SEGUI_UI_DIR}/ccipdSeguiWindow.ui"
  )
  SET( GUI_SEGUI_MOC_FILES
    "${GUI_SEGUI_INCLUDE_DIR}/ccipdSeguiWindow.h"
  )
ENDIF( BUILD_QT_PROJECTS )
###########################################################################################



###########################################################################################
# This is needed for Qt to configure our C++ code.
# http://bit.ly/IL0yRT 
IF( ENABLE_GUI_SEGUI AND BUILD_QT_PROJECTS )

  	IF(Qt5Widgets_FOUND)
		QT5_WRAP_UI (GUI_SEGUI_UI_RESULT ${GUI_SEGUI_UI_FILES}  )
		QT5_WRAP_CPP(GUI_SEGUI_MOC_RESULT ${GUI_SEGUI_MOC_FILES})
	ELSE(Qt5Widgets_FOUND)
		QT4_WRAP_UI (GUI_SEGUI_UI_RESULT ${GUI_SEGUI_UI_FILES}  )
		QT4_WRAP_CPP(GUI_SEGUI_MOC_RESULT ${GUI_SEGUI_MOC_FILES})
	ENDIF(Qt5Widgets_FOUND)	
  
  SET( GUI_SEGUI_FILES
    ${GUI_SEGUI_FILES}
    ${GUI_SEGUI_UI_RESULT}
    ${GUI_SEGUI_MOC_RESULT}
  )
    
  SOURCE_GROUP( "Resources" FILES ${GUI_SEGUI_UI_FILES} )
  SOURCE_GROUP( "Generated" FILES
    ${GUI_SEGUI_UI_RESULT}
    ${GUI_SEGUI_MOC_RESULT}
  )
ENDIF()
###########################################################################################



###########################################################################################
# Link our libraries

IF( ENABLE_GUI_SEGUI )

  SET( GUI_SEGUI_LIBS
    ${GUI_RENDER_MFA_LIBS}
  )

  CCIPD_REMOVE_DUPLICATE_LIBS( GUI_SEGUI_LIBS ${GUI_SEGUI_LIBS} )
  
  ADD_EXECUTABLE( SeguiMain ${GUI_SEGUI_FILES} )
  TARGET_LINK_LIBRARIES( SeguiMain ${GUI_SEGUI_LIBS} )
  
ENDIF( ENABLE_GUI_SEGUI )

###########################################################################################



###########################################################################################
IF(ENABLE_GUI_SEGUI)
MESSAGE (STATUS "Building Segui program: " ${ENABLE_GUI_SEGUI} )
ELSE()
MESSAGE ( "Building Segui program: " ${ENABLE_GUI_SEGUI} )
ENDIF()
###########################################################################################
