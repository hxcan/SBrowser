#include "SbPreCompile.h" //QMainWindow

#include "Toast.h" //Toast

#include "SBrowserCppWebEngine.h" //SBrowserWindow
#include "SWebPageWidget.h" //SWebPageWidget
#include "ui_SBrowserCppWebEngine.h" //Ui_SBrowserMainWindow
#include "cookiejar.h" //CookieJar
#include "DownloadManager.h" //DownloadManager
#include "QrHelper.h" //QrHelper
#include "ui_passworddialog.h" //PasswordDialog

#include "StbShadowProxyFactory.h" //StbShadowProxyFactory
#include "ContainerInterceptor.h" //ContainerInterceptor
#include "UnknownSchemeInterceptor.h" //UnknownSchemeInterceptor

DownloadManager * SBrowserCppWebEngine::s_downloadManager = nullptr; //下载管理器。
  
/*!
 * @brief SBrowserWindow::maybe1stTimeLoadPage 检查当前标签页是否第一次载入。
 * @param nwTbIdx 当前标签页的下标。
 */
void SBrowserCppWebEngine::maybe1stTimeLoadPage(int nwTbIdx)
{
    SWebPageWidget * crtTbWdgt=qobject_cast<SWebPageWidget *>(pgTbs->widget(nwTbIdx)); //获取对应的页面部件。
    
    if (crtTbWdgt->isFirstTimeLoad()) //这是第一次载入。
    {
        // 确保所有依赖 setupUi() 的初始化工作已完成
                crtTbWdgt->initializeDependencies();

        crtTbWdgt->addToolbarActions(); //向工具条加入动作。

        crtTbWdgt->startLoad(); //开始载入页面。
    } //if (crtTbWdgt.isFirstTimeLoad()) #这是第一次载入。
} //SBrowserWindow::maybe1stTimeLoadPage(nwTbIdx)
  
/*!
   * \brief SBrowserWindow::createWindow 创建新窗口。即新的标签页。
   * \param wndTp 窗口类型。
   * \return 新创建的窗口对象的指针。
   */
QWebEngineView *SBrowserCppWebEngine::createWindow(QWebEnginePage::WebWindowType wndTp)
{
    Q_UNUSED(wndTp); //不使用勒个参数。

    QWebEngineView * result; //结果。

    SWebPageWidget * crtTbWdgt=createWebPageWidget(); //創建一個頁面部件.


    pgTbs->addTab(crtTbWdgt,""); //添加标签页。
    updateTabAmount(); //更新标签页个数。



    if (pgTbs->count ()>=2) //有最少2个标签页了。
    {
        pgTbs->setTabsClosable (true); //标签页可以关闭了。
    } //if (pgTbs->count ()>=2) //有最少2个标签页了。

          
    pgTbs->setCurrentWidget(crtTbWdgt); //切换到新的标签页。

    result=crtTbWdgt->getWebView(); //获取新标签页中的网页视图。
    
    return result; //返回结果。
} //def createWindow(wndTp)
  
/*!
 * \brief SBrowserWindow::changeTabIcon 修改对应标签页的图标。
 * \param nwIcon 新的图标。
 */
void SBrowserCppWebEngine::changeTabIcon(QIcon nwIcon)
{
  QWidget * sndr=(QWidget * )(sender()); //获取信号的发送者。
    
  int tbIdx=pgTbs->indexOf(sndr); //获取信号发送者所在的标签页索引。
    
  pgTbs->setTabIcon(tbIdx,nwIcon); //修改标签页图标。

  if (sndr==pgTbs->currentWidget()) //改变标题的是当前标签页，则修改窗口标题。
  {
    setWindowIcon(nwIcon); //修改窗口图标。
  } //if sndr==@pgTbs.currentWidget() #改变标题的是当前标签页，则修改窗口标题。
    
    return;
} //def changeTabIcon(nwIcon)

/*!
 * \brief SBrowserWindow::destroyWebPages 销毁所有网页。
 */
void SBrowserCppWebEngine::destroyWebPages()
{
    int tbsAmt=pgTbs->count(); //获取当前显示的标签页数量。

    for(int tbsCtr=0;tbsCtr<=(tbsAmt-1);tbsCtr++) //一个个地销毁标签页。
    {
        SWebPageWidget * crtWbPgWdgt=(SWebPageWidget *)(pgTbs->widget(tbsCtr)); //获取当前标签页。


        crtWbPgWdgt->destroyWebPage(); //销毁网页《。
    } //for tbsCtr in 0..tbsAmt #一个个地保存标签页信息。



    return;
} //void SBrowserWindow::destroyWebPages()

/*!
 * \brief SBrowserWindow::closeTab 关闭标签页。
 * \param tabIndex 当前需要关闭的标签页的下标。
 */
void SBrowserCppWebEngine::closeTab(const int &tabIndex)
{
    SWebPageWidget * crtTb=(SWebPageWidget *)(pgTbs->widget(tabIndex)); //获取对应的标签页《。

    QString currentUrl=crtTb->getUrl (); //获取URL。


    closedTabUrlStack << currentUrl; //加入到栈中。

    emit closedTabUrlStackIsNotEmpty (); //发射信号，已关闭的标签页栈，不再为空。
    emit restoreClosedTabToolTipChanged(tr("Restore tab ")+currentUrl); //发射信号，恢复标签页按钮的提示文字发生变化。
    
    crtTb->destroyWebPage(); //销毁网页《。

    crtTb->deleteLater (); //稍后删除。
    
    pgTbs->removeTab(tabIndex); //删除对应的标签页。
    updateTabAmount(); //更新标签页个数。
    
    if (pgTbs->count()<=1) //已经只剩下一个标签页了。
    {
        pgTbs->setTabsClosable(false); //不可关闭标签页。
    } //if (@pgTbs.count()<=1) #已经只剩下一个标签页了。

    tabsManager->deleteTab(crtTb); //从标签页管理器中删除该标签页。
} //void SBrowserWindow::closeTab(int tbIdx)

/*!
 * \brief SBrowserWindow::refreshZoomFactor 当前标签页的缩放比例发生改变，则相应地刷新缩放比例。
 * \param newZoomFactor 新的缩放比例。
 */
void SBrowserCppWebEngine::refreshZoomFactor(qreal newZoomFactor)
{
    u->action1x->setText (QString("%1x").arg (newZoomFactor)); //显示缩放比例。

    return;
} //void SBrowserWindow::refreshZoomFactor(qreal newZoomFactor)

/*!
 * \brief SBrowserWindow::rememberZoomFactor 记录缩放比例。
 * \param tabIndex 当前的缩放比例
 */
void SBrowserCppWebEngine::rememberZoomFactor(const qreal tabIndex)
{
    Q_UNUSED(tabIndex) //not use this parameter.

    auto crtPgWdgt=qobject_cast<SWebPageWidget*>(sender()); //获取网页部件对象

    rememberZoomFactor(crtPgWdgt); //记住缩放比例
} //void SBrowserWindow::rememberZoomFactor(const qreal & tabIndex)

/*!
 * \brief SBrowserWindow::rememberZoomFactor 记住缩放比例
 * \param crtPgWdgt 要记住缩放比例的网页部件对象
  */
void SBrowserCppWebEngine::rememberZoomFactor(const SWebPageWidget * crtPgWdgt)
{
  qreal zoomFactor=crtPgWdgt->zoomFactor(); //获取缩放比例。

  auto url=crtPgWdgt->getUrl(); //获取网址。

  auto urlObject=QUrl(url); //解析成网址对象。

  auto hostName=urlObject.host(); //主机名。

  hostNameZoomFactorMap[hostName]=zoomFactor; //记录缩放比例。

//    saveZoomFactorMap(); //Save the zoom factor map.
  saveZoomFactorMapCbor(); //Save the zoom factor map. 陈欣
} //void SBrowserWindow::rememberZoomFactor(const SWebPageWidget * crtPgWdgt)

/*!
 * \brief SBrowserWindow::rememberZoomFactor 记录缩放比例。
 * \param pgTbscurrentIndex 当前标签页的编号。
 */
void SBrowserCppWebEngine::rememberZoomFactor(const int tabIndex)
{
  SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->widget (tabIndex)); //获取当前的标签页。

  rememberZoomFactor(crtPgWdgt); //记住缩放比例
} //void SBrowserWindow::rememberZoomFactor(int pgTbscurrentIndex)

/*!
 * \brief SBrowserCppWebEngine::saveZoomFactorMapCbor Save the zoom factor map. 陈欣
 */
void SBrowserCppWebEngine::saveZoomFactorMapCbor()
{
  QString qrBackgroundImageBaseDirUrl=QDir::homePath()+"/.sbrowser/hostnameZoomFactorMap.yy.cx"; //背景图片的基准目录。
  auto creditDataFileName=qrBackgroundImageBaseDirUrl;

  auto datagram = messageFactory.constructZoomFactorMessage(hostNameZoomFactorMap); // 构造消息，缩放比例映射。陈欣

  QFile creditDataFile(creditDataFileName);

  creditDataFile.open(QIODevice::WriteOnly);
  creditDataFile.write(datagram);
  creditDataFile.close();
} //void SBrowserCppWebEngine::saveZoomFactorMapCbor()

/*!
 * \brief SBrowserWindow::syncZoomFactor 当前标签页网址改变，则相应地同步缩放比例。
 * \param newUrl 新的网址。
 */
void SBrowserCppWebEngine::syncZoomFactor(const QString newUrl)
{
  SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(QObject::sender()); //获取当前的标签页。

  auto url=newUrl; //获取网址。

  syncZoomFactor(url, crtPgWdgt); //同步指定标签页的缩放比例。
} //void SBrowserWindow::syncZoomFactor(const QString newUrl)

/*!
 * \brief SBrowserWindow::syncZoomFactor 同步指定标签页的缩放比例。
 * \param url 网址。
 * \param crtPgWdgt 标签页。
 */
void SBrowserCppWebEngine::syncZoomFactor(const QString url, SWebPageWidget* crtPgWdgt)
{
    auto urlObject=QUrl(url); //解析成网址对象。
    
    auto hostName=urlObject.host(); //主机名。

    if (hostNameZoomFactorMap.contains(hostName)) //记录了它的缩放比例。
    {
        qreal zoomFactor=hostNameZoomFactorMap[hostName]; //获取缩放比例。

        crtPgWdgt->zoom(zoomFactor); //直接缩放到指定比例。
    } //if (hostNameZoomFactorMap.contains(hostName)) //记录了它的缩放比例。
} //void SBrowserWindow::syncZoomFactor(const QString url, const SWebPageWidget* crtPgWdgt)

/*!
 * \brief SBrowserWindow::syncZoomFactor 当前标签页改变，则相应地同步缩放比例。
 * \param tabIndex 当前标签页的编号。
 */
void SBrowserCppWebEngine::syncZoomFactor(const int tabIndex)
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->widget (tabIndex)); //获取当前的标签页。

    auto url=crtPgWdgt->getUrl(); //获取网址。
    
    syncZoomFactor(url, crtPgWdgt); //同步指定标签页的缩放比例。
 } //void SBrowserWindow::syncZoomFactor(const int tabIndex)

/*!
 * \brief SBrowserWindow::refreshZoomFactor 当前标签页改变，则相应地刷新缩放比例。
 * \param tabIndex  当前标签页的编号。
 */
void SBrowserCppWebEngine::refreshZoomFactor(int tabIndex)
{

    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->widget (tabIndex)); //获取当前的标签页。

    qreal zoomFactor=crtPgWdgt->zoomFactor(); //获取缩放比例。

    u->action1x->setText (QString("%1x").arg (zoomFactor)); //显示缩放比例。


    return;
} //void SBrowserWindow::refreshZoomFactor(int tabIndex)
  
/*!
 * @brief SBrowserWindow::refreshWindowTitle 刷新窗口标题。
 * @param currentWidgetIndex 当前标签页的编号。
 */
void SBrowserCppWebEngine::refreshWindowTitle(const int &currentWidgetIndex)
{
    QString txt2Chg=pgTbs->tabText(currentWidgetIndex); //获取对应页面的标题。
    
    setWindowTitle(txt2Chg+tr("-StupidBeauty Browser with WebEngine")); //修改窗口标题。

    return;
} //def refreshWindowTitle(nwTbIdx)

/*!
 * \brief SBrowserWindow::refreshWindowIcon 当前标签页改变，则相应地刷新窗口图标。
 * \param tabIndex 新的标签页下标。
 */
void SBrowserCppWebEngine::refreshWindowIcon(const int &tabIndex)
{
    QIcon txt2Chg=pgTbs->tabIcon (tabIndex); //获取对应页面的图标。

    setWindowIcon(txt2Chg); //修改窗口图标。

    return;
} //void SBrowserWindow::refreshWindowIcon(int tabIndex)
  
/*!
 * \brief SBrowserWindow::changeTabText 修改标签页的标题。
 * \param txt2Chg 要修改的新标题。
 */
void SBrowserCppWebEngine:: changeTabText(QString txt2Chg)
{
    QWidget * sndr=(QWidget * )(sender()); //获取信号的发送者。
    
    
    int tbIdx=pgTbs->indexOf(sndr); //获取信号发送者所在的标签页索引。
    
    pgTbs->setTabText(tbIdx,txt2Chg); //修改标签页标题。
    pgTbs->setTabToolTip(tbIdx,txt2Chg); //设置标签页的悬停提示。

    
    
    if (sndr==pgTbs->currentWidget()) //改变标题的是当前标签页，则修改窗口标题。
    {
        setWindowTitle(txt2Chg+tr("-StupidBeauty Browser with WebEngine")); //修改窗口标题。
    } //if sndr==@pgTbs.currentWidget() #改变标题的是当前标签页，则修改窗口标题。

} //def changeTabText(txt2Chg)

/*!
 * \brief SBrowserCppWebEngine::infactSameUrl The right url is in fact the same to the left url. 陈欣
 * \param url2Open Left url
 * \param rightMostUrl Right url
 * \return Whether they are actually the same url.
 */
bool SBrowserCppWebEngine::infactSameUrl(const QString & url2Open, const QString & rightMostUrl) const
{
    QUrl url1(url2Open);
    QUrl url2(rightMostUrl);

    auto result = url1.matches(url2, QUrl::RemoveFragment); // Match or not

    return result;
} //bool SBrowserCppWebEngine::infactSameUrl(const QString & url2Open, const QString & rightMostUrl) const

/*!
 * \brief SBrowserWindow::openNewTab 打开新的空白标签页。
 */
