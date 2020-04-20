#ifndef _ccipdMFADLL_h
#define _ccipdMFADLL_h

/* Cmake will define MFA_EXPORT on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define MFA_EXPORT when
building a DLL on windows.  Defining __declspec(dllimport)
breaks static libraries so it isn't used.
*/

// If we are using the Visual Studio Compiler and building shared libraries
#if defined( _WIN32 ) && defined( MFA_EXPORTS )
  #define MFA_EXPORT __declspec(dllexport)
#else
  #define MFA_EXPORT
#endif

#endif // _ccipdMFADLL_h
