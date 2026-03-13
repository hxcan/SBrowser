
#include "SbPreCompile.h" //QMainWindow

#include "Toast.h" //Toast
#include "SBrowserDownloader.h" //SBrowserWindow
#include "cookiejar.h" //CookieJar
#include "DownloadManager.h" //DownloadManager
#include "ui_passworddialog.h" //PasswordDialog
#include "StbShadowProxyFactory.h" //StbShadowProxyFactory

DownloadManager * SBrowserDownloader::s_downloadManager = nullptr; //下载管理器。

  
/*!
 * \brief SBrowserWindow::SBrowserWindow 构造函数。
 * \param parent 亲代对象。
 */
SBrowserDownloader::SBrowserDownloader(QWidget *parent):QObject(parent)
{
    initializeMemberObjects(); //初始化成员对象。

    connectSignals(); //连接信号。
    

    loadSetting(); //载入设置信息。

    determinBitAmount(); //确定CPU位数。
    checkNewVersion(); //检查是否有新版本。

    showDownloadManager(); // Show the download manaager.
} //SBrowserWindow::SBrowserWindow(parent=0)

void SBrowserDownloader::addDownloadTaskFromData(const QByteArray &data, const QString &suggestedFilename)
{
        cout << __FILE__ << ", " << __LINE__ << ", " << __func__  <<  ", decodeed data size: " << data.size () << endl;

    // ✅ 1. 确定保存路径
    QString savePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + QFileInfo(suggestedFilename).fileName();

    // ✅ 2. 创建文件对象（堆上）
    QFile* file = new QFile(savePath);
    if (!file->open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file for writing:" << savePath << file->errorString();
        delete file;
        return;
    }

    // ✅ 3. 定义块大小
    const int CHUNK = 128 * 1024; // 128KB

    // ✅ 4. 创建定时器
    QTimer* timer = new QTimer(this);
    int offset = 0;

    // ✅ 5. 连接定时器：把 savePath 和 CHUNK 显式捕获！
    connect(timer, &QTimer::timeout, this, [this, file, data, timer, offset, CHUNK, savePath]() mutable {
        int blockSize = qMin(CHUNK, data.size() - offset);

        if (blockSize > 0) {
            qint64 written = file->write(data.mid(offset, blockSize));
            if (written == -1) {
                qWarning() << "Write failed!";
                file->close();
                timer->stop();
                delete file;
                delete timer;
                return;
            }
            offset += written;
        } else {
            // 所有数据写完
            file->close();
            timer->stop();
            // emit downloadFinished(savePath); // ✅ 现在可以访问 savePath
            delete file;
            delete timer;
        }
    });

    // ✅ 6. 启动定时器
    timer->start(0);
}


// 辅助函数：生成唯一文件名
QString SBrowserDownloader::generateSavePath(const QString &suggestedName)
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QDir().mkpath(dir);

    QString base = QFileInfo(suggestedName).completeBaseName();
    QString suffix = QFileInfo(suggestedName).suffix();
    QString path = QDir(dir).filePath(suggestedName);
    int counter = 1;

    while (QFile::exists(path)) {
        path = QDir(dir).filePath(QString("%1 (%2).%3").arg(base).arg(counter++).arg(suffix));
    }

    return path;
}

/*!
   * \brief SBrowserWindow::connectSignals 连接信号。
   */
void SBrowserDownloader::connectSignals()
{
    connect (downloadManager (),SIGNAL(downloadFinished(QString)),this,SLOT(ttsSayDownloadFinished())); //下载完毕，则语音报告。
    connect (downloadManager (),SIGNAL(downloadFinished(QString)),this,SLOT(qrCollectBgImage(QString))); //下载完毕，则为二维码收集背景图片。
    connect (downloadManager (),SIGNAL(downloadStarted()),this,SLOT(ttsSayDownloadStarted())); //下载开始，则语音报告。
    connect (downloadManager (),SIGNAL(downloadError()),this,SLOT(ttsSayDownloadError())); //下载出错，则语音报告。
    
    connect(&srcSwitch, &SetDirectionService::shouldAddDownloadTask, this, &SBrowserDownloader::addDownloadTask); //应当执行口形序列动作，则执行口形序列动作。
    connect(&srcSwitch, &SetDirectionService::shouldQuit, this, &SBrowserDownloader::quit); //应当退出，则退出。


    connect(&srcSwitch,&SetDirectionService::shouldShowDownloadManager,this,&SBrowserDownloader::showDownloadManager); //应当显示下载管理器，则显示下载管理器。
    connect(&srcSwitch, &SetDirectionService::shouldSyncSetting, this, &SBrowserDownloader::loadSetting); //Sync setting. 陈欣


    // ✅ 新增：连接 Blob 下载任务
    connect(&srcSwitch, &SetDirectionService::shouldAddDownloadTaskFromData,
            this, &SBrowserDownloader::addDownloadTaskFromData);
} //void SBrowserWindow::connectSignals() //连接信号。