void SBrowserCppWebEngine::openNewTab(QString url2Open)
{
    if (url2Open.isEmpty ()) //URL为空。
    {
        url2Open=indexPageUrl; //替换为首页网址。
    } //if (url2Open.isEmpty ()) //URL为空。

    auto rightMostIndex = pgTbs->count() -1; // Get the right most index.

    auto rightMostTabWidgte = pgTbs->widget(rightMostIndex); // Get the right most widget.

    auto rightMostBrowserWidget = qobject_cast<SWebPageWidget*>(rightMostTabWidgte); // Convert to browser widget.

    auto rightMostUrl=tr("ChenXin");

    if (rightMostBrowserWidget) // The widget exists
    {
      rightMostUrl=rightMostBrowserWidget->getUrl(); // Get the right most url.
    } //if (rightMostBrowserWidget) // The widget exists

    if (infactSameUrl(url2Open, rightMostUrl)) // The right most url is already the url. 陈欣
//    if (url2Open==rightMostUrl) // The right most url is already the url. 陈欣
    {
        pgTbs->setCurrentIndex(rightMostIndex); // Toggle to right most tab.
    } //if (url2Open==rightMostUrl) // The right most url is already the url. 陈欣
    else // Need open new tab
    {
        SWebPageWidget * crtTbWdgt=createWebPageWidget(); //創建一個頁面部件.


        crtTbWdgt->setUrl(url2Open); //设置URL。

        if (crtTbWdgt->isFirstTimeLoad()) //这是第一次载入。
        {
            // 确保所有依赖 setupUi() 的初始化工作已完成
                    crtTbWdgt->initializeDependencies();

            crtTbWdgt->addToolbarActions(); //向工具条加入动作。

            crtTbWdgt->startLoad(); //开始载入页面。
        } //if (crtTbWdgt.isFirstTimeLoad()) #这是第一次载入。


//        crtTbWdgt->addToolbarActions(); //向工具条加入动作。

//        crtTbWdgt->startLoad(); //开始载入页面。


        pgTbs->addTab(crtTbWdgt,""); //添加标签页。
        updateTabAmount(); //更新标签页个数。

        pgTbs->setCurrentWidget(crtTbWdgt); //切换到新的标签页。

        if (pgTbs->count()>1) //不止一个标签页。
        {
            pgTbs->setTabsClosable(true); //可关闭标签页。
        } //if (@pgTbs.count()>1) #不止一个标签页。

    } //else // Need open new tab

} //def openNewTab()

/*!
 * \brief SBrowserWindow::saveSessionStore 保存会话信息。
 */
void SBrowserCppWebEngine::saveSessionStore()
{
  if (sessionStoreLoadFinished) //已经完成了会话信息的载入，此时保存会话信息才是安全的。
  {
    QVariantMap ssnObj; //会话信息对象。
    QVariantList wndsVl; //窗口数组。
    QVariantMap crtWnd; //当前窗口对象。
    QVariantList tbsVl; //标签页数组。

    //一个个地加入标签页：
    int tbsAmt=pgTbs->count(); //获取当前显示的标签页数量。

    for(int tbsCtr=0;tbsCtr<=(tbsAmt-1);tbsCtr++) //一个个地保存标签页信息。
    {
      QVariantMap crtTb; //当前标签页的信息。
      QVariantList tbEtrs; //当前标签页中的条目数组。
      QVariantMap crtEntr; //当前条目。

      SWebPageWidget * crtWbPgWdgt=(SWebPageWidget *)(pgTbs->widget(tbsCtr)); //获取当前标签页。

      QString crtUrl=crtWbPgWdgt->getUrl(); //获取当前标签页的URL。
      QString crtTtlRaw=pgTbs->tabText(tbsCtr); //获取当前标签页的标题。
      QString crtTtl=crtTtlRaw; //获取当前标签页的标题。

      QByteArray urlByteArray=crtUrl.toUtf8(); //转换成字节数组。
      QString urlEncoded=urlByteArray.toPercentEncoding(); //转换成百分号编码。

      crtEntr["url"]=urlEncoded; //记录URL。
      crtEntr["title"]=crtTtl; //记录页面标题。

      crtTb["index"]=1; //索引。

      tbEtrs << crtEntr; //加入当前条目。

      crtTb["entries"]=tbEtrs; //条目数组。

      tbsVl << crtTb; //加入当前标签页。
    } //for tbsCtr in 0..tbsAmt #一个个地保存标签页信息。

    if (tbsVl.size ()>0) //有标签页处于打开状态。
    {
      int crtTbIdx=pgTbs->currentIndex (); //获取当前被查看的标签页的下标。
      QVariantMap lstVwngTb=tbsVl.takeAt (crtTbIdx).toMap (); //获取当前被查看的标签页对象。
      tbsVl.insert (0,lstVwngTb); //将最后被查看的那个标签页放置到最前面。
    } //if (tbsVl.size ()>0) //有标签页处于打开状态。

    crtWnd["tabs"]=tbsVl; //加入标签页数组。

    wndsVl << crtWnd; //加入当前窗口对象。

    ssnObj["windows"]=wndsVl; //加入窗口信息。

    //保存到文件中：
    QString ssnStrJs=QDir::homePath()+"/.sbrowser/sessionstore.json"; //会话记录的JSON文件名。
    QString ssnStrDir=QDir::homePath()+"/.sbrowser/"; //会话记录的JSON文件目录。

    QDir ssnStrDirObj=QDir(ssnStrDir); //构造目录对象。
    if (ssnStrDirObj.exists()) //该目录存在。
    {
    } //if (ssnStrDirObj.exists()) //该目录存在。
    else //该目录不存在。
    {
      QDir hmDir=QDir::home(); //获取家目录。
      hmDir.mkdir(".sbrowser"); //创建子目录。
    } //else //该目录不存在。

    QSaveFile ssnStrFl(ssnStrJs); //JSON文件。
    ssnStrFl.open(QIODevice::WriteOnly); //打开文件。

    QJsonObject gameObject=QJsonObject::fromVariantMap (ssnObj); //转换成QJSON对象。
    QJsonDocument saveDoc(gameObject); //JSON文档。

    ssnStrFl.write (saveDoc.toJson ()); //输出内容。

    ssnStrFl.commit(); //关闭文件。
  } //if (sessionStoreLoadFinished) //已经完成了会话信息的载入，此时保存会话信息才是安全的。
} //def saveSessionStore()

/*!
 * \brief SBrowserWindow::enableCache 启用缓存。
 */
void SBrowserCppWebEngine::enableCache()
{
    QString oflnStogDir=QDir::homePath()+"/.sbrowser/diskcache/"; //磁盘缓存文件目录。

    QNetworkDiskCache * diskCache=new QNetworkDiskCache(this); //磁盘缓存对象。
    diskCache->setCacheDirectory (oflnStogDir); //设置缓存目录。

    ntAccsMngr4WebPages.setCache (diskCache); //设置缓存对象。

    return;
} //void SBrowserWindow::enableCache()

/*!
 * \brief SBrowserWindow::initializeWebSettings 初始化网页设置。
 */
void SBrowserCppWebEngine::initializeWebSettings()
{
    // QWebEngineSettings * glbWbStng=QWebEngineSettings::globalSettings(); //获取全局的网页设置对象。


    // glbWbStng->setFontFamily(QWebEngineSettings::StandardFont,"WenQuanYi Zen Hei Mono"); //设置默认字体为文泉驿正黑等宽。
    // glbWbStng->setFontFamily(QWebEngineSettings::SansSerifFont,"WenQuanYi Zen Hei Mono"); //设置默认字体为文泉驿正黑等宽。
    // glbWbStng->setFontFamily(QWebEngineSettings::SerifFont,"WenQuanYi Zen Hei Mono"); //设置默认字体为文泉驿正黑等宽。
    // glbWbStng->setFontFamily(QWebEngineSettings::FixedFont,"WenQuanYi Zen Hei Mono"); //设置默认字体为文泉驿正黑等宽。
    // glbWbStng->setFontFamily(QWebEngineSettings::CursiveFont,"WenQuanYi Zen Hei Mono"); //设置默认字体为文泉驿正黑等宽。
    // glbWbStng->setFontFamily(QWebEngineSettings::FantasyFont,"WenQuanYi Zen Hei Mono"); //设置默认字体为文泉驿正黑等宽。

//    QWebEngineSettings * glbWbStng=QWebEngineSettings::globalSettings(); //获取全局的网页设置对象。
    // glbWbStng->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true); //设置是否启用全屏.


    // 创建 WebChannel
    QWebChannel *webChannel = new QWebChannel(this);
    // downloadMsgHandler = new DownloadMessageHandler(this);
    // webChannel->registerObject("downloadMessageHandler", downloadMsgHandler);

    // 获取当前 profile（通常是 defaultProfile）
    QWebEngineProfile *profile = QWebEngineProfile::defaultProfile();
    // 或者你自定义的 profile
    // QWebEngineProfile *profile = customView->page()->profile();

    // ✅ 将 WebChannel 设置到 profile
    // profile->setWebChannel(webChannel);



    // qDebug() << "[WebEngine] ✅ WebChannel set on profile:" << profile->profileName();
} //void SBrowserWindow::initializeWebSettings()
  
/*!
 * \brief SBrowserWindow::SBrowserWindow 构造函数。
 * \param parent 亲代对象。
 */
SBrowserCppWebEngine::SBrowserCppWebEngine(QWidget *parent):QMainWindow(parent),u(new Ui::SBrowserCppWebEngine)
{
    u->setupUi(this); //构造界面。

    initializeMemberObjects(); //初始化成员对象。

    initializeWebSettings(); //初始化网页设置。

    enableCache(); //启动缓存。

    connectSignals(); //连接信号。
    
    loadSetting(); //载入设置信息。

//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ ; //报告错误码。

    loadCommandLineUrl(); //载入通过命令行传递的网址。

    determinePlatformName(); //确定平台名字。
    determinBitAmount(); //确定CPU位数。
    checkNewVersion(); //检查是否有新版本。

    fixEnvironmentVariableForNss(); //修复与nss相关的环境变量问题。
    debugEnvironmentVariable(); //Debug.

    startRotatePredictor(); //启动旋转预测器。

    loadHostnameZoomFactorMapCbor(); //载入host name zoom factor map。 陈欣
} //SBrowserWindow::SBrowserWindow(parent=0)

/*!
 * \brief SBrowserCppWebEngine::loadHostnameZoomFactorMapCbor 载入host name zoom factor map。 陈欣
 */
void SBrowserCppWebEngine::loadHostnameZoomFactorMapCbor()
{
  QString qrBackgroundImageBaseDirUrl=QDir::homePath()+"/.sbrowser/hostnameZoomFactorMap.yy.cx"; //背景图片的基准目录。
  auto creditDataFileName=qrBackgroundImageBaseDirUrl;

  QFile creditDataFile(creditDataFileName);

  creditDataFile.open(QIODevice::ReadOnly);
  auto datagram=creditDataFile.readAll();
  creditDataFile.close();

  string datagramtoStdString=datagram.toStdString(); //数据包。



  auto decodedMessage=QCborValue::fromCbor (datagram); //解码

  auto creditDataMessage=decodedMessage.toArray(); // 转换成数组.

  auto functionName=decodedMessage.toMap ()[QStringLiteral("function")].toString (); //获取功能名字



//#ifndef Q_OS_MACOS
//    HostnameZoomFactorMapMessage creditDataMessage;

//    creditDataMessage.ParseFromString (datagramtoStdString); //解析数据包。

//    auto fileCreditAmount=creditDataMessage.filecreditlist_size(); //获取文件表现数据个数。

  for(auto currentFileCredit: creditDataMessage) //Add one by one.
  {
    auto currentFileName=currentFileCredit.toMap()[QStringLiteral("hostName")].toString(); // 获取主机名。陈欣

    auto currentzoomFactor=currentFileCredit.toMap()[QStringLiteral("zoomFactor")].toDouble(); // 获取缩放比例。

    hostNameZoomFactorMap[currentFileName]=currentzoomFactor; //记录映射关系。
  } //for(auto currentFileCredit: creditDataMessage.zoomfactormapentry()) //Add one by one.
} //void     SBrowserCppWebEngine::loadHostnameZoomFactorMapCbor()

/*!
 * \brief SBrowserWindow::startRotatePredictor 启动旋转预测器。
 */
void SBrowserCppWebEngine::startRotatePredictor()
{
  QString qtdirValue="/usr/lib64/qt5";

  QString ldlibrarypath="LD_LIBRARY_PATH";

  QString lcnumeric="LC_NUMERIC";
  QString lcnumericValue="C";

  QStringList AptPrms; //命令行参数。
    
  AptPrms << imageFileName; //加入图片文件名。

  rotatePredictorProcess.startDetached(RotateRotatePredictorAptCmd,AptPrms); //启动进程。
} //void SImageView::startRotatePredictor()

/*!
 * \brief SBrowserWindow::fixEnvironmentVariableForNss 修复与nss相关的环境变量问题。
 */
void SBrowserCppWebEngine::fixEnvironmentVariableForNss()
{
    QProcessEnvironment processEnvironment=QProcessEnvironment::systemEnvironment(); //获取系统环境变量。

    QString ldLibraryPathName="LD_LIBRARY_PATH";

    QString ldLibraryPathValue=processEnvironment.value(ldLibraryPathName,"");

    if (ldLibraryPathName=="") //为空。
    {
        ldLibraryPathValue="/usr/lib64";
    } //if (ldLibraryPathName.isEmpty()) //为空。
    else //不为空。
    {
        ldLibraryPathValue+=":/usr/lib64"; //追加。
    } //else //不为空。


    qputenv(ldLibraryPathName.toStdString().c_str(),ldLibraryPathValue.toUtf8()); //设置环境变量。
} //void SBrowserWindow::fixEnvironmentVariableForNss()

/*!
 * \brief SBrowserWindow::debugEnvironmentVariable Debug.
 */
void SBrowserCppWebEngine::debugEnvironmentVariable()
{
    //将环境变量输出成JSON。
    QVariantMap ssnObj; //会话信息对象。

    QProcessEnvironment processEnvironment=QProcessEnvironment::systemEnvironment(); //获取系统环境变量。

    QStringList environmentKeyList=processEnvironment.keys(); //获取键列表。
    QVariantList tbEtrs; //当前标签页中的条目数组。

    for(QString currentKey:environmentKeyList) //一个个地输出。
    {
        QVariantMap crtEntr; //当前条目。

        crtEntr["name"]=currentKey; //记录URL。
        crtEntr["value"]=processEnvironment.value(currentKey); //记录页面标题。

        tbEtrs << crtEntr; //加入当前条目。

        ssnObj["entries"]=tbEtrs; //条目数组。
    } //for tbsCtr in 0..tbsAmt #一个个地保存标签页信息。

    //保存到文件中：
    QString ssnStrJs=QDir::homePath()+"/.sbrowser/environmentVariable.json"; //会话记录的JSON文件名。
    QString ssnStrDir=QDir::homePath()+"/.sbrowser/"; //会话记录的JSON文件目录。

    QDir ssnStrDirObj=QDir(ssnStrDir); //构造目录对象。
    if (ssnStrDirObj.exists()) //该目录存在。
    {
    } //if (ssnStrDirObj.exists()) //该目录存在。
    else //该目录不存在。
    {
        QDir hmDir=QDir::home(); //获取家目录。
        hmDir.mkdir(".sbrowser"); //创建子目录。
    } //else //该目录不存在。

    QFile ssnStrFl(ssnStrJs); //JSON文件。
    ssnStrFl.open(QIODevice::WriteOnly); //打开文件。

    QJsonObject gameObject=QJsonObject::fromVariantMap (ssnObj); //转换成QJSON对象。
    QJsonDocument saveDoc(gameObject); //JSON文档。

    ssnStrFl.write (saveDoc.toJson ()); //输出内容。

    ssnStrFl.close(); //关闭文件。
} //void SBrowserWindow::debugEnvironmentVariable()

/*!
 * \brief SBrowserWindow::bindExtraShortcuts 绑定额外的快捷键。
 */
void SBrowserCppWebEngine::bindExtraShortcuts()
{
    u->actionZoom_in->setShortcuts(QKeySequence::ZoomIn); //放大。
} //void SBrowserWindow::bindExtraShortcuts()

/*!
 * \brief SWebPageWidget::addActions 添加动作。
 */
void SBrowserCppWebEngine::addActions()
{
    QAction * focusSearchEngineAction=new QAction(this); //退出按钮。
    focusSearchEngineAction->setText (tr("")); //设置文字。
    focusSearchEngineAction->setShortcut (QKeySequence("Ctrl+W"));

    addAction (focusSearchEngineAction); //添加动作。

    connect (focusSearchEngineAction,&QAction::triggered,this,&SBrowserCppWebEngine::closeCurrentTab); //关闭当前标签页。
} //void SWebPageWidget::addActions()

