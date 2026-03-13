#include "SbPreCompile.h" //QSslCipher

#include "DownloadManager.h"

#include "autosaver.h"
#include "networkaccessmanager.h"

#include <QtGui/QDesktopServices>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QFileIconProvider>

#include <QtCore/QDebug>

//#include <QtWebKit/QWebSettings>

#include "SBrowserDownloader.h" //SBrowserWindow
#include "Constants.h" //FirefoxUserAgent


/*!
    DownloadManager is a Dialog that contains a list of DownloadItems

    It is a basic download manager.  It only downloads the file, doesn't do BitTorrent,
    extract zipped files or anything fancy.
  */
DownloadManager::DownloadManager(QWidget *parent)
    : QDialog(parent)
    , m_autoSaver(new AutoSaver(this))
    , m_iconProvider(nullptr)
    , m_removePolicy(Never)
{
    setupUi(this);
    
    initializeMemberObjects(); //初始化成员变量。

    connectSignals(); //连接信号槽。
    
    
    listenTimer.start(); //启动监听定时器。
    
} //DownloadManager::DownloadManager(QWidget *parent): QDialog(parent), m_autoSaver(new AutoSaver(this)), m_iconProvider(0)

/*!
 * \brief AptF::connectSignals 连接信号槽。
 */
void DownloadManager::connectSignals()
{
    connect(&listenTimer,&QTimer::timeout,this,&DownloadManager::load); //监听定时器超时，则监听。
    //语音识别：
    
    connect(cleanupButton, SIGNAL(clicked()), this, SLOT(cleanup()));
    
} //void AptF::connectSignals()



/*!
 * \brief AptF::initializeMemberObjects 初始化成员变量。
 */
void DownloadManager::initializeMemberObjects()
{
    listenTimer.setSingleShot(true); //单发。
    
    
    downloadsView->setShowGrid(false);
    downloadsView->verticalHeader()->hide();
    downloadsView->horizontalHeader()->hide();
    downloadsView->setAlternatingRowColors(true);
    downloadsView->horizontalHeader()->setStretchLastSection(true);
    m_model = new DownloadModel(this);
    downloadsView->setModel(m_model);
    
    
    
} //void AptF::initializeMemberObjects()



/*!
 * \brief DownloadManager::~DownloadManager 析构函数。
 */
DownloadManager::~DownloadManager()
{
    m_autoSaver->changeOccurred(); //通知，发生了变更。
    m_autoSaver->saveIfNeccessary(); //必要的情况下进行保存。
    
    if (m_iconProvider)
    {
        delete m_iconProvider;
    } //if (m_iconProvider)
} //DownloadManager::~DownloadManager()

int DownloadManager::activeDownloads() const
{
    int count = 0;
    for (int i = 0; i < m_downloads.count(); ++i) {
        if (m_downloads.at(i)->stopButton->isEnabled())
            ++count;
    }
    return count;
}

/*!
 * \brief DownloadManager::download Download specified url.
 * \param url The url.
 * \param requestFileName Whether we should request file name.
 */
void DownloadManager::download(const QUrl &url, QString referrerUrl)
{
    QNetworkRequest request=QNetworkRequest(url); //网络请求对象。

    request.setRawHeader("User-Agent",FirefoxUserAgent); //使用火狐浏览器的说明字符串。
    request.setRawHeader("Referer", referrerUrl.toUtf8()); //设置引荐网址。
    
    
    //设置选项，使用全部的候选加密方法：
    QSslConfiguration config=QSslConfiguration::defaultConfiguration (); //Debug.
//        config.setProtocol (QSsl::SslV3); //Debug.

    config.setCiphers (QSslConfiguration::supportedCiphers ());

//    config.setSslOption(QSsl::SslOptionDisableCompression,true);
    request.setSslConfiguration (config); //Debug.

    download(request);
} //void DownloadManager::download(const QUrl &url, bool requestFileName = false)

