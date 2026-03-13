
#include "SbPreCompile.h" //QMainWindow

#include "SBrowserCppWebEngine.h" //SBrowserWindow
#include "SWebPageWidget.h" //SWebPageWidget
#include "ui_SBrowserCppWebEngine.h" //Ui_SBrowserMainWindow
#include "cookiejar.h" //CookieJar
#include "DownloadManager.h" //DownloadManager
#include "QrHelper.h" //QrHelper
#include "ui_passworddialog.h" //PasswordDialog
#include "StbShadowProxyFactory.h" //StbShadowProxyFactory
#include "SbAdblockInterceptor.h" //SbAdblockInterceptor
#include "UnknownSchemeInterceptor.h" //UnknownSchemeInterceptor

/*!
 * \brief UnknownSchemeInterceptor::UnknownSchemeInterceptor 默认构造函数。
 */
UnknownSchemeInterceptor::UnknownSchemeInterceptor()
{
  knownSchemeSet=
  {
    "http",
    "https",
    "wss",
    "ws",
    "blob",
    "blob",
    "about",
    "file",
    "chrome",
    "chrome-extension",
  };
} //UnknownSchemeInterceptor::UnknownSchemeInterceptor()

/*!
 * \brief UnknownSchemeInterceptor::interceptRequest 干涉请求。
 * \param info 请求信息对象。
 */
void UnknownSchemeInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
  QUrl url=info.requestUrl (); //获取请求网址。

  QString scheme=url.scheme (); //获取模式。

  if (knownSchemeSet.contains(scheme)) //是已知模式。
  {
  } //if (knownSchemeSet.contains(scheme)) //是已知模式。
  else //是未知模式。
  {
    info.block (true); //应当阻止。

    QDesktopServices::openUrl (url); //使用桌面服务打开网址。
  } //else //是未知模式。
} //void SbAdblockInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
