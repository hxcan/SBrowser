#include "SbPreCompile.h" //QFileDialog

#include "DownloadItem.h" //DownloadItem
#include "SBrowserDownloader.h" //SBrowserWindow

#include "Constants.h" //FirefoxUserAgent

/*!
 * \brief DownloadItem::connectSignals 连接信号槽。
 */
void DownloadItem::connectSignals()
{
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop())); //Clicked stop button, then stop the download.
    connect(openButton, SIGNAL(clicked()), this, SLOT(open()));
//    connect(copyUr)
    
    connect(tryAgainButton, &QPushButton::clicked, this, &DownloadItem::tryAgain); //重试按钮被按，则重试。
    
    
    connect(tryAgainButton,&QPushButton::clicked,&retryTimer,&QTimer::stop); //重试按钮被按，则停止重试定时器。
    connect (&retryTimer,&QTimer::timeout,this,&DownloadItem::tryAgain); //重试定时器超时，则重试。
    connect (&retryTimer,&QTimer::timeout,this,&DownloadItem::countRetryTimes); //重试定时器超时，则对重试次数计数。
} //void DownloadItem::connectSignals()

/*!
 * \brief DownloadItem::setDryRun Set whether to dry run
 * \param dryRuncheckBoxisChecked Dry run or not
 */
void DownloadItem::setDryRun(const bool dryRuncheckBoxisChecked)
{
    dryRun=dryRuncheckBoxisChecked;
} //void DownloadItem::setDryRun(bool dryRuncheckBoxisChecked)

/*!
 * \brief DownloadItem::countRetryTimes 对重试次数计数。
 */
void DownloadItem::countRetryTimes()
{
    retryTimes++;
} //void DownloadItem::countRetryTimes()

/*!
    DownloadItem is a widget that is displayed in the download manager list.
    It moves the data from the QNetworkReply into the QFile as well
    as update the information/progressbar and report errors.
 */
DownloadItem::DownloadItem(QNetworkReply *rpl, QWidget *prn): QWidget(prn), m_reply(rpl)
{
    m_bytesReceived=0; //已接收的数据数量初始化为0.

    setupUi(this);
    QPalette p = downloadInfoLabel->palette();
    p.setColor(QPalette::Text, Qt::darkGray);
    downloadInfoLabel->setPalette(p);
    progressBar->setMaximum(0);
    tryAgainButton->hide();

    connectSignals(); //连接信号槽。

    init(); //初始化。
} //DownloadItem::DownloadItem(QNetworkReply *rpl, bool rFn, QWidget *prn): QWidget(prn), m_reply(rpl), m_requestFileName(rFn)

/*!
 * \brief DownloadItem::init 初始化。
 */
void DownloadItem::init()
{
    if (!m_reply) //回复对象不存在。
    {
        return; //退出函数。
    } //if (!m_reply) //回复对象不存在。

    // attach to the m_reply
    m_url = m_reply->url(); //记录下载的网址。
    m_reply->setParent(this); //设置亲代对象。

    connect(m_reply, &QNetworkReply::readyRead, this, &DownloadItem::downloadReadyRead); //有数据可读，则读取。
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(error(QNetworkReply::NetworkError))); //网络出错，则处理出错情况。
    connect(m_reply,&QNetworkReply::redirected,this,&DownloadItem::reportRedirected); //重定向，则报告重定向。

    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadProgress(qint64,qint64))); //下载进度变化，则处理进度变化情况。
    connect(m_reply, SIGNAL(metaDataChanged()),this, SLOT(metaDataChanged())); //元数据发生变化，则处理元数据的变化。
    connect(m_reply, SIGNAL(finished()),this, SLOT(finished())); //下载完毕，则处理下载完毕情况。

    // reset info
    downloadInfoLabel->clear(); //清空下载信息文字标签页。
    progressBar->setValue(0); //进度值归零。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("init,calling getFileName"); //Debug.

    getFileName(); //获取文件名。

    // start timer for the download estimation
    m_downloadTime.start(); //启动定时器。

    if (m_reply->error() != QNetworkReply::NoError) //网络出错。
    {
        error(m_reply->error()); //处理错误情况。
        finished(); //下载完毕。
    } //if (m_reply->error() != QNetworkReply::NoError) //网络出错。

    retryTimer.setSingleShot (true); //单发。
    retryTimer.setInterval (1000); //超时时间，1秒。
} //void DownloadItem::init()

