#include "SbPreCompile.h" //QVBoxLayout

//#include <ValidFileName.h> //ValidFileName


#include "SWebPageWidget.h" //SWebPageWidget
#include "StbWebView.h" //StbWebView

//Q_D;

/*!
 * \brief SWebPageWidget::~SWebPageWidget 析构函数。
 */
SWebPageWidget::~SWebPageWidget()
{
    delete wbPg;
} //SWebPageWidget::~SWebPageWidget()

/*!
 * \brief SWebPageWidget::destroyWebPage 销毁网页对象。
 */
void SWebPageWidget:: destroyWebPage()
{
    wbPg->disconnect(); //断开信号连接《。

    if (wbPg) //网页视图存在。
    {
        wbPg->destroyWebPage(); //销毁网页。
//        wbPg->setHtml(""); //设置空白《的《内容《。
    } //if @wbPg #网页视图存在。
} //def destroyWebPage()

/*!
 * \brief SWebPageWidget::isFirstTimeLoad 是否是第一次载入。
 * \return  是否是第一次载入。
 */
bool SWebPageWidget::isFirstTimeLoad()
{
    return firstTimeLoad; //是否是第一次载入。
} //def isFirstTimeLoad()
  
void SWebPageWidget:: focusWebView()
{
    wbPg->setFocus(); //设置焦点。
} //def focusWebView()
  
QWebEngineView *SWebPageWidget::getWebView()
{
    return wbPg; //返回网页视图。
} //def getWebView()
  
void SWebPageWidget:: setBrowserWindow(SBrowserCppWebEngine *brsWnd2St)
{
    brsrWnd=brsWnd2St; //记录。
} //def setBrowserWindow(brsWnd2St)
  
/*!
 * \brief SWebPageWidget::createWindow 创建新标签页。
 * \param wndTp 新标签页的窗口类型。
 * \return 新创建的标签页中的网页视图。
 */
QWebEngineView *SWebPageWidget::createWindow(  QWebEnginePage::WebWindowType wndTp)
{
    QWebEngineView * result=brsrWnd->createWindow(wndTp); //要求浏览器窗口本身创建一个新窗口。
    
    return result; //返回结果。
} //def createWindow(wndTp)
  
/*!
 * \brief SWebPageWidget::indiateIconChanged 报告图标发生变化。
 */
void SWebPageWidget:: indiateIconChanged()
{
} //def indiateIconChanged()
  
/*!
 * \brief SWebPageWidget::indicateLoadFinished 报告载入完毕。
 * \param suc 是否载入成功。
 */
void SWebPageWidget::indicateLoadFinished(bool suc)
{
  QString crtTtl; //当前的标题。

  if (suc) //载入成功。
  {
//        logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", url: " << urlBr->text() ; //报告错误码。

    reloadTimer.stop (); // Stop the reload timer.

    crtTtl=wbPg->title(); //获取页面标题。
  } //if (suc) //载入成功。
  else //页面载入出错。
  {
    crtTtl=tr("Page load error"); //设置页面标题。

    auto currentInterval=reloadTimer.interval (); // Get current interval.

    auto randomRate=QRandomGenerator::global ()->generateDouble (); // Get the random rate.

    auto newInternval=currentInterval*(1+ randomRate); // Calculate new interval.

    reloadTimer.setInterval (newInternval); // Set new interval.

    reloadTimer.start(); // Start reload timer, to schedule reload.
  } //else //页面载入出错。

  title=crtTtl; //记录标题。
    
  emit titleChanged(crtTtl); //发射信号。
  emit loadFinished(); //发射信号，载入完毕。
} //def indicateLoadFinished(suc)
  
/*!
 * \brief SWebPageWidget::indicateLoadProgress 报告载入进度。
 * \param crtPrg 进度值。
 */
void SWebPageWidget:: indicateLoadProgress(int crtPrg)
{
    QString crtTtl; //当前的标题。

    crtTtl=QString( "%1% " ).arg (crtPrg); //获取进度值字符串。
    
    progress=crtPrg; //Remember progress
    
    crtTtl+=wbPg->title(); //接上标题。

    title=crtTtl; //记录标题。

    emit titleChanged(crtTtl); //显示进度。
} //def indicateLoadProgress(crtPrg)
  
void SWebPageWidget::indicateLoadStart()
{
    QString crtTtl=tr("Loading..."); //构造标题字符串。

    title=crtTtl; //记录标题。

    progress=0; //Reset progreess

    emit titleChanged(crtTtl); //发射信号。
} //def indicateLoadStart()
  
/*!
 * \brief SWebPageWidget::refreshUrl 刷新网址。
 * \param nwUrl 新的网址。
 */
void SWebPageWidget::refreshUrl(QUrl nwUrl)
{
    urlBr->setText(nwUrl.toString()); //显示新的URL。

    emit urlChanged (nwUrl.toString ()); //发射信号，网址发生改变。
} //def refreshUrl(nwUrl)
  
void SWebPageWidget:: gotoUrlBarUrl()
{
    QString urlTxt=urlBr->text(); //获取路径条文字内容。

    urlTxt=urlTxt.trimmed (); //清除首尾空白。





    
    QString urlTxt2Show=QUrl::fromPercentEncoding (urlTxt.toUtf8 ()); //将百分号编码解码。

    
    urlBr->setText(urlTxt2Show); //重新显示路径条文字。

    emit urlChanged (urlTxt2Show); //发射信号，网址改变。

    
    QUrl raUrl=QUrl(urlTxt); //构造一个裸的URL对象。
    
    
    QString raUrlSch=raUrl.scheme(); //获取协议。


    
    if (vldSchSet.contains(raUrlSch)) //所解析出的协议是本浏览器支持的协议。
    {
    } //if (vldSchSet.contains(raUrlSch)) //所解析出的协议是本浏览器支持的协议。
    else //是不支持的协议，或者是空协议。
    {
        raUrl=QUrl("http://"+urlBr->text()); //在这里补上一个协议头。
        urlBr->setText(raUrl.toString()); //补上HTTP。显示新的URL内容。

    } //else //是不支持的协议，或者是空协议。


      
    
    
    
    
    wbPg->load(raUrl); //转到对应的页面。
    
    wbPg->setFocus(); //焦点转移到网页中。
} //def GotoUrlBarUrl()