/*!
 * \brief SetDirectionService::addDownloadTask 添加下载任务。
 * \param requestUrl 下载网址。
 * \param requestFileName 是否请求文件名。
 */
void SBrowserDownloader::addDownloadTask(const QString & requestUrl, bool requestFileName)
{
    Q_UNUSED(requestFileName); //不使用勒个参数。

    if (downloadUrlSet.contains(requestUrl)) // Already downloading. 陈欣
    {

    } //if (downloadUrlSet.contains(requestUrl)) // Already downloading. 陈欣
    else // Not downloading.
    {
        QNetworkRequest request=QNetworkRequest(requestUrl); //要下载的目标URL对应的请求对象。

        downloadManager()->download(request); //开始下载。

        downloadUrlSet << requestUrl; // Add to downloading set.
    } //else // Not downloading.
    
} //void SetDirectionService::addDownloadTask(const QString & requestUrl, bool requestFileName) 

/*!
 * \brief SBrowserDownloader::loadSetting 载入设置信息。
 */
void SBrowserDownloader::loadSetting()
{
    QSettings settings; //设置信息。s

    settings.beginGroup("SBrowser"); //主程序。


    QVariant dftSzVrt=QVariant(QSize(400, 400)); //默许窗口尺寸。

    //智能代理：
    QVariant crtSzVrt=settings.value("useSmartProxy",dftSzVrt ); //读取是否要使用智能代理。
    bool crtSz=crtSzVrt.toBool (); //转换成逻辑值。
    prxyFctry->setUsingSmartProxy(crtSz); //设置是否要使用智能代理。

    QVariant crtSzVrtallUseProxy=settings.value("allUseProxy",dftSzVrt ); //读取是否要all使用代理。
    bool crtSzcrtSzVrtallUseProxy=crtSzVrtallUseProxy.toBool (); //转换成逻辑值。
    prxyFctry->setAllUsingProxy(crtSzcrtSzVrtallUseProxy); //设置是否要all使用代理。


    QVariant crtSzVrtAutoAddRule=settings.value("autoAddRule",dftSzVrt ); //读取是否要自动添加代理规则。
    bool crtSzcrtSzVrtAutoAddRule=crtSzVrtAutoAddRule.toBool (); //转换成逻辑值。
    prxyFctry->setAutoAddProxyRule (crtSzcrtSzVrtAutoAddRule); //设置是否要自动添加代理规则。

    //是否要通过代理进行DNS解析：
    QVariant crtSzVariantUseProxyForDns=settings.value("useProxyForDns",dftSzVrt ); //读取是否要通过代理来进行DNS解析。
    bool crtSzcrtSzVrtVariantUseProxyForDns=crtSzVariantUseProxyForDns.toBool (); //转换成逻辑值。
    prxyFctry->setUseProxyForDns(crtSzcrtSzVrtVariantUseProxyForDns); //设置是否要通过代理来进行DNS解析。


    quint16 smtPrxyPrt=settings.value("smartProxyPort",QVariant(  9090 )  ).toUInt (); //读取智能代理的端口号。
    prxyFctry->setSmartProxyPort(smtPrxyPrt); //设置智能代理的端口号。

    //http代理端口号：
    quint16 smtPrxyPrtHttp=settings.value("smartProxyPortHttp",QVariant(  8118)  ).toUInt (); //读取智能代理的http端口号。
    prxyFctry->setSmartProxyPortHttp(smtPrxyPrtHttp); //设置智能代理的http端口号。


    QString smartProxyHost=settings.value("smartProxyHost",QVariant("localhost")).toString (); //读取智能代理的主机名。
    prxyFctry->setSmartProxyHost(smartProxyHost); //设置智能代理的主机名。

    //禁用广告：
    QVariant crtSzVrtEnableAdBlock=settings.value("enableAdBlock",dftSzVrt ); //读取是否要启用广告禁用功能。
    bool crtSzEnableAdBlock=crtSzVrtEnableAdBlock.toBool (); //转换成逻辑值。

    prxyFctry->setEnableAdBlock(crtSzEnableAdBlock); //设置是否要启用广告禁用功能。


    //Download directory:
    auto DefaultDownloadDirectory=QStandardPaths::writableLocation (QStandardPaths::DownloadLocation);
    auto downloadDirectory=settings.value ("downloadDirectory", DefaultDownloadDirectory).toString (); //获取用户代理字符串。
//    chen xin
    downloadManager()->setDownloadDirectory(downloadDirectory); //set download directory.

    //界面语言：
    QVariant manuallySelectLanguageVariant=settings.value ("manuallySelectLanguage",dftSzVrt); //读取，是否要手动选择语言。

    QVariant selectedLanguageVariant=settings.value ("selectedLanguage",dftSzVrt); //读取，选择的语言。
    selectedLanguage=selectedLanguageVariant.toString (); //转换成字符串。

    //TTS:
    QString ttsServerAddress=settings.value("ttsServerAddress",QVariant("192.168.2.113")).toString (); //获取tts服务器的地址.

    //是否启用WebGL:

    //是否启用插件：
    bool enablePlugins=settings.value("enabledPlugins",QVariant(false)).toBool (); //是否启用插件。
    qDebug() << __FILE__ << __LINE__ << __func__ << tr("enabled plugins?:") << enablePlugins; //Debug.

    //是否启用Javascript：
    
    //首页：
    indexPageUrl=settings.value ("indexPage",indexPageUrl).toString (); //获取首页网址。
    
    //搜索引擎：
    QString searchEnginesmartProxyHost=settings.value("searchEngine",QVariant("")).toString (); //读取智能代理的主机名。



    
    settings.endGroup(); //关闭分组。
} //void SBrowserWindow::loadSetting() //!<载入设置信息。

