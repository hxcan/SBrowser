
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
#include "ContainerInterceptor.h" //ContainerInterceptor

/*!
 * \brief ContainerInterceptor::addInterceptor 加入干涉器。
 * \param interceptor 要加入的干涉器。
 */
void ContainerInterceptor::addInterceptor(QWebEngineUrlRequestInterceptor * interceptor)
{
    interceptorList << interceptor; //加入到列表中。
} //void ContainerInterceptor::addInterceptor(QWebEngineUrlRequestInterceptor * interceptor)

/*!
 * \brief ContainerInterceptor::interceptRequest 干涉请求。
 * \param info 请求信息对象。
 */
void ContainerInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    for(auto currentInterceptor:interceptorList) //一个个地干涉。
    {
        currentInterceptor->interceptRequest (info); //使用当前干涉器进行干涉。
    } //for(auto currentInterceptor:interceptorList) //一个个地干涉。
} //void SbAdblockInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
