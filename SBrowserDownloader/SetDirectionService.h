#ifndef SETDIRECTIONSERVICE_H
#define SETDIRECTIONSERVICE_H

#include "rep_SetDirectionService_source.h"

/*!
 * \brief The SetDirectionService class
 */
class SetDirectionService : public SetDirectionServiceSimpleSource
{
    Q_OBJECT

public:
    virtual void quit() override; //!< 退出。陈欣
    virtual void syncSetting() override; //!<Sync setting. 陈欣
    virtual void showDownloadManager() override; //!<显示下载管理器。
    virtual void addDownloadTask(const QString & requestUrl, bool requestFileName) override; //!<添加下载任务。
    // ✅ 新增：处理 Blob 数据下载
    virtual void addDownloadTaskFromData(const QByteArray &data, const QString &suggestedFilename) override;

signals:
    void shouldSyncSetting() const; //!<signal, should sync setting.
    void shouldShowDownloadManager(); //!<信号，应当显示下载管理器。
    void shouldAddDownloadTask(const QString & requestUrl, bool requestFileName); //!<信号，应当添加下载任务。
    void shouldQuit(); //!< 信号，应当退出。

    // ✅ 新增：应当添加基于二进制数据的下载任务
    void shouldAddDownloadTaskFromData(const QByteArray &data, const QString &suggestedFilename);
}; //class EXISTQTSHARED_EXPORT GameInfo:public QObject

#endif // GAMEINFO_H
