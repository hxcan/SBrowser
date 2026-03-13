#ifndef UNKNOWNSCHEMEINTERCEPTOR_H
#define UNKNOWNSCHEMEINTERCEPTOR_H

#include "SbPreCompile.h" //QMainWindow

#include "StbProxyFactory.h" //StbProxyFactory
#include "DownloadManager.h" // DownloadManager
//#include "FindDialog.h" //FindDialog
//#include "SbTtsClient.h" //SbTtsClient
#include "SWebPageWidget.h" //SWebPageWidget
//#include "TabManager.h" //QtDocChinese

/*!
 * \brief The SBrowserWindow class 浏览器主窗口类。
 */
class UnknownSchemeInterceptor : public QWebEngineUrlRequestInterceptor
{
  Q_OBJECT

private :
    QSet<QString> knownSchemeSet; //!<已知模式集合。

public :
    UnknownSchemeInterceptor(); //!<默认构造函数。
    void interceptRequest(QWebEngineUrlRequestInfo &info); //!< Intercept the reqeust.
}; // class UnknownSchemeInterceptor : public QWebEngineUrlRequestInterceptor

#endif // UNKNOWNSCHEMEINTERCEPTOR_H
