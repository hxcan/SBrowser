#ifndef SWEBPAGEWIDGET_H
#define SWEBPAGEWIDGET_H

#include "SbPreCompile.h" //QLineEdit

#include "SBrowserCppWebEngine.h" //SBrowserWindow
#include "StbWebView.h" //StbWebView
#include "QrCodeDialog.h" //QrCodeDialog

class StbWebView; //!<StbWebView的前向声明。

class SWebPageWidget :public QWidget
{
  Q_OBJECT

protected:
  void mouseReleaseEvent(QMouseEvent * event); //!<鼠标释放事件。
  bool eventFilter (QObject * eventTarget, QEvent * event); //!<事件过滤器。

private slots:
  void parseSearchEngineAutoCompleteResult(); //!<自动补全请求完成之後，處理其結果。
  void searchEngineSearch(); //!<按回车键，则开始搜索。
  void showStatusBarMessage(QString msg2Show); //!<要求显示状态栏消息，则显示。
  void reportProxyStatus(); //!<开始载入，则报告代理状态。
  void toggleSmartProxy(bool isCkd); //!<按钮状态被切换，则相应地切换智能代理状态。

private :
  QVBoxLayout * vLyt= nullptr; //!<竖直布局器。
    void initializeNavigationBar(); //!< Initialize the navigation bar.
  bool m_uiSetupDone = false;
  int progress=0; //!<Load progress
  QTimer reloadTimer; //!<
  bool handleTextContentLineEditEvent(QEvent * event); //!<处理文本内容输入框的事件。
  void installEventFilters(); //!<安装事件过滤器。
  QMenu * reloadMenu; //!<重新载入的菜单。
  QString title; //!<当前标签页的标题。
  void addReloadToolButton(); //!<添加重新载入按钮。
  QString constructHashTags(); //!<构造标记列表字符串。
  void requestOpenNewTab(); //!<按钮被单击，则请求新建空白标签页。
  QPushButton * nwTbBtn; //!<打开新标签页的按钮。
  void openTwitterShareTab(); //!<按了推特按钮，则打开推特分享标签。
  void connectSignals(); //!<连接信号与信号槽。
  void setupUi(); //!<初始化界面。
  QNetworkAccessManager * ntAccsMngr; //!<网络访问管理器。
  void querySearchEngineAutoComplete(QString currentQueryText); //!<文字变化，则查询自动补全。
  QString filterForFileName(QString pageTitle); //!<过滤为可用的文件名。
  void focusSearchEngine(); //!<焦点定于搜索引擎。
  void addActions(); //!<添加动作。
  QFrame * navBarFrame; //!<导航栏框框。
  void addNavigationBar(QVBoxLayout * vLyt,QHBoxLayout * urlLyt); //!<添加导航栏。
  QHBoxLayout * urlLyt; //!<导航条所在的水平布局器。
  void undoCloseTab(); //!<撤销按钮被按，则撤销刚才被关闭的标签页。
  void addUndoCloseTabButton(QHBoxLayout * urlLyt); //!<向URL布局器中加入撤销刚关闭的标签页按钮。
  void addGlobalMenuButton(QHBoxLayout *urlLyt);
  void addNewTabButton(QHBoxLayout * urlLyt); //!<添加新建标签页按钮。
  void addSmartProxyButton(QHBoxLayout * urlLyt); //!<添加智能代码开关按钮。
  void addQrButton(QHBoxLayout * urlLyt); //!<添加二维码按钮。
  void addTwitterButton(QHBoxLayout * urlLyt); //!<加入推特分享按钮。
  QPointer<QGraphicsColorizeEffect> downloadStartedEffect; //!<下载开始效果。
  void notifyDownloadStarted(); //!<开始下载，则通知用户已经开始下载。
  void assessShowDownloadOnlyUrl(); //!<开始下载，则考虑是否要针对仅仅用来下载的标签页显示完整网
  void showDownloadManager(); //!<下载按钮被按，则显示下载管理器对话框。
  QPointer<QPushButton> downloadButton; //!<下载按钮。
  QPointer <QPushButton> undoCloseTabButton; //!<撤销关闭标签页按钮。
  QPointer <QPushButton> globalMenuButton ;
  void addDownloadButton(QHBoxLayout * urlLyt); //!<向URL布局器中加入下载按钮。
  QLineEdit * searchEngineEdt; //!<搜索引擎内容输入框。
  QToolBar * nvTlBr; //!<导航工具条。
  QrCodeDialog * qrCodeDialog=nullptr; //!<二维码对话框。
  void initValidSchemeSet(); //!<初始化有效的协议集合。
  QSet<QString> vldSchSet; //!<本浏览器所支持的协议集合。
  StbProxyFactory * prxyFctr=0; //!<代理工厂对象。
  QPushButton * smtPrxyBtn; //!<智能代理的开关按钮。
  QLineEdit * urlBr; //!<URL输入条。
  SBrowserCppWebEngine * brsrWnd; //!<浏览器窗口的指针。
  QPointer<StbWebView> wbPg; //!<网页视图对象。
  bool firstTimeLoad=true; //!<是否是第一次载入。

signals:
  void blobDownloadCompleted(const QByteArray &data, const QString &suggestedFilename);

