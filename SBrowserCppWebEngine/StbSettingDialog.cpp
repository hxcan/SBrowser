#include "SbPreCompile.h" //QInputDialog

#include "StbSettingDialog.h"
#include "ui_StbSettingDialog.h"

/*!
 * \brief StbSettingDialog::addCandidateTtsServerIp Add candidate tts server ip.
 */
void StbSettingDialog::addCandidateTtsServerIp()
{

} // void StbSettingDialog::addCandidateTtsServerIp()

/*!
 * \brief StbSettingDialog::setSmartProxyHost 设置智能代理的主机名。
 * \param smartProxyHost 智能代理的主机名。
 */
void StbSettingDialog::setSmartProxyHost(QString smartProxyHost)
{
    ui->smartProxyHostlineEdit->setText (smartProxyHost); //显示到部件中。

} //void StbSettingDialog::setSmartProxyHost(QString smartProxyHost)

/*!
 * \brief StbSettingDialog::selectSearchEngineByName 按照名字来选中搜索引擎。
 * \param smartProxyHost 搜索引擎的名字。
 */
void StbSettingDialog::selectSearchEngineByName(QString smartProxyHost)
{
    ui->searchEnginecomboBox_2->setCurrentText(smartProxyHost);
} //void StbSettingDialog::selectSearchEngineByName(QString smartProxyHost)

/*!
 * \brief StbSettingDialog::getSearchEngineName 获取搜索引擎名字。
 * \return 搜索引擎名字。
 */
QString StbSettingDialog::getSearchEngineName()
{
    return ui->searchEnginecomboBox_2->currentText();
} //QString StbSettingDialog::getSearchEngineName()

/*!
 * \brief StbSettingDialog::getSmartProxyHost 获取智能代理的主机名。
 * \return 智能代理的主机名。
 */
QString StbSettingDialog::getSmartProxyHost()
{
    return ui->smartProxyHostlineEdit->text ();
} //QString StbSettingDialog::getSmartProxyHost()

/*!
 * \brief StbSettingDialog::getPluginsEnabled 是否启用插件。
 * \return 是否启用插件。
 */
bool StbSettingDialog::getPluginsEnabled()
{
    return ui->pluginsEnabledcheckBox->isChecked ();
} //bool StbSettingDialog::getPluginsEnabled()

/*!
 * \brief StbSettingDialog::setPluginsEnabled 设置是否启用插件。
 * \param enablePlugins 是否启用插件。
 */
void StbSettingDialog::setPluginsEnabled(bool enablePlugins)
{
    ui->pluginsEnabledcheckBox->setChecked (enablePlugins); //设置复选框的选中状态。

    return;
} //void StbSettingDialog::setPluginsEnabled(bool enablePlugins)

StbSettingDialog::StbSettingDialog(QWidget *parent) :QDialog(parent),ui(new Ui::StbSettingDialog)
{
    ui->setupUi(this);

    initializeMemberObjects(); //初始化成员对象。
    
    connect(&newTunnelInfoDialog, &QDialog::accepted, this, &StbSettingDialog::AddTunnel); //输入完了隧道信息之后，添加隧道。
}

/*!
 * \brief Sstm::AddTunnel 输入完了隧道信息之后，添加隧道。
 */
