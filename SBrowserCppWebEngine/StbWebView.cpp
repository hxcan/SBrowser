#include "SbPreCompile.h" //QShortcut

#include "StbWebView.h" //StbWebView
#include "StartSlideGoBackTop.h" //StartSlideGoBackTop

#include "StbWebPage.h" //StbWebPage

/*!
 * \brief StbWebView::inspect 分析网页。
 */
void StbWebView::inspect()
{
  QAction * inspectAction=customPage->action(QWebEnginePage::InspectElement); //获取动作。

  qDebug() << __FILE__ << __LINE__ << __func__ << tr("action enabled?:") << inspectAction->isEnabled(); //Debug.

//    cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this:" << this << ", action enabled?: " << inspectAction->isEnabled() << endl; //Debug.


  customPage->triggerAction(QWebEnginePage::InspectElement); //触发动作，分析网页。
} //void StbWebView::inspect()

/*!
 * \brief StbWebView::killRenderProcess 杀死所有的渲染进程。
 */
void StbWebView::killRenderProcess()
{
  auto renderPid=customPage->renderProcessPid (); // 获取进程编号。

  QStringList ninePid=
  {
    "-9",
    QString("%1").arg (renderPid),
  };

  auto killProgram="kill"; // kill 命令。

  QProcess::startDetached (killProgram, ninePid); // 启动进程。
} // void StbWebView::killRenderProcess()

void StbWebView::handleBlobDownload(const QString &blobUrl, const QString &suggestedFilename)
{
    cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this:" << this
         << ",blobUrl:" << blobUrl.toUtf8().data() << endl;

    // 读取 JS 脚本
    QFile jsFile(":/Scripts/handleBlobDownload.i.js");
    if (!jsFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open handleBlobDownload.i.js";
        return;
    }

    QByteArray jsContent = jsFile.readAll();
    jsFile.close();

    // 替换占位符
    QString jsCode = QString::fromUtf8(jsContent);
    jsCode.replace("%1", "'" + blobUrl + "'");
    jsCode.replace("%2", "'" + suggestedFilename + "'");

    // 执行脚本
    customPage->runJavaScript(jsCode);
}

/*!
 * \brief StbWebView::pauseVideo Pause video when lost focus. Chen xin.
 */
void StbWebView::pauseVideo()
{
//  customPage->triggerAction (QWebEnginePage:);

  cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this:" << this << ", current page: " << customPage <<  endl; //Debug.

  QFile removeBlmFileAdsByGoogle(":/Scripts/pauseMedium.i.js"); // extract image urls 的代码文件名

  removeBlmFileAdsByGoogle.open(QIODevice::ReadOnly);

  auto removeBlmScriptFileContentAdsByGoogle=removeBlmFileAdsByGoogle.readAll(); //全部读取

  removeBlmFileAdsByGoogle.close(); //关闭文件。

  auto removeBlmScriptCodeContentAdsByGoogle=QString::fromUtf8(removeBlmScriptFileContentAdsByGoogle); //转换成字符串

  customPage->runJavaScript(removeBlmScriptCodeContentAdsByGoogle); // 运行脚本 Chen xin

} // void StbWebView::pauseVideo()

/*!
 * \brief StbWebView::paste 粘贴。
 */
void StbWebView::paste()
{
  customPage->triggerAction(QWebEnginePage::Paste); //触发动作，粘贴。
} //void StbWebView::paste()

/*!
 * \brief StbWebView::extractMediaUrls Extract media urls.
 */
void StbWebView::extractMediaUrls()
{
  cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this:" << this <<  endl; //Debug.

  QFile removeBlmFileAdsByGoogle(":/Scripts/extractMediaUrls.i.js"); // extract image urls 的代码文件名

  removeBlmFileAdsByGoogle.open(QIODevice::ReadOnly);

  auto removeBlmScriptFileContentAdsByGoogle=removeBlmFileAdsByGoogle.readAll(); //全部读取

  removeBlmFileAdsByGoogle.close(); //关闭文件。

  auto removeBlmScriptCodeContentAdsByGoogle=QString::fromUtf8(removeBlmScriptFileContentAdsByGoogle); //转换成字符串

  customPage->runJavaScript(removeBlmScriptCodeContentAdsByGoogle, *videoUrlCallback); // 运行脚本 Chen xin
} // void StbWebView::extractMediaUrls()