/*!
 * \brief SWebPageWidget::getUrl 获取网址。
 * \return 此页面的网址。
 */
QString SWebPageWidget::getUrl() const
{
  return urlBr->text(); //返回URL输入条的内容。
} //def getUrl()

/*!
 * \brief SWebPageWidget::initializeNavigationBar Initialize the navigation bar.
 */
void SWebPageWidget::initializeNavigationBar()
{
    vLyt=new QVBoxLayout(); //竖直布局器。

    vLyt->setContentsMargins(0,0,0,0);
    vLyt->setSpacing(0);

    setLayout(vLyt); //设置布局器。

    urlBr=new QLineEdit(); //URL输入条。

    urlLyt=new QHBoxLayout(); //水平布局器。

    urlLyt->setContentsMargins(0,0,0,0);
    urlLyt->setSpacing(0);

    nvTlBr=new QToolBar(); //创建工具条。
    urlLyt->addWidget(nvTlBr); //添加工具条到布局中。

    urlLyt->addWidget(urlBr); //添加URL输入条。

    addNewTabButton(urlLyt); //添加新建标签页按钮。

    //加入智能代理的开关按钮：
    addSmartProxyButton(urlLyt); //添加智能代码开关按钮。

    //加入二维码生成按钮：
    addQrButton(urlLyt); //添加二维码按钮。

    //加入推特分享按钮：
    addTwitterButton(urlLyt); //加入推特分享按钮。

    //加入搜索引擎内容输入框：
    searchEngineEdt=new QLineEdit(); //创建搜索引擎内容输入框。
    searchEngineEdt->setPlaceholderText (tr("Enter the search key word(Ctrl+J)")); //设置提示语。
    searchEngineEdt->setMaximumWidth (160); //限制宽度。
    urlLyt->addWidget (searchEngineEdt); //加入输入框。

    addDownloadButton(urlLyt); //向URL布局器中加入下载按钮。

    addUndoCloseTabButton(urlLyt); //向URL布局器中加入撤销刚关闭的标签页按钮。

    addGlobalMenuButton(urlLyt); // 向URL布局器中加入全局菜单按钮。

    addNavigationBar(vLyt,urlLyt); //添加导航栏。

    addActions(); //添加动作。
} // void SWebPageWidget::initializeNavigationBar()
  
/*!
 * \brief SWebPageWidget::setupUi 初始化界面。
 */
void SWebPageWidget::setupUi()
{
//    initializeNavigationBar(); // Initialize the navigation bar.

  wbPg=new StbWebView(); //创建网页视图。

  wbPg->setWebPageWidget(this); //设置自身的指针。

  vLyt->addWidget(wbPg); //添加网页视图。
} //void SWebPageWidget::setupUi()

/*!
 * \brief SWebPageWidget::connectSignals 连接信号与信号槽。
 */
void SWebPageWidget::connectSignals()
{
  connect(wbPg, &StbWebView::iconUrlFound, this, &SWebPageWidget::iconUrlFound); // Report, icon url found.
  connect(wbPg, &StbWebView::videoUrlFound, this, &SWebPageWidget::videoUrlFound); // Report, video url found.

  connect(urlBr,SIGNAL(returnPressed()),this,SLOT(gotoUrlBarUrl())); //按了回车键，则转到URL输入条所指定的URL。

  connect (searchEngineEdt,SIGNAL(returnPressed()),this,SLOT(searchEngineSearch())); //按回车键，则开始搜索。
  connect (searchEngineEdt,&QLineEdit::textChanged,this,&SWebPageWidget::querySearchEngineAutoComplete); //文字变化，则查询自动补全。

  connect(wbPg, &StbWebView::titleChanged, this, &SWebPageWidget::titleChanged); //标题变化，则发射标题变化信号。
  connect(wbPg, &StbWebView::iconChanged, this, &SWebPageWidget::iconChanged); //图标变化，则 发射图标变化信号。 陈欣

  connect(wbPg, &QWebEngineView::urlChanged, this, &SWebPageWidget::refreshUrl); //URL变化，则显示新的URL。
  connect(wbPg,SIGNAL(loadStarted()),this,SLOT(indicateLoadStart())); //开始载入，则显示载入中。
  connect(wbPg,SIGNAL(loadStarted()),this,SLOT(reportProxyStatus())); //开始载入，则报告代理状态。
  connect(wbPg,SIGNAL(loadProgress(int)),this,SLOT(indicateLoadProgress(int))); //载入进度变化，则显示进度变化。
  connect(wbPg,SIGNAL(loadProgress(int)),this,SLOT(reportProxyStatus())); //正在载入，则报告代理状态。
  connect(wbPg,SIGNAL(loadFinished(bool)),this,SLOT(indicateLoadFinished(bool))); //载入完成，则显示进度变化。
  connect(wbPg,SIGNAL(shouldShowStatusBarMessage(QString)),this,SLOT(showStatusBarMessage(QString))); //应当显示状态栏消息，则显示。
  connect (wbPg.data(), &StbWebView::downloadStarted, this, &SWebPageWidget::notifyDownloadStarted); //开始下载，则通知用户已经开始下载。
  connect (wbPg, &StbWebView::downloadStarted, this, &SWebPageWidget::assessShowDownloadOnlyUrl); //开始下载，则考虑是否要针对仅仅用来下载的标签页显示完整网址。

  connect(nwTbBtn,&QPushButton::clicked,this,&SWebPageWidget::requestOpenNewTab); //按钮被单击，则请求新建空白标签页。

  connect (wbPg, &StbWebView::zoomFactorChanged, this, &SWebPageWidget::zoomFactorChanged); //缩放比例发生变化，则原样发射。
  connect(wbPg,&StbWebView::windowCloseRequested,this,&SWebPageWidget::windowCloseRequested); //要求关闭窗口，则关闭窗口。
  connect (wbPg, &StbWebView::printRequested, this, &SWebPageWidget::exportAsPdf); //Export pdf

  connect(&reloadTimer, &QTimer::timeout, this, &SWebPageWidget::refresh); //

  // 在构造函数或初始化时
  connect(wbPg, &StbWebView::blobDownloadCompleted,
          this, &SWebPageWidget::onBlobDownloadCompleted);
} //void SWebPageWidget::connectSignals()