/*!
 * \brief SBrowserWindow::createNetworkManager 创建网络管理器。
 */
void SBrowserDownloader::createNetworkManager()
{
    StbShadowProxyFactory * shadowPrxyFctry=new StbShadowProxyFactory(prxyFctry); //创建影子代理工厂。

    ntAccsMngr4WebPages.setProxyFactory (shadowPrxyFctry); //设置代理工厂。
} //void SBrowserWindow::createNetworkManager() //!<创建网络管理器。

/*!
 * \brief SBrowserWindow::toggleProxyRule 要求切换智能代理规则，则切换。
 * \param enb 是否启用针对此主机的代理规则。
 * \param hstNm 主机名。
 */
void SBrowserDownloader::toggleProxyRule(bool enb,QString hstNm)
{
    if (enb) //要启用针对该主机的代理。
    {
        prxyFctry->addHostname(hstNm); //加入到代理规则中。
    } //if (enb) //要启用针对该主机的代理。
    else //要禁用针对该主机的代理。
    {
        prxyFctry->removeHostname(hstNm); //从代理规则中删除。
    } //else //要禁用针对该主机的代理。
} //void SBrowserWindow::toggleProxyRule(bool enb,QString hstNm) //!<要求切换智能代理规则，则切换。

/*!
 * \brief SBrowserWindow::createCookieJar 创建小甜饼罐。
 */
void SBrowserDownloader::createCookieJar()
{
    CookieJar * crtCkJr=new CookieJar(); //创建小甜饼罐。


    ntAccsMngr4WebPages.setCookieJar (crtCkJr); //设置小甜饼罐。
} //void SBrowserWindow::createCookieJar() //!<创建小甜饼罐。

/*!
 * \brief SBrowserWindow::downloadManager 获取下载管理器指针。
 * \return 下载管理器的指针。
 */
DownloadManager *SBrowserDownloader::downloadManager()
{
    if (!s_downloadManager) //下载管理器还不存在。
    {
        s_downloadManager = new DownloadManager(); //创建一个。


        s_downloadManager->setNetworkAccessManager(&ntAccsMngr4WebPages); //设置网络访问管理器。
    } //if (!s_downloadManager) //下载管理器还不存在。

    return s_downloadManager;
} //DownloadManager *SBrowserWindow::downloadManager()

/*!
 * \brief SBrowserWindow::showDownloadManager 显示下载管理器。
 */
void SBrowserDownloader::showDownloadManager()
{
    downloadManager()->show (); //显示。

    downloadManager()->raise(); //跳到顶部.
} //void SBrowserWindow::showDownloadManager() //!<显示下载管理器。

/*!
 * \brief SBrowserWindow::closeDownloadManager 关闭下载管理器。
 */
void SBrowserDownloader::closeDownloadManager()
{
    downloadManager()->close (); //关闭。

    return;
} //void SBrowserWindow::closeDownloadManager() //!<关闭下载管理器。

/*!
 * \brief SBrowserWindow::checkNewVersion 检查是否有新版本。
 */
void SBrowserDownloader::checkNewVersion()
{
    QString urlString=tr("http://sweetsoft.info:18013/CheckNewVersion/?CurrentVersion=%1&Platform=%2&BitAmount=%3&PackageName=com.stupidbeauty.sbrowser").arg (currentVersionNumber).arg (platformName).arg (bitAmount); //检查更新的URL。

    checkNewVersionRequest=new QNetworkRequest(QUrl(urlString)); //Create the request.
    QNetworkReply* CrtRply= ntAccsMngr4WebPages.get(*checkNewVersionRequest); //发送GET请求。
    connect(CrtRply,SIGNAL(finished()),this,SLOT(ParseCheckNewVersionResult())); //完成之後，處理其結果。
} //void SBrowserWindow::checkNewVersion() //!<检查是否有新版本。