/*!
 * \brief StbWebView::extractImageUrls Extract image urls.
 */
void StbWebView::extractImageUrls()
{
  QFile removeBlmFileAdsByGoogle(":/Scripts/extractImageUrls.i.js"); // extract image urls 的代码文件名

  removeBlmFileAdsByGoogle.open(QIODevice::ReadOnly);

  auto removeBlmScriptFileContentAdsByGoogle=removeBlmFileAdsByGoogle.readAll(); //全部读取

  removeBlmFileAdsByGoogle.close(); //关闭文件。

  auto removeBlmScriptCodeContentAdsByGoogle=QString::fromUtf8(removeBlmScriptFileContentAdsByGoogle); //转换成字符串

    customPage->runJavaScript(removeBlmScriptCodeContentAdsByGoogle, *iconUrlCallback); //运行脚本

} // void StbWebView::extractImageUrls()

/*!
 * \brief StbWebView::downloadCurrentUrl Start download current url.
 */
void StbWebView::downloadCurrentUrl()
{
    customPage->download(customPage->url()); //Start download
} //void StbWebView::downloadCurrentUrl()

/*!
 * \brief StbWebView::exportAsPdf 保存网页为PDF。
 * \param saveFileName 目标文件名。
 */
void StbWebView::exportAsPdf(QString saveFileName)
{
  Q_UNUSED(saveFileName); //不使用勒个参数。

  customPage->printToPdf(saveFileName); //输出成PDF。
} //void StbWebView::exportAsPdf(QString saveFileName)

void StbWebView::selectAll()
{
  triggerPageAction(QWebEnginePage::SelectAll); //触发全选动作。
} //def selectAll()

/*!
 * \brief StbWebView::StbWebView 构造函数。
 * \param parent 亲代对象的指针。
 */
StbWebView::StbWebView(QWebEngineView *parent):QWebEngineView (parent)
{
  initializeMemberObjects(); //初始化成员变量。

  connectSignals(); //连接信号槽。
} //def initialize(parent=nil)

QWebEngineView *StbWebView::createWindow(QWebEnginePage::WebWindowType wndTp)
{
  QWebEngineView * result=wbPgWdgt->createWindow(wndTp); //要求当前网页部件创建新窗口。
    
  return result; //返回结果。
} //def createWindow(wndTp)
  
void StbWebView::setWebPageWidget(SWebPageWidget * wbPgWdgt2St)
{
  wbPgWdgt=wbPgWdgt2St; //记录。
} //def setWebPageWidget(wbPgWdgt2St)
  
/*!
 * \brief StbWebView::connectSignals 连接信号槽。
 */
void StbWebView::connectSignals()
{
  connect (page(),&QWebEnginePage::linkHovered,this,&StbWebView::showLinkTarget); //鼠标悬停于链接上，则显示链接目标。
  connect(page(),&QWebEnginePage::windowCloseRequested,this,&StbWebView::windowCloseRequested); //要求关闭窗口，则关闭窗口。
  connect(page(), &QWebEnginePage::featurePermissionRequested, this, &StbWebView::processFeaturePermissionRequest); //请求获取权限，则处理。
  connect(page(), &QWebEnginePage::printRequested, this, &StbWebView::printRequested); //request to print

  connect(customPage, &QWebEnginePage::fullScreenRequested, this, &StbWebView::processFullScreenRequest); //处理全屏请求
  connect(this, &StbWebView::loadFinished, this, &StbWebView::startGoogleAdsTimer); //载入完毕，则启动定时器，干掉谷歌广告

  connect(&googleAdsTimer, &QTimer::timeout, this, &StbWebView::runScriptRemoveGoogleAds); //定时器超时，则运行脚本，删除谷歌广告
  connect(&googleAdsTimer, &QTimer::timeout, this, &StbWebView::runScriptRemoveBaiduAds); //定时器超时，则运行脚本，删除百度广告
  connect(&googleAdsTimer, &QTimer::timeout, this, &StbWebView::runScriptRemoveNasaAds); //定时器超时，则运行脚本，删除航天局广告
  connect(&googleAdsTimer, &QTimer::timeout, this, &StbWebView::runScriptremoveHeaderByCloudTechnologyExperts); //定时器超时，则运行脚本，删除标题头

  // 连接信号
  connect(downloadMsgHandler, &DownloadMessageHandler::blobDataReady,
          this, &StbWebView::onBlobDataReady);

} //void StbWebView::connectSignals() //!<连接信号槽。