/*!
 * \brief SWebPageWidget::generateQrCode 按了二维码按钮，则生成二维码。
 */
void SBrowserCppWebEngine::generateQrCode()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->generateQrCode(); //由该标签页来查找文字。

} //void SWebPageWidget::generateQrCode() //!<按了二维码按钮，则生成二维码。

  

/*!
 * \brief AptF::showTray 显示托盘图标。
 */
void SBrowserCppWebEngine::showTray()
{
    QAction * QuitAct=new QAction(this); //退出按钮。
    QuitAct->setText (tr("Quit")); //设置文字。

    connect (QuitAct,&QAction::triggered,this,&SBrowserCppWebEngine::quit); //按退出按钮，则退出。
    connect (&TrayIcn,&QSystemTrayIcon::activated,this,&SBrowserCppWebEngine::AsesShow); //点击左键时，显示主窗口。

    QAction * menuBarVisibleAction=new QAction(this); //菜单栏可见性动作。
    menuBarVisibleAction->setText (tr("Menu bar")); //菜单栏。
    menuBarVisibleAction->setCheckable (true); //可选中。
    menuBarVisibleAction->setChecked (u->action_Menu_bar->isChecked ()); //同步可见性。

    connect (menuBarVisibleAction,&QAction::toggled,u->action_Menu_bar,&QAction::setChecked); //同步可见性。
    connect (u->action_Menu_bar,&QAction::toggled,menuBarVisibleAction,&QAction::setChecked); //同步可见性。

    TrayMn.addAction (QuitAct); //退出按钮。
    TrayMn.addAction(menuBarVisibleAction); //菜单栏切换按钮。


//    TrayIcn.setContextMenu (&TrayMn); //设置右键菜单。
    TrayIcn.setIcon (QIcon(":/Images/icon.1.png")); //设置托盘图标的图片。
    TrayIcn.show(); //显示托盘图标。

} //void AptF::showTray()

/*!
 * \brief XyRunner::AsesShow 点击左键时，显示主窗口。
 * \param ActRsn 激活原因。
 */
void SBrowserCppWebEngine::AsesShow(QSystemTrayIcon::ActivationReason ActRsn)
{
    if (ActRsn==QSystemTrayIcon::Trigger) //左键。
    {
        show(); //显示。
    } //if (ActRsn==QSystemTrayIcon::Trigger) //左键。
} //void XyRunner::AsesShow(QSystemTrayIcon::ActivationReason ActRsn) //!<点击左键时，显示主窗口。


/*!
   * \brief SBrowserWindow::loadGeometry 载入几何属性。
   */
void SBrowserCppWebEngine::loadGeometry()
{
    QSettings settings; //设置信息。s

    settings.beginGroup("SBrowserWindow"); //主要窗口。
     
     
    QVariant dftSzVrt=QVariant(QSize(400, 400)); //默许窗口尺寸。
     
    QVariant crtSzVrt=settings.value("size",dftSzVrt ); //读取窗口尺寸。
    QSize crtSz=crtSzVrt.toSize(); //读取窗口尺寸。转换成尺寸对象。
     
     
     
    resize(crtSz); //重置窗口大小。
    move(settings.value("pos",QVariant(  QPoint(200, 200) )  ).toPoint()); //重置窗口位置。
    settings.endGroup(); //关闭分组。
} //SBrowserWindow:: loadGeometry()
  
/*!
   * \brief SBrowserWindow::saveGeometry 保存几何属性。
   */
void SBrowserCppWebEngine:: saveGeometry()
{
    QSettings settings; //设置信息储存对象。

     settings.beginGroup("SBrowserWindow"); //开始分组。
     
     QVariant crtSz=QVariant(size()); //窗口尺寸。
     
     settings.setValue("size", crtSz); //记录窗口尺寸。
     
     
     QVariant crtPos=QVariant(pos()); //窗口位置。
     
     settings.setValue("pos", crtPos); //记录窗口位置。
     
     settings.endGroup(); //关闭分组。
     
     settings.sync(); //同步。
} //void SBrowserWindow:: saveGeometry()
  
/*!
   * \brief SBrowserWindow::loadSessionStore 载入会话记录。
   * \param ssnStrJs 保存着会话记录的JSON字符串。
   */
void SBrowserCppWebEngine::loadSessionStore(QString ssnStrJs)
{
  //从里面解析出当前打开的标签页列表：

  QFile ssnStrJsFl(ssnStrJs); //JSON文件。
  ssnStrJsFl.open (QIODevice::ReadOnly); //打开文件。
  QByteArray ssnStrBtAr=ssnStrJsFl.readAll (); //全部读入。
  ssnStrJsFl.close (); //关闭文件。

  QJsonDocument loadDoc(QJsonDocument::fromJson(ssnStrBtAr)); //创建JSON文档。

  QJsonObject objVrt=loadDoc.object(); //将载入的JSON文档转换成JSON对象。

  QVariantMap obj=objVrt.toVariantMap (); //转换成映射。

  QVariantList wndsVl=obj["windows"].toList (); //windows键的值。

  //一个个窗口地处理：

  int tbCtr=0; //已经载入的标签页数量计数。

  pgTbs->setUpdatesEnabled (false); //批量打开标签页之前，关掉更新界面功能，避免卡。

  foreach(QVariant crtWndVrt,wndsVl) //一个个窗口对象地处理。
  {
    QVariantMap crtWnd=crtWndVrt.toMap (); //转换成映射。
    QVariantList crtTbs=crtWnd["tabs"].toList (); //获取标签页元素的值。

    //一个个标签页地处理：

    foreach(QVariant crtTbVrt,crtTbs) //一个个标签页对象地处理。
    {
      QVariantMap crtTb=crtTbVrt.toMap (); //转换成映射。

      QVariantList crtEtrs=crtTb["entries"].toList (); //获取条目列表。

      int crtIdx=crtTb["index"].toInt (); //获取下标。

      QString crtUrl; //当前URL。
      QString crtTtl; //当前页面的标题。

      if (crtIdx) //存在index元素。
      {
        QVariantMap crtEnt=crtEtrs[crtIdx-1].toMap (); //获取当前条目。

        crtUrl=crtEnt["url"].toString ();  //获取URL。

        crtUrl=QUrl::fromPercentEncoding (crtUrl.toUtf8 ()); //将百分号编码解码。

        crtTtl=crtEnt["title"].toString (); //获取标题。
      } //if (crtIdx) //存在index元素。
      else //不存在index元素，则表示该标签页是一个空标签页。
      {
        crtUrl=""; //空白URL。
        crtTtl=""; //空白标题。

      } //else //不存在index元素，则表示该标签页是一个空标签页。

      loadedTabTitleQueue << crtTtl; //将当前标题加入到队列中。
      loadedTabUrlQueue << crtUrl; //将当前URL加入到队列中。

      tbCtr+=1; //已经载入的标签页数计数。
    } //foreach(QVariant crtTbVrt,crtTbs) //一个个标签页对象地处理。
  } //wndsVl.each do |crtWnd|

  pgTbs->setUpdatesEnabled (true); //批量打开标签页之后，启用更新界面功能。


  if (tbCtr==0) //未载入任何标签页。
  {
    openNewTab(indexPageUrl); //打开新标签页。

    pgTbs->setTabsClosable(false); //不可关闭标签页。
  } //if (tbCtr==0) #未载入任何标签页。
  else //不止一个标签页。
  {
    pgTbs->setTabsClosable(true); //可关闭标签页。
  } //if (pgTbs->count()>1) //不止一个标签页。

  ttsSayLaunchFinished(); //通过语音服务器告知启动完毕。

  addTabTimer.start(); //启动用于添加各个标签页的定时器。
} //def loadSessionStore()
    
void SBrowserCppWebEngine::  loadFromFirefoxSessionStore()
{
    //解析火狐配置目录下的profiles.ini文件，获取到配置文件目录名字：
            
    QString prflsIni=QDir::homePath()+"/.mozilla/firefox/profiles.ini"; //配置文件索引。
    
    
    
    QSettings prflsStng(prflsIni,QSettings::IniFormat); //profiles.ini文件的设置信息。
    
    QString prflPath=prflsStng.value("Profile0/Path").toString(); //读取当前活跃配置信息的路径。
    
    
    QString ssnStrJs=QDir::homePath()+"/.mozilla/firefox/"+prflPath+"/sessionstore.js"; //会话记录的json文件。
    
    loadSessionStore(ssnStrJs); //从指定的JSON文件中读取会话信息。
    

} //def loadFromFirefoxSessionStore()
  
/*!
 * \brief SBrowserWindow::loadFromOwnSessionStore 从自己的会话记录文件中载入会话信息。
 */
void SBrowserCppWebEngine::loadFromOwnSessionStore()
{
    QString ssnStrJs=QDir::homePath()+"/.sbrowser/sessionstore.json"; //会话记录的JSON文件名。
    
    loadSessionStore(ssnStrJs); //从指定的JSON文件中读取会话信息。
} //def loadFromOwnSessionStore()
  
/*!
 * \brief SBrowserWindow::startLoad 开始载入会话数据。
 */
void SBrowserCppWebEngine::startLoad()
{
    QSettings settings; //设置信息。s

    settings.beginGroup("SBrowserWindow"); //主要窗口。
     
     
    bool f1stTmRun=settings.value("firstTimeRun",QVariant(  true )  ).toBool(); //读取配置信息，是否是第一次运行。
     
    QVariant flS=QVariant(false); //假。
    if (f1stTmRun) //是第一次运行。
    {
        settings.setValue("firstTimeRun",flS); //不再是第一次运行了。
       
        loadFromFirefoxSessionStore(); //从火狐的会话中读取当前的各个标签页。
    } //if (f1stTmRun) //是第一次运行。
    else //不是第一次运行。
    {
        loadFromOwnSessionStore(); //从自己的会话中读取当前的各个标签页。
    } //else //不是第一次运行。
     
    settings.endGroup(); //关闭分组。
    settings.sync(); //同步设置信息。
     
     
            
} //def startLoad()

/*!
   * \brief SBrowserWindow::showSettingDialog 显示设置对话框。
   */
void SBrowserCppWebEngine::showSettingDialog()
{
  stngDlg.show(); //显示对话框。

  stngDlg.raise(); //跳到顶部.

  stngDlg.refreshWebsitesWithProxy(); //刷新要使用代理的网站的列表。
  stngDlg.refreshAdBlockUrls(); //刷新要封锁的URL列表。

  stngDlg.setSelectedLanguage(selectedLanguage); //设置，选择的语言。
} //void SBrowserWindow::showSettingDialog() //!<显示设置对话框。

/*!
   * \brief SBrowserWindow::connectSignals 连接信号。
   */
void SBrowserCppWebEngine::connectSignals()
{
  connect(&selfQuitTimer, &QTimer::timeout, qApp, &QApplication::quit); //Quit

  connect (&stngDlg, &StbSettingDialog::accepted, this, &SBrowserCppWebEngine::saveSetting); //设置对话框确认关闭，则保存设置信息。
  connect (&stngDlg, &StbSettingDialog::accepted, this, &SBrowserCppWebEngine::syncSetting); //设置对话框确认关闭，则 Sync 设置信息。陈欣

  connect(&findDialog,SIGNAL(accepted()),this,SLOT(findText())); //查找对话框确认关闭，则查找文字。
//    connect (downloadManager (),SIGNAL(downloadFinished(QString)),this,SLOT(ttsSayDownloadFinished())); //下载完毕，则语音报告。

  connect (downloadManager (),SIGNAL(downloadFinished(QString)),this,SLOT(qrCollectBgImage(QString))); //下载完毕，则为二维码收集背景图片。
  connect (downloadManager (),SIGNAL(downloadStarted()),this,SLOT(ttsSayDownloadStarted())); //下载开始，则语音报告。
  connect (downloadManager (),&DownloadManager::downloadStarted,this,&SBrowserCppWebEngine::toastDownloadStarted); //下载开始，则吐息报告。
  connect (downloadManager (),SIGNAL(downloadError()),this,SLOT(ttsSayDownloadError())); //下载出错，则语音报告。

  connect (&addTabTimer, &QTimer::timeout, this, &SBrowserCppWebEngine::add1LoadedTab); //定时器超时，则添加一个被载入的标签页。

  connect(pgTbs,SIGNAL(currentChanged(int)),this,SLOT(refreshWindowTitle(int))); //当前标签页改变，则相应地刷新窗口标题。
  connect(pgTbs, &QTabWidget::currentChanged, this, &SBrowserCppWebEngine::refreshWindowIcon); //当前标签页改变，则相应地刷新窗口图标。
  connect(pgTbs, &QTabWidget::currentChanged, this, (void (SBrowserCppWebEngine::*)(int))&SBrowserCppWebEngine::refreshZoomFactor); //当前标签页改变，则相应地刷新缩放比例。
  connect(pgTbs, &QTabWidget::currentChanged, this, (void (SBrowserCppWebEngine::*)(int))&SBrowserCppWebEngine::syncZoomFactor); //当前标签页改变，则相应地同步缩放比例。

  connect(pgTbs, &QTabWidget::currentChanged, this, &SBrowserCppWebEngine::maybe1stTimeLoadPage); //当前标签页改变，则检查对应的页面是不是第一次载入。

  connect(pgTbs, &QTabWidget::tabCloseRequested, this, &SBrowserCppWebEngine::closeTab); //某个标签页该关闭了，则关闭该标签页。

  connect (&ntAccsMngr4WebPages,&QNetworkAccessManager::authenticationRequired,this,&SBrowserCppWebEngine::handleAuthentication); //需要认证，则处理认证问题。
  connect (&ntAccsMngr4WebPages,&QNetworkAccessManager::sslErrors,this,&SBrowserCppWebEngine::handleSslErrors); //需要处理SSL错误，则处理。

  QWebEngineProfile * profile=QWebEngineProfile::defaultProfile (); //获取配置档。
  connect (profile, &QWebEngineProfile::downloadRequested, this, &SBrowserCppWebEngine::handleDownloadRequest); //收到下载请求，则处理下载请求。

  connect(ldTimer, &QTimer::timeout,this, &SBrowserCppWebEngine::startLoad); //定时器超时，则开始载入。

  connect(svSsnTmr, &QTimer::timeout, this, &SBrowserCppWebEngine::saveSessionStore); //定时保存会话信息。

  connect (backToTopToolButton, &QToolButton::clicked, this, &SBrowserCppWebEngine::goBackTop); //回到顶端。
  connect (tabsManager,&TabManager::shouldSwitchTab,this,&SBrowserCppWebEngine::switchTab); //应当切换标签页，则切换。

  connect(&searchEngineManager,&SearchEngineManager::shouldOpenNewTab,this,&SBrowserCppWebEngine::openNewTab); //要求打开新标签页，则打开一个。
    
  connect(&launchTimer,&QTimer::timeout,this,&SBrowserCppWebEngine::launchSbrowserDownloader); //启动定时器超时，则启动扬声器子系统。
  connect(&downloaderProcess, QOverload<int , QProcess::ExitStatus >::of(&QProcess::finished), this, &SBrowserCppWebEngine::assessRestartSpeakerSubSystem); //扬声器子系统退出，则考虑是否要重新启动进程。
    
#ifndef Q_OS_MACOS
  connect(&memoryMonitor, &MemoryMonitor::shouldQuit, this, &SBrowserCppWebEngine::quit); //Should quit, then quit
#endif


} //void SBrowserWindow::connectSignals() //连接信号。

/*!
 * \brief SBrowserCppWebEngine::disconnectSignals Diconnect signals. 陈欣.
 */
