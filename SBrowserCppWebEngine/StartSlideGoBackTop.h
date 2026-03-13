#ifndef STARTSLIDEGOBACKTOP_H
#define STARTSLIDEGOBACKTOP_H

#include "SbPreCompile.h" //QWebEngineView
#include "SWebPageWidget.h" //SWebPageWidget
#include "SBrowserCppWebEngine.h" //SBrowserWindow

class SWebPageWidget; //!<SWebPageWidget的前向声明。
class SBrowserCppWebEngine; //!<SBrowserWindow的前向聲明.

/*!
 * \brief The StbWebView class 网页视图类。
 */
class StartSlideGoBackTop
{
protected:
    void wheelEvent(QWheelEvent * event); //!<滚轮事件。
    void mouseReleaseEvent(QMouseEvent * event); //!<鼠标释放事件。


private slots:
    void showStatusBarMessage(QString msg2Show); //!<要求显示状态栏消息，则显示。
    void handleUnsupportedContent(QNetworkReply *reply); //!<处理不支持的内容。

public slots:
    void selectAll(); //!<选中全部内容。
  

public:
    void operator() (const QVariant & clientHeightResult); //!<函子操作符。
    StartSlideGoBackTop(StbWebView *parent=0); //!<构造函数。
    void setWebPageWidget(SWebPageWidget * wbPgWdgt2St); //!<设置网页部件对象的指针。

private:
    void startEagleEyeGoBackTop(const QVariant & clientHeightResult); //!<启动鹰眼效果回到顶端过程。
    void queryClientHeight(); //!<查询显示区高度。
    void eagleEyeGoBackTop(); //!<以鹰眼效果回到顶端。
    void lightSpeedGoBackTop(); //!<以光速回到顶端。
    StbWebView * masterStbWebView; //!<为之服务的目标网页视图。
    void replaceGifImg(); //!<请求完毕，则替换图片。
    SBrowserCppWebEngine * sBrowserWindow; //!<浏览器主窗口。
    void connectSignals(); //!<连接信号槽。
    SWebPageWidget * wbPgWdgt; //!<网页部件的指针。

signals:
    void downloadStarted(); //!<信号，开始了下载动作。
    void shouldShowStatusBarMessage(QString msg2Show); //!<信号。应当显示状态栏消息。
}; //class StbWebView : public QWebView

#endif // PRECOMPILE_H