void SWebPageWidget::onBlobDownloadCompleted(const QByteArray &data, const QString &filename)
{
    // 转发给主控制器
    emit blobDownloadCompleted(data, filename);
}

/*!
   * \brief SWebPageWidget::SWebPageWidget 构造函数。
   * \param parent 亲代部件。
   */
SWebPageWidget::SWebPageWidget(QWidget *parent):QWidget(parent)
{
  initValidSchemeSet(); // 初始化有效的协议集合。

  initializeNavigationBar(); // Initialize the navigation bar.

  // 第一阶段初始化
  reloadTimer.setSingleShot(true);
  reloadTimer.setInterval(1000);

  // 延迟初始化的标记
  m_uiSetupDone = false;
} //SWebPageWidget::SWebPageWidget

void SWebPageWidget::initializeDependencies()
{
    if (!m_uiSetupDone) {
        setupUi();
        installEventFilters(); // 安装事件过滤器。
        connectSignals(); // 连接信号与信号槽。
        m_uiSetupDone = true;
    }
}

/*!
 * \param eventTarget 事件的目标。
 * \param event 事件对象。
 */
bool SWebPageWidget::eventFilter(QObject * eventTarget, QEvent * event)
{
    bool result=false; //结果。

    if (eventTarget==urlBr) //是地址栏。
    {
        result=handleTextContentLineEditEvent(event); //处理文本内容输入框的事件。
    } //if (eventTarget==ui->textContentlineEdit_2) //是文本内容输入框。

    return result;
} //void SWebPageWidget::eventFilter (QObject * eventTarget, QEvent * event)

/*!
 * \brief SWebPageWidget::checkClipboardChange 剪贴板发生变化，则检查变化。
 */
void SWebPageWidget::checkClipboardChange()
{
    QClipboard *clipboard = QApplication::clipboard();

    QString clipBoardText=clipboard->text();
    
    if (clipBoardText ==urlBr->text() ) //地址栏内容被复制。
    {
        QUrl url2Encode=QUrl(clipBoardText); //构造URL。
    
    
    
        QByteArray qrcode=(url2Encode.toEncoded ()); //Encode.
    

        QString encodedUrl=QString::fromUtf8(qrcode);
        
        if (encodedUrl!=clipBoardText)
        {
        clipboard->setText(encodedUrl);    
        } //if (encodedUrl!=clipBoardText)
        
        
    } //if (currentKeyEvent->key () ==Qt::Key_Up ) //上箭头。
} //void SWebPageWidget::checkClipboardChange()

/*!
 * \param event 事件对象。
 */
bool SWebPageWidget::handleTextContentLineEditEvent(QEvent * event)
{
    bool result=false; //结果。

    if (event->type ()==QEvent::Clipboard) //剪贴板变化事件。
    {
        QClipboard *clipboard = QApplication::clipboard();

        QString clipBoardText=clipboard->text();
        
        if (clipBoardText ==urlBr->text() ) //地址栏内容被复制。
        {
            QUrl url2Encode=QUrl(clipBoardText); //构造URL。
        
            QByteArray qrcode=(url2Encode.toEncoded ()); //Encode.

            QString encodedUrl=QString::fromUtf8(qrcode);
            
            clipboard->setText(encodedUrl);

            result=true; //消化此事件。
        } //if (currentKeyEvent->key () ==Qt::Key_Up ) //上箭头。
    } //if (event->type ()==QEvent::KeyPress) //按键按下事件。

    return result;
} //void SWebPageWidget::handleTextContentLineEditEvent(QEvent * event)

/*!
 * \brief TabManager::installEventFilters 安装事件过滤器。
 */
void SWebPageWidget::installEventFilters()
{
    urlBr->installEventFilter (this); //给文本内容输入框安装事件过滤器。
} //void TabManager::installEventFilters()

/*!
 * \brief SWebPageWidget::addNewTabButton 添加新建标签页按钮。
 * \param urlLyt 要向其中添加按钮的布局。
 */
void SWebPageWidget::addNewTabButton(QHBoxLayout * urlLyt)
{
    nwTbBtn=new QPushButton; //新建标签页按钮。
    nwTbBtn->setIcon (QIcon(":/Images/icon.png")); //设置图标。
    nwTbBtn->setShortcut (QKeySequence("Ctrl+T")); //设置快捷键。

    urlLyt->addWidget(nwTbBtn); //添加新建标签页按钮。
} //void SWebPageWidget::addNewTabButton(QHBoxLayout * urlLyt)

/*!
 * \brief SWebPageWidget::addSmartProxyButton 添加智能代码开关按钮。
 * \param urlLyt 要向其中添加按钮的布局器。
 */
void SWebPageWidget::addSmartProxyButton(QHBoxLayout * urlLyt)
{
    smtPrxyBtn=new QPushButton; //智能代理按钮。
    smtPrxyBtn->setIcon (QIcon(":/Images/2113711.png")); //设置图标。
    smtPrxyBtn->setCheckable (true); //可选中。
//    connect (smtPrxyBtn,SIGNAL(toggled(bool)),this,SLOT(toggleSmartProxy(bool))); //按钮状态被切换，则相应地切换智能代理状态。
    connect(smtPrxyBtn,&QAbstractButton::clicked,this,&SWebPageWidget::toggleSmartProxy); //按钮状态被按，则切换智能代理状态。

    urlLyt->addWidget (smtPrxyBtn); //加入到URL条布局中。
} //void SWebPageWidget::addSmartProxyButton(QHBoxLayout * urlLyt)

/*!
 * \brief SWebPageWidget::addQrButton 添加二维码按钮。
 * \param urlLyt 要向其中添加按钮的布局器。
 */
