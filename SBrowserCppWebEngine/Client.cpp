#include "SbPreCompile.h" //QMainWindow

#include "Client.h" //Client

/*!
 * \brief Client::Client Constructor.
 * \param parent The parent widget.
 */
Client::Client(QSharedPointer<SetDirectionServiceReplica> ptr) :QObject(nullptr),reptr(ptr)
{
    initConnections();
} //BlueMainWindow::BlueMainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)

void Client::reportDaemonConnected() //!<连接成功，则报告。
{
} //void Client::reportDaemonConnected()

void Client::initConnections()
{
    connect(reptr.data(),&SetDirectionServiceReplica::initialized,this,&Client::daemonConnected); //连接成功，则报告。
    connect(reptr.data(),&SetDirectionServiceReplica::initialized,this,&Client::reportDaemonConnected); //连接成功，则报告。
    
    connect(reptr.data(),&SetDirectionServiceReplica::stateChanged,this,&Client::judgeDaemonConnectStatus); //状态变化，则判断守护进程连接状态。
    connect(this, &Client::shouldAddDownloadTask, reptr.data(), &SetDirectionServiceReplica::addDownloadTask); //连接信号，要求添加下载任务，则添加下载任务。
    connect(this, &Client::shouldQuit, reptr.data(), &SetDirectionServiceReplica::quit); //连接信号，要求退出，则退出。
    connect(this,&Client::shouldShowDownloadManager, reptr.data(), &SetDirectionServiceReplica::showDownloadManager); //连接信号，要求显示下载管理器，则显示下载管理器。
    connect(this, &Client::shouldSyncSetting, reptr.data(), &SetDirectionServiceReplica::syncSetting); //Sync setting. 陈欣
    

    // ✅ 新增：连接 shouldAddDownloadTaskFromData → 远程 SLOT
    connect(this, &Client::shouldAddDownloadTaskFromData,
            reptr.data(), &SetDirectionServiceReplica::addDownloadTaskFromData);
    //                            ↑↑↑ 这个 SLOT 必须已在 .rep 中定义
} //void Client::initConnections()

/*!
 * \brief Client::judgeDaemonConnectStatus 状态变化，则判断守护进程连接状态。
 * \param status 状态。
 */
void Client::judgeDaemonConnectStatus(QRemoteObjectReplica::State status)
{
    if (status==QRemoteObjectReplica::Valid) //变成有效状态。
    {
        emit daemonConnected();
    } //if (status==QRemoteObjectReplica::Valid) //变成有效状态。
} //void Client::judgeDaemonConnectStatus(QRemoteObjectReplica::State status)
