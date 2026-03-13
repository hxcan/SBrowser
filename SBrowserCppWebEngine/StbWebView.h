#ifndef STBWEBVIEW_H
#define STBWEBVIEW_H

#include "SbPreCompile.h" //QWebEngineView

#include "SWebPageWidget.h" //SWebPageWidget
#include "SBrowserCppWebEngine.h" // SBrowserCppWebEngine
#include "IconUrlCallback.h" // IconUrlCallback
#include "VideoUrlCallback.h" // VideoUrlCallback
#include "downloadmessagehandler.h" // DownloadMessageHandler

#ifdef Q_OS_MAC
#include "MacOsPermissionRequestor.h" // MacOsPermissionRequestor
#endif

class SWebPageWidget; //!<SWebPageWidget的前向声明。
class SBrowserCppWebEngine; //!< SBrowserCppWebEngine 的前向聲明.

QT_FORWARD_DECLARE_CLASS(IconUrlCallback)
QT_FORWARD_DECLARE_CLASS(VideoUrlCallback)

/*!
 * \brief The StbWebView class 网页视图类。
 */
class StbWebView : public QWebEngineView
{
  Q_OBJECT

protected:
  bool eventFilter(QObject*watched,QEvent*event); //!< 事件 過濾器。
  void childEvent(QChildEvent * event); //!<子代对象事件。
  void wheelEvent(QWheelEvent * event); //!<滚轮事件。
  void mouseReleaseEvent(QMouseEvent * event); //!<鼠标释放事件。

private slots:
  void onBlobDataReady(const QByteArray &data, const QString &filename, const QString &url);

  void showLinkTarget(QString tgt2Show); //!<鼠标悬停于链接上，则显示链接目标。
  void showStatusBarMessage(QString msg2Show); //!<要求显示状态栏消息，则显示。

public slots:
  /**
     * @brief handleBlobDownload 处理 blob: URL 下载
     * @param blobUrl blob URL 字符串
     * @param suggestedFilename 建议的文件名
     */
  void handleBlobDownload(const QString &blobUrl, const QString &suggestedFilename);
  void selectAll(); //!<选中全部内容。

public:
  void applyWebChannel(); //!< Apply web channel.

  void killRenderProcess(); //!< 杀死所有的渲染进程。
  void pauseVideo(); //!< Pause video when lost focus. Chen xin.
  void extractMediaUrls(); //!< Extract media urls.
  void extractImageUrls(); //!< Extract image urls.
  void downloadCurrentUrl(); //!<Start download current url.
  void inspect(); //!<分析网页。
  void exportAsPdf(QString saveFileName); //!<保存网页为PDF。
  void paste(); //!<粘贴。
  void zoomRestore(); //!<恢复缩放比例。
  void zoom(const qreal zoomFactor); //!<直接缩放到指定比例。
  void zoomOut(); //!<缩小。
  void zoomIn(); //!< 放大。
  void destroyWebPage(); //!<销毁网页。
  void startSlideGoBackTop (float clientTop); //!<启动滑动动画过程。
  void goBackTop(); //!<回到顶端。
  QWebEngineView * createWindow(QWebEnginePage::WebWindowType wndTp); //!< 创建新窗口。
  StbWebView(QWebEngineView * parent=0); //!<构造函数。
  void setWebPageWidget(SWebPageWidget * wbPgWdgt2St); //!<设置网页部件对象的指针。
  ~StbWebView(); //!<析构函数。

private:

#ifdef Q_OS_MAC
  MacOsPermissionRequestor macOsPermissionRequestor; //!< Mac os permission requestor.
#endif

  DownloadMessageHandler *downloadMsgHandler;
  QWebChannel *webChannel = nullptr;
  void injectWebChannelScript();
  void requestPermissionMacOs(const QWebEnginePage::Feature & feature, const QUrl &securityOrigin); //!< Request permisison mac os.
  IconUrlCallback * iconUrlCallback=nullptr; //!< Icon url callback. Chen xin.
  VideoUrlCallback * videoUrlCallback=nullptr; //!< Video url callback.
  void runScriptRemoveGoogleAds(); //!<定时器超时，则运行脚本，删除谷歌广告
  void runScriptRemoveBaiduAds(); //!<定时器超时，则运行脚本，删除百度广告
  void runScriptRemoveNasaAds(); //!<定时器超时，则运行脚本，删除航天局广告
  void runScriptremoveHeaderByCloudTechnologyExperts(); //!<定时器超时，则运行脚本，删除标题头
  QTimer googleAdsTimer; //!<干掉谷歌广告的定时器。
  void startGoogleAdsTimer(); //!< 载入完毕，则启动定时器，干掉谷歌广告
  void processFeaturePermissionRequest(const QUrl &securityOrigin, QWebEnginePage::Feature feature); //请求获取权限，则处理。
  void processFullScreenRequest(QWebEngineFullScreenRequest requstObject); //!<处理全屏请求
  QPointer<QWebEnginePage > customPage; //!<自定义的页面对象。
  void initializeMemberObjects(); //!< 初始化成员变量。
  void scrollByClientTop(int targetClientTop); //!<按照clientTop滚动。
  void queryClientHeightSlideGoBackTop (); //!<查询显示区高度。
  void slideGoBackTop(); //!<以滑行效果回到顶端。
  void lightSpeedGoBackTop(); //!<以光速回到顶端。
    QWidget * internalWheelHandleWidget; //!<内部的滚轮事件处理部件。
    SBrowserCppWebEngine * sBrowserWindow; //!<浏览器主窗口。
    void connectSignals(); //!<连接信号槽。
    SWebPageWidget * wbPgWdgt; //!<网页部件的指针。

signals:
    void blobDownloadCompleted(const QByteArray &data, const QString &suggestedFilename);

    void iconUrlFound(const QVariant & iconUrl) const; //!< Found icon url.
    void videoUrlFound(const QVariant & iconUrl) const; //!< Found video url.
    void windowCloseRequested(); //!<要求关闭窗口。
    void printRequested(); //!<Request to print
    void zoomFactorChanged(qreal newZoomFactor); //!<信号，缩放比例发生变化。
    void downloadStarted(); //!<信号，开始了下载动作。
    void shouldShowStatusBarMessage(QString msg2Show); //!<信号。应当显示状态栏消息。
}; //class StbWebView : public QWebView

#endif // PRECOMPILE_H