void StbSettingDialog::AddTunnel()
{
    QString CrtSrvrIp=newTunnelInfoDialog.GetServerIp(); //获取服务器IP。
    quint16 CrtSrvrPrt=newTunnelInfoDialog.GetServerPort(); //获取服务器端口号。
    quint16 CrtLclPrt=newTunnelInfoDialog.GetLocalPort(); //获取本地监听的端口号。
    QString currentUserName=newTunnelInfoDialog.getUserName(); //获取用户名。
    bool autoRestart=newTunnelInfoDialog.isAutoRestart(); //是否自动重启。
    bool autoCheckTunnelStatus=newTunnelInfoDialog.isAutoCheckTunnelStatus(); //是否自动检查隧道状态。
    bool autoStart=newTunnelInfoDialog.isAutoStart(); //是否自动启动。

    QVariantMap CrtVrt; //将此文件转换成QVariant。

    CrtVrt.insert("ServerPort",CrtSrvrPrt); //设置服务器端口号。
    CrtVrt.insert("ServerIP",CrtSrvrIp); //设置服务器IP。
    CrtVrt.insert("LocalPort",CrtLclPrt); //设置本地监听端口号。
    CrtVrt.insert ("UserName",currentUserName); //设置用户名。
    CrtVrt.insert ("AutoRestart",autoRestart); //是否自动重启。
    CrtVrt.insert ("AutoCheckTunnelStatus",autoCheckTunnelStatus); //是否自动检查隧道状态。
    CrtVrt.insert ("AutoStart",autoStart); //是否自动启动。


    TnlLstVrnt << CrtVrt; //添加到隧道信息列表中。

    ui->additionalProxieslistWidget_2->addItem(CrtSrvrIp); //显示一个条目到界面上。


    //保存到配置文件中：
    SaveConfig(); //保存配置信息。
} //void Sstm::AddTunnel() //!<输入完了隧道信息之后，添加隧道。


StbSettingDialog::~StbSettingDialog()
{
    delete ui;
}

/*!
 * \brief Sstm::SaveConfig 保存配置信息。
 */
void StbSettingDialog::SaveConfig()
{
    QSettings settings; //用来保存信息的工具。

    settings.setValue("TunnelList", TnlLstVrnt); //保存。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("Tunnel list:") << TnlLstVrnt; //Debug.

    settings.beginGroup("sstm"); //开始分组。

    settings.endGroup(); //关闭分组。

    settings.sync(); //同步。
} //void Sstm::SaveConfig(QString JsnStr2Sv) //!<保存配置信息。


void StbSettingDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool StbSettingDialog::getPauseMediaWhenLostFocus() const
{
  return ui->pauseMediaWhenLostFocusBox->isChecked ();
}

void StbSettingDialog::setPauseMediaWhenLostFocus(bool value)
{
//  PauseMediaWhenLostFocus = value;
  ui->pauseMediaWhenLostFocusBox->setChecked (value);
}

bool StbSettingDialog::getQuitIdle10Minutes() const
{
  return ui->quitIdle10MinutesBox->isChecked ();
}

void StbSettingDialog::setQuitIdle10Minutes(bool value)
{
  ui->quitIdle10MinutesBox->setChecked (value);
}

QString StbSettingDialog::getDownloadDirectory() const
{
    return ui->downloadDirectorylineEdit->text();
}

/*!
 * \brief StbSettingDialog::browseDownloadDirectory Browse download directory.
 */
void StbSettingDialog::browseDownloadDirectory()
{
  auto directoryname=QFileDialog::getExistingDirectory ();

  setDownloadDirectory (directoryname);
} // void StbSettingDialog::browseDownloadDirectory()

void StbSettingDialog::setDownloadDirectory(const QString &value)
{
//    downloadDirectory = value;
    ui->downloadDirectorylineEdit->setText(value);
}

QString StbSettingDialog::getDownloaderType() const
{
    auto downloaderType=DownloaderType::SuperDownloader; //结果
    
    if (ui->superDownloaderradioButton_2->isChecked()) //超级下载器
    {
        downloaderType=DownloaderType::SuperDownloader;
    } //if (ui->superDownloaderradioButton_2->isChecked()) //超级下载器
    else //内置下载器。
    {
        downloaderType=DownloaderType::BuiltinDownloader; //内置下载器
    } //else //内置下载器。

    return downloaderType;
}

