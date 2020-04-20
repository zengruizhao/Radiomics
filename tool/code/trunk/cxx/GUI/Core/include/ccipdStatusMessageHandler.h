#ifndef __ccipdStatusMessageHandlerss_h
#define __ccipdStatusMessageHandlerss_h


#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER
#include <QThread>
#include <QMainWindow>
#include <QStatusBar>
#include <QString>
#include <QLabel>
#include <QTimer>
#include <QObject>


#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

namespace ccipd
{

class StatusMessageHandler : public QThread
{
  Q_OBJECT

public:
  enum /*class*/ MessageType
  {
    ERR     ,
    WARNING ,
    SUCCESS ,
    REGULAR
  };


  StatusMessageHandler(QStatusBar* parent, 
    const QString      messageString
                       , MessageType        messageType
                       , const unsigned int nblinks
                       , const unsigned int blinkInterval);
 
  
  ~StatusMessageHandler();


signals:
  void changeStatusMessage(const QString & str);
  void clearMessage();

  private slots:
    void showMessage();

private:
  void run();
  

  QStatusBar*  m_pStatusBar;
  QString      m_styleSheetString;
  QString      m_originalStyleSheetString;
  unsigned int m_nblinks;
  unsigned int m_blinkInterval;

};


} // namespace


#endif