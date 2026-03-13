#ifndef CONTAINERINTERCEPTOR_H
#define CONTAINERINTERCEPTOR_H


#include "SbPreCompile.h" //QMainWindow

class AboutDialog; //!<AboutDialog的前向声明。
class SWebPageWidget; //!<SWebPageWidget的前向聲明.


#include "StbSettingDialog.h" //StbSettingDialog
#include "StbProxyFactory.h" //StbProxyFactory
#include "DownloadManager.h" //DownloadManager
#include "FindDialog.h" //FindDialog
#include "SbTtsClient.h" //SbTtsClient
#include "AboutDialog.h" //AboutDialog
#include "SWebPageWidget.h" //SWebPageWidget
#include "TabManager.h" //QtDocChinese
//#include "WebEngineOrWebView.h" //WebEngineOrWebView
#include "QrBackgroundImageNormalizer.h" //QrBackgroundImageNormalizer
#include "SbAdblockInterceptor.h" //SbAdblockInterceptor

namespace Ui
{
class SbAdblockInterceptor;
} //namespace Ui


/*!
 * \brief The ContainerInterceptor class  容器干涉器类。
 */
class ContainerInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT

private :
    QList<QWebEngineUrlRequestInterceptor*> interceptorList; //!<干涉器列表。

public :
    void addInterceptor(QWebEngineUrlRequestInterceptor * interceptor); //!<加入干涉器。
    void interceptRequest(QWebEngineUrlRequestInfo &info); //!<干涉请求。
}; //class SBrowserWindow : QMainWindow

#endif // PRECOMPILE_H