void StbSettingDialog::setDownloaderType(const QString &value)
{
    auto downloaderType = value;

    if (downloaderType==DownloaderType::SuperDownloader) //超级下载器
    {
        ui->superDownloaderradioButton_2->setChecked(true);
    } //if (downloaderType==DownloaderType::SuperDownloader) //超级下载器
    else //内置下载器
    {
        ui->builtinDownloaderradioButton->setChecked(true);
    } //else //内置下载器
}

QString StbSettingDialog::getUserAgent() const
{
    return ui->userAgentlineEdit->text ();
}

/*!
 * \brief StbSettingDialog::setQuitLowMemory 设置是否在内存不足时退出。
 * \param quitLowMemoryenableJavascript 是否在内存不足时退出。
 */
void StbSettingDialog::setQuitLowMemory(const bool quitLowMemoryenableJavascript)
{
    ui->lowMemoryQuitcheckBox->setChecked(quitLowMemoryenableJavascript);
} //void StbSettingDialog::setQuitLowMemory(const bool quitLowMemoryenableJavascript)

void StbSettingDialog::setUserAgent(const QString &value)
{
    ui->userAgentlineEdit->setText (value);
}

/*!
 * \brief StbSettingDialog::getQuitLowMemory  是否在内存不足时退出。
 * \return 是否在内存不足时退出。
 */
bool StbSettingDialog::getQuitLowMemory() const
{
    return ui->lowMemoryQuitcheckBox->isChecked();
} //bool StbSettingDialog::getQuitLowMemory() const

bool StbSettingDialog::getJavascriptEnabled() const
{
    return ui->enableJavascriptcheckBox->isChecked();
}

void StbSettingDialog::setJavascriptEnabled(bool value)
{
    ui->enableJavascriptcheckBox->setChecked( value);
}

quint16 StbSettingDialog::getSmartProxyPortHttp() const
{
    return ui->httpProxyPortspinBox->value();
}

void StbSettingDialog::setSmartProxyPortHttp(const quint16 &value)
{
    ui->httpProxyPortspinBox->setValue(value);
}

bool StbSettingDialog::isUseProxyForDns() const
{
    return ui->useProxyForDnscheckBox->isChecked();
}

void StbSettingDialog::setUseProxyForDns(bool value)
{
    ui->useProxyForDnscheckBox->setChecked(value);
}

/*!
 * \brief StbSettingDialog::isUsingSmartProxy 是否使用智能代理。
 * \return 是否使用智能代理。
 */
bool StbSettingDialog::isUsingSmartProxy()
{
    return ui->smtPrxygroupBox->isChecked (); //选中则表示使用智能代理。
} //bool StbSettingDialog::isUsingSmartProxy() //!<是否使用智能代理。

/*!
 * \brief StbSettingDialog::getSmartProxyPort 智能代理的端口号。
 * \return 智能代理的端口号。
 */
quint16 StbSettingDialog::getSmartProxyPort()
{
    return ui->smtPrxyPrtspinBox->value (); //返回端口号。
} //quint16 StbSettingDialog::getSmartProxyPort() //!<智能代理的端口号。

/*!
 * \brief StbSettingDialog::setUsingSmartProxy 设置是否要使用智能代理。
 * \param crtSz 是否要使用智能代理。
 */
void StbSettingDialog::setUsingSmartProxy(bool crtSz)
{
    ui->smtPrxygroupBox->setChecked (crtSz); //设置选中状态。

    return;
} //void StbSettingDialog::setUsingSmartProxy(bool crtSz) //!<设置是否要使用智能代理。

/*!
 * \brief StbSettingDialog::setSmartProxyPort 设置智能代理的端口号。
 * \param smtPrxyPrt 智能代理的端口号。
 */
void StbSettingDialog::setSmartProxyPort(quint16 smtPrxyPrt)
{
  ui->smtPrxyPrtspinBox->setValue (smtPrxyPrt); //设置数值。
} //void StbSettingDialog::setSmartProxyPort(quint16 smtPrxyPrt) //!<设置智能代理的端口号。

