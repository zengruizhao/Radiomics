PROJECT( GUI )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
# Eliminate a warning when building in Windows.
# This policy was introduced in CMake version 2.8.11.
# CMake version 2.8.11.2 warns when the policy is not set
# and uses OLD behavior.
if(POLICY CMP0020)
  CMAKE_POLICY(SET CMP0020 NEW)
endif()
if(POLICY CMP0022)
  CMAKE_POLICY(SET CMP0022 NEW)
endif()
###########################################################################################



###########################################################################################
# This section allows us to "include" this program's build directory in other projects
# and hence we get to link to GUI in other projects!
SET( GUI_BINARY_DIR ${PROJECT_BINARY_DIR} )
SET( LIBRARY_OUTPUT_PATH ${GUI_BINARY_DIR} )

# Where will the "Use" file be located?
SET( GUI_USE_FILE ${GUI_BINARY_DIR}/UseGUI.cmake )

# Where will our libraries be located?
SET( GUI_LIBRARY_DIRS "\"${LIBRARY_OUTPUT_PATH}\"" )

# Configure RegistrationConfig.cmake for the build tree.
CONFIGURE_FILE(${GUI_SOURCE_DIR}/GUIConfig.cmake.in
               ${GUI_BINARY_DIR}/GUIConfig.cmake @ONLY )
  
# Copy the UseGUI.cmake file to the binary tree
CONFIGURE_FILE(${GUI_SOURCE_DIR}/UseGUI.cmake.in
               ${GUI_BINARY_DIR}/UseGUI.cmake @ONLY )
###########################################################################################



###########################################################################################
OPTION( BUILD_EIGEN_PROJECTS "Build projects which depend on Eigen" ON )
OPTION( BUILD_VTK_PROJECTS   "Build projects which depend on VTK"   ON )
OPTION( BUILD_QT_PROJECTS    "Build projects which depend on Qt"    ON )
OPTION( BUILD_QXT_PROJECTS   "Build projects which depend on Qxt"   OFF)
OPTION( BUILD_QWT_PROJECTS   "Build projects which depend on Qwt"   ON )
OPTION( BUILD_TESTING        "Build testing executables"            ON )

OPTION( LIB_BUILD_GUI_PROSTACAD     "Build ProstaCAD GUI program"     ON  )
OPTION( LIB_BUILD_GUI_CAPVIEW       "Build CaPView GUI program"       ON )
OPTION( LIB_BUILD_GUI_PROSTAFUSE "Build Prostafuse GUI program"       ON  )
OPTION( LIB_BUILD_GUI_PROSTALIGN    "Build Prostalign GUI program"    ON  )
OPTION( LIB_BUILD_GUI_SEGUI         "Build Segui program"             ON  )
OPTION( LIB_BUILD_GUI_FEM_RENDERING "Build FEM rendering library"     ON  )
OPTION( LIB_BUILD_GUI_RENDER_MFA    "Build the MFA rendering library" ON  )
OPTION( LIB_BUILD_GUI_CORE          "Build the core GUI library."     ON  )

IF( LIB_BUILD_GUI_PROSTACAD )
  SET( ENABLE_GUI_PROSTACAD TRUE )
ELSE()
  SET( ENABLE_GUI_PROSTACAD FALSE )
ENDIF()

IF( LIB_BUILD_GUI_PROSTAFUSE )
  SET( ENABLE_GUI_CAPVIEW TRUE )
ELSE()
  SET( ENABLE_GUI_CAPVIEW FALSE )
ENDIF()


IF( LIB_BUILD_GUI_PROSTAFUSE )
  SET( ENABLE_GUI_PROSTAFUSE TRUE )
ELSE()
  SET( ENABLE_GUI_PROSTAFUSE FALSE )
ENDIF()



IF( LIB_BUILD_GUI_PROSTALIGN )
  SET( ENABLE_GUI_PROSTALIGN TRUE )
ELSE()
  SET( ENABLE_GUI_PROSTALIGN FALSE )
ENDIF()

IF( LIB_BUILD_GUI_FEM_RENDERING )
  SET( ENABLE_GUI_FEM_RENDERING TRUE )
ELSE()
  SET( ENABLE_GUI_FEM_RENDERING FALSE )
ENDIF()

IF( LIB_BUILD_GUI_RENDER_MFA )
  SET( ENABLE_GUI_RENDER_MFA TRUE )