  void shouldQuerySearchEngine(QString searchKeyWord); //!<信号，应当向搜索引擎查询。
  void windowCloseRequested(); //!<要求关闭窗口。
  void urlChanged(const QString newUrl) const; //!<网址发生改变。
  void zoomFactorChanged(qreal newZoomFactor); //!<信号，缩放比例发生变化。
  void shouldUndoCloseTab(); //!<信号，应当撤销被关闭的标签页。
  void shouldShowDownloadManager(); //!<信号，应当显示下载管理器。
  void loadFinished(); //!<信号，载入完毕。
  void toggleProxyRule(bool isCkd,QString hstNm); //!<信号，要求切换针对当前域名的智能代理规则。
  void shouldOpenNewTab(QString url2Open); //!<应当打开新标签页。
  void titleChanged(QString);
  void iconChanged(QIcon); //!<图标发生变化。
  void iconUrlFound(const QVariant & iconUrl) const; //!< Found icon url.
  void videoUrlFound(const QVariant & iconUrl) const; //!< Found video url.

public slots:
  void onBlobDownloadCompleted(const QByteArray & data,  const QString &filename);
  void generateQrCode(); //!<按了二维码按钮，则生成二维码。
  void gotoUrlBarUrl();
  void refreshUrl(QUrl);
  void indicateLoadStart();
  void indicateLoadProgress(int);
  void indicateLoadFinished(bool);
  void indiateIconChanged(); //!< 報告圖標發生變化。
  void focusWebView();
  
public:
  void handleBlobDownload(const QUrl &url, const QString &filename);
  void initializeDependencies(); // 新增的方法
  void killRenderProcess(); //!< 杀死所有的渲染进程。
  void pauseVideo(); //!< Pause video when lost focus. Chen xin.
  void extractMediaUrls(); //!< Extract media urls.
  void extractImageUrls(); //!< Extract image urls.
  void downloadCurrentUrl(); //!<Start download current url
  ~SWebPageWidget(); //!<析构函数。
  void checkClipboardChange(); //!<剪贴板发生变化，则检查变化。
  void paste(); //!<粘贴。
  void focusAddressBar(); //!<焦点定位到地址栏。
  qreal zoomFactor() const; //!<获取缩放比例。
  void zoomRestore(); //!<恢复缩放比例。
  void zoom(const qreal zoomFactor); //!<直接缩放到指定比例。
  void zoomOut(); //!<缩小。
  void zoomIn(); //!<放大。
  void setTitle(QString crtTtl); //!<设置标题。
  QString getTitle(); //!<获取标题。
  void goBackTop(); //!<回到顶端。
  void updateRestoreTabToolTip(QString newToolTip); //!<应当修改恢复标签页的提示文字。
  void refresh(); //!<刷新。
  void enableUndoClosedTab(); //!<启用恢复标签页按钮。
  void forbidUndoClosedTab(); //!<禁用恢复标签页按钮。
  void setNavigationBarVisible(bool visible); //!<设置可见性。
  void inspect(); //!< 分析网页。
  void exportAsPdf(); //!<导出成PDF。
  void selectAll(); //!<全选文字。
  void applyWebChannel();
  void addToolbarActions(); //!<向工具条加入动作。
  void findText(QString txt2Fnd, bool backward = false); //!<查找文字（支持反向查找）
  void setProxyFactory(StbProxyFactory * prxyFctry); //!<设置代理工厂。
  void setNetworkAccessManager(QNetworkAccessManager * ntAccsMngr4WebPages); //!<设置网络访问管理器的指针。
  SWebPageWidget(QWidget * parent=0); //!<构造函数。
  QWebEngineView *   createWindow(  QWebEnginePage::WebWindowType wndTp); //!<创建新的窗口。
  bool isFirstTimeLoad(); //!<是否是第一次载入。
  void startLoad(); //!<开始载入。
  void setFirstTimeLoad(bool ftl2St); //!<设置是否是第一次载入的标志位。
  void setBrowserWindow(SBrowserCppWebEngine * brsWnd2St); //!<设置浏览器主窗口的指针。
  QWebEngineView *  getWebView(); //!<获取此部件中的网页视图对象。
  void destroyWebPage(); //!<销毁网页页面。
  QString  getUrl() const; //!<获取URL。
  void setUrl(QString url2St); //!<设置URL。
  void setQrCodeDialog(QrCodeDialog *value);
}; //class SWebPageWidget :public QWidget

#endif // PRECOMPILE_H