/*!
 * \brief SBrowserWindow::isMagicLinux 是否是运行于MagicLinux.
 * \return 是否是运行于MagicLinux.
 */
bool SBrowserDownloader::isMagicLinux()
{
    bool result=false; //结果。

    //打开"/etc/os-release"文件来检查：
    QSettings osReleaseFileSettings("/etc/os-release",QSettings::IniFormat); //以配置文件形式打开os-release.



    QString osName=osReleaseFileSettings.value ("NAME").toString (); //获取名字。

    if (osName=="Magic") //名字是Magic.
    {
        result=true; //是MagicLinux.
    } //if (osName=="Magic") //名字是Magic.


    return result;
} //bool SBrowserWindow::isMagicLinux() //!<是否是运行于MagicLinux.

/*!
 * \brief SBrowserWindow::determinBitAmount 确定CPU位数。
 */
void SBrowserDownloader::determinBitAmount()
{
#ifdef Q_PROCESSOR_X86_64 //64位。
    bitAmount=64; //64位。
#else //32位。
    bitAmount=32; //32位。
#endif
} //void SBrowserWindow::determinBitAmount() //!<确定CPU位数。

/*!
 * \brief SBrowserWindow::qrCollectBgImage 下载完毕，则为二维码收集背景图片。
 * \param flNm 可能是图片的文件名。下载的文件不一定是图片。
 */
void SBrowserDownloader::qrCollectBgImage(QString flNm)
{
    QImage CrtPxmp; //The pixmap object.用于尝试载入图片文件的图片对象。

    bool LdRst=false; //Whether loaded successfully.

    try //Load the image, and catch exceptions.
    {
        LdRst=CrtPxmp.load(flNm); //尝试载入。
    } //try //Load the image, and catch exceptions.
    catch(std::bad_alloc & e) //Catch bad alloc exception.
    {

    } //catch(std::bad_alloc e)

    if (LdRst) //Load successfully.
    {
        collectToQrBgImageSet(flNm); //将此图片收集到背景图片集合中。
    } //if (CrtPxmp.load(CrtFlNm)) //Load successfully.
} //void SBrowserWindow::qrCollectBgImage(QString flNm)

/*!
 * \brief SBrowserWindow::collectToQrBgImageSet 将此图片收集到背景图片集合中。
 * \param flNm 要收集的文件名。
 */
void SBrowserDownloader::collectToQrBgImageSet(QString flNm)
{
    QString ssnStrDir=QDir::homePath()+"/.sbrowser/qrBackgroundImage/"; //保存背景图片文件的目录。
    QDir pathMkr; //创建路径的目录对象。
    pathMkr.mkpath (ssnStrDir); //创建路径。

    QFile imgFl(flNm); //要复制的文件。
    QFileInfo imgFlInfo(flNm); //文件信息对象。
    QString pureFlNm=imgFlInfo.fileName (); //获取纯粹的文件名。
    imgFl.copy (ssnStrDir+pureFlNm); //复制文件。
} //void SBrowserWindow::collectToQrBgImageSet(QString flNm)

/*!
 * \brief SBrowserWindow::quit 退出。
 */
void SBrowserDownloader::quit()
{
    QApplication::quit(); //退出。
} //void SBrowserWindow::quit()

/*!
 * \brief SBrowserWindow::~SBrowserWindow 析构函数。
 */
SBrowserDownloader::~SBrowserDownloader()
{
    delete checkNewVersionRequest; //释放内存。

    QList<QNetworkReply*> replies= ntAccsMngr4WebPages. findChildren<QNetworkReply*>(); //获取所有的回复对象。

    foreach(QNetworkReply * currentReply,replies) //一个个地处理。
    {
        currentReply->abort (); //放弃。
    } //foreach(QNetworkReply * currentReply,replies) //一个个地处理。
    
    delete s_downloadManager; //释放下载管理器。
} //SBrowserWindow::~SBrowserWindow()

/*!
 * \brief SBrowserWindow::initializeMemberObjects 初始化成员对象。
 */
void SBrowserDownloader::initializeMemberObjects()
{
//    qsrand (QTime::currentTime ().msecsSinceStartOfDay ()); //初始化随机数生成器。

    prxyFctry=new StbProxyFactory; //创建代理工厂对象。

    createNetworkManager(); //创建网络管理器。

    createCookieJar(); //创建小甜饼罐。

//    srcNode=new QRemoteObjectHost(QUrl(QStringLiteral("tcp://0.0.0.0:13025")));
    srcNode=new QRemoteObjectHost(QUrl(QStringLiteral("local:000013025")));

    srcNode->enableRemoting(&srcSwitch);
} //void SBrowserWindow::initializeMemberObjects()
