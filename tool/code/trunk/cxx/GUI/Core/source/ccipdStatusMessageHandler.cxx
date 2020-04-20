#include "ccipdStatusMessageHandler.h"


#include <iostream>
using namespace std;

namespace ccipd
{

  StatusMessageHandler::StatusMessageHandler(  QStatusBar* parent
                                             , const QString      messageString
                                             , MessageType        messageType
                                             , const unsigned int nblinks
                                             , const unsigned int blinkInterval
                                            ) 
  {
    m_pStatusBar       = parent;
    m_pStatusBar->showMessage(messageString, (3 * nblinks * blinkInterval));
    cout << endl << messageString.toStdString(); ;

    m_originalStyleSheetString = m_pStatusBar->styleSheet();

    m_nblinks       = nblinks       ;
    m_blinkInterval = blinkInterval ;

  switch (messageType)
  {
  case /*MessageType::*/ERR:
    m_styleSheetString = "background-color: red; color: white";
    break;

  case /*MessageType::*/WARNING:
     m_styleSheetString = "background-color: yellow; color: white";
    break;

  case /*MessageType::*/SUCCESS:
     m_styleSheetString = "background-color: green; color: white";
    break;

  default:
     m_styleSheetString = "background-color: white; color: black";

  }

  connect(this, SIGNAL(changeStatusMessage(const QString &)), m_pStatusBar, SLOT(setStyleSheet(const QString &)));
  connect(this, SIGNAL(clearMessage()), m_pStatusBar, SLOT(clearMessage()));


  }

  StatusMessageHandler::~StatusMessageHandler()
  {
  }


void StatusMessageHandler::run()
{
   showMessage();
}

void StatusMessageHandler::showMessage()
{
	static bool bgcolorRed = true;

  for(unsigned int i = 0; i < (m_nblinks * 2); ++i)
  {
      msleep(m_blinkInterval / 2);
	    if (bgcolorRed)
	    {
        emit changeStatusMessage(m_styleSheetString);
	    }
	    else
	    {
        emit changeStatusMessage("QStatusBar {background-color: white; color: black}");
	    }

	    bgcolorRed = !bgcolorRed;
  }

  emit changeStatusMessage(m_originalStyleSheetString);
  //emit clearMessage();
  terminate();
	
}


} // namespace