/*!
 * \brief StbWebView::runScriptremoveHeaderByCloudTechnologyExperts 定时器超时，则运行脚本，删除标题头
 */
void StbWebView::runScriptremoveHeaderByCloudTechnologyExperts()
{
    QFile removeBlmFileAdsByGoogle(":/Scripts/removeHeaderByCloudTechnologyExperts.js"); //删除黑命 贵的代码文件名

    removeBlmFileAdsByGoogle.open(QIODevice::ReadOnly);

    auto removeBlmScriptFileContentAdsByGoogle=removeBlmFileAdsByGoogle.readAll(); //全部读取

    removeBlmFileAdsByGoogle.close(); //关闭文件。


    auto removeBlmScriptCodeContentAdsByGoogle=QString::fromUtf8(removeBlmScriptFileContentAdsByGoogle); //转换成字符串


    customPage->runJavaScript(removeBlmScriptCodeContentAdsByGoogle); //运行脚本

} //void StbWebView::runScriptremoveHeaderByCloudTechnologyExperts()

/*!
 * \brief StbWebView::runScriptRemoveNasaAds 定时器超时，则运行脚本，删除航天局广告
 */
void StbWebView::runScriptRemoveNasaAds()
{
    QFile removeBlmFileAdsByGoogle(":/Scripts/removeAdsByNasa.js"); //删除黑命 贵的代码文件名

    removeBlmFileAdsByGoogle.open(QIODevice::ReadOnly);

    auto removeBlmScriptFileContentAdsByGoogle=removeBlmFileAdsByGoogle.readAll(); //全部读取

    removeBlmFileAdsByGoogle.close(); //关闭文件。


    auto removeBlmScriptCodeContentAdsByGoogle=QString::fromUtf8(removeBlmScriptFileContentAdsByGoogle); //转换成字符串


    customPage->runJavaScript(removeBlmScriptCodeContentAdsByGoogle); //运行脚本

} //void StbWebView::runScriptRemoveNasaAds()

/*!
 * \brief StbWebView::runScriptRemoveBaiduAds 定时器超时，则运行脚本，删除百度广告
 */
void StbWebView::runScriptRemoveBaiduAds()
{
    QFile removeBlmFileAdsByGoogle(":/Scripts/removeAdsByBAidu.zzaqvi.dll.sh.js"); //删除黑命 贵的代码文件名

    removeBlmFileAdsByGoogle.open(QIODevice::ReadOnly);

    auto removeBlmScriptFileContentAdsByGoogle=removeBlmFileAdsByGoogle.readAll(); //全部读取

    removeBlmFileAdsByGoogle.close(); //关闭文件。


    auto removeBlmScriptCodeContentAdsByGoogle=QString::fromUtf8(removeBlmScriptFileContentAdsByGoogle); //转换成字符串


    customPage->runJavaScript(removeBlmScriptCodeContentAdsByGoogle); //运行脚本

} //vvoid StbWebView::runScriptRemoveBaiduAds()

/*!
 * \brief StbWebView::runScriptRemoveGoogleAds 定时器超时，则运行脚本，删除谷歌广告
 */
