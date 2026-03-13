#ifndef STBWEBPAGE_H
#define STBWEBPAGE_H


#include "SbPreCompile.h" //QWebEngineView
#include "SWebPageWidget.h" //SWebPageWidget
#include "SBrowserCppWebEngine.h" //SBrowserWindow

class SWebPageWidget; //!<SWebPageWidget的前向声明。
class SBrowserCppWebEngine; //!<SBrowserWindow的前向聲明.

/*!
 * \brief The StbWebView class 网页视图类。
 */
class StbWebPage:public QWebEnginePage
{
    Q_OBJECT

protected:
    bool certificateError(const QWebEngineCertificateError & certificateError); //!<证书错误。

private:
    QStringList sslTrustedHostList; //!<受信任的主机列表。
    SWebPageWidget * wbPgWdgt; //!<网页部件的指针。

signals:
    void downloadStarted(); //!<信号，开始了下载动作。
    void shouldShowStatusBarMessage(QString msg2Show); //!<信号。应当显示状态栏消息。
}; //class StbWebPage:public QWebEnginePage


#endif // PRECOMPILE_H