void SBrowserCppWebEngine::disconnectSignals()
{
    downloaderProcess.disconnect();

    downloaderProcess.disconnect(this);
} //void SBrowserCppWebEngine::disconnectSignals()

/*!
 * \brief LogicControl::launchSpeakerSubSystem 启动定时器超时，则启动扬声器子系统。
 */
void SBrowserCppWebEngine::launchSbrowserDownloader()
{
    QProcessEnvironment processEnvironment=QProcessEnvironment::systemEnvironment(); //获取系统环境变量。

    QString ldlibrarypathDebug="QTWEBENGINE_REMOTE_DEBUGGING";

    processEnvironment.remove(ldlibrarypathDebug); //删除调试端口环境变量

    RotatePredictorAptCmd.prepend(QCoreApplication::applicationDirPath()+'/'); // 追加路径


    QStringList AptPrms; //命令行参数。

    downloaderProcess.setProcessEnvironment(processEnvironment); //设置环境变量。

    downloaderProcess.start(RotatePredictorAptCmd, AptPrms); //启动进程。
} //void LogicControl::launchSpeakerSubSystem()

/*!
 * \brief LogicControl::assessRestartSpeakerSubSystem 扬声器子系统退出，则考虑是否要重新启动进程。
 * \param exitCode 退出码。
 * \param exitStatus 退出状态。
 */
void SBrowserCppWebEngine::assessRestartSpeakerSubSystem(int exitCode, QProcess::ExitStatus exitStatus)
{
  Q_UNUSED(exitCode); //
    
  if (exitStatus==QProcess::CrashExit) //
  {
    launchSbrowserDownloader(); //启动扬声器子系统进程。
  } //if (exitStatus==QProcess::CrashExit) //
} //void LogicControl::assessRestartSpeakerSubSystem(int exitCode, QProcess::ExitStatus exitStatus)

/*!
 * \brief SBrowserWindow::saveSetting 设置对话框确认关闭，则保存设置信息。
 */
void SBrowserCppWebEngine::saveSetting()
{
  QSettings settings; //设置信息储存对象。

  settings.beginGroup("SBrowser"); //开始分组。

  //智能代理：
  QVariant crtSz=QVariant(stngDlg.isUsingSmartProxy()); //是否使用智能代理。
  settings.setValue("useSmartProxy", crtSz); //记录是否使用智能代理。
  prxyFctry->setUsingSmartProxy (stngDlg.isUsingSmartProxy ()); //设置是否要使用智能代理。

  QVariant crtSzAllProxy=QVariant(stngDlg.isAllUsingProxy()); //是否all 使用代理。
  settings.setValue("allUseProxy", crtSzAllProxy); //记录是否all使用代理。
  prxyFctry->setAllUsingProxy (stngDlg.isAllUsingProxy ()); //设置是否要all使用代理。

  QVariant crtSzAutoAddProxyRule=QVariant(stngDlg.isAutoAddProxyRule()); //是否自动添加代理规则。
  settings.setValue("autoAddRule", crtSzAutoAddProxyRule); //记录是否自动添加代理规则。
  prxyFctry->setAutoAddProxyRule (stngDlg.isAutoAddProxyRule ()); //设置是否自动添加代理规则。

  //是否要通过代理进行DNS解析：
  QVariant crtSzVariantUseProxyForDns=QVariant(stngDlg.isUseProxyForDns()); //是否要通过代理进行DNS解析。
  settings.setValue("useProxyForDns", crtSzVariantUseProxyForDns); //记录是否自动添加代理规则。
  prxyFctry->setUseProxyForDns (stngDlg.isUseProxyForDns ()); //是否要通过代理进行DNS解析。

  QVariant crtPos=QVariant(stngDlg.getSmartProxyPort()); //智能代理的端口号。
  settings.setValue("smartProxyPort", crtPos); //记录智能代理的端口号。
  prxyFctry->setSmartProxyPort(stngDlg.getSmartProxyPort()); //设置智能代理的端口号。

  //http代理端口号：
  QVariant crtPosHttp=QVariant(stngDlg.getSmartProxyPortHttp()); //智能代理的http端口号。
  settings.setValue("smartProxyPortHttp", crtPosHttp); //记录智能代理的http端口号。
  prxyFctry->setSmartProxyPortHttp(stngDlg.getSmartProxyPortHttp()); //设置智能代理的http端口号。

  QVariant smartProxyHost=QVariant(stngDlg.getSmartProxyHost()); //智能代理的主机名。
  settings.setValue ("smartProxyHost",smartProxyHost); //记录智能代理的主机名。
  prxyFctry->setSmartProxyHost(stngDlg.getSmartProxyHost()); //设置智能代理的主机名。

  //禁用广告：
  QVariant crtSzVrtEnableAdBlock=stngDlg.getEnableAdBlock(); //读取是否要启用广告禁用功能。
  settings.setValue ("enableAdBlock",crtSzVrtEnableAdBlock); //保存，是否要启用广告禁用功能。
  prxyFctry->setEnableAdBlock (stngDlg.getEnableAdBlock()); //设置，是否要启用广告禁用功能。

  //界面语言：
  QVariant manuallySelectLanguageVariant(stngDlg.getManuallySelectLanguage()); //是否手动选择界面语言。
  settings.setValue ("manuallySelectLanguage",manuallySelectLanguageVariant); //记录，是否手动选择界面语言。

  QVariant selectedLanguageVariant(stngDlg.getSelectedLanguage()); //获取选择的语言。
  settings.setValue ("selectedLanguage",selectedLanguageVariant); //记录，选择的语言。

  if (stngDlg.getManuallySelectLanguage()) //要手动选择语言。
  {
    loadTranslation(stngDlg.getSelectedLanguage()); //载入对应语言的翻译。
  } //if (stngDlg.getManuallySelectLanguage()) //要手动选择语言。

  //TTS:
  QVariant ttsServerAddress=QVariant(stngDlg.getTtsServerAddress()); //获取TTS服务器的地址。

  settings.setValue("ttsServerAddress",ttsServerAddress); //tts服务器的地址.

  QVariant ttsServerPort=QVariant(stngDlg.getTtsServerPort()); //获取TTS服务器的端口号。

  settings.setValue("ttsServerPort",ttsServerPort); //tts server's port.

  ttsClient.setServerAddress (stngDlg.getTtsServerAddress ()); //设置TTS服务器的地址。
  ttsClient.setServerPort (stngDlg.getTtsServerPort ()); //设置TTS服务器的端口号。

  //WebGl:
  QVariant enblWbGl=QVariant(stngDlg.getWebGlEnabled()); //是否启用WebGl.
  settings.setValue ("enableWebGl",enblWbGl); //是否启用WebGl.

  // Quit when idle for 10 minutes:
  quitIdle10MinutesbGl=stngDlg.getQuitIdle10Minutes(); // Get settings.
  auto quitIdle10MinutesVariant=QVariant(quitIdle10MinutesbGl);
  settings.setValue ("quitIdle10Minutes", quitIdle10MinutesVariant); // Set settings.

  // 失去焦点时暂停播放多媒体:
  pauseMediaWhenLostFocusesbGl=stngDlg.getPauseMediaWhenLostFocus(); // Get settings.
  auto pauseMediaWhenLostFocusesbGlVariant=QVariant(pauseMediaWhenLostFocusesbGl);
  settings.setValue ("pauseMediaWhenLostFocuse", pauseMediaWhenLostFocusesbGlVariant); // Set settings.

  //插件：
  auto enablePluginsBool=stngDlg.getPluginsEnabled();

//    cout << __FILE__ << __LINE__ << __func__ << tr("enabled plugins?:").toStdString ()<< enablePluginsBool << endl; //Debug.

  QVariant enablePlugins=QVariant(enablePluginsBool); //是否启用插件。
  settings.setValue ("enabledPlugins",enablePlugins); //设置值，是否启用插件。
  // QWebEngineSettings * glbWbStng=QWebEngineSettings::globalSettings(); //获取全局的网页设置对象。
  // glbWbStng->setAttribute(QWebEngineSettings::PluginsEnabled, enablePluginsBool); //设置是否启用Javascript.

    //是否启用Javascript：
    bool enableJavascript=stngDlg.getJavascriptEnabled(); //是否启用插件。
    QVariant enableJavascriptVariant=QVariant(enableJavascript);
    settings.setValue("enableJavascript",enableJavascriptVariant);
//    QWebEngineSettings * glbWbStng=QWebEngineSettings::globalSettings(); //获取全局的网页设置对象。
    // glbWbStng->setAttribute(QWebEngineSettings::JavascriptEnabled,enableJavascript); //设置是否启用Javascript.
    
    //是否在内存低时退出：
    bool lowMemoryQuitenableJavascript=stngDlg.getQuitLowMemory(); //是否启用插件。

//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", quit low memory: " << lowMemoryQuitenableJavascript; //报告错误码。


    QVariant lowMemoryQuitenableJavascriptVariant=QVariant(lowMemoryQuitenableJavascript);
    settings.setValue("quitLowMemory",lowMemoryQuitenableJavascriptVariant);
        if (lowMemoryQuitenableJavascript) //要在低内存时退出。
        {
#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
//            memoryMonitor.setLowMemoryBehavior(LowMemoryBehavior::Quit); //
            memoryMonitor.setLowMemoryBehavior(LowMemoryBehavior::RequestQuit); //
#endif
#endif
        } //if (quitLowMemoryenableJavascript) //要在低内存时退出。
        else //不要在低内存时退出。
        {
#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
            memoryMonitor.setLowMemoryBehavior(LowMemoryBehavior::Report); //
#endif
#endif

        } //else //不要在低内存时退出。

//    //是否在内存不足时退出：
//    bool quitLowMemoryenableJavascript=settings.value("quitLowMemory",QVariant(true)).toBool (); //是否启用插件。
//    stngDlg.setQuitLowMemory(quitLowMemoryenableJavascript); //设置是否启用插件。

//    if (quitLowMemoryenableJavascript) //要在低内存时退出。
//    {
//        memoryMonitor.setLowMemoryBehavior(LowMemoryBehavior::Quit); //

//    } //if (quitLowMemoryenableJavascript) //要在低内存时退出。
//    else //不要在低内存时退出。
//    {
//        memoryMonitor.setLowMemoryBehavior(LowMemoryBehavior::Report); //

    //首页：
    QVariant indexPage=QVariant(stngDlg.getIndexPage()); //首页网址。
    settings.setValue ("indexPage",indexPage); //首页网址。
    indexPageUrl=stngDlg.getIndexPage(); //记录首页网址。

    //用户代理字符串：
    QVariant userAgentVariant=QVariant(stngDlg.getUserAgent ()); //获取用户代理字符串。
    settings.setValue ("userAgent", userAgentVariant); //用户代理字符串。
    userAgent=stngDlg.getUserAgent (); //记录用户代理字符串。
    QWebEngineProfile * defaultProfile=QWebEngineProfile::defaultProfile (); //获取默认配置。
    defaultProfile->setHttpUserAgent (userAgent); //设置用户代理字符串。

    //download directory：
    QVariant downloadDirectoryVariant=QVariant(stngDlg.getDownloadDirectory()); //获取用户代理字符串。
    settings.setValue ("downloadDirectory", downloadDirectoryVariant); //用户代理字符串。
//    userAgent=stngDlg.getUserAgent (); //记录用户代理字符串。
//    QWebEngineProfile * defaultProfile=QWebEngineProfile::defaultProfile (); //获取默认配置。
//    defaultProfile->setHttpUserAgent (userAgent); //设置用户代理字符串。

    //搜索引擎：
    QString searchEngineName=stngDlg.getSearchEngineName(); //获取搜索引擎名字。
    QVariant searchEnginesmartProxyHost=QVariant(searchEngineName); //智能代理的主机名。
    settings.setValue ("searchEngine",searchEnginesmartProxyHost); //记录智能代理的主机名。

    //下载器类型：
    downloaderType=stngDlg.getDownloaderType(); //获取下载器类型。
    auto downloaderTypeVariant=QVariant(downloaderType); //转换成变种
    settings.setValue("downloaderType", downloaderTypeVariant); //设置内容
//    downloaderType=settings.value("downloaderType", DownloaderType::SuperDownloader).toString();
//    stngDlg.setDownloaderType(downloaderType); //设置下载器类型


    settings.endGroup(); //关闭分组。

    settings.sync(); //同步。
} //void SBrowserWindow::saveSetting() //!<设置对话框确认关闭，则保存设置信息。

/*!
 * \brief SBrowserWindow::loadSetting 载入设置信息。
 */