/*!
 * \brief DownloadItem::reportRedirected 重定向，则报告重定向。
 * \param url 重定向的网址。
 */
void DownloadItem::reportRedirected(const QUrl &url)
{
    Q_UNUSED(url)
} //void DownloadItem::reportRedirected(const QUrl &url)

/*!
 * \brief DownloadItem::getFileName 获取文件保存时的文件名。
 */
void DownloadItem::getFileName()
{
//    QSettings settings; //设置对象。
//    settings.beginGroup(QLatin1String("downloadmanager")); //开启分组，下载管理器。

    QDir homeDir(QStandardPaths::writableLocation (QStandardPaths::HomeLocation)); //家目录。
//    QDir downloadDir(QStandardPaths::writableLocation (QStandardPaths::HomeLocation)+tr("/Download/")); //下载目录。
    QDir downloadDir(downloadDirectory); //下载目录。

    if (downloadDir.exists()) //下载目录存在。
    {
    } //if (downloadDir.exists()) //下载目录存在。
    else //下载目录不存在。
    {
        homeDir.mkpath(downloadDirectory); //创建下载目录。
    } //else //下载目录不存在。

    QString defaultLocation=QStandardPaths::writableLocation (QStandardPaths::HomeLocation)+tr("/Download/"); //默认下载目录。

//    QString downloadDirectory = settings.value(QLatin1String("downloadDirectory"), defaultLocation).toString();

    if (!downloadDirectory.isEmpty()) //目录字符串有效。
    {
        downloadDirectory += QLatin1Char('/'); //追加斜杠。
    } //if (!downloadDirectory.isEmpty()) //目录字符串有效。

    QString defaultFileName = saveFileName(downloadDirectory); //Get the default file name.

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("default file name:") << defaultFileName; //Debug.

    QString fileName = defaultFileName; //Final file name.

    m_output.setFileName(fileName); //Set the file name.


    autoFitFileName(); //Auto fit file name. Elide too long file name.


} //void DownloadItem::getFileName()

/*!
 * \brief DownloadItem::autoFitFileName Auto fit file name. Elide too long file name.
 */
void DownloadItem::autoFitFileName()
{
    auto imageFileName=QFileInfo(m_output.fileName()).fileName(); //File name.

    QFont currentFont=fileNameLabel->font (); //获取当前字体。
    QFontMetrics fontMetrics(currentFont);
    QString elidedText=fontMetrics.elidedText (imageFileName,Qt::ElideMiddle, fileNameLabel->width ()); //获取带省略号的文字内容。

    fileNameLabel->setText(elidedText); //显示文件名。


//    fileNameLabel->setText(QFileInfo(m_output.fileName()).fileName()); //Show the file name.

} //void DownloadItem::autoFitFileName()

/*!
 * \brief DownloadItem::saveFileName 获取适当的保存文件名。
 * \param directory 目标目录。
 * \return 适当的保存文件名。
 */
