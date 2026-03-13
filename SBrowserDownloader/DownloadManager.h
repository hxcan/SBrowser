#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "ui_DownloadManager.h"
#include "ui_DownloadItem.h"

//#include <QtNetwork/QNetworkReply>

#include "DownloadItem.h" //DownloadItem

class AutoSaver;
class DownloadModel;
QT_BEGIN_NAMESPACE
class QFileIconProvider;
QT_END_NAMESPACE

/*!
 * \brief The DownloadManager class 下载管理器。
 */
class DownloadManager : public QDialog, public Ui_DownloadManager
{
  Q_OBJECT
  Q_PROPERTY(RemovePolicy removePolicy READ removePolicy WRITE setRemovePolicy)
    Q_ENUMS(RemovePolicy)

public:
    enum RemovePolicy {
        Never,
        Exit,
        SuccessFullDownload
    };

    void setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages); //!<设置网络访问管理器。
    DownloadManager(QWidget *parent = 0);
    ~DownloadManager();
    int activeDownloads() const;
    RemovePolicy removePolicy() const;
    void setRemovePolicy(RemovePolicy policy);
    void setDownloadDirectory(const QString &value);

public slots:
    void download(const QNetworkRequest &request);
    void download(const QUrl &url,  QString referrerUrl); //!<Download specified url.
    void handleUnsupportedContent(QNetworkReply *reply);
    void cleanup();

private slots:
    void save() const;
    void updateRow();

private:
    void initializeMemberObjects(); //!<初始化成员变量。
    void connectSignals(); //!<连接信号槽。
    QTimer listenTimer; //!<监听定时器。
    void addItem(DownloadItem *item);
    void updateItemCount();
    void load(); //!<载入下载记录。
    QString downloadDirectory; //!<Download directory。
    AutoSaver *m_autoSaver; //!<自动保存下载记录的对象。
    DownloadModel *m_model;
    QNetworkAccessManager *m_manager; //!<网络访问管理器。
    QFileIconProvider *m_iconProvider;
    QList<DownloadItem*> m_downloads;
    RemovePolicy m_removePolicy;
    friend class DownloadModel;

signals:
    void downloadError(); //!<下载出错。
    void downloadStarted(); //!<信号，下载开始。
    void downloadFinished(QString dlddFlnm); //!<信号，下载完毕。
}; //class DownloadManager : public QDialog, public Ui_DownloadDialog

class DownloadModel : public QAbstractListModel
{
  friend class DownloadManager;
  Q_OBJECT

public:
    DownloadModel(DownloadManager *downloadManager, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
    DownloadManager *m_downloadManager;
}; // class DownloadModel : public QAbstractListModel

#endif // DOWNLOADMANAGER_H