void SBrowserCppWebEngine::loadSetting()
{
  QSettings settings; //设置信息。s

  settings.beginGroup("SBrowser"); //主程序。

  QVariant dftSzVrt=QVariant(QSize(400, 400)); //默许窗口尺寸。

  //智能代理：
  QVariant crtSzVrt=settings.value("useSmartProxy",dftSzVrt ); //读取是否要使用智能代理。
  bool crtSz=crtSzVrt.toBool (); //转换成逻辑值。
  stngDlg.setUsingSmartProxy(crtSz); //设置是否要使用智能代理。
  prxyFctry->setUsingSmartProxy(crtSz); //设置是否要使用智能代理。

  QVariant crtSzVrtallUseProxy=settings.value("allUseProxy",dftSzVrt ); //读取是否要all使用代理。
  bool crtSzcrtSzVrtallUseProxy=crtSzVrtallUseProxy.toBool (); //转换成逻辑值。
  stngDlg.setAllUsingProxy(crtSzcrtSzVrtallUseProxy); //设置是否要all use代理。
  prxyFctry->setAllUsingProxy(crtSzcrtSzVrtallUseProxy); //设置是否要all使用代理。

  QVariant crtSzVrtAutoAddRule=settings.value("autoAddRule",dftSzVrt ); //读取是否要自动添加代理规则。
  bool crtSzcrtSzVrtAutoAddRule=crtSzVrtAutoAddRule.toBool (); //转换成逻辑值。
  stngDlg.setAutoAddProxyRule(crtSzcrtSzVrtAutoAddRule); //设置是否要自动添加代理规则。
  prxyFctry->setAutoAddProxyRule (crtSzcrtSzVrtAutoAddRule); //设置是否要自动添加代理规则。

  //是否要通过代理进行DNS解析：
  QVariant crtSzVariantUseProxyForDns=settings.value("useProxyForDns",dftSzVrt ); //读取是否要通过代理来进行DNS解析。
  bool crtSzcrtSzVrtVariantUseProxyForDns=crtSzVariantUseProxyForDns.toBool (); //转换成逻辑值。
  stngDlg.setUseProxyForDns(crtSzcrtSzVrtVariantUseProxyForDns); //设置是否要通过代理来进行DNS解析。
  prxyFctry->setUseProxyForDns(crtSzcrtSzVrtVariantUseProxyForDns); //设置是否要通过代理来进行DNS解析。

  quint16 smtPrxyPrt=settings.value("smartProxyPort",QVariant(  9090 )  ).toUInt (); //读取智能代理的端口号。
  stngDlg.setSmartProxyPort(smtPrxyPrt); //设置智能代理的端口号。
  prxyFctry->setSmartProxyPort(smtPrxyPrt); //设置智能代理的端口号。

  //http代理端口号：
  quint16 smtPrxyPrtHttp=settings.value("smartProxyPortHttp",QVariant(  8118)  ).toUInt (); //读取智能代理的http端口号。
  stngDlg.setSmartProxyPortHttp(smtPrxyPrtHttp); //设置智能代理的http端口号。
  prxyFctry->setSmartProxyPortHttp(smtPrxyPrtHttp); //设置智能代理的http端口号。

  QString smartProxyHost=settings.value("smartProxyHost",QVariant("localhost")).toString (); //读取智能代理的主机名。
  stngDlg.setSmartProxyHost(smartProxyHost); //设置智能代理的主机名。
  prxyFctry->setSmartProxyHost(smartProxyHost); //设置智能代理的主机名。

  //禁用广告：
  QVariant crtSzVrtEnableAdBlock=settings.value("enableAdBlock",dftSzVrt ); //读取是否要启用广告禁用功能。
  bool crtSzEnableAdBlock=crtSzVrtEnableAdBlock.toBool (); //转换成逻辑值。

  stngDlg.setEnableAdBlock(crtSzEnableAdBlock); //设置是否要启用广告禁用功能。
  prxyFctry->setEnableAdBlock(crtSzEnableAdBlock); //设置是否要启用广告禁用功能。

  //界面语言：
  QVariant manuallySelectLanguageVariant=settings.value ("manuallySelectLanguage",dftSzVrt); //读取，是否要手动选择语言。
  bool manuallySelectLanguage=manuallySelectLanguageVariant.toBool (); //转换成逻辑值。
  stngDlg.setManuallySelectLanguage(manuallySelectLanguage); //设置，是否要手动选择语言。

  QVariant selectedLanguageVariant=settings.value ("selectedLanguage",dftSzVrt); //读取，选择的语言。
  selectedLanguage=selectedLanguageVariant.toString (); //转换成字符串。
  stngDlg.setSelectedLanguage(selectedLanguage); //设置，选择的语言。

  if (manuallySelectLanguage) //要手动选择语言。
  {
    loadTranslation(selectedLanguage); //载入对应语言的翻译。
  } //if (manuallySelectLanguage) //要手动选择语言。

  //TTS:
  QString ttsServerAddress=settings.value("ttsServerAddress",QVariant("192.168.2.113")).toString (); //获取tts服务器的地址.

  ttsClient.setServerAddress(ttsServerAddress); //设置服务器地址。
  stngDlg.setTtsServerAddress(ttsServerAddress); //设置TTS服务器地址。

  quint16 ttsServerPort=settings.value("ttsServerPort",QVariant(11245)).toUInt (); //获取tts服务器的端口号。

  ttsClient.setServerPort(ttsServerPort); //设置服务器端口号。
  stngDlg.setTtsServerPort(ttsServerPort); //设置TTS服务器的端口号。

  //是否启用WebGL:
  bool enblWbGl=settings.value ("enableWebGl",QVariant(false)).toBool (); //是否启用WebGl.
  stngDlg.setWebglEnabled(enblWbGl); //设置是否启用WebGl.

  // Quit when idle for 10 minutes:
  quitIdle10MinutesbGl=settings.value ("quitIdle10Minutes", QVariant(true)).toBool (); // 是否 quit when idle for 10 minutes.
  stngDlg.setQuitIdle10Minutes(quitIdle10MinutesbGl); // 设置是否 quit when idle for 10 minutes.

  // 失去焦点时暂停播放多媒体内容:
  pauseMediaWhenLostFocusesbGl=settings.value ("pauseMediaWhenLostFocus", QVariant(true)).toBool (); // 是否 quit when idle for 10 minutes.
  stngDlg.setPauseMediaWhenLostFocus(pauseMediaWhenLostFocusesbGl); // 设置是否 quit when idle for 10 minutes.

  //是否启用插件：
  bool enablePlugins=settings.value("enabledPlugins",QVariant(false)).toBool (); //是否启用插件。
  qDebug() << __FILE__ << __LINE__ << __func__ << tr("enabled plugins?:") << enablePlugins; //Debug.
//    cout << __FILE__ << __LINE__ << __func__ << tr("enabled plugins?:").toStdString ()<< enablePlugins << endl; //Debug.
  stngDlg.setPluginsEnabled(enablePlugins); //设置是否启用插件。
  // QWebEngineSettings * glbWbStng=QWebEngineSettings::globalSettings(); //获取全局的网页设置对象。
  // glbWbStng->setAttribute(QWebEngineSettings::PluginsEnabled,enablePlugins); //设置是否启用Javascript.

  //是否启用Javascript：
  bool enableJavascript=settings.value("enableJavascript",QVariant(true)).toBool (); //是否启用插件。
  stngDlg.setJavascriptEnabled(enableJavascript); //设置是否启用插件。
//    QWebEngineSettings * glbWbStng=QWebEngineSettings::globalSettings(); //获取全局的网页设置对象。
  // glbWbStng->setAttribute(QWebEngineSettings::JavascriptEnabled,enableJavascript); //设置是否启用Javascript.
    
  //是否在内存不足时退出：
  bool quitLowMemoryenableJavascript=settings.value("quitLowMemory",QVariant(true)).toBool (); //是否启用插件。
  stngDlg.setQuitLowMemory(quitLowMemoryenableJavascript); //设置是否启用插件。

  if (quitLowMemoryenableJavascript) //要在低内存时退出。
  {
#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
//        memoryMonitor.setLowMemoryBehavior(LowMemoryBehavior::Quit); //
    memoryMonitor.setLowMemoryBehavior(LowMemoryBehavior::RequestQuit); //
#endif
#endif
    } //if (quitLowMemoryenableJavascript) //要在低内存时退出。
    else //不要在低内存时退出。
    {
   #ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
        memoryMonitor.setLowMemoryBehavior(LowMemoryBehavior::Report); //
#endif
#endif
    } //else //不要在低内存时退出。


    //首页：
    indexPageUrl=settings.value ("indexPage",indexPageUrl).toString (); //获取首页网址。
    stngDlg.setIndexPage(indexPageUrl); //设置首页网址。
    
    //用户代理字符串：
    userAgent=settings.value ("userAgent", DefaultUserAgent).toString (); //获取用户代理字符串。
    stngDlg.setUserAgent(userAgent); //设置首页网址。
    QWebEngineProfile * defaultProfile=QWebEngineProfile::defaultProfile (); //获取默认配置。
    defaultProfile->setHttpUserAgent (userAgent); //设置用户代理字符串。

//    QDir downloadDir(QStandardPaths::writableLocation (QStandardPaths::HomeLocation)+tr("/Download/")); //下载目录。

//    QStandardPaths::DownloadLocation
    auto DefaultDownloadDirectory=QStandardPaths::writableLocation (QStandardPaths::DownloadLocation);

    //Download directory：
    auto downloadDirectory=settings.value ("downloadDirectory", DefaultDownloadDirectory).toString (); //获取用户代理字符串。
    stngDlg.setDownloadDirectory(downloadDirectory); //设置首页网址。
//    QWebEngineProfile * defaultProfile=QWebEngineProfile::defaultProfile (); //获取默认配置。
//    defaultProfile->setHttpUserAgent (userAgent); //设置用户代理字符串。


    //下载器类型：
    downloaderType=settings.value("downloaderType", DownloaderType::SuperDownloader).toString();
    stngDlg.setDownloaderType(downloaderType); //设置下载器类型


    //搜索引擎：
    QString searchEnginesmartProxyHost=settings.value("searchEngine",QVariant("")).toString (); //读取智能代理的主机名。
    
    if (searchEnginesmartProxyHost.isEmpty()) //空白。
    {
    } //if (smartProxyHost.isEmpty()) //空白。
    else //不是空白。
    {
        selectAudioInputDeviceByName(searchEnginesmartProxyHost); //按照名字选择对应的声音输入设备。
    } //else //不是空白。

    settings.endGroup(); //关闭分组。
} //void SBrowserWindow::loadSetting() //!<载入设置信息。

/*!
 * \brief AptF::selectAudioInputDeviceByName 按照名字选择对应的声音输入设备。
 * \param smartProxyHost 声音输入设备的名字。
 */
void SBrowserCppWebEngine::selectAudioInputDeviceByName(QString smartProxyHost)
{
    stngDlg.selectSearchEngineByName(smartProxyHost); //按照名字来选中搜索引擎。
} //void AptF::selectAudioInputDeviceByName(QString smartProxyHost)

/*!
 * \brief SBrowserWindow::createNetworkManager 创建网络管理器。
 */
void SBrowserCppWebEngine::createNetworkManager()
{
    StbShadowProxyFactory * shadowPrxyFctry=new StbShadowProxyFactory(prxyFctry); //创建影子代理工厂。

    ntAccsMngr4WebPages.setProxyFactory (shadowPrxyFctry); //设置代理工厂。
} //void SBrowserWindow::createNetworkManager() //!<创建网络管理器。

/*!
 * \brief SBrowserWindow::toggleProxyRule 要求切换智能代理规则，则切换。
 * \param enb 是否启用针对此主机的代理规则。
 * \param hstNm 主机名。
 */
void SBrowserCppWebEngine::toggleProxyRule(bool enb,QString hstNm)
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
 * \brief SBrowserWindow::closeSettingDialog 关闭设置对话框。
 */
void SBrowserCppWebEngine::closeSettingDialog()
{
    stngDlg.close (); //关闭对话框。
} //void SBrowserWindow::closeSettingDialog() //!<关闭设置对话框。

/*!
 * \brief SBrowserWindow::createCookieJar 创建小甜饼罐。
 */
void SBrowserCppWebEngine::createCookieJar()
{
    QWebEngineProfile * profile=QWebEngineProfile::defaultProfile (); //获取配置档。

    QWebEngineCookieStore * cookieStore=profile->cookieStore(); //获取小甜饼存储。

    CookieJar * crtCkJr=new CookieJar(); //创建小甜饼罐。


    connect(cookieStore, &QWebEngineCookieStore::cookieAdded, crtCkJr, &CookieJar::insertCookie);
    connect(cookieStore, &QWebEngineCookieStore::cookieRemoved, crtCkJr, &CookieJar::deleteCookie);

    ntAccsMngr4WebPages.setCookieJar (crtCkJr); //设置小甜饼罐。
} //void SBrowserWindow::createCookieJar() //!<创建小甜饼罐。

/*!
 * \brief SBrowserWindow::downloadManager 获取下载管理器指针。
 * \return 下载管理器的指针。
 */
DownloadManager *SBrowserCppWebEngine::downloadManager()
{
    if (!s_downloadManager) //下载管理器还不存在。
    {
        s_downloadManager = new DownloadManager(); //创建一个。

        s_downloadManager->setRswitch(rswitch); //设置远程进程调用客户端。

        s_downloadManager->setNetworkAccessManager(&ntAccsMngr4WebPages); //设置网络访问管理器。
    } //if (!s_downloadManager) //下载管理器还不存在。

    return s_downloadManager;
} //DownloadManager *SBrowserWindow::downloadManager()

/*!
 * \brief SBrowserWindow::showFindDialog 显示查找对话框。
 */
void SBrowserCppWebEngine::showFindDialog()
{
    findDialog.show(); //显示出查找对话框。
} //void SBrowserWindow::showFindDialog() //!<显示查找对话框。

/*!
 * \brief SBrowserWindow::findText 查找对话框确认关闭，则查找文字。
 */
void SBrowserCppWebEngine::findText()
{
  QString txt2Fnd=findDialog.getSearchingText(); //获取要搜索的文字。

  SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

  crtPgWdgt->findText(txt2Fnd); //由该标签页来查找文字。
} //void SBrowserWindow::findText() //!<查找对话框确认关闭，则查找文字。

/*!
 * \brief SBrowserCppWebEngine::findTextPrevious 查找上一个匹配项（Shift+F3）
 */
void SBrowserCppWebEngine::findTextPrevious()
{
    QString txt2Fnd = findDialog.getSearchingText(); //获取要搜索的文字
    SWebPageWidget *crtPgWdgt = (SWebPageWidget *)(pgTbs->currentWidget()); //获取当前标签页

    // 传递反向查找标志
    crtPgWdgt->findText(txt2Fnd, true); // 第二个参数表示反向查找
}

/*!
 * \brief SBrowserWindow::showDownloadManager 显示下载管理器。
 */
void SBrowserCppWebEngine::showDownloadManager()
{
//    downloadManager()->show (); //显示。

//    downloadManager()->raise(); //跳到顶部.
    
    
//    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", " << endl; //Debug.
    
    rswitch->shouldShowDownloadManager(); //应当显示下载管理器。
} //void SBrowserWindow::showDownloadManager() //!<显示下载管理器。

void SBrowserCppWebEngine::applyWebChannel()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->applyWebChannel(); //由该标签页来全选文字。

} // void SBrowserCppWebEngine::applyWebChannel()

/*!
 * \brief SBrowserWindow::selectAll 全选。
 */
void SBrowserCppWebEngine::selectAll()
{


    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->selectAll(); //由该标签页来全选文字。

} //void SBrowserWindow::selectAll() //!<全选。

/*!
 * \brief SBrowserWindow::ttsSayLoadFinished 通过TTS报告载入完成。
 */
void SBrowserCppWebEngine::ttsSayLoadFinished()
{
    ttsClient.saySentence(tr("Webpage load finished.")); //说话，载入完毕。

    return;
} //void SBrowserWindow::ttsSayLoadFinished() //!<通过TTS报告载入完成。

/*!
 * \brief SBrowserWindow::ttsSayLaunchFinished 通过语音服务器告知启动完毕。
 */
void SBrowserCppWebEngine::ttsSayLaunchFinished()
{
    ttsClient.saySentence(tr("Browser start finished.")); //说话，启动完毕。

    return;
} //void SBrowserWindow::ttsSayLaunchFinished() //!<通过语音服务器告知启动完毕。

/*!
 * \brief SBrowserWindow::ttsSayDownloadFinished 语音报告下载完毕。
 */
void SBrowserCppWebEngine::ttsSayDownloadFinished()
{
    ttsClient.saySentence(tr("File download finished.")); //说话，下载完毕。

    return;
} //void SBrowserWindow::ttsSayDownloadFinished() //!<语音报告下载完毕。

/*!
 * \brief SBrowserWindow::toastDownloadStarted 下载开始，则吐息报告。
 */
void SBrowserCppWebEngine::toastDownloadStarted()
{
#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    QWidget * context = this;
    QString text = tr("File download started.");
    Toast::DurationLength duration = Toast::LENGTH_SHORT;
    Toast * toast = Toast::makeText(context, text, duration);
    toast->show();
#endif
#endif
} //void SBrowserWindow::toastDownloadStarted()

/*!
 * \brief SBrowserWindow::ttsSayDownloadStarted 语音报告下载开始。
 */
void SBrowserCppWebEngine::ttsSayDownloadStarted()
{
    ttsClient.saySentence(tr("File download started.")); //说话，下载开始。

    return;
} //void SBrowserWindow::ttsSayDownloadStarted() //!<语音报告下载开始。

/*!
 * \brief SBrowserWindow::showAboutDialog 显示出关于对话框。
 */
void SBrowserCppWebEngine::showAboutDialog()
{
    mAboutDialog->show(); //显示出关于对话框。
} //void SBrowserWindow::showAboutDialog() //!<显示出关于对话框。

/*!
 * \brief SBrowserWindow::checkNewVersion 检查是否有新版本。
 */
void SBrowserCppWebEngine::checkNewVersion()
{
  QString urlString=tr("http://sweetsoft.info:18013/CheckNewVersion/?CurrentVersion=%1&Platform=%2&BitAmount=%3&PackageName=com.stupidbeauty.sbrowser").arg (currentVersionNumber).arg (platformName).arg (bitAmount); //检查更新的URL。

  checkNewVersionRequest=new QNetworkRequest(QUrl(urlString)); //Create the request.
  QNetworkReply* CrtRply= ntAccsMngr4WebPages.get(*checkNewVersionRequest); //发送GET请求。

  connect(CrtRply,SIGNAL(finished()),this,SLOT(ParseCheckNewVersionResult())); //完成之後，處理其結果。
} //void SBrowserWindow::checkNewVersion() //!<检查是否有新版本。

