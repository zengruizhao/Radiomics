


////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCaPViewMainWindow.h" // for a GUI


#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// Qt includes
#include <QApplication>




////////////////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{

  
  Q_INIT_RESOURCE(ccipdCaPViewMain);


  QApplication app( argc, argv );
  


  QCoreApplication::setOrganizationName("CCIPD");
  QCoreApplication::setOrganizationDomain("ccipd.case.edu");
  QCoreApplication::setApplicationName("CaPView");




  ccipd::CaPViewMainWindow mainGUI;
  mainGUI.show();
 





  return app.exec();
////////////////////////////////////////////////////////////////////////////////////////////////////



} // main




  #ifdef _MSC_VER
  #pragma warning( pop )
  #endif // _MSC_VER