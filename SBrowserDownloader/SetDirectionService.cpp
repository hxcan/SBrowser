#include "SbPreCompile.h" //cout


#include "SetDirectionService.h"  //SetDirectionService

/*!
 * \brief SetDirectionService::syncSetting Sync setting. 陈欣
 */
void SetDirectionService::syncSetting()
{
    emit shouldSyncSetting(); //Emit signal, should sync setting.
} //void SetDirectionService::syncSetting()

/*!
 * \brief SetDirectionService::showDownloadManager 显示下载管理器。
 */
void SetDirectionService::showDownloadManager()
{
    emit shouldShowDownloadManager(); //发射信号，应当显示下载管理器。
} //void SetDirectionService::showDownloadManager()

/*!
 * \brief SetDirectionService::addDownloadTaskFromData 接收已解码的二进制数据，直接转发
 * \param data 已经从 Base64 解码后的原始数据（例如图片、文件等）
 * \param suggestedFilename 建议的文件名
 */
void SetDirectionService::addDownloadTaskFromData(const QByteArray &data, const QString &suggestedFilename)
{
    // ✅ 不再解码！直接向上层服务报告原始数据
    qDebug() << "[SetDirectionService] ✅ Forwarding binary data as-is. Size:" << data.size()
             << "Filename:" << suggestedFilename;

    // 直接 emit 给 DownloadHandler
    emit shouldAddDownloadTaskFromData(data, suggestedFilename);
}

/*!
 * \brief SetDirectionService::addDownloadTask 添加下载任务。
 * \param requestUrl 下载网址。
 * \param requestFileName 是否请求文件名。
 */
void SetDirectionService::addDownloadTask(const QString & requestUrl, bool requestFileName)
{
    emit shouldAddDownloadTask(requestUrl,requestFileName); //发射信号，应当添加下载任务。
} //void SetDirectionService::addDownloadTask(const QString & requestUrl, bool requestFileName) 

/*!
  * \brief SetDirectionService::quit 退出。陈欣
  */
 void SetDirectionService::quit()
 {
     emit shouldQuit(); // 发射信号，应当退出。
 } //void SetDirectionService::quit()