QString DownloadItem::saveFileName(const QString &directory) const
{
    QString name =""; //完整的文件名。

    //先尝试从协议头中取得文件名：
    QString onlyFileName=getHeaderFileName(m_reply); //从协议头中取得文件名。
    QString baseName=""; //基本名字。
    QString endName=""; //后缀名字。

    if (onlyFileName.isEmpty ()) //为空，则尝试根据网址来取得文件名。
    {
        QString path = m_url.path(); //获取请求的网址。
        QFileInfo info(path); //构造文件信息对象。
        baseName = info.completeBaseName(); //获取完整的基本名字。
        endName = info.suffix(); //获取后缀名字。

        if (baseName.isEmpty())  //基本名字为空。
        {
            baseName = QLatin1String("unnamed_download"); //使用默认的基本名字。
            qDebug() << "DownloadManager:: downloading unknown file:" << m_url; //Debug.
        } //if (baseName.isEmpty())  //基本名字为空。

        if (endName.isEmpty ()) //后缀名字为空。
        {
            endName=tr("SBrowser"); //使用默认的后缀名字。
        } //if (endName.isEmpty ()) //后缀名字为空。


#ifndef Q_OS_MACOS
        baseName=ValidFileName::filterForFileName (baseName,ValidFileName::GenericFileName); //过滤文件名前缀。
        endName=ValidFileName::filterForFileName (endName,ValidFileName::GenericFileName); //过滤文件名后缀。
#endif


        name = directory + baseName + QLatin1Char('.') + endName; //构造完整的文件名。

    } //if (onlyFileName.isEmpty ()) //为空，则尝试根据网址来取得文件名。
    else //不为空，则就使用这个。
    {
#ifndef Q_OS_MACOS
        onlyFileName=ValidFileName::filterForFileName (onlyFileName,ValidFileName::GenericFileName); //过滤文件名。
#endif

        name=directory + onlyFileName; //构造完整的文件名。
    } //else //不为空，则就使用这个。

    if (QFile::exists(name))  //该文件已经存在了。
    {
        if (supportRange) //We know the content length.
        {
            QFile lastUsedDownloadFile(downloadFileName); //The last used download file.
            qint64 lastUsedFileSize=lastUsedDownloadFile.size(); //Get the size of the last used download file.

            if  ((lastUsedDownloadFileExists()) && (m_bytesReceived==lastUsedFileSize)) //The last file size is correct.
            {
                //use last file name:

                name=downloadFileName; //Use last download file name.
            } //if (m_bytesReceived==lastUsedFileSize) //The last file size is correct.
            else //last used file size mismatch.
            {
                QFile existingFile(name); //The existing file.
                qint64 existingFileSize=existingFile.size(); //Get the size of the existing file.

                if (m_bytesReceived==existingFileSize) //This is a part file.
                {
                    //just use this name.
                } //if (m_bytesReceived==existingFileSize) //This is a part file.
                else //This is not the part file we are downloading.
                {
                    //We should use a new file name.


                    int i = 1; //文件改名计数器。
                    do //改了名还存在。
                    {
                        name = directory + baseName + QLatin1Char('-') + QString::number(i++) + QLatin1Char('.') + endName; //构造个新的名字。
                    } while (QFile::exists(name)); //do //改了名还存在。


                } //else //This is the part file we are downloading.
            } //else //last used file size mismatch.


        } //if (contentLength) //We know the content length.
        else //We don't know the content length.
        {
            // already exists, don't overwrite
            int i = 1; //文件改名计数器。
            do //改了名还存在。
            {
                name = directory + baseName + QLatin1Char('-') + QString::number(i++) + QLatin1Char('.') + endName; //构造个新的名字。
            } while (QFile::exists(name)); //do //改了名还存在。

        } //else //We don't know the content length.

    } //if (QFile::exists(name))  //该文件已经存在了。

    return name;
} //QString DownloadItem::saveFileName(const QString &directory) const

/*!
 * \brief DownloadItem::lastUsedDownloadFileExists Whether the last used download file exists.
 * \return Whether the last used download file exists.
 */
bool DownloadItem::lastUsedDownloadFileExists() const
{
    return QFile::exists (downloadFileName);
} //bool DownloadItem::lastUsedDownloadFileExists() const

/*!
 * \brief DownloadItem::copyUrl 复制下载地址。
 */
void DownloadItem::copyUrl()
{
    auto downloadUrl=m_url.toString(); //获取网址。

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(downloadUrl);

} //void DownloadItem::copyUrl()

