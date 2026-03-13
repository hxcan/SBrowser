#ifndef SBADBLOCKINTERCEPTOR_H
#define SBADBLOCKINTERCEPTOR_H


#include "SbPreCompile.h" //QMainWindow

class TabManager; //!<QtDocChinese的前向声明。
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
 * \brief The SBrowserWindow class 浏览器主窗口类。
 */
class SbAdblockInterceptor : public QWebEngineUrlRequestInterceptor
{
  Q_OBJECT

private :
  StbProxyFactory * proxyFactory; //!<代理工厂对象。

public :
  void interceptRequest(QWebEngineUrlRequestInfo &info); //!<干涉请求。
  StbProxyFactory *getProxyFactory() const;
  void setProxyFactory(StbProxyFactory *value);
}; //class SBrowserWindow : QMainWindow

#endif // PRECOMPILE_H
