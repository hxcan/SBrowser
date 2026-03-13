#ifndef SBROWSERDOWNLOADER_H
#define SBROWSERDOWNLOADER_H


#include "SbPreCompile.h" //QMainWindow

//class TabManager; //!<QtDocChinese的前向声明。
class AboutDialog; //!<AboutDialog的前向声明。
class SWebPageWidget; //!<SWebPageWidget的前向聲明.


#include "StbProxyFactory.h" //StbProxyFactory
#include "DownloadManager.h" //DownloadManager
//#include "StopListener.h" //StopListener
//#include "MemoryMonitor.h" //MemoryMonitor
#include "SetDirectionService.h" //SetDirectionService

/*!
 * \brief The SBrowserWindow class 浏览器主窗口类。
 */
class SBrowserDownloader : public QObject
{
    Q_OBJECT

private slots:
    void quit(); //!<退出。
    void qrCollectBgImage(QString flNm); //!<下载完毕，则为二维码收集背景图片。
    void closeDownloadManager(); //!<关闭下载管理器。
    void showDownloadManager(); //!<显示下载管理器。
    void toggleProxyRule(bool enb,QString hstNm); //!<要求切换智能代理规则，则切换。

private :
    QSet<QString> downloadUrlSet; //!< Already downloading url set. 陈欣
    void addDownloadTask(const QString & requestUrl, bool requestFileName); //!<添加下载任务。
    void addDownloadTaskFromData(const QByteArray &data, const QString &suggestedFilename); // ✅ 新增

    SetDirectionService srcSwitch; //!<
    QRemoteObjectHost * srcNode;

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    MemoryMonitor memoryMonitor; //!<内存状态监视器。
    StopListener stopListener; //!<急停监听器。
#endif
#endif

    // ✅ 新增：生成唯一保存路径
    QString generateSavePath(const QString &suggestedName);
    QNetworkRequest * checkNewVersionRequest; //!<用于检查新版本的请求对象。
    QStringList sslTrustedHostList; //!<受信任的主机列表。
    QString selectedLanguage; //!<选择的语言。
    QLabel * tabAmountLabel; //!<标签页个数文字标签。
    QString indexPageUrl="http://stupidbeauty.com"; //!<首页网址。
    void initializeMemberObjects(); //!<初始化成员对象。
    bool sessionStoreLoadFinished=false; //!<是否已经完成了会话信息的载入。
    void collectToQrBgImageSet(QString flNm); //!<将此图片收集到背景图片集合中。
    int addTabCtr=0; //!<已添加的标签页数量的计数器。
    void determinBitAmount(); //!<确定CPU位数。
    bool isMagicLinux(); //!<是否是运行于MagicLinux.
    quint32 bitAmount=64; //!<平台的CPU位数。
    QString platformName="magiclinux"; //!<平台名字。
    QString currentVersionNumber="2014.6.28"; //!<当前的版本号。
    void checkNewVersion(); //!<检查是否有新版本。
    static DownloadManager *s_downloadManager; //!<下载管理器。
    void createCookieJar(); //!<创建小甜饼罐。
    void createNetworkManager(); //!<创建网络管理器。
    QNetworkAccessManager ntAccsMngr4WebPages; //!<用于网页的网络访问管理器。
    StbProxyFactory * prxyFctry; //!<代理工厂对象。
    void loadSetting(); //!<载入设置信息。
    void connectSignals(); //!<连接信号。

public :
    ~SBrowserDownloader(); //!<析构函数。
    DownloadManager * downloadManager(); //!<获取下载管理器指针。
    SBrowserDownloader(QWidget * parent=0); //!<构造函数。

signals:
    void restoreClosedTabToolTipChanged(QString trRestoretabcurrentUrl); //!<信号，恢复标签页按钮的提示文字发生变化。
    void closedTabUrlStackIsEmpty(); //!<信号，关闭的标签页栈为空。
    void closedTabUrlStackIsNotEmpty (); //!<信号，已关闭的标签页栈，不再为空。
}; //class SBrowserWindow : QMainWindow

#endif // PRECOMPILE_H
