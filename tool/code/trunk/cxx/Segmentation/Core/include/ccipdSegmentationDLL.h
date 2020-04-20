#ifndef _ccipdSegmentationDLL_h
#define _ccipdSegmentationDLL_h

/* Cmake will define Segmentation_EXPORT on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define Segmentation_EXPORT when
building a DLL on windows.  Defining __declspec(dllimport)
breaks static libraries so it isn't used.
*/

// If we are using the Visual Studio Compiler and building shared libraries
#if defined( _WIN32 ) && defined( Segmentation_EXPORTS )
  #define Segmentation_EXPORT __declspec(dllexport)
#else
  #define Segmentation_EXPORT
#endif

#endif // _ccipdSegmentationDLL_h