/*!
 * \brief SBrowserWindow::ParseCheckNewVersionResult 完成之後，處理 check new version 的結果。
 */
void SBrowserCppWebEngine::ParseCheckNewVersionResult()
{
  QNetworkReply* Rst=(QNetworkReply *)(sender()); //獲取發射信號的回復對象的指針。

  QString WholeRest=""; //整个结果。

  QTextCodec * gbk_codec=QTextCodec::codecForName("UTF8"); //文字解码器。

  QByteArray ssnStrBtAr=Rst->readAll (); //读取全部内容。

  WholeRest=gbk_codec->toUnicode(  ssnStrBtAr); //转义成UTF8字符串。

  QJsonDocument loadDoc(QJsonDocument::fromJson(ssnStrBtAr)); //创建JSON文档。

    QJsonObject objVrt=loadDoc.object(); //将载入的JSON文档转换成JSON对象。

    QVariantMap obj=objVrt.toVariantMap (); //转换成映射。

    bool isSuccess=obj["success"].toBool (); //获取属性，是否检查成功。

    if (isSuccess) //检查更新成功。
    {
        bool hasNewVersion=obj["hasnewversion"].toBool(); //获取属性，是否有新版本。

        if (hasNewVersion) //有新版本。
        {
            QString newVersion=obj["newversion"].toString (); //获取属性，新版本的版本号。

            QString packageUrl=obj["packageurl"].toString (); //获取属性，新版本安装包的下载地址。

            mAboutDialog->setNewVersionNumber(newVersion); //设置新版本的版本号。
            mAboutDialog->setPackgeUrl(packageUrl); //设置新版本安装包的下载URL。

            u->menu_New_version->show (); //显示出新版本菜单。
        } //if (hasNewVersion) //有新版本。

    } //if (isSuccess) //检查更新成功。
    else //检查更新失败。
    {
        QString failReason=obj["failreason"].toString (); //获取失败原因。


    } //else //检查更新失败。

    delete Rst; //释放内存。


    return;
} //void CheckPhoneNumberService::ParseCheckPhoneResult() //!<完成之後，處理欧飞服务器检查手机号码的結果。

/*!
 * \brief SBrowserWindow::determinePlatformName 确定平台名字。
 */
void SBrowserCppWebEngine::determinePlatformName()
{
#ifdef Q_OS_LINUX //GNU/Linux系统。
    //按照GNU/Linux系统来处理：

    determinPlatformNameGnuLinux(); //确定具体的GNU/Linux发行版名字。

#endif

#ifdef Q_OS_WIN //Windows系统。
    platformName="windows"; //平台名字是windows.
#endif


    return;
} //void SBrowserWindow::determinePlatformName()

/*!
 * \brief SBrowserWindow::determinPlatformNameGnuLinux 确定具体的GNU/Linux发行版名字。
 */
void SBrowserCppWebEngine::determinPlatformNameGnuLinux()
{
    if (isMagicLinux()) //是运行于MagicLinux.
    {


        platformName="magiclinux"; //平台名字是magiclinux.
    } //if (isMagicLinux()) //是运行于MagicLinux.
    else //不是运行于MagicLinux.
    {
        platformName="gnulinux"; //平台名字是一般的gnu/linux.
    } //else //不是运行于MagicLinux.

    return;
} //void SBrowserWindow::determinPlatformNameGnuLinux()

/*!
 * \brief SBrowserWindow::isMagicLinux 是否是运行于MagicLinux.
 * \return 是否是运行于MagicLinux.
 */
bool SBrowserCppWebEngine::isMagicLinux()
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
void SBrowserCppWebEngine::determinBitAmount()
{
#ifdef Q_PROCESSOR_X86_64 //64位。
    bitAmount=64; //64位。
#else //32位。
    bitAmount=32; //32位。
#endif
} //void SBrowserWindow::determinBitAmount() //!<确定CPU位数。

/*!
 * \brief SBrowserWindow::add1LoadedTab 定时器超时，则添加一个被载入的标签页。
 */
void SBrowserCppWebEngine::add1LoadedTab()
{
    if (loadedTabTitleQueue.size ()>0) //队列中仍有标签页未添加到界面中。
    {
        //打开相应的标签页：
        SWebPageWidget * crtTbWdgt=createWebPageWidget(); //創建一個頁面部件.


        QString crtTtl=loadedTabTitleQueue.dequeue (); //获取一个标题字符串。

        QString crtUrl=loadedTabUrlQueue.dequeue(); //获取一个URL字符串。

        crtTbWdgt->setTitle(crtTtl); //设置标题。
        crtTbWdgt->setUrl(crtUrl); //设置URL。
        pgTbs->insertTab (addTabCtr,crtTbWdgt,crtTtl); //插入标签页。
        updateTabAmount(); //更新标签页个数。
        pgTbs->setTabToolTip(addTabCtr,crtTtl); //设置标签页的悬停提示。



        addTabCtr++; //已添加的标签页计数器增加。

    } //if (loadedTabTitleQueue.size ()>0) //队列中仍有标签页未添加到界面中。
    else //标签页都已添加完毕。
    {
        addTabTimer.stop (); //停止添加标签页定时器。
        sessionStoreLoadFinished=true; //记录，会话信息已经载入完毕。
        svSsnTmr->start(); //启动保存会话信息的定时器。
    } //else //标签页都已添加完毕。

    return;
} //void SBrowserWindow::add1LoadedTab()

/*!
 * \brief SBrowserWindow::ttsSayDownloadError 下载出错，则语音报告。
 */
void SBrowserCppWebEngine::ttsSayDownloadError()
{
    ttsClient.saySentence(tr("File download error.")); //说话，下载出错。



    return;
} //void SBrowserWindow::ttsSayDownloadError()

/*!
 * \brief SBrowserWindow::qrCollectBgImage 下载完毕，则为二维码收集背景图片。
 * \param flNm 可能是图片的文件名。下载的文件不一定是图片。
 */
void SBrowserCppWebEngine::qrCollectBgImage(QString flNm)
{


    QImage CrtPxmp; //The pixmap object.用于尝试载入图片文件的图片对象。

    bool LdRst=false; //Whether loaded successfully.

    try
    {
        LdRst=CrtPxmp.load(flNm); //尝试载入。
    }
    catch(std::bad_alloc & e)
    {
        Q_UNUSED (e);
    } //catch(std::bad_alloc e)

    if (LdRst) //Load successfully.
    {
        collectToQrBgImageSet(flNm); //将此图片收集到背景图片集合中。
    } //if (CrtPxmp.load(CrtFlNm)) //Load successfully.
} //void SBrowserWindow::qrCollectBgImage(QString flNm)

/*!
 * \brief SBrowserWindow::recognizeQrCode 识别该图片中的二维码。
 * \param qrCdImgWthBgImg 要识别的图片。
 * \return 识别结果。
 */
QString SBrowserCppWebEngine::recognizeQrCode(QImage qrCdImgWthBgImg)
{
    Q_UNUSED(qrCdImgWthBgImg); //暂不使用勒个参数。

    QString result=""; //结果。





    return result;
} //QString SBrowserWindow::recognizeQrCode(QImage qrCdImgWthBgImg)

/*!
 * \brief SBrowserWindow::collectToQrBgImageSet 将此图片收集到背景图片集合中。
 * \param flNm 要收集的文件名。
 */
void SBrowserCppWebEngine::collectToQrBgImageSet(QString flNm)
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
 * \brief SWebPageWidget::undoCloseTab 撤销按钮被按，则撤销刚才被关闭的标签页。
 */
void SBrowserCppWebEngine::undoCloseTab()
{
    QString currentUrl=closedTabUrlStack.pop(); //获取最后一个被关闭的标签页的URL。

    QWebEngineView * newTab=createWindow (QWebEnginePage::WebBrowserWindow); //创建新标签页。

    QString searchEnginePrefix="http://www.google.com.hk/search"; //搜索引擎的网址前缀。

    QString searchUrlString=searchEnginePrefix; //构造搜索路径字符串。
    QUrl searchUrl(searchUrlString); //构造URL。

    QUrlQuery srchQry; //查询对象。

    searchUrl.setQuery (srchQry); //设置查询对象。

    newTab->load (currentUrl); //载入搜索引擎网页。

    if (closedTabUrlStack.empty ()) //栈已经为空。
    {
      emit closedTabUrlStackIsEmpty(); //发射信号，关闭的标签页栈为空。
    } //if (closedTabUrlStack.empty ()) //栈已经为空。
} //void SWebPageWidget::undoCloseTab()

/*!
 * \brief SBrowserCppWebEngine::quitForIdle Quit when idle.
 */
void SBrowserCppWebEngine::quitForIdle()
{
  if (quitIdle10MinutesbGl) // Should quit.
  {
    quit(); // Quit.
  } // if (quitIdle10MinutesbGl) // Should quit.
} // void SBrowserCppWebEngine::quitForIdle()

/*!
 * \brief SBrowserWindow::quit 退出。
 */
void SBrowserCppWebEngine::quit()
{
  disconnectSignals(); // Diconnect signals. 陈欣.

  downloadManager()->quit(); // 要求下载器进程退出。

  downloaderProcess.kill(); // 退出下载器进程。

  qrBackgroundImageNormalizer.quit(); //退出。

//  communicator.quit(); //Reqeust quit

  killRenderProcesses(); // 杀死所有的渲染进程。

  hide(); // Hide window

  selfQuitTimer.start(); //Start hte self quit timer
} //void SBrowserWindow::quit()

/*!
 * \brief SBrowserWindow::toggleMenuBarVisable 切换菜单栏的可见性。
 * \param visible 是否可见。
 */
void SBrowserCppWebEngine::toggleMenuBarVisable(bool visible)
{
    u->menubar->setVisible (visible); //设置可见性。
} //void SBrowserWindow::toggleMenuBarVisable(bool visible)

/*!
 * \brief SBrowserWindow::toggleTabBarVisible 切换标签栏的可见性。
 * \param visible 是否可见。
 */
void SBrowserCppWebEngine::toggleTabBarVisible(bool visible)
{
    u->pgstabWidget->tabBar ()->setVisible (visible); //设置可见性。
} //void SBrowserWindow::toggleTabBarVisible(bool visible)

/*!
 * \brief SBrowserWindow::toggleNavigationBarVisible 切换导航栏的可见性。
 * \param visible 是否可见。
 */
void SBrowserCppWebEngine::toggleNavigationBarVisible(bool visible)
{
    SWebPageWidget * currentWebPage=(SWebPageWidget*)(u->pgstabWidget->currentWidget ()); //获取当前网页部件。

    currentWebPage->setNavigationBarVisible(visible); //设置可见性。

    return;
} //void SBrowserWindow::toggleNavigationBarVisible(bool visible)

/*!
 * \brief SBrowserWindow::toggleStatusBarVisible 切换状态栏的可见性。
 * \param visible 是否可见。
 */
void SBrowserCppWebEngine::toggleStatusBarVisible(bool visible)
{
  u->statusbar->setVisible (visible); //设置可见性。
} //void SBrowserWindow::toggleStatusBarVisible(bool visible)

/*!
 * \brief SBrowserCppWebEngine::killRenderProcesses 杀死所有的渲染进程。
 */
void SBrowserCppWebEngine::killRenderProcesses()
{
  //一个个地加入标签页：
  int tbsAmt=pgTbs->count(); //获取当前显示的标签页数量。

  for(int tbsCtr=0;tbsCtr<=(tbsAmt-1);tbsCtr++) //一个个地保存标签页信息。
  {
    QVariantMap crtTb; //当前标签页的信息。
    QVariantList tbEtrs; //当前标签页中的条目数组。
    QVariantMap crtEntr; //当前条目。

    SWebPageWidget * crtWbPgWdgt=(SWebPageWidget *)(pgTbs->widget(tbsCtr)); //获取当前标签页。

    crtWbPgWdgt->killRenderProcess(); // 杀死渲染进程。
  } //for tbsCtr in 0..tbsAmt #一个个地保存标签页信息。
} // void SBrowserCppWebEngine::killRenderProcesses()

/*!
 * \brief SBrowserWindow::~SBrowserWindow 析构函数。
 */
SBrowserCppWebEngine::~SBrowserCppWebEngine()
{
  cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this: " << this  << endl; //Debug.
  delete u; //释放内存。
  delete checkNewVersionRequest; //释放内存。

  QList<QNetworkReply*> replies= ntAccsMngr4WebPages. findChildren<QNetworkReply*>(); //获取所有的回复对象。

  cout << __FILE__ << "," << __LINE__ << "," << __func__ << ", reply amout: " << replies.length() << endl; //Debug.

  foreach(QNetworkReply * currentReply,replies) //一个个地处理。
  {
    cout << __FILE__ << "," << __LINE__ << "," << __func__ << ", reply: " << currentReply  << endl; //Debug.
    cout << __FILE__ << "," << __LINE__ << "," << __func__ << ", reply: " << currentReply->metaObject()->className()  << endl; //Debug.

//        currentReply->abort (); //放弃。
  } //foreach(QNetworkReply * currentReply,replies) //一个个地处理。
    
  delete s_downloadManager; //释放下载管理器。
} //SBrowserWindow::~SBrowserWindow()

/*!
 * \brief SBrowserWindow::initializeMemberObjects 初始化成员对象。
 */
void SBrowserCppWebEngine::initializeMemberObjects()
{
  initializeUi(); //对界面做一些初始化。

  prxyFctry=new StbProxyFactory; //创建代理工厂对象。
  QNetworkProxyFactory::setApplicationProxyFactory (prxyFctry); //设置应用程序的代理工厂。

  ldTimer=new QTimer(); //定时器。

  ldTimer->setInterval(0); //0秒超时。
  ldTimer->setSingleShot(true); //单发。

  ldTimer->start(); //启动定时器。

  selfQuitTimer.setInterval(5000);
  selfQuitTimer.setSingleShot(true);

  addTabTimer.setInterval (100); //100毫秒超时。

  svSsnTmr=new QTimer(); //创建定时器。用于定时保存会话信息的定时器。
  svSsnTmr->setInterval(45*1000); //45秒超时。
  svSsnTmr->setSingleShot(false); //连发。

  createNetworkManager(); //创建网络管理器。

  createCookieJar(); //创建小甜饼罐。

  stngDlg.setProxyFactory(prxyFctry); //设置代理工厂。

  installRequestInterceptor(); //安装请求干涉器。
    
  searchEngineManager.setSettingsDialog(&stngDlg); //设置选项对话框的指针。
    
  launchTimer.setSingleShot(true); //单发。
  launchTimer.start(); //启动定时器，启动各个子系统。
    
  repNode.connectToNode(QUrl(QStringLiteral("local:000013025"))); //连接到服务器端。
  ptr.reset(repNode.acquire<SetDirectionServiceReplica>());

  rswitch=new Client(ptr);
    
  userScriptManager=new UserScriptManager; //创建用户脚本管理器
} //void SBrowserWindow::initializeMemberObjects()

/*!
 * \brief SBrowserWindow::installRequestInterceptor 安装请求干涉器。
 */
void SBrowserCppWebEngine::installRequestInterceptor()
{
  QWebEngineProfile * profile=QWebEngineProfile::defaultProfile (); //获取配置档。

  SbAdblockInterceptor * interceptor=new SbAdblockInterceptor(); //构造广告禁用干涉器。

  interceptor->setProxyFactory(prxyFctry); //设置代理工厂。

  UnknownSchemeInterceptor * unknownSchemeInterceptor=new UnknownSchemeInterceptor; //未知模式干涉器。

  ContainerInterceptor * containerInterceptor=new ContainerInterceptor; //创建容器干涉器，它是其它干涉器的容器。

  containerInterceptor->addInterceptor(interceptor); //加入干涉器。
  containerInterceptor->addInterceptor(unknownSchemeInterceptor); //加入干涉器。

  profile->setUrlRequestInterceptor(containerInterceptor); //设置请求的干涉器。
} //void SBrowserWindow::installRequestInterceptor()

