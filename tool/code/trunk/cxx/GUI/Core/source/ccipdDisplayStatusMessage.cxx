#include "ccipdDisplayStatusMessage.h"
#include "ccipdDisableWarningsMacro.h"

#include <QStatusBar>
#include <QString>


#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{

void DisplayStatusMessage(  QStatusBar* statusBar
                          , const QString messageString
                          , StatusMessageHandler::MessageType  messageType
                           )
  {

    int nBlinks = 1;
    int blinkInterval = 200;

  // set timing and blinking
  switch (messageType)
  {
  case StatusMessageHandler/*::MessageType*/::ERR:
   nBlinks = 12;
   blinkInterval = 250;

    break;

  case StatusMessageHandler/*::MessageType*/::WARNING:
     nBlinks = 6;
     blinkInterval = 500;
    break;

  case StatusMessageHandler/*::MessageType*/::SUCCESS:
        nBlinks = 1;
        blinkInterval = 1000;
    break;

  default:
     break;
  }

      StatusMessageHandler *pMessageThread = new StatusMessageHandler(statusBar
                                                      , messageString
                                                      , messageType
                                                      , nBlinks
                                                      , blinkInterval);
      pMessageThread->start();

  }

} // namespace