/*!
 * \brief StbSettingDialog::refreshWebsitesWithProxy 刷新要使用代理的网站的列表。
 */
void StbSettingDialog::refreshWebsitesWithProxy()
{
  QStringList wbstLstNdPrxy=prxyFctr->getWebsiteListNeedsProxy(); //获取需要通过代理访问的网站列表。

  ui->prxyWbLstlistWidget->clear (); //清空列表。

  std::sort(wbstLstNdPrxy.begin (), wbstLstNdPrxy.end ()); // 排序。

  foreach(QString crtWbst,wbstLstNdPrxy) //一个个地显示。
  {
    ui->prxyWbLstlistWidget->addItem (crtWbst); //添加一条。
  } //foreach(crtWbst,wbstLstNdPrxy) //一个个地显示。
} //void StbSettingDialog::refreshWebsitesWithProxy() //!<刷新要使用代理的网站的列表。

/*!
 * \brief StbSettingDialog::setProxyFactory 设置代理工厂。
 * \param prxyFctry 要设置的代理工厂。
 */
void StbSettingDialog::setProxyFactory(StbProxyFactory * prxyFctry)
{
  prxyFctr=prxyFctry; //记录。
} //void StbSettingDialog::setProxyFactory(StbProxyFactory * prxyFctry) //!<设置代理工厂。

/*!
 * \brief StbSettingDialog::addNeedProxyWebsite 添加一个需要通过代理访问的网站。
 */
void StbSettingDialog::addNeedProxyWebsite()
{
    bool Acpted=false; //用户是否按了确定。

    QString CrtPt=QInputDialog::getText (this,tr("New website"),tr("Enter the website host name"),QLineEdit::Normal,"",&Acpted); //获取网站主机名。

    if (Acpted) //接受了。
    {
        ui->prxyWbLstlistWidget->addItem (tr("%1").arg (CrtPt)); //添加。

        prxyFctr->addHostname (CrtPt); //加入到代理工厂中。
    } //if (Acpted) //接受了。

    return;
} //void StbSettingDialog::addNeedProxyWebsite() //!<添加一个需要通过代理访问的网站。

/*!
 * \brief StbSettingDialog::addAdBlockUrl 添加一个要封锁的URL。
 */
void StbSettingDialog::addAdBlockUrl()
{
  bool Acpted=false; //用户是否按了确定。

  QString CrtPt=QInputDialog::getMultiLineText (this,tr("New url"),tr("Enter the url pattern"),"",&Acpted); //获取URL列表。

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
        ui->adBlockUrllistWidget->addItem (tr("%1").arg (currentUrl)); //添加。

        prxyFctr->addAdBlockUrl (currentUrl); //加入到代理工厂中。
      } //else //不是空行。
    } //foreach(QString currentUrl,urlList) //一行行地处理。
  } //if (Acpted) //接受了。
} //void StbSettingDialog::addAdBlockUrl() //!<添加一个要封锁的URL。

/*!
 * \brief StbSettingDialog::addAdditionalProxy 添加额外的代理。
 */
void StbSettingDialog::addAdditionalProxy()
{
    PopNewTunnelDialog(); //弹出“新隧道”对话框。
} //void StbSettingDialog::addAdditionalProxy()

/*!
 * \brief Sstm::PopNewTunnelDialog 弹出“新隧道”对话框。
 */
void StbSettingDialog::PopNewTunnelDialog()
{
    newTunnelInfoDialog.show(); //显示对话框。
} //void Sstm::PopNewTunnelDialog() //!<弹出“新隧道”对话框。

/*!
 * \brief StbSettingDialog::refreshAdBlockUrls 刷新要封锁的URL列表。
 */
