IF(ENABLE_GUI_PROSTAFUSE)
	IF(NOT ENABLE_GUI_CORE)
  		MESSAGE( "ProstaFuse requires the GUI Core library.")
	ENDIF()

	IF( NOT BUILD_QT_PROJECTS )
		MESSAGE( "ProstaFuse requires Qt.")
	ENDIF()

	IF(NOT ENABLE_GUI_CORE OR NOT BUILD_QT_PROJECTS)
		SET( ENABLE_GUI_PROSTAFUSE FALSE )
	ENDIF()
ENDIF()

IF(NOT ENABLE_GUI_PROSTAFUSE)
	MESSAGE( "Disabling ProstaFuse." )
	RETURN()
ENDIF()


###########################################################################################
# Include the submodules
SET( GUI_PROSTAFUSE_INCLUDE_DIR  "${GUI_PROSTAFUSE_DIR}/include"  )
SET( GUI_PROSTAFUSE_SOURCE_DIR   "${GUI_PROSTAFUSE_DIR}/source"   )
SET( GUI_PROSTAFUSE_PROJECTS_DIR "${GUI_PROSTAFUSE_DIR}/projects" )
SET( GUI_PROSTAFUSE_UI_DIR       "${GUI_PROSTAFUSE_DIR}/ui"       )

SET( GUI_INCLUDE_DIRS  ${GUI_INCLUDE_DIRS}
                      "${GUI_PROSTAFUSE_INCLUDE_DIR}" 
	)
###########################################################################################



###########################################################################################
# List our files
SET( GUI_PROSTAFUSE_FILES
  "${GUI_PROSTAFUSE_INCLUDE_DIR}/ccipdProstaFuseOptions.h"
  "${GUI_PROSTAFUSE_INCLUDE_DIR}/ccipdProstaFuseResults.h"
  "${GUI_PROSTAFUSE_INCLUDE_DIR}/ccipdProstaFuseResultsOptions.h"
  "${GUI_PROSTAFUSE_INCLUDE_DIR}/ccipdProstaFuseWindow.h"
  "${GUI_PROSTAFUSE_PROJECTS_DIR}/ccipdProstaFuseMain.cxx"
  "${GUI_PROSTAFUSE_SOURCE_DIR}/ccipdProstaFuseOptions.cxx"
  "${GUI_PROSTAFUSE_SOURCE_DIR}/ccipdProstaFuseResults.cxx"
  "${GUI_PROSTAFUSE_SOURCE_DIR}/ccipdProstaFuseResultsOptions.cxx"
  "${GUI_PROSTAFUSE_SOURCE_DIR}/ccipdProstaFuseWindow.cxx"
)
SET( GUI_PROSTAFUSE_MOC_FILES
  "${GUI_PROSTAFUSE_INCLUDE_DIR}/ccipdProstaFuseOptions.h"
  "${GUI_PROSTAFUSE_INCLUDE_DIR}/ccipdProstaFuseResults.h"
  "${GUI_PROSTAFUSE_INCLUDE_DIR}/ccipdProstaFuseResultsOptions.h"
  "${GUI_PROSTAFUSE_INCLUDE_DIR}/ccipdProstaFuseWindow.h"
)
SET( GUI_PROSTAFUSE_UI_FILES
  "${GUI_PROSTAFUSE_UI_DIR}/ccipdProstaFuseWindow.ui"
  "${GUI_PROSTAFUSE_UI_DIR}/ccipdProstaFuseOptions.ui"
  "${GUI_PROSTAFUSE_UI_DIR}/ccipdProstaFuseResults.ui"
  "${GUI_PROSTAFUSE_UI_DIR}/ccipdProstaFuseResultsOptions.ui"
)
###########################################################################################



###########################################################################################
# This is needed for Qt to configure our C++ code.
# http://bit.ly/IL0yRT 
IF( ENABLE_GUI_PROSTAFUSE AND BUILD_QT_PROJECTS )
  
  	IF(Qt5Widgets_FOUND)
		QT5_WRAP_UI ( GUI_PROSTAFUSE_UI_RESULT  ${GUI_PROSTAFUSE_UI_FILES}  )
        QT5_WRAP_CPP( GUI_PROSTAFUSE_MOC_RESULT ${GUI_PROSTAFUSE_MOC_FILES} )
	ELSE(Qt5Widgets_FOUND)
        QT4_WRAP_UI ( GUI_PROSTAFUSE_UI_RESULT  ${GUI_PROSTAFUSE_UI_FILES}  )
        QT4_WRAP_CPP( GUI_PROSTAFUSE_MOC_RESULT ${GUI_PROSTAFUSE_MOC_FILES} )
	ENDIF(Qt5Widgets_FOUND)	
  
  
  
  SOURCE_GROUP( "Resources" FILES ${GUI_PROSTAFUSE_UI_FILES} )
  SOURCE_GROUP( "Generated" FILES
    ${GUI_PROSTAFUSE_UI_RESULT}
    ${GUI_PROSTAFUSE_MOC_RESULT}
  )
ENDIF()
###########################################################################################



###########################################################################################
# Create our test programs
IF( ENABLE_GUI_PROSTAFUSE )
  ADD_EXECUTABLE( ProstaFuseMain
    ${GUI_PROSTAFUSE_FILES}
    ${GUI_PROSTAFUSE_UI_RESULT}
    ${GUI_PROSTAFUSE_MOC_RESULT}
  )
ENDIF( ENABLE_GUI_PROSTAFUSE )
###########################################################################################



###########################################################################################
# Link our libraries
IF( ENABLE_GUI_PROSTAFUSE )

  SET( PROSTAFUSE_MAIN_LIBS ${GUI_CORE_LIBS} )
    
  #CCIPD_REMOVE_DUPLICATE_LIBS( PROSTAFUSE_MAIN_LIBS ${PROSTAFUSE_MAIN_LIBS} )

  TARGET_LINK_LIBRARIES( ProstaFuseMain ${PROSTAFUSE_MAIN_LIBS} )
  
ENDIF( ENABLE_GUI_PROSTAFUSE )
###########################################################################################



###########################################################################################
IF(ENABLE_GUI_PROSTAFUSE)
MESSAGE (STATUS "Building Prostafuse: " ${ENABLE_GUI_PROSTAFUSE} )
ELSE()
MESSAGE ( "Building Prostafuse: " ${ENABLE_GUI_PROSTAFUSE} )
ENDIF()
###########################################################################################

