#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "SbPreCompile.h" // QNetworkAccessManager

#include "Client.h" //Client

class DownloadModel;
QT_BEGIN_NAMESPACE
class QFileIconProvider;
QT_END_NAMESPACE

/*!
 * \brief The DownloadManager class 下载管理器。
 */
class DownloadManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RemovePolicy removePolicy READ removePolicy WRITE setRemovePolicy)
    Q_ENUMS(RemovePolicy)

public:
    enum RemovePolicy
    {
      Never,
      Exit,
      SuccessFullDownload
    };

    /**
     * @brief downloadBlobData 处理通过 fetch 获取的 Blob 二进制数据
     * @param data 二进制数据
     * @param suggestedFilename 建议的文件名
     */
    void downloadBlobData(const QByteArray &data, const QString &suggestedFilename);

    void syncSetting() const; //!<设置对话框确认关闭，则 Sync 设置信息。陈欣
    void setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages); //!< 设置网络访问管理器。
    DownloadManager(QWidget *parent = 0);
    RemovePolicy removePolicy() const;
    void setRemovePolicy(RemovePolicy policy);
    void quit(); //!< 要求下载器进程退出。
    Client *getRswitch() const;
    void setRswitch(Client *value);
    
public slots:
    void download(const QNetworkRequest &request,bool requestFileName = false);
    void download(const QUrl &url,  QString referrerUrl, bool requestFileName = false); //!<Download specified url.

private:
    void initializeMemberObjects(); //!< 初始化成员变量。
    Client * rswitch; //!<远程进程调用客户端。
    void distributeWorkToDownloader(const QNetworkRequest & request, bool requestFileName); //!<将任务分发给下载器。
    DownloadModel *m_model;
    QNetworkAccessManager *m_manager; //!<网络访问管理器。
    RemovePolicy m_removePolicy;
    friend class DownloadModel;

signals:
    void downloadStarted(); //!<信号，下载开始。
}; //class DownloadManager : public QDialog, public Ui_DownloadDialog

#endif // DOWNLOADMANAGER_H
