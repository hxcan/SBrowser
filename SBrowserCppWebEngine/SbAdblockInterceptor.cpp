
#include "SbPreCompile.h" //QMainWindow

//#include "Toast.h" //Toast

#include "SBrowserCppWebEngine.h" //SBrowserWindow
#include "SWebPageWidget.h" //SWebPageWidget
#include "cookiejar.h" //CookieJar
#include "DownloadManager.h" //DownloadManager
#include "QrHelper.h" //QrHelper
#include "ui_passworddialog.h" //PasswordDialog

#include "StbShadowProxyFactory.h" //StbShadowProxyFactory
#include "SbAdblockInterceptor.h" //SbAdblockInterceptor

/*!
 * \brief SbAdblockInterceptor::interceptRequest 干涉请求。
 * \param info 请求信息对象。
 */
StbProxyFactory *SbAdblockInterceptor::getProxyFactory() const
{
  return proxyFactory;
}

/*!
 * \brief SbAdblockInterceptor::setProxyFactory 設置代理工廠。
 * \param value 要設置的代理工廠。
 */
void SbAdblockInterceptor::setProxyFactory(StbProxyFactory *value)
{
  proxyFactory = value;
}

void SbAdblockInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
  if (proxyFactory->isBlockingUrl(info.requestUrl())) //应当阻止此网址。
  {
    info.block(true); //阻止。
  } //if (prxyFctry->isBlockingUrl(info.requestUrl())) //应当阻止此网址。
} //void SbAdblockInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