ELSE()
  SET( ENABLE_GUI_RENDER_MFA FALSE )
ENDIF()

IF( LIB_BUILD_GUI_SEGUI )
  SET( ENABLE_GUI_SEGUI TRUE )
ELSE()
  SET( ENABLE_GUI_SEGUI FALSE )
ENDIF()

IF( LIB_BUILD_GUI_CORE )
  SET( ENABLE_GUI_CORE TRUE )
ELSE()
  SET( ENABLE_GUI_CORE FALSE )
ENDIF()
###########################################################################################



###########################################################################################
LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_BINARY_DIR}" )
INCLUDE( UseGUI )
###########################################################################################



###########################################################################################
# We have to say to link the QVTK library.

IF( VTK_MAJOR_VERSION EQUAL "5" AND VTK_USE_QT )
  MESSAGE( "Enable QVTK library explicitly." )
  SET( VTK_LIBRARIES ${VTK_LIBRARIES} "QVTK" )
ENDIF()
###########################################################################################



###########################################################################################
# Additional cmake modules
LIST( APPEND CMAKE_MODULE_PATH "${ccipdCMakeCode_DIR}" )
INCLUDE( ccipdCompilerOptions  )
INCLUDE( ccipdRemoveDuplicates )
INCLUDE( ccipdUseGDCM          )

IF ( BUILD_EIGEN_PROJECTS )
  INCLUDE( ccipdUseEigen )
ENDIF ( BUILD_EIGEN_PROJECTS )
###########################################################################################



###########################################################################################
# Include the submodules

SET( GUI_CORE_DIR          "${PROJECT_SOURCE_DIR}/Core"         )
SET( GUI_FEM_RENDERING_DIR "${PROJECT_SOURCE_DIR}/FEMRendering" )
SET( GUI_RENDER_MFA_DIR    "${PROJECT_SOURCE_DIR}/RenderMFA"    )
SET( GUI_PROSTAFUSE_DIR    "${PROJECT_SOURCE_DIR}/ProstaFuse"   )
SET( GUI_PROSTALIGN_DIR    "${PROJECT_SOURCE_DIR}/Prostalign"   )
SET( GUI_PROSTACAD_DIR     "${PROJECT_SOURCE_DIR}/ProstaCAD"    )
SET( GUI_CAPVIEW_DIR       "${PROJECT_SOURCE_DIR}/CaPView"      )
SET( GUI_SEGUI_DIR         "${PROJECT_SOURCE_DIR}/Segui"        )

# Make sure it's cleared when we start
SET( GUI_INCLUDE_DIRS "" )

INCLUDE( "${GUI_CORE_DIR}/CMakeLists.txt"          )
INCLUDE( "${GUI_FEM_RENDERING_DIR}/CMakeLists.txt" )
INCLUDE( "${GUI_RENDER_MFA_DIR}/CMakeLists.txt"    )
INCLUDE( "${GUI_PROSTACAD_DIR}/CMakeLists.txt"     )
INCLUDE( "${GUI_CAPVIEW_DIR}/CMakeLists.txt"       )
INCLUDE( "${GUI_PROSTAFUSE_DIR}/CMakeLists.txt"    )
INCLUDE( "${GUI_PROSTALIGN_DIR}/CMakeLists.txt"    )
INCLUDE( "${GUI_SEGUI_DIR}/CMakeLists.txt"         )

###########################################################################################



###########################################################################################
# Reconfigure things which have changed.

# Copy the UseGUI.cmake file to the binary tree
CONFIGURE_FILE(${GUI_SOURCE_DIR}/UseGUI.cmake.in
               ${GUI_BINARY_DIR}/UseGUI.cmake @ONLY )

# Configure RegistrationConfig.cmake for the build tree.
CONFIGURE_FILE(${GUI_SOURCE_DIR}/GUIConfig.cmake.in
               ${GUI_BINARY_DIR}/GUIConfig.cmake @ONLY )
###########################################################################################



###########################################################################################
# Include all the appropriate directories in this project

SET( GUI_INCLUDE_DIRS
  ${GUI_INCLUDE_DIRS}
  "${GUI_BINARY_DIR}"
)

INCLUDE_DIRECTORIES( BEFORE ${GUI_INCLUDE_DIRS} )
###########################################################################################


