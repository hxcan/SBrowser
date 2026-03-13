#ifndef DOWNLOADITEM_H
#define DOWNLOADITEM_H

#include "SbPreCompile.h" //QNetworkAccessManager

#include "ui_DownloadItem.h" //Ui_DownloadItem

/*!
 * \brief The DownloadItem class 单个下载条目。
 */
class DownloadItem : public QWidget, public Ui_DownloadItem
{
    Q_OBJECT

signals:
    void downloadError(); //!<信号，下载出错。
    void statusChanged(); //!<状态发生变化。

public:
    void setDryRun(const bool dryRuncheckBoxisChecked); //!<Set whether to dry run
    void tryAgain(); //!<重试。
    void setNetworkAccessManager(QNetworkAccessManager * networkAccessManager2Set); //!<设置网络访问管理器。
    DownloadItem(QNetworkReply *rpl = 0, QWidget *prn = 0); //!<构造函数。
    bool downloading() const; //!<是否正在下载。
    bool downloadedSuccessfully() const; //!<是否已经成功下载。
    QUrl m_url; //!<下载的地址。
    QFile m_output; //!<文件的输出路径。
    qint64 rangeStartSize=0; //!<The range of this re-transfer session.
    QNetworkReply *m_reply; //!<对应于下载请求的网络回复对象。

    void setDownloadDirectory(const QString &value);

private slots:
    void stop(); //!<停止下载。
    void open(); //!<打开已下载的目标文件。
    void copyUrl(); //!<复制下载地址。
    void downloadReadyRead();
    void error(QNetworkReply::NetworkError errorCode); //!<下载出错。
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void metaDataChanged();
    void finished();

private:
    QString downloadDirectory; //!<Download directory。
    bool dryRun=false; //!<Whehter to dry run.
    void autoFitFileName(); //!<Auto fit file name. Elide too long file name.
    int MaxDownloadRetryTimes=43; //!<最大重试下载次数。
    int retryTimes=0; //!<累积重试次数。
    void countRetryTimes(); //!<对重试次数计数。
    bool explicitlyStop=false; //!<Whether the user explicitly clicked the stop button.
    void connectSignals(); //!<连接信号槽。
    QTimer retryTimer; //!<重试定时器。
    void reportRedirected(const QUrl &url); //!<重定向，则报告重定向。
    bool lastUsedDownloadFileExists() const; //!<Whether the last used download file exists.
    bool supportRange=false; //!<Whether this download supports range header.
    qint64 ProgressBarMaximumValueFor64Bit=142859; //!<针对64位的数值而使用的最大进度值。
    QString getHeaderFileName( QNetworkReply * reply) const; //!<从协议头中取得文件名。
    QNetworkAccessManager * networkAccessManager; //!<网络访问管理器。
    void getFileName();
    void init();
    void updateInfoLabel();
    QString dataString(int size) const;
    void animateProgress(int currentValue,int maximumValue); //!<以动画显示下载进度。
    QString saveFileName(const QString &directory) const;
    int progressAnimationDuration=200; //!<进度更新动画的持续时间，以毫秒为单位。
    qint64 m_bytesReceived;
    QString downloadFileName; //!<The download file name ,used to support continue transfer.
    QElapsedTimer m_downloadTime;
}; //class DownloadItem : public QWidget, public Ui_DownloadItem

#endif // DOWNLOADITEM_H