/*!
 * \brief DownloadItem::stop 停止下载。
 */
void DownloadItem::stop()
{
    explicitlyStop=true; //Explicitly clicked stop button.

    setUpdatesEnabled(false);
    stopButton->setEnabled(false);
    stopButton->hide();
    tryAgainButton->setEnabled(true);
    tryAgainButton->show();
    setUpdatesEnabled(true);

    if (m_reply) //回复对象存在。
    {
        m_reply->abort();
    } //if (m_reply) //回复对象存在。
} //void DownloadItem::stop()

/*!
 * \brief DownloadItem::open 打开已下载的目标文件。
 */
void DownloadItem::open()
{
    QFileInfo info(m_output); //创建对应的文件信息对象。
    QUrl url = QUrl::fromLocalFile(info.absoluteFilePath()); //构造完整的路径。



    QDesktopServices::openUrl(url); //打开该路径。
} //void DownloadItem::open()

/*!
 * \brief DownloadItem::tryAgain 重试。
 */
void DownloadItem::tryAgain()
{
    if (!tryAgainButton->isEnabled()) //按钮被禁用。
    {
    } //if (!tryAgainButton->isEnabled()) //按钮被禁用。
    else //Button is enabled.
    {
        tryAgainButton->setEnabled(false); //禁用按钮。
        tryAgainButton->setVisible(false); //隐藏按钮。
        stopButton->setEnabled(true); //启用停止按钮。
        stopButton->setVisible(true); //显示停止按钮。
        progressBar->setVisible(true); //显示进度条。
        
        QUrl url2Dld; //要下载的URL。

        if (m_reply) //回复对象存在。
        {
            m_reply->deleteLater(); //日后删除。

            url2Dld=m_reply->url (); //要下载的URL。
        }
        else //回复对象不存在。
        {
            url2Dld=m_url; //使用外部设置的下载网址。
        } //else //回复对象不存在。

            QNetworkRequest networkRequest=QNetworkRequest(url2Dld); //Create the network request.
            
            networkRequest.setRawHeader("User-Agent",FirefoxUserAgent); //使用火狐浏览器的说明字符串。

            //试验TLS：
            QSslConfiguration config=QSslConfiguration::defaultConfiguration (); //Debug.
            config.setProtocol (QSsl::TlsV1_0); //Debug.
            networkRequest.setSslConfiguration (config); //Debug.

            if (supportRange) //We know the content length.
            {
              int DownedSize=m_bytesReceived; //Get the size of previous download.

              rangeStartSize=DownedSize; //Remember the range start size.

              QString Range="bytes="+QString::number(DownedSize)+"-";//告诉服务器从DownedSize起开始传输

              networkRequest.setRawHeader ("Range",Range.toUtf8 ()); //Set the range header.
            } //if (contentLength) //We know the content length.

            QString userAgent=networkRequest.rawHeader("User-Agent"); //Debug.
            
            m_reply=networkAccessManager->get (networkRequest); //重新开始下载。

        if (supportRange) //We know the content length.
        {
        } //if (contentLength) //We know the content length.
        else //We don't know the content length.
        {
            if (m_output.exists()) //文件存在。
            {
                m_output.remove(); //删除文件。
            } //if (m_output.exists()) //文件输出流存在。
        } //else //We don't know the content length.

        explicitlyStop=false; //Clear the explicit stop status flag.

        init(); //初始化。
        emit statusChanged(); //发射信号，状态改变。
    } //else //Button is enabled.
} //void DownloadItem::tryAgain()

/*!
 * \brief DownloadItem::downloadReadyRead Read downloaded data.
 */