void SWebPageWidget::addQrButton(QHBoxLayout * urlLyt)
{
    QPushButton * qrBtn=new QPushButton; //二维码按钮。
    qrBtn->setIcon (QIcon(":/Images/stupidbeauty.80.32.png")); //设置图标。
    qrBtn->setShortcut (QKeySequence("Ctrl+R")); //设置快捷键。
    connect(qrBtn,SIGNAL(clicked()),this,SLOT(generateQrCode())); //按了二维码按钮，则生成二维码。

    urlLyt->addWidget (qrBtn); //加入按钮。
} //void SWebPageWidget::addQrButton(QHBoxLayout * urlLyt)

/*!
 * \brief SWebPageWidget::addTwitterButton 加入推特分享按钮。
 * \param urlLyt 要向其中添加按钮的布局器。
 */
void SWebPageWidget::addTwitterButton(QHBoxLayout * urlLyt)
{
    QPushButton * qrBtn=new QPushButton; //二维码按钮。
    qrBtn->setIcon (QIcon(":/Images/twitter-logo_22.258.png")); //设置图标。
    qrBtn->setShortcut (QKeySequence("Ctrl+I")); //设置快捷键。
    connect(qrBtn,&QPushButton::clicked,this,&SWebPageWidget::openTwitterShareTab); //按了推特按钮，则打开推特分享标签。

    urlLyt->addWidget (qrBtn); //加入按钮。
} //void SWebPageWidget::addTwitterButton(QHBoxLayout * urlLyt)

void SWebPageWidget:: setFirstTimeLoad(bool ftl2St)
{
    firstTimeLoad=ftl2St; //记录。
} //def setFirstTimeLoad(ftl2St)

/*!
 * \brief SWebPageWidget::setUrl 设置网址。
 * \param url2St 要设置的网址。
 */
void SWebPageWidget::setUrl(QString url2St)
{
    urlBr->setText(url2St); //显示URL。

    emit urlChanged (url2St); //发射信号，网址改变。
} //void SWebPageWidget::  setUrl(QString url2St)
  
/*!
 * \brief SWebPageWidget::startLoad 开始载入。
 */
void SWebPageWidget:: startLoad()
{
    firstTimeLoad=false; //不再是第一次载入了。
    
    gotoUrlBarUrl(); //载入URL输入栏中的路径。
} //def startLoad()

/*!
   * \brief SWebPageWidget::setNetworkAccessManager 设置网络访问管理器的指针。
   * \param ntAccsMngr4WebPages 要设置的网络访问管理器的指针。
   */
void SWebPageWidget::setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages)
{
    ntAccsMngr=ntAccsMngr4WebPages; //记录。
} //void SWebPageWidget::setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages) //!<设置网络访问管理器的指针。

/*!
 * \brief SWebPageWidget::toggleSmartProxy 按钮状态被切换，则相应地切换智能代理状态。
 * \param isCkd 按钮是否处于选中状态。
 */
void SWebPageWidget::toggleSmartProxy(bool isCkd)
{
  QUrl crtUrl(urlBr->text ()); //获取当前URL。
  QString hstNm=crtUrl.host (); //获取主机名。

  emit toggleProxyRule(isCkd,hstNm); //发射信号，要求切换针对当前域名的智能代理规则。
} //void SWebPageWidget::toggleSmartProxy(bool isCkd) //!<按钮状态被切换，则相应地切换智能代理状态。

/*!
 * \brief SWebPageWidget::reportProxyStatus 开始载入，则报告代理状态。
 */
void SWebPageWidget::reportProxyStatus()
{
    QUrl crtUrl(urlBr->text ()); //获取当前URL。
    QString hstNm=crtUrl.host (); //获取主机名。

    if (prxyFctr->isUsingProxyForHostname(hstNm)) //是在为此主机使用代理。
    {
        smtPrxyBtn->setChecked (true); //按钮处于按下状态。
    } //if (prxyFctr->isUsingProxyForHostname(hstNm)) //是在为此主机使用代理。
    else //未为此主机使用代理。
    {
        smtPrxyBtn->setChecked (false); //按钮处于未按下状态。
    } //else //未为此主机使用代理。
} //void SWebPageWidget::reportProxyStatus() //!<开始载入，则报告代理状态。

/*!
 * \brief SWebPageWidget::setProxyFactory 设置代理工厂。
 * \param prxyFctry 要设置的代理工厂。
 */
void SWebPageWidget::setProxyFactory(StbProxyFactory * prxyFctry)
{
    prxyFctr=prxyFctry; //记录。
} //void SWebPageWidget::setProxyFactory(StbProxyFactory * prxyFctry) //!<设置代理工厂。

/*!
 * \brief SWebPageWidget::initValidSchemeSet 初始化有效的协议集合。
 */
void SWebPageWidget::initValidSchemeSet()
{
    vldSchSet << "http" << "ftp" << "https" << "file" << "chrome"; //加入所支持的各个协议。
} //void SWebPageWidget::initValidSchemeSet() //!<初始化有效的协议集合。

/*!
 * \brief SWebPageWidget::openTwitterShareTab 按了推特按钮，则打开推特分享标签。
 */
void SWebPageWidget::openTwitterShareTab()
{
    if (urlBr->text ().isEmpty ()) //地址栏为空，不动作。
    {
    } //if (urlBr->text ().isEmpty ()) //地址栏为空，不动作。
    else //地址栏非空。
    {
        QString url2Share=urlBr->text (); //获取要分享的网址。
        QString title2Share=wbPg->title (); //获取要分享的网页标题。

        QUrl url2Encode=QUrl(url2Share); //构造URL。



        QString url2ShareEncoded=(url2Encode.toEncoded ().toPercentEncoding ()); //Encode.



        QString hashTags=constructHashTags().toUtf8 ().toPercentEncoding (); //构造标记列表字符串。

        title2Share=title2Share.toUtf8 ().toPercentEncoding (); //进行百分号编码。

        QString twitterShareUrl=tr("https://twitter.com/intent/tweet?text=%1 %2&url=%3").arg (title2Share   ).arg(hashTags).arg (url2ShareEncoded); //构造要分享的网址。

        emit shouldOpenNewTab (twitterShareUrl); //发射信号，应当打开新的标签页。
    } //else //地址栏非空。
} //void SWebPageWidget::openTwitterShareTab()

