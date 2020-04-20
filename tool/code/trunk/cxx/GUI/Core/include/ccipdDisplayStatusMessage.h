#ifndef __ccipdDisplayStatusMessage_h__
#define __ccipdDisplayStatusMessage_h__

#include "ccipdStatusMessageHandler.h"


namespace ccipd
{

  /*
    Displays the message string on the given statusbar 
    colored and blinking according to its type
    Blinking is done over another thread
  
  */
  void DisplayStatusMessage(  QStatusBar* statusBar
                            , const QString messageString
                            , StatusMessageHandler::MessageType  messageType
                           );



  } //namespace

#endif