void StbWebView::runScriptRemoveGoogleAds()
{
    QFile removeBlmFileAdsByGoogle(":/Scripts/removeAdsByGoogle.zzaqvi.dll.sh.js"); //删除黑命 贵的代码文件名

    removeBlmFileAdsByGoogle.open(QIODevice::ReadOnly);

    auto removeBlmScriptFileContentAdsByGoogle=removeBlmFileAdsByGoogle.readAll(); //全部读取

    removeBlmFileAdsByGoogle.close(); //关闭文件。


    auto removeBlmScriptCodeContentAdsByGoogle=QString::fromUtf8(removeBlmScriptFileContentAdsByGoogle); //转换成字符串


    customPage->runJavaScript(removeBlmScriptCodeContentAdsByGoogle); //运行脚本
} //void StbWebView::runScriptRemoveGoogleAds()

/*!
 * \brief StbWebView::startGoogleAdsTimer 载入完毕，则启动定时器，干掉谷歌广告
 */
void StbWebView::startGoogleAdsTimer()
{
    googleAdsTimer.start(); //启动定时器。
} //void StbWebView::startGoogleAdsTimer()

/*!
 * \brief StbWebView::processFullScreenRequest 处理全屏请求
 * \param requstObject 请求对象
 */
void StbWebView::processFullScreenRequest(QWebEngineFullScreenRequest requstObject)
{
    requstObject.accept(); //接受
} //void StbWebView::processFullScreenRequest(const QWebEngineFullScreenRequest & requstObject)

/*!
 * \brief StbWebView::requestPermissionMacOs Request permisison mac os.
 * \param feature The feature.
 */
void StbWebView::requestPermissionMacOs(const QWebEnginePage::Feature & feature, const QUrl & securityOrigin)
{
  (void)(feature);
  (void)(securityOrigin);

#ifdef Q_OS_MAC
  macOsPermissionRequestor.requestPermission(feature, customPage, securityOrigin); // Request permission.
#endif
} // void StbWebView::requestPermissionMacOs(const QWebEnginePage::Feature & feature)

/*!
 * \brief StbWebView::processFeaturePermissionRequest 请求获取权限，则处理。
 * \param securityOrigin 请求来源。
 * \param feature 请求的特性。
 */