void StbSettingDialog::refreshAdBlockUrls()
{
  QStringList wbstLstNdPrxy=prxyFctr->getAdBlockUrls(); //获取需要封锁的URL列表。

  ui->adBlockUrllistWidget->clear (); //清空列表。

  std::sort(wbstLstNdPrxy.begin (), wbstLstNdPrxy.end ()); // 排序。

  foreach(QString crtWbst,wbstLstNdPrxy) //一个个地显示。
  {
    ui->adBlockUrllistWidget->addItem (crtWbst); //添加一条。
  } //foreach(crtWbst,wbstLstNdPrxy) //一个个地显示。
} //void StbSettingDialog::refreshAdBlockUrls() //!<刷新要封锁的URL列表。

/*!
 * \brief StbSettingDialog::deleteNeedProxyWebsite 删除一个需要通过代理访问的网站。
 */
void StbSettingDialog::deleteNeedProxyWebsite()
{
    int wbstIdx=ui->prxyWbLstlistWidget->currentRow (); //获取当前条目的下标。



    if (wbstIdx>=0) //行号有效。
    {
        QString websiteDomain=ui->prxyWbLstlistWidget->item (wbstIdx)->text (); //获取该行的域名。



        prxyFctr->removeHostname (websiteDomain); //删除该域名。

        ui->prxyWbLstlistWidget->takeItem (wbstIdx); //删除该条目。

    } //if (wbstIdx>=0) //行号有效。


    return;
} //void StbSettingDialog::deleteNeedProxyWebsite() //!<删除一个需要通过代理访问的网站。

/*!
 * \brief StbSettingDialog::deleteAdblockRule 删除无用的广告禁用规则。
 */
void StbSettingDialog::deleteAdblockRule()
{
  int wbstIdx=ui->adBlockUrllistWidget->currentRow (); //获取当前条目的下标。

  if (wbstIdx>=0) //行号有效。
  {
    QString websiteDomain=ui->adBlockUrllistWidget->item (wbstIdx)->text (); //获取该行的规则。

    prxyFctr->removeAdblockUrl(websiteDomain); //删除该规则。

    ui->adBlockUrllistWidget->takeItem (wbstIdx); //删除该条目。
  } //if (wbstIdx>=0) //行号有效。
} //void StbSettingDialog::deleteAdblockRule()

/*!
 * \brief StbSettingDialog::getTtsServerAddress 获取TTS服务器的地址。
 * \return TTS服务器的地址。
 */
QString StbSettingDialog::getTtsServerAddress()
{
  QString result; //结果。

  result=ui->ttsServerAddresslineEdit->text (); //返回输入框内容。

  return result;
} //QString StbSettingDialog::getTtsServerAddress() //!<获取TTS服务器的地址。

/*!
 * \brief StbSettingDialog::getTtsServerPort 获取TTS服务器的端口号。
 * \return TTS服务器的端口号。
 */
quint16 StbSettingDialog::getTtsServerPort()
{
    quint16 result=0; //结果。

    result=ui->ttsServerPortspinBox->value (); //结果即为旋转框的值。

    return result;
} //quint16 StbSettingDialog::getTtsServerPort() //!<获取TTS服务器的端口号。

/*!
 * \brief StbSettingDialog::setTtsServerAddress 设置TTS服务器地址。
 * \param ttsServerAddress 要设置的TTS服务器地址。
 */
void StbSettingDialog::setTtsServerAddress(QString ttsServerAddress)
{
    ui->ttsServerAddresslineEdit->setText (ttsServerAddress); //显示。

    return;
} //void StbSettingDialog::setTtsServerAddress(QString ttsServerAddress) //!<设置TTS服务器地址。

/*!
 * \brief StbSettingDialog::setTtsServerPort 设置TTS服务器的端口号。
 * \param ttsServerPort 要设置的TTS服务器端口号。
 */
void StbSettingDialog::setTtsServerPort(quint16 ttsServerPort)
{
    ui->ttsServerPortspinBox->setValue (ttsServerPort); //显示。

    return;
} //void StbSettingDialog::setTtsServerPort(quint16 ttsServerPort) //!<设置TTS服务器的端口号。

