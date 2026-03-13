#ifndef SBROWSERCPPWEBENGINE_H
#define SBROWSERCPPWEBENGINE_H

#include "SbPreCompile.h" //QMainWindow

class TabManager; //!< TabManager 的前向声明。
class AboutDialog; //!<AboutDialog的前向声明。
//class SWebPageWidget; //!<SWebPageWidget的前向聲明.


#include "StbSettingDialog.h" // StbSettingDialog
#include "StbProxyFactory.h" //StbProxyFactory
#include "DownloadManager.h" //DownloadManager
#include "FindDialog.h" //FindDialog
#include "SbTtsClient.h" //SbTtsClient
#include "AboutDialog.h" // AboutDialog
#include "SWebPageWidget.h" //SWebPageWidget
#include "TabManager.h" //QtDocChinese
//#include "Communicator.h" //GreeterClient

#include "QrBackgroundImageNormalizer.h" // QrBackgroundImageNormalizer
#include "SbAdblockInterceptor.h" //SbAdblockInterceptor
#include "SearchEngineManager.h" //SearchEngineManager
#include "rep_SetDirectionService_replica.h" //SetDirectionServiceReplica
#include "Client.h" //Client
#include "QrCodeDialog.h" //QrCodeDialog
#include "DownloaderType.h" // DownloaderType
#include "UserScriptManager.h" //UserScriptManager
#include "MessageFactory.h" // MessageFactory
#include "downloadmessagehandler.h" // DownloadMessageHandler

namespace Ui
{
    class SBrowserCppWebEngine;
} //namespace Ui


/*!
 * \brief The SBrowserWindow class 浏览器主窗口类。
 */
class SBrowserCppWebEngine : public QMainWindow
{
  Q_OBJECT

protected:
  void changeEvent(QEvent *e);

private slots:
  void showTabOnLeftSide(); //!< Show the tab on left side.
  void showTabOnRightSide(); //!< Show the tab on Right side.
  void openWithSystemBrowser(); //!< 使用系统浏览器打开.陈欣
  void paste(); //!<粘贴。
  void generateQrCode(); //!<按了二维码按钮，则生成二维码。
  void focusAddressBar(); //!<焦点定位到地址栏。
  void inspect(); //!<分析网页。
  void exportAsPdf(); //!<导出成PDF。
  void zoomRestore(); //!<恢复缩放比例。
  void zoomOut(); //!<页面缩小。
  void zoomIn(); //!< 页面放大。
  void downloadCurrentUrl(); //!<下载当前网址内容。
  void downloadCurrentUrlUniversal(); //!<Download current url use universal method
  void downloadCurrentUrlDownloadManager(); //!<Use downlaod manager. Download current url
  void downloadAllImages(); //!< Download all images. Chen xin
  void downloadAllMedium(); //!< Download all medium.
  void showTabsManager(); //!<显示标签页管理器。
  void refresh(); //!<刷新。
  void batchOpenUrls(); //!<批量打開網址.
  void toggleDarkMode(bool useDarkMode); //!<切换暗夜模式。
  void toggleStatusBarVisible(bool visible); //!<切换状态栏的可见性。
  void toggleNavigationBarVisible(bool visible); //!<切换导航栏的可见性。
  void toggleTabBarVisible(bool visible); //!<切换标签栏的可见性。
  void toggleMenuBarVisable(bool visible); //!<切换菜单栏的可见性。
  void qrCollectBgImage(QString flNm); //!<下载完毕，则为二维码收集背景图片。
  void ttsSayDownloadError(); //!<下载出错，则语音报告。
  void add1LoadedTab(); //!<定时器超时，则添加一个被载入的标签页。
  void ParseCheckNewVersionResult(); //!<检查更新完成之后，解析检查更新的结果。
  void showAboutDialog(); //!<显示出关于对话框。
  void ttsSayDownloadStarted(); //!< 语音报告下载开始。
  void toastDownloadStarted(); //!<下载开始，则吐息报告。
  void ttsSayDownloadFinished(); //!<语音报告下载完毕。
  void ttsSayLoadFinished(); //!<通过TTS报告载入完成。
  void selectAll(); //!<全选。
  void applyWebChannel();
  void showDownloadManager(); //!<显示下载管理器。
  void findText(); //!<查找对话框确认关闭，则查找文字。
  void findTextPrevious(); //!< 查找上一个匹配项（Shift+F3）
  void showFindDialog(); //!<显示查找对话框。
  void toggleProxyRule(bool enb,QString hstNm); //!<要求切换智能代理规则，则切换。
  void showSettingDialog(); //!<显示设置对话框。

public slots:
  void onBlobDownloadCompleted(const QByteArray &data, const QString &filename);
  void quit(); //!<退出。
  void closeSettingDialog(); //!<关闭设置对话框。
  void startLoad(); //!<开始载入。
  void saveGeometry(); //!< 保存几何属性。
  void saveSessionStore();  //!<保存会话数据。
  void openNewTab(QString url2Open); //!<打开新的标签页。