void StbWebView::processFeaturePermissionRequest(const QUrl &securityOrigin, QWebEnginePage::Feature feature)
{

//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", url: " << securityOrigin.toString() << ", feature: " << feature; //报告错误码。

    QSet<QWebEnginePage::Feature> feautreSetNeedConfirm=
    {
      QWebEnginePage::Geolocation,
    };



//    QWebEnginePage::MediaAudioCapture2Audio capture devices, such as microphones.
//    QWebEnginePage::MediaVideoCapture3Video devices, such as cameras.
//    QWebEnginePage::MediaAudioVideoCapture4Both audio and video capture devices.

    QSet<QWebEnginePage::Feature> featureSetNeedAsync=
    {
          QWebEnginePage::MediaAudioCapture ,
          QWebEnginePage::MediaVideoCapture ,
          QWebEnginePage::MediaAudioVideoCapture ,

    };


    if (feautreSetNeedConfirm.contains(feature)) //Need explicit confirm
    {
        QMessageBox msgBox(this);
        msgBox.setText(tr("%1 wants to know your location").arg(securityOrigin.host()));
        msgBox.setInformativeText(tr("Do you want to send your current location to this website?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);

    //    this->page=page;
    //    this->securityOrigin=securityOrigin;
    //    this->feature=feature;

    //          QTimer::singleShot(200, this, SLOT(updateCaption()));

            if (msgBox.exec() == QMessageBox::Yes) {
    //            updateCaption();
    //    sleep(2);

            customPage->setFeaturePermission(
                securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
            } else {
                customPage->setFeaturePermission(
                    securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
            }

    } //if (feautreSetNeedConfirm.contains(feature)) //Need explicit confirm
    else if (featureSetNeedAsync.contains(feature))
    {
      cout  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", url: " << QThread::currentThreadId() << ", feature: " << feature << endl; //报告错误码。

#ifdef Q_OS_MAC
      requestPermissionMacOs(feature, securityOrigin); // Request permisison mac os.
#else
      QWebEnginePage::PermissionPolicy policy=QWebEnginePage::PermissionGrantedByUser;
      customPage->setFeaturePermission(securityOrigin, feature, policy);

#endif

    } // else if (featureSetNeedAsync.contains(feature))
    else //if (feautreSetNeedConfirm.contains(feature)) //Need explicit confirm
    {
      cout  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", url: " << securityOrigin.toString().toStdString() << ", feature: " << feature << endl; //报告错误码。


      QWebEnginePage::PermissionPolicy policy=QWebEnginePage::PermissionGrantedByUser;
      customPage->setFeaturePermission(securityOrigin, feature, policy);
    } //if (feautreSetNeedConfirm.contains(feature)) //Need explicit confirm
} //void StbWebView::processFeaturePermissionRequest(const QUrl &securityOrigin, QWebEnginePage::Feature feature)

///*!
// * \brief StbWebView::downloadRequested 处理下载请求。
// * \param request 下载请求。
// */
//void StbWebView::downloadRequested(const QNetworkRequest &request)
//{
//    sBrowserWindow->downloadManager()->download(request); //由下载管理器来处理。

//    emit downloadStarted(); //发射信号，开始了下载动作。
//} //void StbWebView::downloadRequested(const QNetworkRequest &request)

/*!
 * \brief StbWebView::showStatusBarMessage 要求显示状态栏消息，则显示。
 * \param msg2Show 要显示的消息内容。
 */
void StbWebView::showStatusBarMessage(QString msg2Show)
{
    emit shouldShowStatusBarMessage(msg2Show); //发送信号。
} //void StbWebView::showStatusBarMessage(QString msg2Show) //!<要求显示状态栏消息，则显示。

/*!
 * \brief StbWebView::showLinkTarget 鼠标悬停于链接上，则显示链接目标。
 * \param tgt2Show 要显示的链接目标。
 */
void StbWebView::showLinkTarget(QString tgt2Show)
{
    QString tgt2ShowDecoded=QUrl::fromPercentEncoding (tgt2Show.toUtf8 ()); //对百分号编码进行解码。
    emit shouldShowStatusBarMessage(tgt2ShowDecoded); //发送信号。
} //void StbWebView::showLinkTarget(QString tgt2Show) //!<鼠标悬停于链接上，则显示链接目标。

/*!
 * \brief StbWebView::mouseReleaseEvent 鼠标释放事件。
 * \param event 事件。
 */
void StbWebView::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button ()== Qt::BackButton) //是后退按钮。
    {
        triggerPageAction (QWebEnginePage::Back); //后退。
    } //if (event->button ()== Qt::BackButton) //是后退按钮。
    else if (event->button ()==Qt::ForwardButton) //是前进按钮。
    {
        triggerPageAction (QWebEnginePage::Forward); //前进。
    } //else if (event->button ()==Qt::ForwardButton) //是前进按钮。
    else //其它按钮。
    {
        QWebEngineView::mouseReleaseEvent (event); //让超类来处理。
    } //else //其它按钮。

    return;
} //void SWebPageWidget::mouseReleaseEvent(QMouseEvent * event)

/*!
 * \brief StbWebView::zoom 直接缩放到指定比例。
 * \param zoomFactor 缩放比例。
 */
void StbWebView::zoom(const qreal zoomFactor)
{
    setZoomFactor(zoomFactor); //指定缩放比例。
    
    emit zoomFactorChanged(zoomFactor); //发射信号，缩放比例发生变化。
} //void StbWebView::zoom(const qreal zoomFactor)

/*!
 * \brief StbWebView::zoomRestore 恢复缩放比例。
 */
void StbWebView::zoomRestore()
{
    qreal newZoomFactor=1; //默认的缩放因子。


    setZoomFactor (newZoomFactor); //设置新的缩放因子。

    emit zoomFactorChanged(zoomFactor()); //发射信号，缩放比例发生变化。

    return;
} //void StbWebView::zoomRestore()

/*!
 * \brief StbWebView::zoomOut 缩小。
 */
void StbWebView::zoomOut()
{
    qreal newZoomFactor=zoomFactor(); //获取当前的缩放因子。

    newZoomFactor/=1.1; //减小缩放因子。

    setZoomFactor (newZoomFactor); //设置新的缩放因子。


    emit zoomFactorChanged(zoomFactor()); //发射信号，缩放比例发生变化。

    return;
} //void StbWebView::zoomOut()

/*!
 * \brief StbWebView::zoomIn 放大。
 */
void StbWebView::zoomIn()
{

    qreal newZoomFactor=zoomFactor(); //获取当前的缩放因子。

    newZoomFactor*=1.1; //增加缩放因子。

    setZoomFactor (newZoomFactor); //设置新的缩放因子。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Zoom factor:") << zoomFactor(); //Debug.

    emit zoomFactorChanged(zoomFactor()); //发射信号，缩放比例发生变化。

    return;
} //void StbWebView::zoomIn()

/*!
 * \brief SWebPageWidget::wheelEvent 滚轮事件。
 * \param event 事件对象。
 */
void StbWebView::wheelEvent(QWheelEvent * event)
{
    QPoint numDegrees=event->angleDelta ()/8; //获取滚动的角度。

    if (numDegrees.isNull ()) //未滚动。
    {
        QWebEngineView::wheelEvent (event); //超类处理。
    } //if (numDegrees.isNull ()) //未滚动。
    else //滚动了。
    {
        Qt::KeyboardModifiers modifiers=event->modifiers (); //获取键盘修饰键。

        if ((modifiers & Qt::ControlModifier )==(Qt::ControlModifier)) //按住了Ctrl键。
        {
            if (numDegrees.y ()>0) //正向滚动，放大。
            {
                zoomIn (); //放大。
            } //if (numDegrees.y ()>0) //正向滚动，放大。
            else //缩小。
            {
                zoomOut (); //缩小。
            } //else //缩小。

            event->accept (); //消化事件。
        } //if (modifiers&& Qt::ControlModifier ) //按住了Ctrl键。
        else //未按住Ctrl键。
        {
            QWebEngineView::wheelEvent (event); //超类处理。
        } //else //未按住Ctrl键。

    } //else //滚动了。

    return;
} //void SWebPageWidget::wheelEvent(QWheelEvent * event)

/*!
 * \brief StbWebView::childEvent 子代对象事件。
 * \param event 事件对象。
 */
void StbWebView::childEvent(QChildEvent * event)
{
    if (event->type()==QEvent::ChildAdded) //是添加子代对象。
    {
        if (event->child ()->isWidgetType ()) //是部件。
        {
            event->child ()->installEventFilter (this); //安装事件过滤器。

            internalWheelHandleWidget=(QWidget*)(event->child ()); //记录内部的滚轮事件处理部件。
        } //if (event->child ()->isWidgetType ()) //是部件。
    } //if (event->type()==QEvent::ChildAdded) //是添加子代对象。
} //void StbWebView::childEvent(QChildEvent * event)

/*!
 * \brief StbWebView::eventFilter 事件过滤器。
 * \param watched 被监视的对象。
 * \param event 事件。
 * \return 是否消化。
 */
bool StbWebView::eventFilter(QObject * watched, QEvent * event)
{
    bool result=false; //结果。

    if (watched==internalWheelHandleWidget) //是内部的滚轮事件处理部件。
    {
        if (event->type ()==QEvent::Wheel) //是滚轮事件。
        {
            wheelEvent ((QWheelEvent*)event); //按照滚轮事件来处理。
        } //if (event->type ()==QEvent::Wheel) //是滚轮事件。
    } //if (watched==internalWheelHandleWidget) //是内部的滚轮事件处理部件。

    return result;
} //bool StbWebView::eventFilter(QObject * watched, QEvent * event)

/*!
 * \brief StbWebView::goBackTop 回到顶端。
 */
void StbWebView::goBackTop()
{
//    lightSpeedGoBackTop(); //以光速回到顶端。

//    eagleEyeGoBackTop(); //以鹰眼效果回到顶端。

    slideGoBackTop(); //以滑行效果回到顶端。

    return;
} //void StbWebView::goBackTop()

/*!
 * \brief StbWebView::lightSpeedGoBackTop 以光速回到顶端。
 */
void StbWebView::lightSpeedGoBackTop()
{
    //向自己发送Home键：
    QKeyEvent * homeKeyPress=new QKeyEvent(QEvent::KeyPress,Qt::Key_Home,Qt::NoModifier); //HOme键被按。

    qApp->postEvent (internalWheelHandleWidget,homeKeyPress); //Home键被按。

    QKeyEvent * homeKeyRelease=new QKeyEvent(QEvent::KeyRelease,Qt::Key_Home,Qt::NoModifier); //HOme键被松。

    qApp->postEvent (internalWheelHandleWidget,homeKeyRelease); //发送事件。


    return;
} //void StbWebView::lightSpeedGoBackTop()

/*!
 * \brief StbWebView::slideGoBackTop 以滑行效果回到顶端。
 */
void StbWebView::slideGoBackTop()
{
    queryClientHeightSlideGoBackTop (); //查询显示区高度。
} //void StbWebView::slideGoBackTop()

/*!
 * \brief StbWebView::queryClientHeightSlideGoBackTop 查询显示区高度。
 */
void StbWebView::queryClientHeightSlideGoBackTop ()
{
    QFile clientHeightScriptSourceFile(":/Scripts/scrollTop.js"); //获取滚动位置的文件。
    clientHeightScriptSourceFile.open (QIODevice::ReadOnly); //打开文件。
    QByteArray clientHeightScriptSourceByteArray=clientHeightScriptSourceFile.readAll(); //读取全部内容。
    clientHeightScriptSourceFile.close (); //关闭。

    QString clientHeightScriptSource=QString::fromUtf8 (clientHeightScriptSourceByteArray); //转换成字符串。

    page()->runJavaScript (clientHeightScriptSource, StartSlideGoBackTop(this)); //获取显示区高度和网页高度。
} //void StbWebView::queryClientHeightSlideGoBackTop ()

/*!
 * \brief StbWebView::startSlideGoBackTop 启动滑动动画过程。
 * \param clientTop 当前的clientTop值。
 */
void StbWebView::startSlideGoBackTop (float clientTop)
{
    QTimeLine *timeLine = new QTimeLine(800, this);

    timeLine->setDirection (QTimeLine::Backward);
    timeLine->setFrameRange (0,clientTop); //设置帧范围。
    timeLine->setEasingCurve(QEasingCurve::InOutExpo); //弹跳。

    connect(timeLine, &QTimeLine::frameChanged,this,&StbWebView::scrollByClientTop); //帧发生变化，则滚动。

    timeLine->start (); //启动时间线。
    
    


} //void StbWebView::startSlideGoBackTop (float clientTop)

/*!
 * \brief StbWebView::scrollByClientTop 按照clientTop滚动。
 * \param targetClientTop 目标clientTop值。
 */
void StbWebView::scrollByClientTop(int targetClientTop)
{
  QString scriptSource(QString("window.scroll(window.pageXOffset,%1);").arg (targetClientTop)); //脚本内容。

  page()->runJavaScript (scriptSource); //运行脚本。
} //void StbWebView::scrollByClientTop(int targetClientTop)


/*!
 * \brief StbWebView::initializeMemberObjects 初始化成员变量。
 */
void StbWebView::initializeMemberObjects()
{
  googleAdsTimer.setInterval(10000); //10秒运行一次

  customPage=new StbWebPage; //创建网页对象。

  setPage(customPage); //设置页面。

  iconUrlCallback=new IconUrlCallback;

  iconUrlCallback->setWebView(this); // Set web view pointer.

  videoUrlCallback=new VideoUrlCallback; // Create call back object.
  videoUrlCallback->setWebView(this); // Set web view pointer.

  // 在页面初始化时（比如 StbWebView 构造函数中）
  webChannel = new QWebChannel(this);
  downloadMsgHandler = new DownloadMessageHandler(this);

  // 注册消息处理器
  webChannel->registerObject("downloadMessageHandler", downloadMsgHandler);


  applyWebChannel(); // Apply web channel.

} //void StbWebView::initializeMemberObjects()

/*!
 * \brief StbWebView::applyWebChannel Apply web channel.
 */
void StbWebView::applyWebChannel()
{
    customPage->setWebChannel(webChannel);

    // ✅ 注入 WebChannel JS
    injectWebChannelScript();

} //void StbWebView::applyWebChannel()

void StbWebView::onBlobDataReady(const QByteArray &data, const QString &filename, const QString &url)
{
    cout << __FILE__ << "," << __LINE__ << "," << __func__
         << ", Got blob data, size=" << data.size() << ", filename=" << filename.toUtf8().data() << endl;

    // ✅ 数据到手！下一步：通知主进程，转发给下载器
    emit blobDownloadCompleted(data, filename); // 你可以定义这个信号
}

/**
 * @brief StbWebView::injectWebChannelScript
 * 注入 qwebchannel.js 到页面中，确保 qt.webChannelTransport 可用
 * 使用 Qt 内置资源: qrc:///qtwebchannel/qwebchannel.js
 */
void StbWebView::injectWebChannelScript()
{
    // 读取 Qt 内置的 qwebchannel.js

    // qrc:/Scripts/qwebchannel.js
    // :/Scripts/qwebchannel.js

    // QFile chFile("qrc:/qtwebchannel/qwebchannel.js");
    QFile chFile(":/qtwebchannel/qwebchannel.js");
    // QFile chFile("qrc:/Scripts/qwebchannel.js");


    if (!chFile.open(QIODevice::ReadOnly)) {
        cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this:" << this << ", Failed to open qrc:///qtwebchannel/qwebchannel.js " << endl; //Debug.

        qWarning() << "Failed to open qrc:///qtwebchannel/qwebchannel.js";
        return;
    }

    // 读取 JS 内容
    QByteArray chData = chFile.readAll();
    chFile.close();

    QString chScript = QString::fromUtf8(chData);


       cout << __FILE__ << "," << __LINE__ << "," << __func__ << ",this:" << this << ", webchannel js content: " << chScript.toStdString () << endl; //Debug.


    // 创建注入脚本
    QWebEngineScript script;
    script.setName("qt.webchannel");
    script.setSourceCode(chScript);
    script.setWorldId(QWebEngineScript::MainWorld);           // 主世界，可访问页面 JS
    script.setInjectionPoint(QWebEngineScript::DocumentReady); // DOM 就绪时注入
    script.setRunsOnSubFrames(false);

    // 插入脚本
    customPage->scripts().insert(script);

    qDebug() << "Injected qwebchannel.js, size =" << chData.size() << "bytes";
}

/*!
 * \brief StbWebView::destroyWebPage 销毁网页。
 */
void StbWebView::destroyWebPage()
{
    page()->triggerAction (QWebEnginePage::Stop); //停止网页活动。
} //void StbWebView::destroyWebPage()

/*!
 * \brief StbWebView::~StbWebView 析构函数。
 */
StbWebView::~StbWebView()
{
    if (customPage) //自定义页面存在。
    {
        delete customPage; //删除自定义页面。
    } //if (customPage) //自定义页面存在。
} //StbWebView::~StbWebView()