/*!
 * \brief StbSettingDialog::getWebGlEnabled 是否启用WebGl.
 * \return 是否启用WebGl.
 */
bool StbSettingDialog::getWebGlEnabled()
{
    return ui->enableWebglcheckBox->isChecked ();
} //bool StbSettingDialog::getWebGlEnabled()

/*!
 * \brief StbSettingDialog::setWebglEnabled 设置是否启用WebGl.
 * \param enblWbGl 是否启用WebGl.
 */
void StbSettingDialog::setWebglEnabled(bool enblWbGl)
{
    ui->enableWebglcheckBox->setChecked (enblWbGl); //设置选中状态。

    return;
} //void StbSettingDialog::setWebglEnabled(bool enblWbGl)

/*!
 * \brief StbSettingDialog::isAllUsingProxy 是否all 使用代理。
 * \return 是否all 使用代理。
 */
bool StbSettingDialog::isAllUsingProxy()
{
    return ui->allUseProxycheckBox->isChecked ();
} //bool StbSettingDialog::isAllUsingProxy()

/*!
 * \brief StbSettingDialog::setAllUsingProxy 设置是否要all use代理。
 * \param crtSzcrtSzVrtallUseProxy 是否要all use代理。
 */
void StbSettingDialog::setAllUsingProxy(bool crtSzcrtSzVrtallUseProxy)
{
    ui->allUseProxycheckBox->setChecked (crtSzcrtSzVrtallUseProxy); //Set is in the ui.

    ui->autoAddRulecheckBox->setEnabled (crtSzcrtSzVrtallUseProxy); //切换自动添加代理规则按钮的可用性。

    return;
} //void StbSettingDialog::setAllUsingProxy(bool crtSzcrtSzVrtallUseProxy)

/*!
 * \brief StbSettingDialog::setAutoAddRule 设置是否要自动添加代理规则。
 * \param crtSzcrtSzVrtAutoAddRule 是否要自动添加代理规则。
 */
void StbSettingDialog::setAutoAddProxyRule(bool crtSzcrtSzVrtAutoAddRule)
{
    ui->autoAddRulecheckBox->setChecked (crtSzcrtSzVrtAutoAddRule); //设置状态。

    return;
} //void StbSettingDialog::setAutoAddRule(bool crtSzcrtSzVrtAutoAddRule)

/*!
 * \brief StbSettingDialog::isAutoAddProxyRule 是否要自动添加代理规则。
 * \return 是否要自动添加代理规则。
 */
bool StbSettingDialog::isAutoAddProxyRule()
{
    return ui->autoAddRulecheckBox->isChecked ();
} //bool StbSettingDialog::isAutoAddProxyRule()

/*!
 * \brief StbSettingDialog::toggleAutoAddEnabled 切换自动添加代理规则复选框的可用性。
 * \param enabled2Set 要切换到的目标可用性。
 */
void StbSettingDialog::toggleAutoAddEnabled(bool enabled2Set)
{
    ui->autoAddRulecheckBox->setEnabled (enabled2Set); //设置状态。

    return;
} //void StbSettingDialog::toggleAutoAddEnabled(bool enabled2Set)

/*!
 * \brief StbSettingDialog::getIndexPage 获取首页网址。
 * \return 首页网址。
 */
QString StbSettingDialog::getIndexPage()
{
    return ui->indexPagelineEdit->text();
} //QString StbSettingDialog::getIndexPage()

/*!
 * \brief StbSettingDialog::setIndexPage 设置首页网址。
 * \param indexPageUrl2Set 要设置的首页网址。
 */
void StbSettingDialog::setIndexPage(QString indexPageUrl2Set)
{
    ui->indexPagelineEdit->setText (indexPageUrl2Set); //显示。

    return;
} //void StbSettingDialog::setIndexPage(QString indexPageUrl2Set)

/*!
 * \brief StbSettingDialog::getManuallySelectLanguage 是否手动选择界面语言。
 * \return 是否手动选择界面语言。
 */
