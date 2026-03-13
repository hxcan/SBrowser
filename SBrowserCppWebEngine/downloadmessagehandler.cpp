#include "downloadmessagehandler.h"
#include <QVariantMap>
#include <QDebug>

void DownloadMessageHandler::onWebChannelMessage(const QVariant &message)
{
    QVariantMap msg = message.toMap();
    QString type = msg["type"].toString();

    if (type == "blob_download_data") {
        // 提取 data 数组并转为 QByteArray
        QVariantList dataList = msg["data"].toList();
        QByteArray data;
        data.reserve(dataList.size());
        for (const QVariant &v : dataList) {
            data.append(static_cast<char>(v.toInt()));
        }

        QString filename = msg["suggestedFilename"].toString();
        QString url = msg["url"].toString();

        emit blobDataReady(data, filename, url);
    }
    else if (type == "blob_download_error") {
        qWarning() << "Blob download error:" << msg["error"] << "URL:" << msg["url"];
    }
}

#include "downloadmessagehandler.h"
#include <QByteArray>
#include <QFileInfo>
#include <QDebug>

/**
 * @brief 从 Base64 数据中提取原始数据并发射信号
 * @param base64Data Base64 编码的数据
 * @param suggestedFilename 建议的文件名
 * @param sourceUrl（可选）源 URL（如果 JS 能传的话）
 *
 * 本方法仅解码并转发，不进行文件写入
 */
void DownloadMessageHandler::addDownloadTaskFromDataBase64(const QString &base64Data, const QString &suggestedFilename)
{
    qDebug() << "[C++] addDownloadTaskFromDataBase64:" << suggestedFilename << "Base64 length:" << base64Data.length();

    // ✅ 1. 输入验证
    if (base64Data.isEmpty()) {
        qWarning() << "[C++] ❌ Base64 data is empty!";
        return;
    }

    // ✅ 2. Base64 解码
    QByteArray decodedData = QByteArray::fromBase64(base64Data.toUtf8());

    cout << __FILE__ << ", " << __LINE__ << ", " << __func__  <<  ", decodeed data size: " << decodedData.size () << endl;

    if (decodedData.isEmpty()) {
        qWarning() << "[C++] ❌ Failed to decode Base64 data!";
        return;
    }

    qDebug() << "[C++] ✅ Decoded successfully. Size:" << decodedData.size() << "bytes";

    // ✅ 3. 提取安全文件名（防止路径注入）
    QString safeFilename = QFileInfo(suggestedFilename).fileName();
    if (safeFilename.isEmpty()) {
        safeFilename = "download_bin";
    }

    // ✅ 4. 发射信号！数据已就绪 ✅
    emit blobDataReady(decodedData, safeFilename, "blob:from-js-webchannel");

    // 🎉 完成！其他模块监听 blobDataReady 即可处理数据
}

DownloadMessageHandler::DownloadMessageHandler(QObject *parent) : QObject(parent)
{}
