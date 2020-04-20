IF(ENABLE_GUI_PROSTACAD)
	IF( NOT ENABLE_GUI_CORE )
  		MESSAGE( "ProstaCAD requires the GUI Core library. Disabling ProstaCAD." )
	ENDIF()
	
	IF( NOT ENABLE_CLASSIFICATION_CORE )
  		MESSAGE( "ProstaCAD requires the Classification Core library. Disabling ProstaCAD." )
	ENDIF()

	IF( NOT BUILD_QT_PROJECTS )
		MESSAGE( "ProstaCAD requires Qt. Disabling ProstaCAD." )
	ELSE()
		#IF( NOT BUILD_QXT_PROJECTS )
		#	MESSAGE( "ProstaCAD requires Qxt. Disabling ProstaCAD." )
		#ENDIF()
		IF( NOT BUILD_QWT_PROJECTS )
			MESSAGE( "ProstaCAD requires Qwt. Disabling ProstaCAD." )
		ENDIF()
	ENDIF()

	IF(NOT ENABLE_GUI_CORE OR NOT ENABLE_CLASSIFICATION_CORE OR NOT BUILD_QT_PROJECTS OR NOT BUILD_QWT_PROJECTS
          # OR NOT BUILD_QXT_PROJECTS  
          )
		SET( ENABLE_GUI_PROSTACAD FALSE )
	ENDIF()
ENDIF()

IF(NOT ENABLE_GUI_PROSTACAD)
	RETURN()
ENDIF()

###########################################################################################

###########################################################################################
# Include the submodules
SET( GUI_PROSTACAD_INCLUDE_DIR  "${GUI_PROSTACAD_DIR}/include"  )
SET( GUI_PROSTACAD_SOURCE_DIR   "${GUI_PROSTACAD_DIR}/source"   )
SET( GUI_PROSTACAD_PROJECTS_DIR "${GUI_PROSTACAD_DIR}/projects" )
SET( GUI_PROSTACAD_UI_DIR       "${GUI_PROSTACAD_DIR}/ui"       )

SET( GUI_INCLUDE_DIRS ${GUI_INCLUDE_DIRS} "${GUI_PROSTACAD_INCLUDE_DIR}" )
###########################################################################################



###########################################################################################
# List our files
SET( GUI_PROSTACAD_FILES
  "${GUI_PROSTACAD_INCLUDE_DIR}/ccipdProstaCADMainWindow.h"
  "${GUI_PROSTACAD_SOURCE_DIR}/ccipdProstaCADMainWindow.cxx"
  "${GUI_PROSTACAD_PROJECTS_DIR}/ccipdProstaCADMain.cxx"
)
SET( GUI_PROSTACAD_MOC_FILES
  "${GUI_PROSTACAD_INCLUDE_DIR}/ccipdProstaCADMainWindow.h"
)
SET( GUI_PROSTACAD_UI_FILES
  "${GUI_PROSTACAD_UI_DIR}/ccipdProstaCADMainWindow.ui"
)
###########################################################################################



###########################################################################################
# This is needed for Qt to configure our C++ code.
IF( ENABLE_GUI_PROSTACAD )

	IF(Qt5Widgets_FOUND)
		QT5_WRAP_UI ( GUI_PROSTACAD_UI_RESULT  ${GUI_PROSTACAD_UI_FILES}  )
		QT5_WRAP_CPP( GUI_PROSTACAD_MOC_RESULT ${GUI_PROSTACAD_MOC_FILES} )
	ELSE(Qt5Widgets_FOUND)
		QT4_WRAP_UI ( GUI_PROSTACAD_UI_RESULT  ${GUI_PROSTACAD_UI_FILES}  )
		QT4_WRAP_CPP( GUI_PROSTACAD_MOC_RESULT ${GUI_PROSTACAD_MOC_FILES} )
	ENDIF(Qt5Widgets_FOUND)	
	
  SOURCE_GROUP( "Resources" FILES ${GUI_PROSTACAD_UI_FILES} )
  SOURCE_GROUP( "Generated" FILES ${GUI_PROSTACAD_UI_RESULT} ${GUI_PROSTACAD_MOC_RESULT} )
  
###########################################################################################



###########################################################################################
# Create our executables
  ADD_EXECUTABLE( ProstaCADMain
    ${GUI_PROSTACAD_FILES}
    ${GUI_PROSTACAD_UI_RESULT}
    ${GUI_PROSTACAD_MOC_RESULT}
  )
###########################################################################################



###########################################################################################

  SET( PROSTACAD_MAIN_LIBS
	${GUI_CORE_LIBS}
	${CLASSIFICATION_CORE_LIBS}
	${CLASSIFICATION_VALIDATION_LIBS}
	${QT_LIBRARIES}
	${QXT_LIBRARIES}
	${QWT_LIBRARY}
    )
	
	CCIPD_REMOVE_DUPLICATE_LIBS( PROSTACAD_MAIN_LIBS ${PROSTACAD_MAIN_LIBS} )
	
  TARGET_LINK_LIBRARIES( ProstaCADMain  ${PROSTACAD_MAIN_LIBS})
  
ENDIF( ENABLE_GUI_PROSTACAD )
###########################################################################################



###########################################################################################
MESSAGE ( "Building ProstaCAD: " ${ENABLE_GUI_PROSTACAD} )
###########################################################################################