/*!
 * \brief SWebPageWidget::generateQrCode 按了二维码按钮，则生成二维码。
 */
void SWebPageWidget::generateQrCode()
{
    if (urlBr->text ().isEmpty ()) //地址栏为空，不动作。
    {
    } //if (urlBr->text ().isEmpty ()) //地址栏为空，不动作。
    else //地址栏非空。
    {
        qrCodeDialog->hide (); //先隐藏。
        qrCodeDialog->show(); //显示二维码对话框。

        qrCodeDialog->setUrl(urlBr->text ()); //设置二维码。
        qrCodeDialog->setWebPageTitle(wbPg->title()); //Set the web page title.

//        cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << tr(", web page title: ").toStdString() << wbPg->title().toStdString()  <<  endl; //Debug.

        qrCodeDialog->stackUnder (this); //放到栈下面。

        qrCodeDialog->raise (); //升到最前面。
        qrCodeDialog->activateWindow (); //激活窗口。
    } //else //地址栏非空。
} //void SWebPageWidget::generateQrCode() //!<按了二维码按钮，则生成二维码。

/*!
 * \brief StbWebView::showStatusBarMessage 要求显示状态栏消息，则显示。
 * \param msg2Show 要显示的消息内容。
 */
void SWebPageWidget::showStatusBarMessage(QString msg2Show)
{
  brsrWnd->statusBar ()->showMessage(msg2Show); //显示消息。
} //void StbWebView::showStatusBarMessage(QString msg2Show) //!<要求显示状态栏消息，则显示。

/*!
 * \brief SWebPageWidget::downloadCurrentUrl Start download current url
 */
void SWebPageWidget::downloadCurrentUrl()
{
  wbPg->downloadCurrentUrl(); //Start download current url.
} //void SWebPageWidget::downloadCurrentUrl()

/*!
 * \brief SWebPageWidget::killRenderProcess 杀死所有的渲染进程。
 */
void SWebPageWidget::killRenderProcess()
{
    if (wbPg)
    {
        wbPg->killRenderProcess(); // 杀死渲染进程。

    } // if (wbPg)
} // void SWebPageWidget::killRenderProcess()

void SWebPageWidget::handleBlobDownload(const QUrl &url, const QString &filename)
{
    wbPg->handleBlobDownload(url.toString(), filename);
}

/*!
 * \brief SWebPageWidget::pauseVideo Pause video when lost focus. Chen xin.
 */
void SWebPageWidget::pauseVideo()
{
  wbPg->pauseVideo(); // Pause video.
} // void SWebPageWidget::pauseVideo()

/*!
 * \brief SWebPageWidget::extractMediaUrls Extract media urls.
 */
void SWebPageWidget::extractMediaUrls()
{
  wbPg->extractMediaUrls(); // Extract media urls. Chen xin
} // void SWebPageWidget::extractMediaUrls()

/*!
 * \brief SWebPageWidget::extractImageUrls Extract image urls.
 */
void SWebPageWidget::extractImageUrls()
{
  wbPg->extractImageUrls(); // Extract image urls. Chen xin
} //void SWebPageWidget::extractImageUrls()

/*!
 * \brief SWebPageWidget::findText 查找文字。
 * \param txt2Fnd 要查找的文字。
 * \param backward 是否反向查找。
 */
void SWebPageWidget::findText(QString txt2Fnd, bool backward)
{
    QWebEnginePage::FindFlags flags;
    if (backward)
    {
        flags |= QWebEnginePage::FindBackward;
    }
    // options.setCaseSensitivity(Qt::CaseInsensitive);
    // options.setFindBackward(backward);

    wbPg->findText(txt2Fnd, flags); // 执行查找
}

/*!
 * \brief SWebPageWidget::addToolbarActions 向工具条加入动作。
 */
void SWebPageWidget::addToolbarActions()
{
    //导航工具条：
    QAction * bckAct=wbPg->pageAction(QWebEnginePage::Back); //获取后退按钮。
    nvTlBr->addAction(bckAct); //添加后退按钮。

    addReloadToolButton(); //添加重新载入按钮。


    QAction * forwardAct=wbPg->pageAction(QWebEnginePage::Forward); //获取前进按钮。
    nvTlBr->addAction(forwardAct); //添加前进按钮。



    QAction * stpAct=wbPg->pageAction (QWebEnginePage::Stop); // 获取停止载入按钮。
    nvTlBr->addAction(stpAct); //添加停止载入按钮。
} //void SWebPageWidget::addToolbarActions() //!<向工具条加入动作。

/*!
 * \brief SWebPageWidget::selectAll 全选文字。
 */
void SWebPageWidget::selectAll()
{
    wbPg->selectAll (); //全选文字。
} //void SWebPageWidget::selectAll() //!<全选文字。

void SWebPageWidget::applyWebChannel()
{
    wbPg->applyWebChannel (); //全选文字。

} // void SWebPageWidget::applyWebChannel()

/*!
 * \brief SWebPageWidget::searchEngineSearch 按回车键，则开始搜索。
 */
void SWebPageWidget::searchEngineSearch()
{
    QString searchKeyWord=searchEngineEdt->text(); //获取搜索关键词。
    
    emit shouldQuerySearchEngine(searchKeyWord); //发射信号，应当向搜索引擎查询。
} //void SWebPageWidget::searchEngineSearch() //!<按回车键，则开始搜索。

/*!
 * \brief SWebPageWidget::addDownloadButton 向URL布局器中加入下载按钮。
 * \param urlLyt 要向其中加入下载按钮的水平布局器。
 */