/*!
 * \brief DownloadManager::download Download specified request.
 * \param request The request.
 * \param requestFileName Whether we should request file name.
 */
void DownloadManager::download(const QNetworkRequest &request)
{
    if (request.url().isEmpty()) //url is empty , don't handle it .
    {
    } //if (request.url().isEmpty()) //url is empty , don't handle it .
    else //url is not empty , handle it.
    {
        handleUnsupportedContent(m_manager->get(request)); //Issue a get request using the network access manager.
    } //else //url is not empty , handle it.
} //void DownloadManager::download(const QNetworkRequest &request, bool requestFileName)

/*!
 * \brief DownloadManager::handleUnsupportedContent Handle the download reply.
 * \param reply The download reply.
 * \param requestFileName Whether we should request file name.
 */
void DownloadManager::handleUnsupportedContent(QNetworkReply *reply)
{
    if (!reply || reply->url().isEmpty()) //The reply is invalid , or the replied url is empty.
    {
            return;
    } //if (!reply || reply->url().isEmpty()) //The reply is invalid , or the replied url is empty.
    else //It's normal, we should proceed.
    {
        QVariant header = reply->header(QNetworkRequest::ContentLengthHeader); //Get the content length header.
        bool ok; //Whether we have successfully got the size of the reply content.
        int size = header.toInt(&ok); //Try to convert the content length header into an integer.

        qDebug() << __FILE__ << __LINE__ << __func__ << tr("content length header:") << header; //Debug.

        if (ok) //Converting succeeded .
        {
            if (size==0) //The file is zero sized.
            {

            } //if (size==0) //The file is zero sized.
            else //Not zero sized.
            {
                DownloadItem *item = new DownloadItem(reply, this); //Create the download item.
                item->setNetworkAccessManager(m_manager); //设置网络访问管理器。

                item->setDryRun(dryRuncheckBox->isChecked()); //Set whether to dry run
                item->setDownloadDirectory(downloadDirectory); //Set download directory.

                addItem(item); //Add the item.
            } //else //Not zero sized.
        } //if (ok) //Converting succeeded .
        else //Don't know its size.
        {

            DownloadItem *item = new DownloadItem(reply, this);
            item->setNetworkAccessManager(m_manager); //设置网络访问管理器。
            item->setDryRun(dryRuncheckBox->isChecked()); //Set whether to dry run
            item->setDownloadDirectory(downloadDirectory); //Set download directory.


            addItem(item);
        } //else //Don't know its size.


    } //else //It's normal, we should proceed.
} //void DownloadManager::handleUnsupportedContent(QNetworkReply *reply, bool requestFileName)

/*!
 * \brief DownloadManager::addItem 添加一个下载项。
 * \param item 要添加的下载项。
 */
void DownloadManager::addItem(DownloadItem *item)
{
    connect(item, SIGNAL(statusChanged()), this, SLOT(updateRow())); //状态发生变化，则更新对应的行。
    connect(item,SIGNAL(downloadError()),this,SIGNAL(downloadError())); //下载出错，则将信号转发。


    int row = m_downloads.count(); //获取当前的行数。
    m_model->beginInsertRows(QModelIndex(), row, row); //开始插入行。
    m_downloads.append(item); //添加一个条目。
    m_model->endInsertRows(); //插入行完毕。
    updateItemCount(); //更新条目数量。
    if (row == 0) //行数为0,即为，刚刚开始下载第一个下载项。
    {
        show(); //显示本窗口。
    } //if (row == 0) //行数为0,即为，刚刚开始下载第一个下载项。

    downloadsView->setIndexWidget(m_model->index(row, 0), item); //设置当前选中的部件。
    QIcon icon = style()->standardIcon(QStyle::SP_FileIcon); //获取该下载项对应的图标。
    item->fileIcon->setPixmap(icon.pixmap(48, 48)); //设置图标。
    downloadsView->setRowHeight(row, item->sizeHint().height()); //设置行高。

    emit downloadStarted(); //发射信号，下载开始。
} //void DownloadManager::addItem(DownloadItem *item)

