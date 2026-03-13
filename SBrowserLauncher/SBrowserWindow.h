#ifndef SBROWSERWINDOW_H
#define SBROWSERWINDOW_H


#include "SbPreCompile.h" //QMainWindow

#include "WebSocketServiceManager.h" //WebSocketServiceManager

namespace Ui
{
    class SBrowserLauncher;
} //namespace Ui

/*!
 * \brief The SBrowserWindow class 浏览器主窗口类。
 */
class SBrowserWindow : public QObject
{
    Q_OBJECT

private slots:
    void quit(); //!<退出。

private :
    void connectWebSocketServiceManagerSignal(WebSocketServiceManager * networkThreadWebSocket); //!<连接网页套接字服务器管理器对象的信号。
    void viewImage(const QString & imageFileName, const QByteArray & imageContent); //!<要求显示图片，则显示图片
    void Quit(); //!<按退出按钮，则退出。
    QTimer selfQuitTimer; //!<hte self quit timer.
    void listenWebSocket(); //!<监听网页套接字
    WebSocketServiceManager * networkThreadWebSocket=nullptr; //!<网页套接字服务器管理器。
    void reportSubSystemStandardError(); //!<摄像头子系统给出输出内容，则报告输出内容。
    void reportSubSystemOutput(); //!<摄像头子系统给出输出内容，则报告输出内容。
    QProcess sbrowserCppWebengineProcess; //!<用于启动真正浏览器的进程。
    QString AptCmd="sbrowsercppwebengine"; //!<apt-get命令行。
    void launchSBrowser(); //!<启动漂亮的笨蛋浏览器。
    void connectSignals(); //!<连接信号。

public :
    SBrowserWindow(QObject *parent=0); //!<构造函数。
}; //class SBrowserWindow : QMainWindow

#endif // PRECOMPILE_H