void SWebPageWidget::addDownloadButton(QHBoxLayout * urlLyt)
{
    downloadButton=new QPushButton; //创建下载按钮。

    urlLyt->addWidget (downloadButton); //添加到界面中。

    QIcon downloadIcon(":/Images/download.png"); //下载图标。

    downloadButton->setIcon (downloadIcon); //设置图标，下载图标。

    connect (downloadButton.data (),&QPushButton::clicked,this,&SWebPageWidget::showDownloadManager); //下载按钮被按，则显示下载管理器对话框。
}

void SWebPageWidget::setQrCodeDialog(QrCodeDialog *value)
{
    qrCodeDialog = value;
} //void SWebPageWidget::addDownloadButton(QHBoxLayout * urlLyt)

/*!
 * \brief SWebPageWidget::showDownloadManager 下载按钮被按，则显示下载管理器对话框。
 */
void SWebPageWidget::showDownloadManager()
{
    emit shouldShowDownloadManager(); //发射信号，应当显示下载管理器。
} //void SWebPageWidget::showDownloadManager()

/*!
 * \brief SWebPageWidget::assessShowDownloadOnlyUrl 开始下载，则考虑是否要针对仅仅用来下载的标签页显示完整网址。
 */
void SWebPageWidget::assessShowDownloadOnlyUrl()
{
    if (urlBr->text ()=="http://") //要补全。
    {
        auto downloadOnlyUrl=wbPg->url ().toString (); //获取网址。

        setUrl (downloadOnlyUrl); //显示新的网址。
    } //if (urlBr->text ()=="http://") //要补全。
} //void SWebPageWidget::assessShowDownloadOnlyUrl()

/*!
 * \brief SWebPageWidget::notifyDownloadStarted 开始下载，则通知用户已经开始下载。
 */
void SWebPageWidget::notifyDownloadStarted()
{

    QPropertyAnimation * notifyDownloadStartEffectAnimation=new QPropertyAnimation; //创建属性动画。

    downloadStartedEffect=new QGraphicsColorizeEffect; //创建颜色化效果。

    downloadButton->setGraphicsEffect (downloadStartedEffect); //设置图形效果。

    notifyDownloadStartEffectAnimation->setTargetObject (downloadStartedEffect); //设置目标对象。
    notifyDownloadStartEffectAnimation->setDuration (4500); //设置持续时间。
    notifyDownloadStartEffectAnimation->setEndValue (QColor(0,0,0)); //设置结束值。
    notifyDownloadStartEffectAnimation->setStartValue (QColor(0,0,255)); //设置开始值。
    notifyDownloadStartEffectAnimation->setPropertyName ("color"); //设置属性名字。



    notifyDownloadStartEffectAnimation->start(); //启动动画。
} //void SWebPageWidget::notifyDownloadStarted()

/*!
 * \brief SWebPageWidget::addGlobalMenuButton 向URL布局器中加入全局菜单按钮。
 * \param urlLyt URL布局器。
 */
void SWebPageWidget::addGlobalMenuButton(QHBoxLayout *urlLyt)
{
    // 创建菜单按钮（使用 QToolButton）
    globalMenuButton = new QPushButton;

    // 添加到界面布局中
    urlLyt->addWidget(globalMenuButton);

    // 设置系统图标（兼容性更好）
    QIcon menuIcon = QIcon::fromTheme("application-menu", QIcon(":/Images/icon-menu.png")); // 默认图标回退
    globalMenuButton->setIcon(menuIcon);

    // 设置文本（可选）
    // globalMenuButton->setText("菜单");

    // 设置按钮风格（图标+文字）
    // globalMenuButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // 设置按钮大小策略（可选）
    // globalMenuButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    // 连接点击事件，使用主窗口的 menuBar()
    connect(globalMenuButton, &QToolButton::clicked, this, [this]() {
        // 创建一个临时菜单
        QMenu menu;

        // 获取主窗口的菜单栏
        QMenuBar *mainMenuBar = brsrWnd->menuBar();

        // 将菜单栏中的所有顶级菜单添加到临时菜单中
        for (auto *action : mainMenuBar->actions()) {
            if (QMenu *subMenu = action->menu()) {
                menu.addMenu(subMenu);
            } else {
                menu.addAction(action);
            }
        }

        // 计算弹出位置（按钮下方）
        QPoint pos = globalMenuButton->mapToGlobal(QPoint(0, globalMenuButton->height()));

        // 弹出菜单
        menu.exec(pos);
    });
}

/*!
 * \brief SWebPageWidget::addUndoCloseTabButton 向URL布局器中加入撤销刚关闭的标签页按钮。
 * \param urlLyt URL布局器。
 */
void SWebPageWidget::addUndoCloseTabButton(QHBoxLayout * urlLyt)
{
    undoCloseTabButton=new QPushButton; //创建撤销关闭标签页按钮。

    urlLyt->addWidget (undoCloseTabButton); //添加到界面中。

    QIcon downloadIcon(":/Images/icon 1.1.png"); //撤销图标。

    undoCloseTabButton->setIcon (downloadIcon); //设置图标，撤销图标。
    undoCloseTabButton->setEnabled (false); //默认处于禁用状态。

    connect (undoCloseTabButton.data (),&QPushButton::clicked,this,&SWebPageWidget::undoCloseTab); //撤销按钮被按，则撤销刚才被关闭的标签页。
} //void SWebPageWidget::addUndoCloseTabButton(QHBoxLayout * urlLyt)

/*!
 * \brief SWebPageWidget::undoCloseTab 撤销按钮被按，则撤销刚才被关闭的标签页。
 */
void SWebPageWidget::undoCloseTab()
{
    emit shouldUndoCloseTab(); //发射信号，应当撤销被关闭的标签页。

    QString searchEnginePrefix="http://www.google.com.hk/search"; //搜索引擎的网址前缀。

    QString searchUrlString=searchEnginePrefix; //构造搜索路径字符串。
    QUrl searchUrl(searchUrlString); //构造URL。

    QUrlQuery srchQry; //查询对象。

    searchUrl.setQuery (srchQry); //设置查询对象。
} //void SWebPageWidget::undoCloseTab()



/*!
 * \brief SWebPageWidget::inspect 分析网页。
 */