  void changeTabText(QString); //!<修改标签页的文字内容。
  void refreshWindowTitle(const int & currentWidgetIndex); //!< 更新窗口标题。
  void closeTab(const int & tabIndex); //!< 关闭标签页。
  void changeTabIcon(QIcon); //!<修改标签页的图标。
  void maybe1stTimeLoadPage(int); //!<可能是第一次载入该标签页。

private:

  bool switchToTabByUrl(const QString  & clipBoardText); //!< Switched to the tab of this url
  QSet<QString> clipboardUrlSet; //!< The set of clipboard url.
  void killRenderProcesses(); //!< 杀死所有的渲染进程。
  void batchDownloadImageUrl(const QVariant &iconUrl); //!< Remember icon url
  void disconnectSignals(); //!< Diconnect signals. 陈欣.
  bool infactSameUrl(const QString & url2Open, const QString & rightMostUrl) const; //!< The right url is in fact the same to the left url. 陈欣
  MessageFactory messageFactory; //!< 消息构造工厂.陈欣
  QTimer selfQuitTimer; //!<hte self quit timer.
  UserScriptManager * userScriptManager=nullptr; //!<用户脚本管理器
  QString downloaderType=DownloaderType::SuperDownloader; //!<下载器类型。
  bool quitIdle10MinutesbGl=true; //!< 是否 quit when idle for 10 minutes.
  bool pauseMediaWhenLostFocusesbGl=true; //!< 失去焦点时暂停多媒体播放。
  void saveZoomFactorMapCbor(); //!< Save the zoom factor map. 陈欣
  void bindExtraShortcuts(); //!<绑定额外的快捷键。
  void assessRestartSpeakerSubSystem(int exitCode, QProcess::ExitStatus exitStatus); //!<扬声器子系统退出，则考虑是否要重新启动进程。
  QrCodeDialog qrCodeDialog; //!< 二维码生成对话框。
  void startRotatePredictor(); //!<启动旋转预测器。
  QString userAgent; //!<用户代理字符串。
  QString imageFileName; //!<要显示的图片文件名。
  QProcess  rotatePredictorProcess; //!<旋转预测器进程。
  Client * rswitch; //!<远程进程调用客户端。
  QSharedPointer<SetDirectionServiceReplica> ptr;
  QRemoteObjectNode repNode;
  QTimer launchTimer; //!<启动各个子进程的定时器。
  void launchSbrowserDownloader(); //!<启动定时器超时，则启动下载器进程。
  void selectAudioInputDeviceByName(QString smartProxyHost); //!<按照名字选择对应的声音输入设备。
  void saveSetting(); //!<设置对话框确认关闭，则保存设置信息。

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
  MemoryMonitor memoryMonitor; //!<内存状态监视器。
  StopListener stopListener; //!<急停监听器。
#endif
#endif