bool StbSettingDialog::getManuallySelectLanguage()
{
    return ui->manualSelectUiLanguagegroupBox_4->isChecked ();
} //bool StbSettingDialog::getManuallySelectLanguage()

/*!
 * \brief StbSettingDialog::getSelectedLanguage 获取选择的语言。
 * \return 获取选择的语言。
 */
QString StbSettingDialog::getSelectedLanguage()
{
    QString result=""; //结果。

    qint64 currentIndex=ui->uiLanguagecomboBox->currentIndex (); //获取当前语言下标。

    result=UiLanguageComboBoxIndexLanguageNameMap[currentIndex]; //将下标转换为语言名字。

    return result;
} //QString StbSettingDialog::getSelectedLanguage()

/*!
 * \brief StbSettingDialog::setManuallySelectLanguage 设置，是否要手动选择语言。
 * \param manuallySelectLanguage 是否要手动选择语言。
 */
void StbSettingDialog::setManuallySelectLanguage(bool manuallySelectLanguage)
{
    ui->manualSelectUiLanguagegroupBox_4->setChecked (manuallySelectLanguage); //设置部件的状态。

    return;
} //void StbSettingDialog::setManuallySelectLanguage(bool manuallySelectLanguage)

/*!
 * \brief StbSettingDialog::setSelectedLanguage 设置，选择的语言。
 * \param selectedLanguage 选择的语言。
 */
void StbSettingDialog::setSelectedLanguage(QString selectedLanguage)
{



    qint64 targetIndex=UiLanguageNameIndexMap[selectedLanguage]; //转换成下标。



    ui->uiLanguagecomboBox->setCurrentIndex (targetIndex); //选择目标下标。



    return;
} //void StbSettingDialog::setSelectedLanguage(QString selectedLanguage)

/*!
 * \brief StbSettingDialog::initializeMemberObjects 初始化成员对象。
 */
void StbSettingDialog::initializeMemberObjects()
{
    UiLanguageComboBoxIndexLanguageNameMap=
    {
        {0,"zh_CN"},
        {1,"ru"},
    };


    UiLanguageNameIndexMap=
    {
        {"zh_CN",0},
        {"ru",1},
    };

    return;
} //void StbSettingDialog::initializeMemberObjects()

/*!
 * \brief StbSettingDialog::testSelectIndex 试验选中当前标签页。
 */
void StbSettingDialog::testSelectIndex()
{
    ui->uiLanguagecomboBox->setCurrentIndex (1); //选中当前下标。

    return;
} //void StbSettingDialog::testSelectIndex()

/*!
 * \brief StbSettingDialog::setEnableAdBlock 设置，是否启用广告禁用功能。
 * \param enableAdBlock 是否启用广告禁用功能。
 */
void StbSettingDialog::setEnableAdBlock(bool enableAdBlock)
{
    ui->enableAdBlockcheckBox->setChecked (enableAdBlock); //设置状态。

    return;
} //void StbSettingDialog::setEnableAdBlock(bool enableAdBlock)

/*!
 * \brief StbSettingDialog::getEnableAdBlock 是否要启用广告禁用功能。
 * \return 是否要启用广告禁用功能。
 */
bool StbSettingDialog::getEnableAdBlock()
{
    return ui->enableAdBlockcheckBox->isChecked ();
} //bool StbSettingDialog::getEnableAdBlock()

/*!
 * \brief StbSettingDialog::clearDiskCache 清除磁盘缓存。
 */
void StbSettingDialog::clearDiskCache()
{
    QString diskCacheDirectoryName=QDir::homePath()+"/.sbrowser/diskcache/"; //磁盘缓存文件目录。
    QDir diskCacheDirectory(diskCacheDirectoryName); //磁盘缓存目录 。

    diskCacheDirectory.removeRecursively (); //递归删除。

    return;
} //void StbSettingDialog::clearDiskCache()
