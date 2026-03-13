#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

//#include <QDialog>

#include "StbProxyFactory.h" //StbProxyFactory
#include "TunnelInfoDialog.h" //TunnelInfoDialog
#include "DownloaderType.h" //DownloaderType

namespace Ui
{
class StbSettingDialog;
} //namespace Ui

class StbSettingDialog : public QDialog
{
  Q_OBJECT

private slots:
  void browseDownloadDirectory(); //!< Browse download directory.
  void addCandidateTtsServerIp(); //!< Add candidate tts server ip.
  void addAdditionalProxy(); //!<添加额外的代理。
  void clearDiskCache(); //!<清除磁盘缓存。
  void testSelectIndex(); //!<试验选中当前标签页。
  void deleteAdblockRule(); //!<删除无用的广告禁用规则。
  void toggleAutoAddEnabled(bool enabled2Set); //!<切换自动添加代理规则复选框的可用性。
  void deleteNeedProxyWebsite(); //!< 删除一个需要通过代理访问的网站。
  void addAdBlockUrl(); //!<添加一个要封锁的URL。
  void addNeedProxyWebsite(); //!<添加一个需要通过代理访问的网站。
    
public:
  QString getSearchEngineName(); //!<获取搜索引擎名字。
  void selectSearchEngineByName(QString smartProxyHost); //!<按照名字来选中搜索引擎。
  void setSmartProxyHost(QString smartProxyHost); //!<设置智能代理的主机名。
  QString getSmartProxyHost(); //!<获取智能代理的主机名。
  bool getPluginsEnabled(); //!<是否启用插件。
  void setPluginsEnabled(bool enablePlugins); //!<设置是否启用插件。
  bool getEnableAdBlock(); //!<是否要启用广告禁用功能。
  void setEnableAdBlock(bool enableAdBlock); //!<是否启用广告禁用功能。
  void setSelectedLanguage(QString selectedLanguage); //!<设置，选择的语言。
  void setManuallySelectLanguage(bool manuallySelectLanguage); //!<设置，是否要手动选择语言。
  QString getSelectedLanguage(); //!<获取选择的语言。
  bool getManuallySelectLanguage(); //!<是否手动选择界面语言。
  void setIndexPage(QString indexPageUrl2Set); //!<设置首页网址。
  QString getIndexPage(); //!<获取首页网址。
  void setAutoAddProxyRule(bool crtSzcrtSzVrtAutoAddRule); //!<设置是否要自动添加代理规则。
  bool isAutoAddProxyRule(); //!<是否要自动添加代理规则。
  void setAllUsingProxy(bool crtSzcrtSzVrtallUseProxy); //!<设置是否要all use代理。
  bool isAllUsingProxy(); //!<是否all 使用代理。
  void setWebglEnabled(bool enblWbGl); //!<设置是否启用WebGl.
  bool getWebGlEnabled(); //!<是否启用WebGl.
  void setTtsServerPort(quint16 ttsServerPort); //!<设置TTS服务器的端口号。
  void setTtsServerAddress(QString ttsServerAddress); //!<设置TTS服务器地址。
  quint16 getTtsServerPort(); //!<获取TTS服务器的端口号。
  QString getTtsServerAddress(); //!<获取TTS服务器的地址。
  void refreshAdBlockUrls(); //!<刷新要封锁的URL列表。
  void setProxyFactory(StbProxyFactory * prxyFctry); //!<设置代理工厂。
  void refreshWebsitesWithProxy(); //!<刷新要使用代理的网站的列表。
  void setSmartProxyPort(quint16 smtPrxyPrt); //!<设置智能代理的端口号。
  void setUsingSmartProxy(bool crtSz); //!<设置是否要使用智能代理。
  quint16 getSmartProxyPort(); //!<智能代理的端口号。
  bool isUsingSmartProxy(); //!<是否使用智能代理。
  explicit StbSettingDialog(QWidget *parent = 0); //!<构造函数。
  ~StbSettingDialog(); //!<析构函数。
  bool isUseProxyForDns() const;
  void setUseProxyForDns(bool value);
  quint16 getSmartProxyPortHttp() const;
  void setSmartProxyPortHttp(const quint16 &value);
  bool getJavascriptEnabled() const;
  bool getQuitLowMemory() const; //!<是否在内存不足时退出。
  void setJavascriptEnabled(bool value);
  QString getUserAgent() const;
  void setUserAgent(const QString &value);
  void setQuitLowMemory(const bool quitLowMemoryenableJavascript); //!<设置是否在内存不足时退出。
  QString getDownloaderType() const;
  void setDownloaderType(const QString &value);
  QString getDownloadDirectory() const;
  void setDownloadDirectory(const QString &value);
  bool getQuitIdle10Minutes() const;

  void setQuitIdle10Minutes(bool value);

  bool getPauseMediaWhenLostFocus() const;
  void setPauseMediaWhenLostFocus(bool value);

protected:
  void changeEvent(QEvent *e); //!<变更事件。

private:
//  bool PauseMediaWhenLostFocus; //!<
  void AddTunnel(); //!<输入完了隧道信息之后，添加隧道。
  QVariantList TnlLstVrnt; //!<隧道信息列表。
  void SaveConfig(); //!<保存配置信息。
  TunnelInfoDialog newTunnelInfoDialog; //!<新建隧道信息对话框。
  void PopNewTunnelDialog(); //!<弹出“新隧道”对话框。
  quint16 smartProxyPortHttp; //!<智能代理的HTTP端口号。
  bool useProxyForDns=false; //!<是否要通过代理来解析DNS。
  QMap<QString,qint64> UiLanguageNameIndexMap; //!<界面语言名字与界面下标之间的映射。
  void initializeMemberObjects(); //!<初始化成员对象。
  QMap<qint64,QString> UiLanguageComboBoxIndexLanguageNameMap; //!<下标和语言名字之间的映射。
  StbProxyFactory * prxyFctr; //!<代理工厂对象。
  Ui::StbSettingDialog *ui;
};

#endif // SETTINGDIALOG_H