  void installRequestInterceptor(); //!<安装请求干涉器。
  void fixEnvironmentVariableForNss(); //!<修复与nss相关的环境变量问题。
  void debugEnvironmentVariable(); //!<Debug.
  void closeRequestedTab(); //!<要求关闭窗口，则关闭发起要求的标签页。
  QrBackgroundImageNormalizer qrBackgroundImageNormalizer; //!< 二维码背景图片归一化处理器。
  QNetworkRequest * checkNewVersionRequest; //!<用于检查新版本的请求对象。
  void loadCommandLineUrl(); //!<载入通过命令行传递的网址。
  void switchTab(SWebPageWidget * tabToSwitch); //!<切换标签页。
  TabManager * tabsManager; //!<标签页管理器。
  void goBackTop(); //!<回到顶端。
  QToolButton * backToTopToolButton; //!<用来回到顶端的按钮。
  void initializeStatusBar(); //!<初始化状态栏。
  void initializeUi(); //!<对界面做一些初始化。
  QTimer * ldTimer; //!<驱动载入过程的定时器。
  SearchEngineManager searchEngineManager; //!<搜索引擎管理器。
  void handleDownloadRequest(QWebEngineDownloadRequest * downloadItem); //!< 处理下载请求。
  void syncSetting(); //!<设置对话框确认关闭，则 Sync 设置信息。陈欣
  void showAnimationGoBackTop(); //!<显示动画效果，回到顶端。
  void enableCache(); //!<启用缓存。
  void closeCurrentTab(); //!<关闭当前标签页。
  void addActions(); //!<添加动作。
  QStringList sslTrustedHostList; //!<受信任的主机列表。
  QString selectedLanguage; //!<选择的语言。
  void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &error); //!<处理SSL错误。
  void loadTranslation(QString stngDlggetSelectedLanguage); //!<载入对应语言的翻译。
  void handleAuthentication(QNetworkReply  *  reply ,  QAuthenticator  *  authenticator); //!<需要认证，则处理认证问题。
  void updateTabAmount(); //!<更新标签页个数。
  QLabel * tabAmountLabel; //!< 标签页个数文字标签。
  void initializeWebSettings(); //!<初始化网页设置。
  QString indexPageUrl="http://stupidbeauty.com"; //!<首页网址。
  SWebPageWidget * createWebPageWidget(); //!<創建一個頁面部件.
  QString normalApplicationStyleSheet; //!<普通状态下（非暗夜模式）的应用程序样式单。
  QGraphicsColorizeEffect * darkEffect; //!< 暗夜效果。
  void refreshWindowIcon(const int & tabIndex); //!< 当前标签页改变，则相应地刷新窗口图标。
  void refreshZoomFactor(int tabIndex); //!<当前标签页改变，则相应地刷新缩放比例。
  void syncZoomFactor(const int tabIndex); //!<当前标签页改变，则相应地同步缩放比例。
  void syncZoomFactor(const QString newUrl); //!<当前标签页网址改变，则相应地同步缩放比例。
  void syncZoomFactor(const QString url, SWebPageWidget *crtPgWdgt); //!<同步指定标签页的缩放比例。
  void rememberZoomFactor(const int tabIndex); //!<记录缩放比例。
  void rememberZoomFactor(const qreal tabIndex); //!<记录缩放比例。
  void rememberZoomFactor(const SWebPageWidget * crtPgWdgt); //!<记住缩放比例
  QMap<QString, qreal> hostNameZoomFactorMap; //!<主机名与缩放比例之间的映射。
  void     loadHostnameZoomFactorMapCbor(); //!<载入host name zoom factor map。 陈欣
  void refreshZoomFactor(qreal newZoomFactor); //!<当前标签页的缩放比例发生改变，则相应地刷新缩放比例。
  void initializeMemberObjects(); //!<初始化成员对象。
  QMenu TrayMn; //!<托盘图标的菜单。
  void AsesShow(QSystemTrayIcon::ActivationReason ActRsn); //!<点击左键时，显示主窗口。
  QSystemTrayIcon TrayIcn; //!<托盘图标。
  void showTray(); //!<显示托盘图标。
  QStack<QString> closedTabUrlStack; //!<被关闭的标签页URL组成的栈。
  void undoCloseTab(); //!<撤销按钮被按，则撤销刚才被关闭的标签页。
  bool sessionStoreLoadFinished=false; //!<是否已经完成了会话信息的载入。
  void collectToQrBgImageSet(QString flNm); //!<将此图片收集到背景图片集合中。
  QString recognizeQrCode(QImage qrCdImgWthBgImg); //!<识别该图片中的二维码。
  QQueue<QString> loadedTabUrlQueue; //!<载入的标签页的URL字符串组成的队列。
  int addTabCtr=0; //!<已添加的标签页数量的计数器。
  QQueue<QString> loadedTabTitleQueue; //!<载入的标签页的标题字符串组成的队列。
  QTimer addTabTimer; //!< 用于添加各个标签页的定时器。
  void determinBitAmount(); //!<确定CPU位数。
  bool isMagicLinux(); //!<是否是运行于MagicLinux.
  void determinPlatformNameGnuLinux(); //!<确定具体的GNU/Linux发行版名字。
  void determinePlatformName(); //!<确定平台名字。
  quint32 bitAmount=64; //!<平台的CPU位数。
  QString platformName="magiclinux"; //!<平台名字。
  QString currentVersionNumber="2014.6.28"; //!<当前的版本号。
  QProcess  downloaderProcess; //!<扬声器子系统进程。
  QString RotatePredictorAptCmd="sbrowserdownloader"; //!<扬声器子系统命令行。
  QString RotateRotatePredictorAptCmd="simageviewrotatepredictor"; //!<旋转预测程序命令行。
  void checkNewVersion(); //!<检查是否有新版本。
  AboutDialog * mAboutDialog; //!< 关于对话框。
  void ttsSayLaunchFinished(); //!<通过语音服务器告知启动完毕。
  SbTtsClient ttsClient; //!<TTS客户端。
  FindDialog findDialog; //!<查找对话框。
  static DownloadManager *s_downloadManager; //!<下载管理器。
  void createCookieJar(); //!<创建小甜饼罐。
  void createNetworkManager(); //!<创建网络管理器。
  QNetworkAccessManager ntAccsMngr4WebPages; //!<用于网页的网络访问管理器。
  StbProxyFactory * prxyFctry; //!<代理工厂对象。
  void loadSetting(); //!< 载入设置信息。
  void connectSignals(); //!<连接信号。
  StbSettingDialog stngDlg; //!<设置对话框。
  QTimer * svSsnTmr; //!< 用于定时保存会话信息的定时器。
  void loadFromOwnSessionStore(); //!<从自己的会话记录文件中载入会话信息。
  void   loadFromFirefoxSessionStore(); //!<从火狐的会话记录文件中载入会话信息。
  void  loadSessionStore(QString ssnStrJs); //!<从指定文件中载入会话信息。
  void loadGeometry(); //!< 载入几何属性。
  QTabWidget * pgTbs; //!<用于容纳各个网页的标签页部件。
  Ui::SBrowserCppWebEngine * u; //!<用户界面。

public :
    void checkClipboardUrl(); //!< Check clipboard url
    void pauseVideo(); //!< Pause video when lost focus. Chen xin.
    void quitForIdle(); //!< Quit when idle.
    void destroyWebPages(); //!<销毁所有网页。
    ~SBrowserCppWebEngine(); //!<析构函数。
    DownloadManager * downloadManager(); //!<获取下载管理器指针。
    QWebEngineView *  createWindow( QWebEnginePage::WebWindowType wndTp); //!<创建新窗口。
    SBrowserCppWebEngine(QWidget * parent=nullptr); //!<构造函数。

signals:
  void restoreClosedTabToolTipChanged(QString trRestoretabcurrentUrl); //!<信号，恢复标签页按钮的提示文字发生变化。
  void closedTabUrlStackIsEmpty(); //!<信号，关闭的标签页栈为空。
  void closedTabUrlStackIsNotEmpty (); //!<信号，已关闭的标签页栈，不再为空。
}; //class SBrowserWindow : QMainWindow

#endif // PRECOMPILE_H
