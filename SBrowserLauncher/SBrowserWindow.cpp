
#include "SbPreCompile.h" //QMainWindow

#include "SBrowserWindow.h" //SBrowserWindow

/*!
 * \brief SBrowserWindow::SBrowserWindow 构造函数。
 * \param parent 亲代对象。
 */
SBrowserWindow::SBrowserWindow(QObject *parent): QObject(parent)
{
    selfQuitTimer.setInterval(10000);
    selfQuitTimer.setSingleShot(true);


    connectSignals(); //连接信号。

    listenWebSocket(); //监听网页套接字。
    
    launchSBrowser(); //启动浏览器进程。
} //SBrowserWindow::SBrowserWindow(parent=0)

/*!
 * \brief UdpServer::listen //Listen websocket , for simageview process to request showing image
 */
void SBrowserWindow::listenWebSocket()
{
    networkThreadWebSocket=new WebSocketServiceManager ; //网页套接字服务器管理器。

    connectWebSocketServiceManagerSignal(networkThreadWebSocket); //连接网页套接字服务器管理器对象的信号。

    quint16 systemManagementPort=14250; // Quit 网页套接字端口号。

    QList<quint16> portList= //要监听的端口号列表。
    {
            systemManagementPort,
    };

    networkThreadWebSocket->start(portList); //开始工作，接收消息队列中的消息。


} //void UdpServer::listen()


/*!
 * \brief LogicControl::connectWebSocketServiceManagerSignal 连接网页套接字服务器管理器对象的信号。
 * \param networkThreadWebSocket 要连接其信号的网页套接字服务器管理器对象。
 */
void SBrowserWindow::connectWebSocketServiceManagerSignal(WebSocketServiceManager * networkThreadWebSocket)
{
    connect(networkThreadWebSocket, &WebSocketServiceManager::shouldViewImage, this, &SBrowserWindow::viewImage); //要求显示图片，则显示图片

    connect(networkThreadWebSocket, &WebSocketServiceManager::shouldQuit, this, &SBrowserWindow::quit); //Quit

//    connect(networkThreadWebSocket, &WebSocketServiceManager::shouldViewImage, this, &SBrowserWindow::show); //要求显示图片，则显示图片

    connect(networkThreadWebSocket, &WebSocketServiceManager::listenFailed, this, &SBrowserWindow::Quit); //Listen failed , quit
} //void LogicControl::connectWebSocketServiceManagerSignal(WebSocketServiceManager * networkThreadWebSocket)

/*!
 * \brief XyRunner::Quit 按退出按钮，则退出。
 */
void SBrowserWindow::Quit()
{
    QCoreApplication::quit (); //退出。
} //void XyRunner::Quit() //!<按退出按钮，则退出。



/*!
 * \brief SImageView::viewImage 要求显示图片，则显示图片
 * \param imageFileName 图片文件名
 * \param imageContent 图片内容
 */
void SBrowserWindow::viewImage(const QString & imageFileName, const QByteArray & imageContent)
{
    Q_UNUSED(imageFileName) //Not use this parameter
    Q_UNUSED(imageContent) //Not use this parameter.



} //void SImageView::viewImage(const QString & imageFileName, const QByteArray & imageContent)




/*!
   * \brief SBrowserWindow::connectSignals 连接信号。
   */
void SBrowserWindow::connectSignals()
{
    connect(&selfQuitTimer, &QTimer::timeout, qApp, &QCoreApplication::quit); //Quit

    connect(&sbrowserCppWebengineProcess, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &SBrowserWindow::quit); //浏览器进程退出，则退出。
    
    connect(&sbrowserCppWebengineProcess, &QProcess::readyRead, this, &SBrowserWindow::reportSubSystemOutput); //摄像头子系统给出输出内容，则报告输出内容。
    connect(&sbrowserCppWebengineProcess, &QProcess::readyReadStandardError, this, &SBrowserWindow::reportSubSystemStandardError); //摄像头子系统给出输出内容，则报告输出内容。
} //void SBrowserWindow::connectSignals() //连接信号。

/*!
 * \brief SBrowserWindow::reportSubSystemStandardError 子进程给出输出内容，则报告输出内容。
 */
void SBrowserWindow::reportSubSystemStandardError()
{
    QProcess * outputingProcess= qobject_cast<QProcess*>(sender()); //获取报告内容的进程的指针。
    
    QByteArray content=outputingProcess->readAllStandardError(); //全部读取。
    
    cout << content.toStdString() << endl; //输出到终端。
    
} //void LogicControl::reportSubSystemStandardError()

/*!
 * \brief SBrowserWindow::reportSubSystemOutput 子进程给出输出内容，则报告输出内容。
 */
void SBrowserWindow::reportSubSystemOutput()
{
    QProcess * outputingProcess= qobject_cast< QProcess*>(sender()); //获取报告内容的进程的指针。
    
    QByteArray content=outputingProcess->readAll(); //全部读取。
    
    cout << content.toStdString() << endl; //输出到终端。
} //void LogicControl::reportSubSystemOutput()

/*!
 * \brief SBrowserWindow::launchSBrowser   启动  浏览器进程。
 */
void SBrowserWindow::launchSBrowser()
{
  QProcessEnvironment processEnvironment=QProcessEnvironment::systemEnvironment(); //获取系统环境变量。

  //加入额外的环境变量：

  QString ldlibrarypath="LD_LIBRARY_PATH";

  QStringList ldlibrarypathValueList=
  {
    "/usr/lib64/qt5-qtwebengine-freeworld/",
    "/usr/lib64",
    "/root/SoftwareDevelop/sbrowser/SBrowserCpp/SBrowserCppWebEngine/../../../../../usr/lib/",
  };

  QString ldlibrarypathValue=ldlibrarypathValueList.join (":");

  processEnvironment.insert(ldlibrarypath,ldlibrarypathValue);

  QString ldlibrarypathDebug="QTWEBENGINE_REMOTE_DEBUGGING";

  QString ldlibrarypathValueDebug="0.0.0.0:19110";

  processEnvironment.insert(ldlibrarypathDebug, ldlibrarypathValueDebug);


  // 980  export QTWEBENGINE_DISABLE_SANDBOX=1
  QString disableSandboxug="QTWEBENGINE_DISABLE_SANDBOX"; // Disable sandbox.
  QString disableSandboxValueug="1"; // Disable sandbox.

  processEnvironment.insert(disableSandboxug, disableSandboxValueug);

  QStringList AptPrms; //命令行参数。

  AptPrms << "--no-sandbox"; //Add parameter, no sandbox.

  sbrowserCppWebengineProcess.setProcessEnvironment(processEnvironment); //设置环境变量。

  sbrowserCppWebengineProcess.start(AptCmd,AptPrms); //启动进程。
} //void SBrowserWindow::checkNewVersion() //!<检查是否有新版本。

/*!
 * \brief SBrowserWindow::quit 退出。
 */
void SBrowserWindow::quit()
{
    selfQuitTimer.start(); //Start hte self quit timer


//    QCoreApplication::quit(); //退出。
} //void SBrowserWindow::quit()