/*!
 * \brief SBrowserWindow::toggleDarkMode 切换暗夜模式。
 * \param useDarkMode 是否使用暗夜模式。
 */
void SBrowserCppWebEngine::toggleDarkMode(bool useDarkMode)
{
  QApplication * applicationInstance=(QApplication *) (QApplication::instance());
  // QWebEngineSettings * webSettings=QWebEngineSettings::globalSettings (); //获取全局网页设置。
  // Q_UNUSED(webSettings); //不使用这个参数。

  QString dataUrlPrefix="data:text/css;charset=utf-8;base64,"; //data url的前缀。

    QFile dataModeCssFile(":/DarkMode.css"); //暗夜模式的CSS文件。


    if (useDarkMode) //要使用暗夜模式。
    {
        QFile f(":qdarkstyle/style.qss");
        if (!f.exists())
        {
            printf("Unable to set stylesheet, file not found\n");
        }
        else
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);

            applicationInstance->setStyleSheet(ts.readAll()); //设置样式单。
        }

        dataModeCssFile.setFileName (":/DarkMode.css"); //设置文件名。
    } //if (useDarkMode) //要使用暗夜模式。
    else //不使用暗夜模式。
    {
        applicationInstance->setStyleSheet(normalApplicationStyleSheet); //设置样式单。

        dataModeCssFile.setFileName (":/NormalMode.css"); //设置文件名。
    } //else //不使用暗夜模式。


    dataModeCssFile.open (QIODevice::ReadOnly); //打开文件。
    QByteArray dataModeCss=dataModeCssFile.readAll(); //读取全部内容。
    dataModeCssFile.close (); //关闭文件。
    QString dataModeCssBase64=dataModeCss.toBase64(); //编码。
    QString darkModeDataUrl=dataUrlPrefix+dataModeCssBase64; //构造样式单字符串。

} //void SBrowserWindow::toggleDarkMode(bool useDarkMode)

/*!
 * \brief SBrowserWindow::batchOpenUrls 批量打開網址.
 */
void SBrowserCppWebEngine::batchOpenUrls()
{
    bool Acpted=false; //用户是否按了确定。

    QString CrtPt=QInputDialog::getMultiLineText (this,tr("New url"),tr("Enter the urls"),"",&Acpted); //获取URL列表。

    if (Acpted) //接受了。
    {
        QStringList urlList=CrtPt.split ("\n"); //按行分割。

        foreach(QString currentUrl,urlList) //一行行地处理。
        {
            currentUrl=currentUrl.trimmed (); //去除首尾空白。

            if (currentUrl.isEmpty ()) //是空行。
            {

            } //if (currentUrl.isEmpty ()) //是空行。
            else //不是空行。
            {
                SWebPageWidget * crtTbWdgt=createWebPageWidget(); //創建一個頁面部件.


                crtTbWdgt->setUrl(currentUrl); //设置URL。
                pgTbs->addTab (crtTbWdgt,""); //插入标签页。
                updateTabAmount(); //更新标签页个数。

                crtTbWdgt->addToolbarActions(); //向工具条加入动作。


                crtTbWdgt->startLoad(); //开始载入页面。

            } //else //不是空行。
        } //foreach(QString currentUrl,urlList) //一行行地处理。

    } //if (Acpted) //接受了。


    return;
} //void SBrowserWindow::batchOpenUrls()

/*!
 * \brief SBrowserCppWebEngine::rememberIconUrl  Remember icon url
 * \param iconUrl The icon url
 */
void SBrowserCppWebEngine::batchDownloadImageUrl(const QVariant & iconUrl)
{
  auto urlList=iconUrl.toList();

  QSet<QString> extraPackageNameSet; // Image url set.

  for(auto url: urlList)
  {
    auto urlString=url.toString (); // Comvert to string.

    if (!extraPackageNameSet.contains(urlString)) // Not contained.
    {
      extraPackageNameSet.insert (urlString); // Add to set.

      downloadManager ()->download (QUrl(urlString), urlString); // Request download.
    } // if (!extraPackageNameSet.contains(url)) // Not contained.
  }
} //void SBrowserCppWebEngine::rememberIconUrl(const QString & iconUrl)


/*!
 * \brief SBrowserWindow::createWebPageWidget 創建一個頁面部件.
 * \return 創建好的頁面部件.
 */
SWebPageWidget * SBrowserCppWebEngine::createWebPageWidget()
{
  SWebPageWidget *crtTbWdgt=new SWebPageWidget(); //创建一个页面部件。
  crtTbWdgt->setProxyFactory(prxyFctry); //设置代理工厂。
  crtTbWdgt->setNetworkAccessManager(&ntAccsMngr4WebPages); //设置网络访问管理器的指针。
  crtTbWdgt->setBrowserWindow(this); //设置窗口的指针。
    
  crtTbWdgt->setQrCodeDialog(&qrCodeDialog); //设置二维码生成对话框。

  tabsManager->addTab(crtTbWdgt); //向标签页管理器中加入一个标签页。

  connect (crtTbWdgt,SIGNAL(loadFinished()),this,SLOT(ttsSayLoadFinished())); //载入完毕，则通过TTS报告载入完成。
  connect(crtTbWdgt, &SWebPageWidget::toggleProxyRule, this, &SBrowserCppWebEngine::toggleProxyRule); // 要求切换智能代理规则，则切换。
  connect(crtTbWdgt, &SWebPageWidget::shouldOpenNewTab, this, &SBrowserCppWebEngine::openNewTab); //要求打开新标签页，则打开一个。

  connect(crtTbWdgt, &SWebPageWidget::titleChanged, this, &SBrowserCppWebEngine::changeTabText); //标题变化，则修改对应标签页的标题。

  connect(crtTbWdgt, &SWebPageWidget::iconChanged, this, &SBrowserCppWebEngine::changeTabIcon); //图标变化，则修改对应标签页的图标。

  connect(crtTbWdgt, &SWebPageWidget::iconUrlFound, this, &SBrowserCppWebEngine::batchDownloadImageUrl); // batch download url. Chen xin
  connect(crtTbWdgt, &SWebPageWidget::videoUrlFound, this, &SBrowserCppWebEngine::batchDownloadImageUrl); // batch download url. Chen xin

//    connect(&downloaderProcess, QOverload<int , QProcess::ExitStatus >::of(&QProcess::finished), this, &SBrowserWindow::assessRestartSpeakerSubSystem); //扬声器子系统退出，则考虑是否要重新启动进程。
    
  connect(crtTbWdgt, &SWebPageWidget::urlChanged, this, QOverload<QString>::of(&SBrowserCppWebEngine::syncZoomFactor)); //网址变化，则同步对应标签页的缩放比例。
    
    
  connect(crtTbWdgt,SIGNAL(iconChanged(QIcon)),this,SLOT(changeTabIcon(QIcon))); //图标变化，则修改对应标签页的图标。
  connect (crtTbWdgt,&SWebPageWidget::shouldShowDownloadManager,this,&SBrowserCppWebEngine::showDownloadManager); //标签页请求显示下载管理器，则显示下载管理器。
  connect (crtTbWdgt,&SWebPageWidget::shouldUndoCloseTab,this,&SBrowserCppWebEngine::undoCloseTab); //标签页请求撤销被关闭的标签页，则撤销被关闭的标签页。
  connect (this,&SBrowserCppWebEngine::closedTabUrlStackIsEmpty,crtTbWdgt,&SWebPageWidget::forbidUndoClosedTab); //被关闭的标签页栈已为空，则应当禁用恢复标签页按钮。
  connect (this,&SBrowserCppWebEngine::closedTabUrlStackIsNotEmpty,crtTbWdgt,&SWebPageWidget::enableUndoClosedTab); //被关闭的标签页栈已不为空，则应当启用恢复标签页按钮。
  connect (this,&SBrowserCppWebEngine::restoreClosedTabToolTipChanged,crtTbWdgt,&SWebPageWidget::updateRestoreTabToolTip); //应当修改恢复标签页的提示文字。

  connect (crtTbWdgt, &SWebPageWidget::zoomFactorChanged, this, (void (SBrowserCppWebEngine::*)(qreal))&SBrowserCppWebEngine::refreshZoomFactor); //缩放比例发生变化，则更新缩放比例。

  connect (crtTbWdgt, &SWebPageWidget::zoomFactorChanged, this, (void (SBrowserCppWebEngine::*)(qreal))&SBrowserCppWebEngine::rememberZoomFactor); //缩放比例发生变化，则更新缩放比例。

  connect(crtTbWdgt,&SWebPageWidget::windowCloseRequested,this,&SBrowserCppWebEngine::closeRequestedTab); //要求关闭窗口，则关闭发起要求的标签页。
    
  connect(crtTbWdgt, &SWebPageWidget::shouldQuerySearchEngine, &searchEngineManager, &SearchEngineManager::searchEngineSearch); //应当向搜索引擎进行查询，则向搜索引擎进行查询。陈欣

  // 在构造函数中连接
  connect(crtTbWdgt, &SWebPageWidget::blobDownloadCompleted,
          this, &SBrowserCppWebEngine::onBlobDownloadCompleted);

  return crtTbWdgt;
} //SWebPageWidget * SBrowserWindow::createWebPageWidget()

void SBrowserCppWebEngine::onBlobDownloadCompleted(const QByteArray &data, const QString &filename)
{
    // ✅ 终于！数据到达主进程
    // 现在通过 Qt Remote Object 发送给下载器进程
    downloadManager()->downloadBlobData(data, filename);
}

/*!
 * \brief SBrowserWindow::updateTabAmount 更新标签页个数。
 */
void SBrowserCppWebEngine::updateTabAmount()
{
  quint32 tabAmount=pgTbs->count (); //获取标签页个数。

  tabAmountLabel->setText (tr("%1 tabs").arg (tabAmount)); //更新文字内容。
} //void SBrowserWindow::updateTabAmount()

/*!
 * \brief SBrowserCppWebEngine::checkClipboardUrl Check clipboard url
 */
void SBrowserCppWebEngine::checkClipboardUrl()
{
  QClipboard *clipboard = QApplication::clipboard();

  QString clipBoardText=clipboard->text();

  if (clipBoardText.trimmed ().startsWith("http")) // start with http.
  {
    if (clipboardUrlSet.contains(clipBoardText)) // Processed
    {
    } // if (clipboardUrlSet.contains(clipBoardText))
    else // NOt processed
    {
      if (switchToTabByUrl(clipBoardText)) // Switched to the tab of this url
      {
      } // if (switchToTabByUrl(clipBoardText)) // Switched to the tab of this url
      else // NOt switched
      {
        openNewTab(clipBoardText);
      } // else // NOt switched

      clipboardUrlSet << clipBoardText;
    } // else // NOt processed
  } // if (clipBoardText.trimmed ().startsWith("http")) // start with http.
} // void SBrowserCppWebEngine::checkClipboardUrl()

/*!
 * \brief SBrowserCppWebEngine::switchToTabByUrl Switched to the tab of this url
 * \param clipBoardText The url to switch to
 * \return Wehter switched successfully.
 */
bool SBrowserCppWebEngine::switchToTabByUrl(const QString  & clipBoardText)
{
  //  https://app.mi.com/details?id=com.shenzhoubb.engineer&ref=search

  auto tabAmount=pgTbs->count (); // Get tab amount
  auto tabCounter=0; // Tab counter.

  auto foundTab=false; // Whether Found tab.
  SWebPageWidget * tabFoundBySearch=nullptr; // Found tab.

  for(; tabCounter < tabAmount; tabCounter++) // Check one by one
  {
    auto currentTab=pgTbs->widget (tabCounter); // Get current tab.

    auto webWidet=qobject_cast<SWebPageWidget*>(currentTab);

    auto url=webWidet->getUrl (); // Get the url.

    auto urlObject=QUrl(url); // GEt url object.

    auto host=urlObject.host (); // Get host.

    if (url==clipBoardText) // It is the url
    {
      foundTab=true; // Found tab.

      tabFoundBySearch=webWidet; // Remember it.

      break;
    } // if (host=="app.mi.com") // It is the host
  } // for(; tabCounter < tabAmount; tabCounter++) // Check one by one

  auto useExistingTab=foundTab; // Whether to use existing tab.

  if (useExistingTab) // Found tab.
  {
    pgTbs->setCurrentWidget (tabFoundBySearch); // Change tab.
  } // if (foundTab) // Found tab.

  return useExistingTab;
} // bool SBrowserCppWebEngine::switchToTabByUrl(const QString  & clipBoardText)

/*!
 * \brief SBrowserCppWebEngine::pauseVideo Pause video when lost focus. Chen xin.
 */
void SBrowserCppWebEngine::pauseVideo()
{
  if (pauseMediaWhenLostFocusesbGl) // 要暂停
  {
    auto crtPgWdget=qobject_cast<SWebPageWidget*>(pgTbs->currentWidget ()); // Get the current tab.

    crtPgWdget->pauseVideo(); // Paues video.
  } // if (pauseMediaWhenLostFocusesbGl) // 要暂停
} // void SBrowserCppWebEngine::pauseVideo()

/*!
 * \brief SBrowserWindow::refresh 刷新。
 */
void SBrowserCppWebEngine::refresh()
{
  SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

  crtPgWdgt->refresh(); //由该标签页来刷新。
} //void SBrowserWindow::refresh()

/*!
 * \brief SBrowserCppWebEngine::openWithSystemBrowser 使用系统浏览器打开.陈欣
 */
void SBrowserCppWebEngine::openWithSystemBrowser()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    auto url=crtPgWdgt->getUrl(); // 获取网址

//    QUrl url2Encode=QUrl(url); //构造URL。

//    QByteArray qrcode=(url2Encode.toEncoded ()); //Encode.

//    QString encodedUrl=QString::fromUtf8(qrcode);

    QDesktopServices::openUrl(url); // 打开.
} //void SBrowserCppWebEngine::openWithSystemBrowser()

/*!
 * \brief SBrowserWindow::paste 粘贴。
 */
void SBrowserCppWebEngine::paste()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->paste(); //粘贴。
} //void SBrowserWindow::paste()

/*!
 * \brief SBrowserWindow::focusAddressBar 焦点定位到地址栏。
 */
void SBrowserCppWebEngine::focusAddressBar()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->focusAddressBar(); //焦点定位到地址栏。
} //void SBrowserWindow::focusAddressBar()

/*!
 * \brief SBrowserWindow::handleAuthentication 需要认证，则处理认证问题。
 * \param reply 回复对象。
 * \param authenticator 认证信息对象。
 */
void SBrowserCppWebEngine::handleAuthentication(QNetworkReply  *  reply ,  QAuthenticator  *  authenticator)
{
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Sheet);

    Ui::PasswordDialog passwordDialog;
    passwordDialog.setupUi(&dialog);

    passwordDialog.iconLabel->setText(QString());
    passwordDialog.iconLabel->setPixmap(style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, this).pixmap(32, 32));

    QString introMessage = tr("<qt>Enter username and password for \"%1\" at %2</qt>");
    introMessage = introMessage.arg(QString(reply->url().toString()).toHtmlEscaped ()).arg(QString(reply->url().toString()).toHtmlEscaped ());
    passwordDialog.introLabel->setText(introMessage);
    passwordDialog.introLabel->setWordWrap(true);

    if (dialog.exec() == QDialog::Accepted) {
        authenticator->setUser(passwordDialog.userNameLineEdit->text());
        authenticator->setPassword(passwordDialog.passwordLineEdit->text());
    }
} //void SBrowserWindow::handleAuthentication(QNetworkReply  *  reply ,  QAuthenticator  *  authenticator)