void DownloadItem::downloadReadyRead()
{
    if (!m_output.isOpen()) //The output file is not open.
    {
        // in case someone else has already put a file there
        getFileName(); //Auto Choose right file name.

        QIODevice::OpenMode openMode; //The open mode.

        if (supportRange) //We know the content length.
        {
            openMode=QIODevice::Append; //Append.
        } //if (contentLength) //We know the content length.
        else //We don't know the content length.
        {
            openMode=QIODevice::WriteOnly; //Write only.
        } //else //We don't know the content length.

        qDebug() << __FILE__ << __LINE__ << __func__ << tr("downloadReadyRead,opening by mode:") << openMode; //Debug.

        bool openResult=true;

        if (dryRun) // Dry run, not write file at all.
        {
        } //if (dryRun) // Dry run, not write file at all.
        else // Not dry run.
        {
            openResult=m_output.open(openMode); // Try to open file.
        } //else // Not dry run.

        if (!openResult) //open failed.
        {
            downloadInfoLabel->setText(tr("Error opening save file: %1").arg(m_output.errorString()));
            stopButton->click();
            emit statusChanged();
            return;
        } //if (!m_output.open(openMode)) //Try to open the file for writing.
        else //Open successfully.
        {
            //Remember the download file name , in case we have to continue transfer the file due to connection broken.
            downloadFileName=m_output.fileName (); //Remember the file name.

            emit statusChanged(); //Notify status changed.
        } //else //Open successfully.
    } //if (!m_output.isOpen()) //The output file is not open.

    auto downloadConente=m_reply->readAll(); //Readd all content

    if (dryRun) // Dry run, no file writing
    {
    } //if (dryRun) //Dry run, no file wrting
    else //not dry run, write file
    {
        if (-1 == m_output.write(downloadConente)) //Try to write, and check failure
        {
            downloadInfoLabel->setText(tr("Error saving: %1").arg(m_output.errorString()));
            stopButton->click();
        } //if (-1 == m_output.write(m_reply->readAll())) //Try to write, and check failure
    } //else //not dry run, write file
} //void DownloadItem::downloadReadyRead()

/*!
 * \brief DownloadItem::error 处理下载出错事件。
 */
void DownloadItem::error(QNetworkReply::NetworkError errorCode)
{
    Q_UNUSED(errorCode)

    downloadInfoLabel->setText(tr("Network Error: %1").arg(m_reply->errorString())); //显示错误信息。
    tryAgainButton->setEnabled(true); //启用重试按钮。
    tryAgainButton->setVisible(true); //显示重试按钮。

    emit downloadError(); //发射信号，下载出错。

    if (explicitlyStop) //User explicitly clicked the stop button.
    {
    } //if (explicitlyStop) //User explicitly clicked the stop button.
    else //Normal error.
    {
        if (retryTimes>=MaxDownloadRetryTimes) //超过最大重试次数。
        {
        } //if (retryTimes>=MaxDownloadRetryTimes) //超过最大重试次数。
        else //未超过最大重试次数。
        {
            retryTimer.start(); //启动重试定时器。
        } //else //未超过最大重试次数。
    } //else //Normal error.
} //void DownloadItem::error(QNetworkReply::NetworkError)

/*!
 * \brief DownloadItem::metaDataChanged Handle meta data changes.
 */
void DownloadItem::metaDataChanged()
{
    QByteArray acceptRanges=m_reply->rawHeader ("Accept-Ranges"); //Get the header of accept ranges.

    if (acceptRanges=="bytes") //Accept ranges.
    {
        supportRange=true; //Remember , accept ranges.
    } //if (acceptRanges=="bytes") //Accept ranges.
} //void DownloadItem::metaDataChanged()

/*!
 * \brief DownloadItem::downloadProgress 下载进度更新。
 * \param bytesReceived 已接收的字节数。
 * \param bytesTotal 总的字节数。
 */