void SWebPageWidget::inspect()
{
//    cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this:" << this << endl; //Debug.

    wbPg->inspect(); //分析网页。
} //void SWebPageWidget::inspect()

/*!
 * \brief SWebPageWidget::exportAsPdf 导出成PDF。
 */
void SWebPageWidget::exportAsPdf()
{
    QString pageTitle=wbPg->title(); //页面标题。
    QString pageTitleFiltered=filterForFileName(pageTitle); //过滤为可用的文件名。

    QString saveFileName=QFileDialog::getSaveFileName (this,tr("Export web page as pdf"),pageTitleFiltered+".pdf",tr("pdf(*.pdf)")); //选择保存目标。

    if (saveFileName.isEmpty ()) //文件名为空，则用户取消了。
    {

    } //if (saveFileName.isEmpty ()) //文件名为空，则用户取消了。
    else //文件名有效。
    {
        wbPg->exportAsPdf(saveFileName); //保存文件。
    } //else //文件名有效。
} //void SWebPageWidget::exportAsPdf()

/*!
 * \brief SWebPageWidget::setNavigationBarVisible 设置可见性。
 * \param visible 是否可见。
 */
void SWebPageWidget::setNavigationBarVisible(bool visible)
{

    navBarFrame->setVisible(visible); //设置可见性。
} //void SWebPageWidget::setNavigationBarVisible(bool visible)

/*!
 * \brief SWebPageWidget::addNavigationBar 添加导航栏。
 * \param vLyt 要将导航栏添加到的目标布局。
 * \param urlLyt 包含着导航栏内容的布局。
 */
void SWebPageWidget::addNavigationBar(QVBoxLayout * vLyt,QHBoxLayout * urlLyt)
{
    navBarFrame=new QFrame; //创建框框。

    navBarFrame->setSizePolicy (QSizePolicy::Minimum,QSizePolicy::Maximum); //设置尺寸策略。

    navBarFrame->setLayout (urlLyt); //设置布局器。

    vLyt->addWidget (navBarFrame); //将框框添加到布局器中。
} //void SWebPageWidget::addNavigationBar(QVBoxLayout * vLyt,QHBoxLayout * urlLyt)

/*!
 * \brief SWebPageWidget::addActions 添加动作。
 */
void SWebPageWidget::addActions()
{
    QAction * focusSearchEngineAction=new QAction(this); //退出按钮。
    focusSearchEngineAction->setText (tr("")); //设置文字。
    focusSearchEngineAction->setShortcut (QKeySequence("Ctrl+J"));

    addAction (focusSearchEngineAction); //添加动作。

    connect (focusSearchEngineAction,&QAction::triggered,this,&SWebPageWidget::focusSearchEngine); //焦点定于搜索引擎。
} //void SWebPageWidget::addActions()

/*!
 * \brief SWebPageWidget::focusSearchEngine 焦点定于搜索引擎。
 */
void SWebPageWidget::focusSearchEngine()
{
    searchEngineEdt->setFocus (); //设置焦点。
} //void SWebPageWidget::focusSearchEngine()

/*!
 * \brief SWebPageWidget::mouseReleaseEvent 鼠标释放事件。
 * \param event 事件。
 */
void SWebPageWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button ()== Qt::BackButton) //是后退按钮。
    {
        wbPg->triggerPageAction (QWebEnginePage::Back); //后退。
    } //if (event->button ()== Qt::BackButton) //是后退按钮。
    else if (event->button ()==Qt::ForwardButton) //是前进按钮。
    {
        wbPg->triggerPageAction (QWebEnginePage::Forward); //前进。
    } //else if (event->button ()==Qt::ForwardButton) //是前进按钮。
    else //其它按钮。
    {
        QWidget::mouseReleaseEvent (event); //让超类来处理。
    } //else //其它按钮。
} //void SWebPageWidget::mouseReleaseEvent(QMouseEvent * event)

/*!
 * \brief SWebPageWidget::filterForFileName 过滤为可用的文件名。
 * \param pageTitle 要过滤的原始文件名。
 * \return 过滤结果。
 */
QString SWebPageWidget::filterForFileName(QString pageTitle)
{
    QString result=pageTitle; //结果。


#ifndef Q_OS_MACOS
    result=ValidFileName::filterForFileName(pageTitle,ValidFileName::GenericFileName); //过滤得到有效的文件名。
#endif


    return result;
} //QString SWebPageWidget::filterForFileName(QString pageTitle)

/*!
 * \brief SWebPageWidget::forbidUndoClosedTab 禁用恢复标签页按钮。
 */
void SWebPageWidget::forbidUndoClosedTab()
{
    undoCloseTabButton->setEnabled (false); //不可按了。
} //void SWebPageWidget::forbidUndoClosedTab()

/*!
 * \brief SWebPageWidget::enableUndoClosedTab 启用恢复标签页按钮。
 */
void SWebPageWidget::enableUndoClosedTab()
{
    undoCloseTabButton->setEnabled (true); //可按了。
} //void SWebPageWidget::enableUndoClosedTab()

/*!
 * \brief SWebPageWidget::querySearchEngineAutoComplete 文字变化，则查询自动补全。
 * \param currentQueryText 当前的查询文字内容。
 */
void SWebPageWidget::querySearchEngineAutoComplete(QString currentQueryText)
{
    QNetworkRequest * CrtGcmRqst; //用于向GCM服务器发送请求的请求对象。

    QString urlString=tr("http://suggestqueries.google.com/complete/search?client=firefox&q=%1").arg (currentQueryText); //检查补全的URL。


    CrtGcmRqst=new QNetworkRequest(QUrl(urlString)); //Create the request.
    QNetworkReply* CrtRply= ntAccsMngr->get(*CrtGcmRqst); //发送GET请求。
    connect(CrtRply,SIGNAL(finished()),this,SLOT(parseSearchEngineAutoCompleteResult())); //完成之後，處理其結果。
} //void SWebPageWidget::querySearchEngineAutoComplete(QString currentQueryText)

/*!
 * \brief SWebPageWidget::parseSearchEngineAutoCompleteResult 自动补全请求完成之後，處理其結果。
 */