/*!
 * \brief SBrowserWindow::loadTranslation 载入对应语言的翻译。
 * \param stngDlggetSelectedLanguage 要载入的翻译。
 */
void SBrowserCppWebEngine::loadTranslation(QString stngDlggetSelectedLanguage)
{
    QTranslator * XyTrns=new QTranslator;
    XyTrns->load(":/SBrowser_"+stngDlggetSelectedLanguage);




    qApp->installTranslator (XyTrns); //安装翻译文件。
} //void SBrowserWindow::loadTranslation(QString stngDlggetSelectedLanguage)

void SBrowserCppWebEngine::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        u->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
 * \brief SBrowserWindow::handleSslErrors 处理SSL错误。
 * \param reply 发生错误的网络响应对象。
 * \param error 错误列表。
 */
void SBrowserCppWebEngine::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    // check if SSL certificate has been trusted already
    QString replyHost = reply->url().host() + QString(":%1").arg(reply->url().port());
    if(! sslTrustedHostList.contains(replyHost)) {
        QStringList errorStrings;
        for (int i = 0; i < error.count(); ++i)
            errorStrings += error.at(i).errorString();
        QString errors = errorStrings.join(QLatin1String("\n"));
        int ret = QMessageBox::warning(this, QCoreApplication::applicationName(),
                tr("SSL Errors:\n\n%1\n\n%2\n\n"
                        "Do you want to ignore these errors for this host?").arg(reply->url().toString()).arg(errors),
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            reply->ignoreSslErrors();
            sslTrustedHostList.append(replyHost);
        }
    }
} //void SBrowserWindow::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &error)

/*!
 * \brief SBrowserCppWebEngine::showTabOnRightSide Show the tab on Right side.
 */
void SBrowserCppWebEngine::showTabOnRightSide()
{
  auto closeTab =pgTbs->currentIndex (); // get the current 标签页 index 。

  auto newIndex=closeTab+1; // The new index.

  newIndex += pgTbs->count (); // Normalize the new index.
  newIndex %= pgTbs->count (); // Normalize the new index.

  pgTbs->setCurrentIndex (newIndex); // Change tab.
} // void SBrowserCppWebEngine::showTabOnRightSide()

/*!
 * \brief SBrowserCppWebEngine::showTabOnLeftSide Show the tab on left side.
 */
void SBrowserCppWebEngine::showTabOnLeftSide()
{
  auto closeTab =pgTbs->currentIndex (); // get the current 标签页 index 。

  auto newIndex=closeTab-1; // The new index.
  cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this: " << this << ", url: " << newIndex << endl; //Debug.

  newIndex += pgTbs->count (); // Normalize the new index.
  newIndex %= pgTbs->count (); // Normalize the new index.

  cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this: " << this << ", url: " << newIndex << endl; //Debug.

  pgTbs->setCurrentIndex (newIndex); // Change tab.
} // void SBrowserCppWebEngine::showTabOnLeftSide()

/*!
 * \brief SBrowserWindow::closeCurrentTab 关闭当前标签页。
 */
void SBrowserCppWebEngine::closeCurrentTab()
{
  closeTab (pgTbs->currentIndex ()); //关闭标签页。
} //void SBrowserWindow::closeCurrentTab()

/*!
 * \brief SBrowserWindow::syncSetting 设置对话框确认关闭，则 Sync 设置信息。陈欣
 */
void SBrowserCppWebEngine::syncSetting()
{
    downloadManager ()->syncSetting (); //Request to sync setting.
} //void SBrowserWindow::syncSetting() const

/*!
 * \brief SBrowserCppWebEngine::handleDownloadRequest 处理下载请求。
 * \param downloadItem 下载请求信息。
 */
void SBrowserCppWebEngine::handleDownloadRequest(QWebEngineDownloadRequest *downloadItem)
{
    QUrl downloadUrl = downloadItem->url(); // 获取网址。

    toastDownloadStarted(); // 报告，开始下载．

    SWebPageWidget *crtTb = (SWebPageWidget *)(pgTbs->currentWidget()); // 获取对应的标签页
    QString currentUrl = crtTb->getUrl();
    QString referrerUrl = currentUrl;

    if (currentUrl == "http://") // 实际是空网址。此页面直接被用来做下载。
    {
        referrerUrl = downloadUrl.toString();
        crtTb->setUrl(downloadUrl.toString());
    }
    else
    {
        referrerUrl = currentUrl;
    }

    // ✅ 只有在使用 SuperDownloader 时，才需要特殊处理 blob:
    if (downloaderType == DownloaderType::SuperDownloader)
    {
        // 🔍 在 SuperDownloader 分支内判断 blob
        if (downloadUrl.scheme() == "blob")
        {
            downloadItem->accept();


            // 走自定义 blob 处理流程
            // crtTb->handleBlobDownload(downloadUrl, downloadItem->downloadFileName());
            // downloadItem->cancel(); // 阻止 WebEngine 内部处理
        }
        else
        {
            // 正常 URL，通知下载器进程下载
            downloadManager()->download(downloadUrl, referrerUrl);
            downloadItem->cancel(); // 阻止 WebEngine 内部处理
        }
    }
    else // 使用内置下载器
    {
        // ✅ 不做任何 blob 判断，直接 accept，交给 WebEngine 内部处理
        downloadItem->accept();
    }
}

/*!
 * \brief SBrowserWindow::initializeUi 对界面做一些初始化。
 */
void SBrowserCppWebEngine::initializeUi()
{
    addActions(); //添加动作。

    bindExtraShortcuts(); //绑定额外的快捷键。

    showTray(); //显示托盘图标。

    loadGeometry(); //载入几何属性。

    mAboutDialog=new AboutDialog; //创建关于对话框。

    tabsManager=new TabManager; //创建标签页管理器对话框。

    normalApplicationStyleSheet=qApp->styleSheet(); //记住现在的样式单。

    pgTbs=u->pgstabWidget; //网页标签页部件。
    pgTbs->setTabsClosable(true); //标签页可以关闭。
    pgTbs->setStyleSheet("QTabBar::tab{max-width:150px;min-width:150px;text-align:left}"); //设置样式单，限制标签页的最大宽度。
    pgTbs->setContentsMargins(1,1,1,1); //不要边距。
    
    tabsManager->setPageTabs(pgTbs); //设置标签页部件。
    

    initializeStatusBar(); //初始化状态栏。



    u->menu_New_version->menuAction ()->setVisible (false); //隐藏新版本菜单。
} //void SBrowserWindow::initializeUi()

/*!
 * \brief SBrowserWindow::initializeStatusBar 初始化状态栏。
 */
void SBrowserCppWebEngine::initializeStatusBar()
{
    //标签页个数：
    tabAmountLabel=new QLabel(tr("%1 tabs").arg (0)); //标签页个数文字标签。
    u->statusbar->addPermanentWidget (tabAmountLabel); //添加标签页个数文字标签。


    //回到顶端：
    backToTopToolButton=new QToolButton; //创建按钮。
    backToTopToolButton->setIcon (QIcon(":/Images/sign_up.png")); //设置图标。
    u->statusbar->addPermanentWidget (backToTopToolButton); //添加回到顶端按钮。
} //void SBrowserWindow::initializeStatusBar()

/*!
 * \brief SBrowserWindow::closeRequestedTab 要求关闭窗口，则关闭发起要求的标签页。
 */
void SBrowserCppWebEngine::closeRequestedTab()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(sender()); //获取发起请求的标签页。

    int tbIdx=pgTbs->indexOf(crtPgWdgt); //获取信号发送者所在的标签页索引。

    closeTab(tbIdx); //关闭标签页。
} //void SBrowserWindow::closeRequestedTab()

/*!
 * \brief SBrowserWindow::goBackTop 回到顶端。
 */
void SBrowserCppWebEngine::goBackTop()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->goBackTop(); //回到顶端。
    
    showAnimationGoBackTop(); //显示动画效果，回到顶端。
} //void SBrowserWindow::goBackTop()

/*!
 * \brief SBrowserWindow::showAnimationGoBackTop 显示动画效果，回到顶端。
 */
void SBrowserCppWebEngine::showAnimationGoBackTop()
{
//    u->ArrowBlacklabel->show (); //显示箭头。
//    ui->ArrowRedlabel->show(); //显示红箭头。
//    ArrowUpAni.start(); //启动箭头向上的动画。
    
} //void SBrowserWindow::showAnimationGoBackTop()

/*!
 * \brief SBrowserWindow::showTabsManager 显示标签页管理器。
 */
void SBrowserCppWebEngine::showTabsManager()
{
    tabsManager->show (); //显示。

    tabsManager->raise(); //跳到顶部.

    tabsManager->focusFilterInputWidget(); //焦点设置到过滤内容输入框。
} //void SBrowserWindow::showTabsManager()

/*!
 * \brief SBrowserWindow::switchTab 切换标签页。
 * \param tabToSwitch 要切换到的目标标签页。
 */
void SBrowserCppWebEngine::switchTab(SWebPageWidget * tabToSwitch)
{
  pgTbs->setCurrentWidget (tabToSwitch); //切换。
} //void SBrowserWindow::switchTab(SWebPageWidget * tabToSwitch)

/*!
 * \brief SBrowserWindow::downloadCurrentUrlUniversal Download current url use universal method
 */
void SBrowserCppWebEngine::downloadCurrentUrlUniversal()
{
  SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

  crtPgWdgt->downloadCurrentUrl(); //Start download current url
} //void SBrowserWindow::downloadCurrentUrlUniversal()

/*!
 * \brief SBrowserWindow::downloadCurrentUrlDownloadManager Use downlaod manager. Download current url
 */
void SBrowserCppWebEngine::downloadCurrentUrlDownloadManager()
{
  SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

  QString currentUrl=crtPgWdgt->getUrl (); //获取网址。

  QUrl downloadUrl(currentUrl); //获取网址。

  toastDownloadStarted(); //报告，开始下载．

    downloadManager ()->download (downloadUrl, currentUrl); //下载。

} //void SBrowserWindow::downloadCurrentUrlDownloadManager()

/*!
 * \brief SBrowserCppWebEngine::downloadAllMedium Download all medium.
 */
void SBrowserCppWebEngine::downloadAllMedium()
{
  cout  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", arguments: " << endl; //报告错误码。
  qWarning()  << __FILE__ << ", " << __LINE__ << ", " << __func__ ; //报告错误码。

  auto crtTb=(SWebPageWidget *)(pgTbs->currentWidget()); //获取对应的标签页《。

  crtTb->extractMediaUrls(); // Extract media urls.
} // void SBrowserCppWebEngine::downloadAllMedium(); //!< Download all medium.

/*!
 * \brief SBrowserCppWebEngine::downloadAllImages Download all images. Chen xin
 */
void SBrowserCppWebEngine::downloadAllImages()
{
  auto crtTb=(SWebPageWidget *)(pgTbs->currentWidget()); //获取对应的标签页《。

  crtTb->extractImageUrls(); // Extract image urls.
} // void SBrowserCppWebEngine::downloadAllImages()

/*!
 * \brief SBrowserWindow::downloadCurrentUrl 下载当前网址内容。
 */
void SBrowserCppWebEngine::downloadCurrentUrl()
{
//    downloadCurrentUrlDownloadManager(); //Use downlaod manager

    downloadCurrentUrlUniversal(); //Download current url use universal method

} //void SBrowserWindow::downloadCurrentUrl()

/*!
 * \brief SBrowserWindow::loadCommandLineUrl 载入通过命令行传递的网址。
 */
void SBrowserCppWebEngine::loadCommandLineUrl()
{
//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ ; //报告错误码。

    QCommandLineParser parser;

//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ ; //报告错误码。
    parser.addPositionalArgument("url", QCoreApplication::translate("SBrowserWindow", "The url to load."));

//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ ; //报告错误码。
    // Process the actual command line arguments given by the user
//    parser.process(*app); //Process arguments.
    parser.parse (qApp->arguments ()); //Parse arguments.
//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ ; //报告错误码。

    const QStringList args = parser.positionalArguments();

//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", arguments: " ; //报告错误码。
//    cout  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", arguments: " << endl; //报告错误码。

    for(auto currentArgument: args) //Debug.
    {
//         logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", argument: " << currentArgument ; //报告错误码。
//         cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", argument: " << currentArgument.toStdString () << endl; //报告错误码。
     } //for(auto currentArgument: args) //Debug.

     if (args.size ()) //有参数。
     {
         QString initialUrl=args.at (0); //获取初始URL。


         QFile parseUrl(initialUrl); //Construct the url object.

         QUrl targetUrl; //The target url.

         if (parseUrl.exists ()) //The file exists.
         {
             targetUrl=QUrl::fromLocalFile (initialUrl); //Construct url from local file.
         } //if (parseUrl.exists ())
         else //Construct url directly.
         {
             targetUrl=QUrl(initialUrl); //Construct url directly.
         } //else //Construct url directly.


         SWebPageWidget * crtTbWdgt=createWebPageWidget(); //創建一個頁面部件.


         crtTbWdgt->setUrl(targetUrl.toString ()); //设置URL。
         pgTbs->addTab (crtTbWdgt,""); //插入标签页。
         updateTabAmount(); //更新标签页个数。

         crtTbWdgt->addToolbarActions(); //向工具条加入动作。


         crtTbWdgt->startLoad(); //开始载入页面。
     } //if (args.size ()) //有参数。
} //void SBrowserWindow::loadCommandLineUrl()

/*!
 * \brief SBrowserWindow::zoomIn 页面放大。
 */
void SBrowserCppWebEngine::zoomIn()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->zoomIn(); //放大。

    refreshZoomFactor (pgTbs->currentIndex ()); //刷新缩放比例。

    rememberZoomFactor(pgTbs->currentIndex()); //记录缩放比例。
} //void SBrowserWindow::zoomIn()

/*!
 * \brief SBrowserWindow::zoomOut 页面缩小。
 */
void SBrowserCppWebEngine::zoomOut()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->zoomOut(); //缩小。

    refreshZoomFactor (pgTbs->currentIndex ()); //刷新缩放比例。

    rememberZoomFactor(pgTbs->currentIndex()); //记录缩放比例。
} //void SBrowserWindow::zoomOut()

/*!
 * \brief SBrowserWindow::inspect 分析网页。
 */
void SBrowserCppWebEngine::inspect()
{
    QLoggingCategory category("sbrowser"); //日志分类。
    qCDebug(category) << __FILE__ << __LINE__ << __func__ ; //Debug.

//    cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this:" << this << endl; //Debug.


    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->inspect(); //分析网页。
} //void SBrowserWindow::inspect()

/*!
 * \brief SBrowserWindow::exportAsPdf 导出成PDF。
 */
void SBrowserCppWebEngine::exportAsPdf()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->exportAsPdf(); //导出成PDF。
} //void SBrowserWindow::exportAsPdf()

/*!
 * \brief SBrowserWindow::zoomRestore 恢复缩放比例。
 */
void SBrowserCppWebEngine::zoomRestore()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    crtPgWdgt->zoomRestore(); //恢复缩放比例。

    refreshZoomFactor (pgTbs->currentIndex ()); //刷新缩放比例。
    
    rememberZoomFactor(pgTbs->currentIndex()); //记录缩放比例。
    
} //void SBrowserWindow::zoomRestore()
