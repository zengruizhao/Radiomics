
###########################################################################################
IF(ENABLE_GUI_CAPVIEW)
	IF( NOT ENABLE_GUI_CORE )
  		MESSAGE( "CaPView requires the GUI Core library. Disabling CaPView." )
	ENDIF()
	IF( NOT BUILD_QT_PROJECTS )
		MESSAGE( "CaPView requires Qt. Disabling CaPView." )
	ELSE()
		IF( NOT BUILD_QXT_PROJECTS )
			MESSAGE( "CaPView requires Qxt. Disabling CaPView." )
		ENDIF()
	ENDIF()

	IF(NOT ENABLE_GUI_CORE OR NOT BUILD_QT_PROJECTS OR NOT BUILD_QXT_PROJECTS )
		SET( ENABLE_GUI_CAPVIEW FALSE )
	ENDIF()
ENDIF()

IF(NOT ENABLE_GUI_CAPVIEW)
	RETURN()
ENDIF()


###########################################################################################

###########################################################################################
# Include the submodules
SET( GUI_CAPVIEW_INCLUDE_DIR  "${GUI_CAPVIEW_DIR}/include"    )
SET( GUI_CAPVIEW_SOURCE_DIR   "${GUI_CAPVIEW_DIR}/source"     )
SET( GUI_CAPVIEW_PROJECTS_DIR "${GUI_CAPVIEW_DIR}/projects"   )
SET( GUI_CAPVIEW_RESOURCES_DIR "${GUI_CAPVIEW_DIR}/resources" )
SET( GUI_CAPVIEW_UI_DIR       "${GUI_CAPVIEW_DIR}/ui"         )

SET( GUI_INCLUDE_DIRS
  ${GUI_INCLUDE_DIRS}
  "${GUI_CAPVIEW_INCLUDE_DIR}" )
###########################################################################################

###########################################################################################
# List our files
SET( GUI_CAPVIEW_FILES
   "${GUI_CAPVIEW_INCLUDE_DIR}/ccipdCaPViewMainWindow.h"
   "${GUI_CAPVIEW_INCLUDE_DIR}/ccipdCaPViewDCEWindow.h"
	  
   "${GUI_CAPVIEW_SOURCE_DIR}/ccipdCaPViewMainWindow.cxx"
   "${GUI_CAPVIEW_SOURCE_DIR}/ccipdCaPViewDCEWindow.cxx"
	  
   "${GUI_CAPVIEW_PROJECTS_DIR}/ccipdCaPViewMain.cxx"
)

SET( GUI_CAPVIEW_MOC_FILES
  "${GUI_CAPVIEW_INCLUDE_DIR}/ccipdCaPViewMainWindow.h"
  "${GUI_CAPVIEW_INCLUDE_DIR}/ccipdCaPViewDCEWindow.h"
)

SET( GUI_CAPVIEW_UI_FILES
  "${GUI_CAPVIEW_UI_DIR}/ccipdCaPViewMainWindow.ui"
  "${GUI_CAPVIEW_UI_DIR}/ccipdCaPViewDCEWindow.ui"
)
SET( GUI_CAPVIEW_RESOURCE_FILES
  "${GUI_CAPVIEW_RESOURCES_DIR}/ccipdCaPViewMain.qrc"
)
###########################################################################################



###########################################################################################
# This is needed for Qt to configure our C++ code.
IF( ENABLE_GUI_CAPVIEW )

	IF(Qt5Widgets_FOUND)
		QT5_WRAP_UI ( GUI_CAPVIEW_UI_RESULT  ${GUI_CAPVIEW_UI_FILES}  )
		QT5_WRAP_CPP( GUI_CAPVIEW_MOC_RESULT ${GUI_CAPVIEW_MOC_FILES} )
		QT5_ADD_RESOURCES(GUI_CAPVIEW_RESOURCE_RESULT ${GUI_CAPVIEW_RESOURCE_FILES})
	ELSE(Qt5Widgets_FOUND)
		QT4_WRAP_UI ( GUI_CAPVIEW_UI_RESULT  ${GUI_CAPVIEW_UI_FILES}  )
		QT4_WRAP_CPP( GUI_CAPVIEW_MOC_RESULT ${GUI_CAPVIEW_MOC_FILES} )
		QT4_ADD_RESOURCES(GUI_CAPVIEW_RESOURCE_RESULT ${GUI_CAPVIEW_RESOURCE_FILES})
	ENDIF(Qt5Widgets_FOUND)	
	
  SOURCE_GROUP( "Resource Files" FILES ${GUI_CAPVIEW_RESOURCE_FILES})
  SOURCE_GROUP( "Form Files"     FILES ${GUI_CAPVIEW_UI_FILES}      )
  SOURCE_GROUP( "Generated"      FILES ${GUI_CAPVIEW_UI_RESULT} ${GUI_CAPVIEW_MOC_RESULT} ${GUI_CAPVIEW_RESOURCE_RESULT})
  
###########################################################################################



###########################################################################################
# Create our executables
  ADD_EXECUTABLE( CaPViewMain ${GUI_CAPVIEW_FILES}
                              ${GUI_CAPVIEW_UI_RESULT}
                              ${GUI_CAPVIEW_MOC_RESULT}
                              ${GUI_CAPVIEW_RESOURCE_RESULT}
  )
###########################################################################################



###########################################################################################

  SET( CAPVIEW_MAIN_LIBS
    ${GUI_CORE_LIBS}
    ${CLASSIFICATION_CORE_LIBS}
	${QT_LIBRARIES}
	${QXT_LIBRARIES}
	${QWT_LIBRARY}
    )
    
  CCIPD_REMOVE_DUPLICATE_LIBS( CAPVIEW_MAIN_LIBS ${CAPVIEW_MAIN_LIBS} )

  TARGET_LINK_LIBRARIES( CaPViewMain ${CAPVIEW_MAIN_LIBS} )
  
ENDIF( ENABLE_GUI_CAPVIEW )
###########################################################################################



###########################################################################################
MESSAGE ( "Building CaPView: " ${ENABLE_GUI_CAPVIEW} )
###########################################################################################

