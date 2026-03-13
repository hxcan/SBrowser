#include "SbPreCompile.h" //QSslCipher

#include "DownloadManager.h"

//#include "networkaccessmanager.h"

//#include <QtGui/QDesktopServices>
//#include <QtWidgets/QFileDialog>
//#include <QtWidgets/QHeaderView>
//#include <QtWidgets/QFileIconProvider>

//#include <QtCore/QDebug>

//#include <QtWebKit/QWebSettings>

#include "SBrowserCppWebEngine.h" //SBrowserWindow
#include "Constants.h" //FirefoxUserAgent


/*!
    DownloadManager is a Dialog that contains a list of DownloadItems

    It is a basic download manager.  It only downloads the file, doesn't do BitTorrent,
    extract zipped files or anything fancy.
  */
DownloadManager::DownloadManager(QWidget *parent) : QObject(parent) , m_removePolicy(Never)
{
    initializeMemberObjects(); //初始化成员变量。

} //DownloadManager::DownloadManager(QWidget *parent): QDialog(parent), m_autoSaver(new AutoSaver(this)), m_iconProvider(0)

/*!
 * \brief AptF::initializeMemberObjects 初始化成员变量。
 */
void DownloadManager::initializeMemberObjects()
{
} //void AptF::initializeMemberObjects()

/*!
 * \brief DownloadManager::download Download specified url.
 * \param url The url.
 * \param requestFileName Whether we should request file name.
 */
void DownloadManager::download(const QUrl &url, QString referrerUrl, bool requestFileName )
{
    QNetworkRequest request=QNetworkRequest(url); //网络请求对象。

    request.setRawHeader("User-Agent",FirefoxUserAgent); //使用火狐浏览器的说明字符串。
    request.setRawHeader("Referer", referrerUrl.toUtf8()); //设置引荐网址。
    
    
    //设置选项，使用全部的候选加密方法：
    QSslConfiguration config=QSslConfiguration::defaultConfiguration (); //Debug.
//        config.setProtocol (QSsl::SslV3); //Debug.

    request.setSslConfiguration (config); //Debug.

    download(request, requestFileName);
} //void DownloadManager::download(const QUrl &url, bool requestFileName = false)

/*!
 * \brief DownloadManager::download Download specified request.
 * \param request The request.
 * \param requestFileName Whether we should request file name.
 */
void DownloadManager::download(const QNetworkRequest &request, bool requestFileName)
{
    if (request.url().isEmpty()) //url is empty , don't handle it .
    {
    } //if (request.url().isEmpty()) //url is empty , don't handle it .
    else //url is not empty , handle it.
    {
//        handleUnsupportedContent(m_manager->get(request), requestFileName); //Issue a get request using the network access manager.
        
        
        distributeWorkToDownloader(request,requestFileName); //将任务分发给下载器。
    } //else //url is not empty , handle it.
} //void DownloadManager::download(const QNetworkRequest &request, bool requestFileName)

/*!
 * \brief DownloadManager::syncSetting 设置对话框确认关闭，则 Sync 设置信息。陈欣
 */
void DownloadManager::syncSetting() const
{
    rswitch->shouldSyncSetting(); //Emit signal, should sync setting.
} //void DownloadManager::syncSetting()

/*!
 * \brief DownloadManager::distributeWorkToDownloader 将任务分发给下载器。
 * \param request 下载请求对象。
 * \param requestFileName 是否要请求文件名。
 */
void DownloadManager::distributeWorkToDownloader(const QNetworkRequest & request, bool requestFileName)
{
    rswitch->shouldAddDownloadTask(request.url().toString(),requestFileName); //发射信号，应当添加下载任务。
} //void DownloadManager::distributeWorkToDownloader(const QNetworkRequest & request, bool requestFileName)


void DownloadManager::downloadBlobData(const QByteArray &data, const QString &suggestedFilename)
{
    // 通过 Client 的信号发送任务
    emit rswitch->shouldAddDownloadTaskFromData(data, suggestedFilename);
    qDebug() << "Emitted shouldAddDownloadTaskFromData, size =" << data.size();
}


/*!
 * \brief DownloadManager::quit 要求下载器进程退出。
 */
void DownloadManager::quit()
{
    rswitch->shouldQuit(); // 发射信号，应当退出。
} //void DownloadManager::quit()

DownloadManager::RemovePolicy DownloadManager::removePolicy() const
{
    return m_removePolicy;
}

void DownloadManager::setRemovePolicy(RemovePolicy policy)
{
    if (policy == m_removePolicy)
        return;
    m_removePolicy = policy;
}

Client *DownloadManager::getRswitch() const
{
    return rswitch;
}

void DownloadManager::setRswitch(Client *value)
{
    rswitch = value;
} //void DownloadManager::load()

/*!
 * \brief DownloadManager::setNetworkAccessManager 设置网络访问管理器。
 * \param ntAccsMngr4WebPages 要设置的网络访问管理器。
 */
void DownloadManager::setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages)
{
    m_manager=ntAccsMngr4WebPages; //记录。

    return;
} //void DownloadManager::setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages) //!<设置网络访问管理器。