void DownloadItem::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{


    m_bytesReceived =rangeStartSize+ bytesReceived; //记录已接收的字节数。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("download progress,bytes received:") << bytesReceived << tr("bytes total:") << bytesTotal << tr("m_bytesReceived:") << m_bytesReceived << tr("range start size:") << rangeStartSize << tr("support ranges:") << supportRange ; //Debug.

    if (bytesTotal == -1)  //不确定总字节数。
    {
        progressBar->setValue(0); //当前值重置为0.
        progressBar->setMaximum(0); //最大值设置为0,将使得进度条进入不确定状态。
    } //if (bytesTotal == -1)  //不确定总字节数。
    else //确定总字节数。
    {
        qint64 bytesTotalWithContentLength=(bytesTotal+rangeStartSize); //The bytes total take contentLength into account.

        int currentValue=0; //计算出的当前进度值。
        int maximumValue=0; //计算出的最大进度值。

        if (bytesTotalWithContentLength>=(qint64)(2147483648)) //超过32位了。
        {
            currentValue=((int)((m_bytesReceived*ProgressBarMaximumValueFor64Bit)/bytesTotalWithContentLength)); //换算进度值。
            maximumValue=ProgressBarMaximumValueFor64Bit; //使用小一点的最大值。
        } //if (bytesTotal>=4,294,967,296) //超过32位了。
        else //未超过32位。
        {
            currentValue=m_bytesReceived; //就用这个进度值。
            maximumValue=bytesTotalWithContentLength; //就用这个最大值。
        } //else //未超过32位。

        animateProgress(currentValue,maximumValue); //以动画显示下载进度。
    } //else //确定总字节数。

    updateInfoLabel(); //更新信息标签。

    return;
} //void DownloadItem::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)

void DownloadItem::updateInfoLabel()
{
    if (m_reply->error() == QNetworkReply::NoError)
        return;

    qint64 bytesTotal = progressBar->maximum();
    bool running = !downloadedSuccessfully();

    // update info label
    double speed = m_bytesReceived * 1000.0 / m_downloadTime.elapsed();
    double timeRemaining = ((double)(bytesTotal - m_bytesReceived)) / speed;
    QString timeRemainingString = tr("seconds");
    if (timeRemaining > 60) {
        timeRemaining = timeRemaining / 60;
        timeRemainingString = tr("minutes");
    }
    timeRemaining = floor(timeRemaining);

    // When downloading the eta should never be 0
    if (timeRemaining == 0)
        timeRemaining = 1;

    QString info;
    if (running) {
        QString remaining;
        if (bytesTotal != 0)
            remaining = tr("- %4 %5 remaining")
            .arg(timeRemaining)
            .arg(timeRemainingString);
        info = tr("%1 of %2 (%3/sec) %4")
            .arg(dataString(m_bytesReceived))
            .arg(bytesTotal == 0 ? tr("?") : dataString(bytesTotal))
            .arg(dataString((int)speed))
            .arg(remaining);
    } else {
        if (m_bytesReceived == bytesTotal)
            info = dataString(m_output.size());
        else
            info = tr("%1 of %2 - Stopped")
                .arg(dataString(m_bytesReceived))
                .arg(dataString(bytesTotal));
    }
    downloadInfoLabel->setText(info);
}

QString DownloadItem::dataString(int size) const
{
    QString unit;
    if (size < 1024) {
        unit = tr("bytes");
    } else if (size < 1024*1024) {
        size /= 1024;
        unit = tr("kB");
    } else {
        size /= 1024*1024;
        unit = tr("MB");
    }
    return QString(QLatin1String("%1 %2")).arg(size).arg(unit);
}

/*!
 * \brief DownloadItem::downloading 查询状态，是否正在下载。
 * \return 是否正在下载。
 */
bool DownloadItem::downloading() const
{
    return (progressBar->isVisible());
} //bool DownloadItem::downloading() const

/*!
 * \brief DownloadItem::downloadedSuccessfully 是否已经成功下载。
 * \return 是否已经成功下载。
 */
bool DownloadItem::downloadedSuccessfully() const
{
    return (stopButton->isHidden() && tryAgainButton->isHidden());
} //bool DownloadItem::downloadedSuccessfully() const

/*!
 * \brief DownloadItem::finished 下载完毕。
 */
