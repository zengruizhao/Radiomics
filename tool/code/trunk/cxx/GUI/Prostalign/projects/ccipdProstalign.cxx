


////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdProstalignChooser.h" // for a GUI

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// Qt includes
#include <QApplication>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER
////////////////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{



////////////////////////////////////////////////////////////////////////////////////////////////////
  // QT Stuff
  QApplication app( argc, argv );
 
  ccipd::ProstalignChooser mainGUI;
  mainGUI.show();
 
  return app.exec();
////////////////////////////////////////////////////////////////////////////////////////////////////



} // main