/*!
 * \brief DownloadManager::updateRow 更新行的状态。
 */
void DownloadManager::updateRow()
{
    DownloadItem *item = qobject_cast<DownloadItem*>(sender()); //获取发生变化的那个下载条目。
    int row = m_downloads.indexOf(item); //获取该条目的行号。
    if (-1 == row) //未能获取到行号。
    {
        return; //退出函数。
    } //if (-1 == row) //未能获取到行号。

    if (!m_iconProvider) //图标提供者不存在。
    {
        m_iconProvider = new QFileIconProvider(); //创建一个新的图标提供者。
    } //if (!m_iconProvider) //图标提供者不存在。

    QIcon icon = m_iconProvider->icon(  QFileInfo (item->m_output.fileName() ) ); //获取对应的图标。
    if (icon.isNull()) //图标不存在。
    {
        icon = style()->standardIcon(QStyle::SP_FileIcon); //获取系统的标准图标。
    } //if (icon.isNull()) //图标不存在。

    item->fileIcon->setPixmap(icon.pixmap(48, 48)); //设置图标。
    downloadsView->setRowHeight(row, item->minimumSizeHint().height()); //设置行高。

    bool remove = false; //是否要删除。

    if (item->downloadedSuccessfully() && removePolicy() == DownloadManager::SuccessFullDownload) //下载完毕。
    {
        remove = true; //要删除。
    } //if (item->downloadedSuccessfully() && removePolicy() == DownloadManager::SuccessFullDownload) //下载完毕。

    if (item->downloadedSuccessfully ()) //下载完成。
    {
        emit downloadFinished(item->m_output.fileName ()); //发射信号，下载完毕。
    } //if (item->downloadedSuccessfully ()) //下载完成。

    if (remove) //是要删除。
    {
        m_model->removeRow(row); //删除一行。
    } //if (remove) //是要删除。

    cleanupButton->setEnabled(m_downloads.count() - activeDownloads() > 0); //启用清空按钮。
} //void DownloadManager::updateRow()

DownloadManager::RemovePolicy DownloadManager::removePolicy() const
{
    return m_removePolicy;
}

void DownloadManager::setRemovePolicy(RemovePolicy policy)
{
    if (policy == m_removePolicy)
        return;
    m_removePolicy = policy;
    m_autoSaver->changeOccurred();
}

/*!
 * \brief DownloadManager::save 保存下载记录。
 */
void DownloadManager::save() const
{
    QSettings settings;
    settings.beginGroup(QLatin1String("downloadmanager"));
    QMetaEnum removePolicyEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("RemovePolicy"));
    settings.setValue(QLatin1String("removeDownloadsPolicy"), QLatin1String(removePolicyEnum.valueToKey(m_removePolicy)));
    settings.setValue(QLatin1String("size"), size());
    if (m_removePolicy == Exit)
        return;

    for (int i = 0; i < m_downloads.count(); ++i) {
        QString key = QString(QLatin1String("download_%1_")).arg(i);
        settings.setValue(key + QLatin1String("url"), m_downloads[i]->m_url);
        settings.setValue(key + QLatin1String("location"), QFileInfo(m_downloads[i]->m_output).filePath());
        settings.setValue(key + QLatin1String("done"), m_downloads[i]->downloadedSuccessfully());
    }
    int i = m_downloads.count();
    QString key = QString(QLatin1String("download_%1_")).arg(i);
    while (settings.contains(key + QLatin1String("url"))) {
        settings.remove(key + QLatin1String("url"));
        settings.remove(key + QLatin1String("location"));
        settings.remove(key + QLatin1String("done"));
        key = QString(QLatin1String("download_%1_")).arg(++i);
    }
} //void DownloadManager::save() const

/*!
 * \brief DownloadManager::load 载入下载记录。
 */