void DownloadItem::finished()
{
    progressBar->hide(); //隐藏进度条。
    stopButton->setEnabled(false); //禁用停止按钮。
    stopButton->hide(); //隐藏停止按钮。
    m_output.close(); //关闭文件输出流。
    updateInfoLabel(); //更新信息标签。
    emit statusChanged(); //发射信号，状态发生变化。
}

void DownloadItem::setDownloadDirectory(const QString &value)
{
    downloadDirectory = value;
} //void DownloadItem::finished()

/*!
 * \brief DownloadItem::setNetworkAccessManager 设置网络访问管理器。
 * \param networkAccessManager2Set 要设置的网络访问管理器。
 */
void DownloadItem::setNetworkAccessManager(QNetworkAccessManager * networkAccessManager2Set)
{
    networkAccessManager=networkAccessManager2Set; //记录。

    return;
} //void DownloadItem::setNetworkAccessManager(QNetworkAccessManager * networkAccessManager2Set)

/*!
 * \brief DownloadItem::getHeaderFileName 从协议头中取得文件名。
 * \param reply 回复对象。
 * \return 文件名。
 */
QString DownloadItem::getHeaderFileName( QNetworkReply * reply) const
{
    QString result=""; //结果。

    QString cnttDscrptHeader=reply->header(QNetworkRequest::ContentDispositionHeader).toString (); //获取内容描述协议头内容。





    cnttDscrptHeader=reply->rawHeader ("Content-Disposition"); //获取原始协议头内容。



    QStringList cnttDscrptHeaderList=cnttDscrptHeader.split (";"); //使用分号分割。

    if (cnttDscrptHeaderList.length ()>1) //分割之后有内容。
    {
        QString fileNameIs=cnttDscrptHeaderList[1]; //获取“filename=”部分。

        QStringList fileNameIsList=fileNameIs.split ("="); //使用等号分割。

        if (fileNameIsList.length ()>1) //分割之后有内容。
        {
            QString fileNameQuota=fileNameIsList[1]; //获取带引号的文件名部分。

            QString fileName=fileNameQuota.replace ("\"",""); //去掉引号。

            result=fileName; //结果。

        } //if (fileNameIsList.length ()>1) //分割之后有内容。


    } //if (cnttDscrptHeaderList.length ()>1) //分割之后有内容。


    return result;
} //QString DownloadItem::getHeaderFileName( QNetworkReply * reply) const

/*!
 * \brief DownloadItem::animateProgress 以动画显示下载进度。
 * \param currentValue 当前进度值。
 * \param maximumValue 最大进度值。
 */
void DownloadItem::animateProgress(int currentValue,int maximumValue)
{
    //动画更新最大值：
    QPropertyAnimation * maximumPropertyAnimation=new QPropertyAnimation; //创建动画。
    maximumPropertyAnimation->setTargetObject (progressBar); //设置目标对象。
    maximumPropertyAnimation->setDuration (progressAnimationDuration); //设置持续时间。
    maximumPropertyAnimation->setPropertyName ("maximum"); //设置属性名字。
    maximumPropertyAnimation->setStartValue (progressBar->maximum ()); //设置启动值。
    maximumPropertyAnimation->setEndValue (maximumValue); //设置终止值。
    maximumPropertyAnimation->start (QAbstractAnimation::DeleteWhenStopped); //启动。

    //动画更新最大值：
    QPropertyAnimation * valuePropertyAnimation=new QPropertyAnimation; //创建动画。
    valuePropertyAnimation->setTargetObject (progressBar); //设置目标对象。
    valuePropertyAnimation->setDuration (progressAnimationDuration); //设置持续时间。
    valuePropertyAnimation->setPropertyName ("value"); //设置属性名字。
    valuePropertyAnimation->setStartValue (progressBar->value ()); //设置启动值。
    valuePropertyAnimation->setEndValue (currentValue); //设置终止值。
    valuePropertyAnimation->start (QAbstractAnimation::DeleteWhenStopped); //启动。


    return;
} //void DownloadItem::animateProgress(int currentValue,int maximumValue)

