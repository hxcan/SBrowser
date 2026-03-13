#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "SbPreCompile.h" //QTimer

#include "rep_SetDirectionService_replica.h" //SetDirectionServiceReplica

/*!
 * \brief The BlueMainWindow class The main window of product blue.
 */
class Client : public QObject
{
    Q_OBJECT

public:
    Client(QSharedPointer<SetDirectionServiceReplica> ptr);
    void initConnections(); //!<建立与远程进程调用服务器之间的信号连接。

signals:
    void shouldShowDownloadManager(); //!< 应当显示下载管理器。
    void shouldAddDownloadTask(const QString & requestUrl, bool requestFileName); //!<信号，应当添加下载任务。
    void shouldQuit(); //!< 信号，应当退出。
    void directionChanged(QString direction); //!<方向变化，则报告，方向变化。
    void finished(); //!<方向变化，则报告，完毕。
    void echoSwitchState(QString switchState,quint8 textSource); //!<信号，要求显示文字。
    void shouldExecutePhenemSequence(QByteArray phenes); //!<信号，应当执行口形序列动作。
    void shouldSyncSetting() const; //!<signal, should sync setting. 陈欣
    void daemonConnected(); //!<连接成功，则报告。
    void startAction(); //!<要求开始做动作。
    // ✅ 新增：应当添加基于二进制数据的下载任务
    void shouldAddDownloadTaskFromData(const QByteArray &data, const QString &suggestedFilename);

private:
    void judgeDaemonConnectStatus(QRemoteObjectReplica::State status); //!<状态变化，则判断守护进程连接状态。
    QSharedPointer<SetDirectionServiceReplica> reptr;

    void reportDaemonConnected(); //!<连接成功，则报告。
    QRemoteObjectNode repNode;
    QString AptCmd="./redaemon"; //!<apt-get命令行。
    QString directionNumberString="0"; //!<方向编号字符串。
    QTimer * ldTimer; //!<驱动载入过程的定时器。
}; //class Client : public QObject

#endif

