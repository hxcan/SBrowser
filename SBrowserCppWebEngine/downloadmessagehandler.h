#pragma once
#include <QObject>
#include <QByteArray>

class DownloadMessageHandler : public QObject
{
    Q_OBJECT

public:
DownloadMessageHandler(QObject *parent = nullptr);

signals:
    /**
     * @brief blobDataReady Blob 数据已准备好
     * @param data 二进制数据
     * @param suggestedFilename 建议文件名
     * @param sourceUrl 原始 blob URL
     */
    void blobDataReady(const QByteArray &data, const QString &suggestedFilename, const QString &sourceUrl);

public slots:
    /**
     * @brief onWebChannelMessage 接收来自 JS 的消息
     * @param message 消息对象
     */
    void onWebChannelMessage(const QVariant &message);

    void addDownloadTaskFromDataBase64(const QString &base64Data, const QString &filename);

};