void SWebPageWidget::parseSearchEngineAutoCompleteResult()
{


    QNetworkReply* Rst=(QNetworkReply *)(sender()); //獲取發射信號的回復對象的指針。


    QString WholeRest=""; //整个结果。

    QTextCodec * gbk_codec=QTextCodec::codecForName("UTF8"); //文字解码器。



    QByteArray ssnStrBtAr=Rst->readAll (); //读取全部内容。

    WholeRest=gbk_codec->toUnicode(  ssnStrBtAr); //转义成UTF8字符串。








    delete Rst; //释放内存。
} //void SWebPageWidget::parseSearchEngineAutoCompleteResult()

/*!
 * \brief SWebPageWidget::requestOpenNewTab 按钮被单击，则请求新建空白标签页。
 */
void SWebPageWidget::requestOpenNewTab()
{
    emit shouldOpenNewTab (""); //请求打开空白标签页。
} //void SWebPageWidget::requestOpenNewTab()

/*!
 * \brief SWebPageWidget::constructHashTags 构造标记列表字符串。
 * \return 构造的结果。
 */
QString SWebPageWidget::constructHashTags()
{
    QString result=""; //结果。

//    wbPg->page()->




    result=result.trimmed (); //去除空白。

    return result;
} //QString SWebPageWidget::constructHashTags()

/*!
 * \brief SWebPageWidget::paste 粘贴。
 */
void SWebPageWidget::paste()
{
    wbPg->paste(); //粘贴。

    focusWebView(); //让网页视图获取焦点。
} //void SWebPageWidget::paste()

/*!
 * \brief SWebPageWidget::focusAddressBar 焦点定位到地址栏。
 */
void SWebPageWidget::focusAddressBar()
{
    urlBr->setFocus(); //设置焦点。

} //void SWebPageWidget::focusAddressBar()

/*!
 * \brief SWebPageWidget::refresh 刷新。
 */
void SWebPageWidget::refresh()
{
    wbPg->reload (); //重新载入。
} //void SWebPageWidget::refresh()

/*!
 * \brief SWebPageWidget::updateRestoreTabToolTip 修改恢复标签页的提示文字。
 * \param newToolTip 新的提示文字。
 */
void SWebPageWidget::updateRestoreTabToolTip(QString newToolTip)
{
    undoCloseTabButton->setToolTip (newToolTip); //设置提示文字。
} //void SWebPageWidget::updateRestoreTabToolTip(QString newToolTip)

/*!
 * \brief SWebPageWidget::goBackTop 回到顶端。
 */
void SWebPageWidget::goBackTop()
{
    wbPg->goBackTop(); //回到顶端。
} //void SWebPageWidget::goBackTop()

/*!
 * \brief SWebPageWidget::addReloadToolButton 添加重新载入按钮。
 */
void SWebPageWidget::addReloadToolButton()
{
    QAction * rldAct=wbPg->pageAction(QWebEnginePage::Reload); //获取刷新按钮。
    connect(rldAct,SIGNAL(triggered()),this,SLOT(focusWebView())); //重新载入，则给网页赋予焦点。

    QAction * reloadRegardCacheAction=wbPg->pageAction (QWebEnginePage::ReloadAndBypassCache); //获取按钮，重新载入且无视缓存。
    connect (reloadRegardCacheAction,&QAction::triggered,this,&SWebPageWidget::focusWebView); //重新载入，则给网页赋予焦点。


    reloadMenu=new QMenu; //创建重新载入菜单。

    reloadMenu->addAction(rldAct); //添加刷新按钮。
    reloadMenu->addAction(reloadRegardCacheAction); //添加忽略缓存重新载入按钮。

    QToolButton * reloadToolButton=new QToolButton; //创建工具按钮。
    reloadToolButton->setMenu (reloadMenu); //设置菜单。

    nvTlBr->addWidget (reloadToolButton); //加入重新载入按钮。
} //void SWebPageWidget::addReloadToolButton()

/*!
 * \brief SWebPageWidget::getTitle 获取标题。
 * \return 标题。
 */
QString SWebPageWidget::getTitle()
{
    return title;
} //QString SWebPageWidget::getTitle()

/*!
 * \brief SWebPageWidget::setTitle 设置标题。
 * \param crtTtl 要设置的标题。
 */
void SWebPageWidget::setTitle(QString crtTtl)
{
    title=crtTtl; //记录标题。

    emit titleChanged (crtTtl); //发射信号，标题改变。
} //void SWebPageWidget::setTitle(QString crtTtl)

/*!
 * \brief SWebPageWidget::zoomIn 放大。
 */
void SWebPageWidget::zoomIn()
{
    wbPg->zoomIn(); //放大。
} //void SWebPageWidget::zoomIn()

/*!
 * \brief SWebPageWidget::zoomOut 缩小。
 */
void SWebPageWidget::zoomOut()
{
    wbPg->zoomOut(); //缩小。
} //void SWebPageWidget::zoomOut()

/*!
 * \brief SWebPageWidget::zoomFactor 获取缩放比例。
 * \return 缩放比例。
 */
qreal SWebPageWidget::zoomFactor() const
{
    qreal result = 1.0;

    if (wbPg) // The web view exists
    {
        result = wbPg->zoomFactor ();
    }

    return result;
} //qreal SWebPageWidget::zoomFactor() const

/*!
 * \brief SWebPageWidget::zoomRestore 恢复缩放比例。
 */
void SWebPageWidget::zoomRestore()
{
    wbPg->zoomRestore(); //恢复缩放比例。
} //void SWebPageWidget::zoomRestore()

/*!
 * \brief SWebPageWidget::zoom 直接缩放到指定比例。
 * \param zoomFactor 缩放比例。
 */
void SWebPageWidget::zoom(const qreal zoomFactor)
{
    if (wbPg) // The view exists
    {
        wbPg->zoom(zoomFactor); //缩放到指定比例。

    } // if (wbPg) // The view exists
} //void SWebPageWidget::zoom(const qreal zoomFactor)