void DownloadManager::load()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("downloadmanager"));
    QSize size = settings.value(QLatin1String("size")).toSize();
    if (size.isValid())
        resize(size);
    QByteArray value = settings.value(QLatin1String("removeDownloadsPolicy"), QLatin1String("Never")).toByteArray();
    QMetaEnum removePolicyEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("RemovePolicy"));
    m_removePolicy = removePolicyEnum.keyToValue(value) == -1 ?
                        Never :
                        static_cast<RemovePolicy>(removePolicyEnum.keyToValue(value));

    int i = 0;
    QString key = QString(QLatin1String("download_%1_")).arg(i);
    while (settings.contains(key + QLatin1String("url"))) {
        QUrl url = settings.value(key + QLatin1String("url")).toUrl();
        QString fileName = settings.value(key + QLatin1String("location")).toString();
        bool done = settings.value(key + QLatin1String("done"), true).toBool();
        if (!url.isEmpty() && !fileName.isEmpty()) //网址有效。文件名有效。
        {
            DownloadItem *item = new DownloadItem(nullptr, this);
            item->setNetworkAccessManager(m_manager); //设置网络访问管理器。
            item->m_output.setFileName(fileName);
            item->fileNameLabel->setText(QFileInfo(item->m_output.fileName()).fileName());
            item->m_url = url;
            item->stopButton->setVisible(false);
            item->stopButton->setEnabled(false);
            item->tryAgainButton->setVisible(!done);
            item->tryAgainButton->setEnabled(!done);
            item->progressBar->setVisible(!done);
            
            item->tryAgain(); //重试。
            item->setDownloadDirectory(downloadDirectory); //Set download directory.

            addItem(item); //加入条目。
        } //if (!url.isEmpty() && !fileName.isEmpty()) //网址有效。文件名有效。
        
        key = QString(QLatin1String("download_%1_")).arg(++i);
    }
    cleanupButton->setEnabled(m_downloads.count() - activeDownloads() > 0);
}

void DownloadManager::setDownloadDirectory(const QString &value)
{
    downloadDirectory = value;
} //void DownloadManager::load()

void DownloadManager::cleanup()
{
    if (m_downloads.isEmpty())
        return;
    m_model->removeRows(0, m_downloads.count());
    updateItemCount();
    if (m_downloads.isEmpty() && m_iconProvider) {
        delete m_iconProvider;
        m_iconProvider = nullptr;
    }
    m_autoSaver->changeOccurred();
}

void DownloadManager::updateItemCount()
{
    int count = m_downloads.count();
    itemCount->setText(count == 1 ? tr("1 Download") : tr("%1 Downloads").arg(count));
}

DownloadModel::DownloadModel(DownloadManager *downloadManager, QObject *parent)
    : QAbstractListModel(parent)
    , m_downloadManager(downloadManager)
{
}

QVariant DownloadModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount(index.parent()))
        return QVariant();
    if (role == Qt::ToolTipRole)
        if (!m_downloadManager->m_downloads.at(index.row())->downloadedSuccessfully())
            return m_downloadManager->m_downloads.at(index.row())->downloadInfoLabel->text();
    return QVariant();
}

int DownloadModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : m_downloadManager->m_downloads.count();
}

bool DownloadModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    int lastRow = row + count - 1;
    for (int i = lastRow; i >= row; --i) {
        if (m_downloadManager->m_downloads.at(i)->downloadedSuccessfully()
            || m_downloadManager->m_downloads.at(i)->tryAgainButton->isEnabled()) {
            beginRemoveRows(parent, i, i);
            m_downloadManager->m_downloads.takeAt(i)->deleteLater();
            endRemoveRows();
        }
    }
    m_downloadManager->m_autoSaver->changeOccurred();
    return true;
}

/*!
 * \brief DownloadManager::setNetworkAccessManager 设置网络访问管理器。
 * \param ntAccsMngr4WebPages 要设置的网络访问管理器。
 */
void DownloadManager::setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages)
{
    m_manager=ntAccsMngr4WebPages; //记录。

    return;
} //void DownloadManager::setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages) //!<设置网络访问管理器。
