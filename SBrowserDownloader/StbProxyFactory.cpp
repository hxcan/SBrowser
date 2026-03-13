#include "SbPreCompile.h" //cout

#include "StbProxyFactory.h"

/*!
 * \brief StbProxyFactory::StbProxyFactory 构造函数。
 */
StbProxyFactory::StbProxyFactory()
{
    intnlPrxyHstNmSt.insert ("stupidbeauty.com"); //加入漂亮的笨蛋的主机名。

    loadExternalProxyHostSet(); //载入外部需代理主机名列表。
    loadAdBlockUrlSet(); //载入需要封锁的URL集合。
} //StbProxyFactory::StbProxyFactory()

/*!
 * \brief StbProxyFactory::isBlockingUrl 是否应当阻止此网址。
 * \param inforequestUrl 要对其进行查询的网址。
 * \return 是否应当阻止。
 */
bool StbProxyFactory::isBlockingUrl(QUrl inforequestUrl)
{
    QString peerUrl=inforequestUrl.toString(); //获取对方的URL。

    QNetworkProxy blockProxyObj; //起封锁作用的代理对象。
    blockProxyObj.setType (QNetworkProxy::Socks5Proxy); //socks5代理。
    blockProxyObj.setHostName ("127.0.0.1"); //本机。
    blockProxyObj.setPort (11053); //随手写的一个端口。

    return (shouldBlockUrl(peerUrl)); //返回结果。
} //bool StbProxyFactory::isBlockingUrl(QUrl inforequestUrl)

/*!
 * \brief StbProxyFactory::queryProxy 查询代理规则。
 * \param query 查询对象。
 * \return 对应的代理规则列表。
 */
QList<QNetworkProxy> StbProxyFactory::queryProxy(const QNetworkProxyQuery &query)
{
    QList<QNetworkProxy> result; //结果。

    QString peerUrl=query.url ().toString(); //获取对方的URL。

    QNetworkProxy blockProxyObj; //起封锁作用的代理对象。
    blockProxyObj.setType (QNetworkProxy::Socks5Proxy); //socks5代理。
    blockProxyObj.setHostName ("127.0.0.1"); //本机。
    blockProxyObj.setPort (11053); //随手写的一个端口。

    if (shouldBlockUrl(peerUrl)) //该URL要封锁。
    {
        result << blockProxyObj; //加入封锁代理对象。
    } //if (adBlckUrlSt.contains (peerUrl)) //该URL要封锁。
    else //该URL不要封锁，则按照正常流程来判断。尽力让浏览器能够连接到它。
    {
        //智能代理，socks5:
        QNetworkProxy smtPrxyObj; //智能代理。
        smtPrxyObj.setType (QNetworkProxy::Socks5Proxy); //socks5代理。
        smtPrxyObj.setHostName (smartProxyHost); //本机。
        smtPrxyObj.setPort (usnSmtPrxyPrt); //端口号。

        if (useProxyForDns) //要通过代理来解析DNS。
        {
            QNetworkProxy::Capabilities proxyCapability=smtPrxyObj.capabilities(); //获取已有的能力。
            proxyCapability|=QNetworkProxy::HostNameLookupCapability; //加入域名解析能力。
            smtPrxyObj.setCapabilities(proxyCapability); //更新能力。
        } //if (useProxyForDns) //要通过代理来解析DNS。


        //智能代理，http:
        QNetworkProxy smtPrxyObjHttp; //智能代理。
        smtPrxyObjHttp.setType (QNetworkProxy::HttpProxy); //http代理。
        smtPrxyObjHttp.setHostName (smartProxyHost); //本机。
        smtPrxyObjHttp.setPort (smartProxyPortHttp); //端口号。

        if (useProxyForDns) //要通过代理来解析DNS。
        {
            QNetworkProxy::Capabilities proxyCapability=smtPrxyObjHttp.capabilities(); //获取已有的能力。
            proxyCapability|=QNetworkProxy::HostNameLookupCapability; //加入域名解析能力。
            smtPrxyObjHttp.setCapabilities(proxyCapability); //更新能力。
        } //if (useProxyForDns) //要通过代理来解析DNS。


        QString prHstNm=query.peerHostName (); //获取对方的主机名。

        if (usngSmtPrxy) //要使用智能代理。
        {
            if (allUseProxy) //Should use proxy for all hosts.
            {
                result << smtPrxyObj; //加入智能代理。
                result << smtPrxyObjHttp; //加入HTTP代理。

                if (autoAddRule) //要自动添加规则。
                {
                    extnlPrxHstSt << prHstNm; //添加到规则中。

                    saveExternalProxyHostSet(); //保存列表。
                } //if (autoAddRule) //要自动添加规则。
            } //if (allUseProxy) //Should use proxy for all hosts.
            else //Not use proxy for all hosts.
            {
                if (intnlPrxyHstNmSt.contains(prHstNm)  || extnlPrxHstSt.contains (prHstNm)) //包含在内部的需代理主机名集合中，则考虑对之使用代理。
                {
                    result << smtPrxyObj; //加入智能代理。
                    result << smtPrxyObjHttp; //加入HTTP代理。
                    
                } //if (intnlPrxyHstNmSt.contains(prHstNm)) //包含在内部的需代理主机名集合中，则考虑对之使用代理。
            } //else //Not use proxy for all hosts.
        } //if (usngSmtPrxy) //要使用智能代理。



        QNetworkProxy dftPrxy; //默认代理。
        dftPrxy.setType (QNetworkProxy::DefaultProxy); //类型为默认代理。

        result << dftPrxy; //加入默认代理。

        QNetworkProxy noProxy; //无代理。
        dftPrxy.setType (QNetworkProxy::NoProxy); //类型为，无代理。

        result << noProxy; //加入无代理。
    } //else //该URL不要封锁，则按照正常流程来判断。

    return result;
} //QList<QNetworkProxy> StbProxyFactory::queryProxy(const QNetworkProxyQuery &query = QNetworkProxyQuery()) //!<查询代理规则。

/*!
 * \brief StbProxyFactory::setEnableAdBlock 设置，是否启用广告禁用功能。
 * \param enableAdBlock 是否启用广告禁用功能。
 */
void StbProxyFactory::setEnableAdBlock(bool enableAdBlock)
{
    _enableAdBlock=enableAdBlock; //记录。

    return;
} //void StbProxyFactory::setEnableAdBlock(bool enableAdBlock)

/*!
 * \brief StbSettingDialog::setUsingSmartProxy 设置是否要使用智能代理。
 * \param crtSz 是否要使用智能代理。
 */
void StbProxyFactory::setUsingSmartProxy(bool crtSz)
{
    usngSmtPrxy=crtSz; //记录。

    return;
} //void StbSettingDialog::setUsingSmartProxy(bool crtSz) //!<设置是否要使用智能代理。

/*!
 * \brief StbProxyFactory::setSmartProxyHost 设置智能代理的主机名。
 * \param stngDlggetSmartProxyHost 智能代理的主机名。
 */
void StbProxyFactory::setSmartProxyHost(QString stngDlggetSmartProxyHost)
{
    smartProxyHost=stngDlggetSmartProxyHost; //记录。
} //void StbProxyFactory::setSmartProxyHost(QString stngDlggetSmartProxyHost)

/*!
 * \brief StbSettingDialog::setSmartProxyPort 设置智能代理的端口号。
 * \param smtPrxyPrt 智能代理的端口号。
 */
void StbProxyFactory::setSmartProxyPort(quint16 smtPrxyPrt)
{
    usnSmtPrxyPrt=smtPrxyPrt; //记录。

    return;
} //void StbSettingDialog::setSmartProxyPort(quint16 smtPrxyPrt) //!<设置智能代理的端口号。

/*!
 * \brief StbProxyFactory::addHostname 加入到代理规则中。
 * \param hstNm 要加入的主机名。
 */
void StbProxyFactory::addHostname(QString hstNm)
{
    extnlPrxHstSt.insert(hstNm); //加入到外部需代理主机名集合中。



    saveExternalProxyHostSet(); //保存列表。

    return;
} //void StbProxyFactory::addHostname(QString hstNm) //!<加入到代理规则中。

/*!
 * \brief StbProxyFactory::removeHostname 从代理规则中删除。
 * \param hstNm 要删除的主机名。
 */
void StbProxyFactory::removeHostname(QString hstNm)
{
    extnlPrxHstSt.remove (hstNm); //删除。



    saveExternalProxyHostSet(); //保存列表。

    return;
} //void StbProxyFactory::removeHostname(QString hstNm) //!<从代理规则中删除。

/*!
 * \brief StbProxyFactory::saveExternalProxyHostSet 保存内部需代理的主机名列表。
 */
void StbProxyFactory::saveExternalProxyHostSet()
{
    QVariantMap ssnObj; //代理主机名集合信息对象。


    QVariantList extnlPrxHstStVrLst; //外部需代理主机名集合列表。

    foreach (QString crtHst, extnlPrxHstSt) //一个个地加入列表中。
    {
        QVariant crtHstVrt(crtHst); //构造当前对象。

        extnlPrxHstStVrLst << crtHstVrt; //加入列表中。
    }

    ssnObj["proxyHostnames"]=extnlPrxHstStVrLst; //加入集合信息。



    //保存到文件中：
    QString ssnStrJs=QDir::homePath()+"/.sbrowser/externalproxyhostnames.json"; //会话记录的JSON文件名。
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

    return;
} //void StbProxyFactory::saveExternalProxyHostSet() //!<保存内部需代理的主机名列表。

/*!
 * \brief StbProxyFactory::loadExternalProxyHostSet 载入外部需代理主机名列表。
 */
void StbProxyFactory::loadExternalProxyHostSet()
{
    QString ssnStrJs=QDir::homePath()+"/.sbrowser/externalproxyhostnames.json"; //外部需代理主机名集合记录的JSON文件名。

    //从里面解析出当前打开的标签页列表：

    QFile ssnStrJsFl(ssnStrJs); //JSON文件。
    ssnStrJsFl.open (QIODevice::ReadOnly); //打开文件。
    QByteArray ssnStrBtAr=ssnStrJsFl.readAll (); //全部读入。
    ssnStrJsFl.close (); //关闭文件。




    QString json=QString::fromUtf8 (ssnStrBtAr); //将JSON内容转换成字符串。

    QJsonDocument loadDoc(QJsonDocument::fromJson(ssnStrBtAr)); //创建JSON文档。

    QJsonObject objVrt=loadDoc.object(); //将载入的JSON文档转换成JSON对象。

    QVariantMap obj=objVrt.toVariantMap (); //转换成映射。

    QVariantList wndsVl=obj["proxyHostnames"].toList (); //获取主机名集合。

    //一个个主机名地处理：

    foreach(QVariant crtWndVrt,wndsVl) //一个个窗口对象地处理。
    {
        QString crtWnd=crtWndVrt.toString (); //转换成字符串。当前主机名。

        extnlPrxHstSt.insert (crtWnd); //加入当前主机名。

    } //wndsVl.each do |crtWnd|


    return;
} //void StbProxyFactory::loadExternalProxyHostSet() //!<载入外部需代理主机名列表。

/*!
 * \brief StbProxyFactory::isUsingProxyForHostname 是否在为此主机使用代理。
 * \param hstNm 主机名。
 * \return 是否针对该主机名使用代理。
 */
bool StbProxyFactory::isUsingProxyForHostname(QString hstNm)
{
    bool result=false; //结果。

        if (usngSmtPrxy) //要使用智能代理。
        {
            if (allUseProxy) //Should use proxy for all the hosts.
            {
                result=true; //结果为真。
            } //if (allUseProxy) //Should use proxy for all the hosts.
            else //Not use proxy for all the hosts.
            {
                if (intnlPrxyHstNmSt.contains (hstNm) || (extnlPrxHstSt.contains (hstNm))) //内部或外部主机名集合包含此主机，则使用代理。
                {
                    result=true; //结果为真。
                } //if (intnlPrxyHstNmSt.contains (hstNm) || (extnlPrxHstSt.contains (hstNm))) //内部或外部主机名集合包含此主机，则使用代理。
            } //else //Not use proxy for all the hosts.


        } //if (usngSmtPrxy) //要使用智能代理。


    return result;
} //bool StbProxyFactory::isUsingProxyForHostname(QString hstNm) //!<是否在为此主机使用代理。

/*!
 * \brief StbProxyFactory::getWebsiteListNeedsProxy 获取需要通过代理访问的网站列表。
 * \return 需要通过代理访问的网站列表。
 */
QStringList StbProxyFactory::getWebsiteListNeedsProxy()
{
    QStringList result; //结果。

    QSet<QString> whlSt=intnlPrxyHstNmSt.unite (extnlPrxHstSt); //计算并集。

    result=QStringList(whlSt.begin (), whlSt.end ()); // Convert to list.

    // result=whlSt.toList (); //转换成列表。

    return result;
} //QStringList StbProxyFactory::getWebsiteListNeedsProxy() //!<获取需要通过代理访问的网站列表。

/*!
 * \brief StbProxyFactory::addAdBlockUrl 加入一个要封锁的URL。
 * \param CrtPt 要封锁的URL。
 */
void StbProxyFactory::addAdBlockUrl (QString CrtPt)
{
    adBlckUrlSt.insert(CrtPt); //加入到要封锁的URL集合中。

    saveAdBlockUrlSet(); //保存要封锁的URL列表。

    return;
} //void StbProxyFactory::addAdBlockUrl (QString CrtPt) //!<加入一个要封锁的URL。

/*!
 * \brief StbProxyFactory::saveAdBlockUrlSet 保存要封锁的URL列表。
 */
void StbProxyFactory::saveAdBlockUrlSet()
{
    QVariantMap ssnObj; //代理主机名集合信息对象。


    QVariantList extnlPrxHstStVrLst; //外部需代理主机名集合列表。

    foreach (QString crtHst, adBlckUrlSt) //一个个地加入列表中。
    {
        QVariant crtHstVrt(crtHst); //构造当前对象。

        extnlPrxHstStVrLst << crtHstVrt; //加入列表中。
    }

    ssnObj["adBlockUrls"]=extnlPrxHstStVrLst; //加入集合信息。



    //保存到文件中：
    QString ssnStrJs=QDir::homePath()+"/.sbrowser/adblockurls.json"; //会话记录的JSON文件名。
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

    return;
} //void StbProxyFactory::saveAdBlockUrlSet() //!<保存要封锁的URL列表。

/*!
 * \brief StbProxyFactory::loadAdBlockUrlSet 载入需要封锁的URL集合。
 */
void StbProxyFactory::loadAdBlockUrlSet()
{
    QString ssnStrJs=QDir::homePath()+"/.sbrowser/adblockurls.json"; //要封锁的URL集合记录的JSON文件名。


    QFile ssnStrJsFl(ssnStrJs); //JSON文件。
    ssnStrJsFl.open (QIODevice::ReadOnly); //打开文件。
    QByteArray ssnStrBtAr=ssnStrJsFl.readAll (); //全部读入。
    ssnStrJsFl.close (); //关闭文件。




    QString json=QString::fromUtf8 (ssnStrBtAr); //将JSON内容转换成字符串。


    QJsonDocument loadDoc(QJsonDocument::fromJson(ssnStrBtAr)); //创建JSON文档。

    QJsonObject objVrt=loadDoc.object(); //将载入的JSON文档转换成JSON对象。

    QVariantMap obj=objVrt.toVariantMap (); //转换成映射。

    QVariantList wndsVl=obj["adBlockUrls"].toList (); //获取URL集合。

    //一个个URL地处理：

    foreach(QVariant crtWndVrt,wndsVl) //一个个窗口对象地处理。
    {
        QString crtWnd=crtWndVrt.toString (); //转换成字符串。当前主机名。

        adBlckUrlSt.insert (crtWnd); //加入当前URL。

    } //wndsVl.each do |crtWnd|

    return;
} //void StbProxyFactory::loadAdBlockUrlSet() //!<载入需要封锁的URL集合。

/*!
 * \brief StbProxyFactory::getAdBlockUrls 获取需要封锁的URL列表。
 * \return 需要封锁的URL列表。
 */
QStringList StbProxyFactory::getAdBlockUrls()
{
  QStringList result; //结果。

    // result=adBlckUrlSt.toList (); //转换成列表。
  result = QStringList(adBlckUrlSt.begin (), adBlckUrlSt.end ()); //转换成列表。

  return result;
} //QStringList StbProxyFactory::getAdBlockUrls() //!<获取需要封锁的URL列表。

/*!
 * \brief StbProxyFactory::shouldBlockUrl 该URL是否要封锁。
 * \param peerUrl 要进行判断的目标URL。
 * \return 是否应当封锁。
 */
bool StbProxyFactory::getUseProxyForDns() const
{
    return useProxyForDns;
}

void StbProxyFactory::setUseProxyForDns(bool value)
{
    useProxyForDns = value;
}

quint16 StbProxyFactory::getSmartProxyPortHttp() const
{
    return smartProxyPortHttp;
}

void StbProxyFactory::setSmartProxyPortHttp(const quint16 &value)
{
    smartProxyPortHttp = value;
}

bool StbProxyFactory::shouldBlockUrl(QString peerUrl)
{
    bool result=false; //结果。
    
    if (_enableAdBlock) //启用广告禁用功能。
    {
        foreach(QString crtBlckUrlReg,adBlckUrlSt) //一个个正则式地尝试匹配。
        {
            auto regexpPcre = QRegularExpression::wildcardToRegularExpression (crtBlckUrlReg); // Conver to regular expression.
            QRegularExpression rx(regexpPcre); //正则式。
            // rx.setPatternSyntax(QRegExp::Wildcard); //设置为通配符模式。

            QRegularExpressionValidator v(rx,0); // 校验器。
            int pos=0; //匹配的起始位置。

            if (v.validate(peerUrl,pos)== QRegularExpressionValidator::Acceptable) // 匹配这个正则式。
            {
                result=true; //应当封锁。

                break; //跳出。
            } //if (v.validate(peerUrl)==QRegExpValidator::Acceptable) //匹配这个正则式。

        } //foreach(crtBlckUrlReg,adBlckUrlSt) //一个个正则式地尝试匹配。

    } //if (_enableAdBlock) //启用广告禁用功能。


    return result;
} //bool StbProxyFactory::shouldBlockUrl(QString peerUrl) //!<该URL是否要封锁。

/*!
 * \brief StbProxyFactory::setAllUsingProxy 设置是否要all使用代理。
 * \param stngDlgisAllUsingProxy 是否要all使用代理。
 */
void StbProxyFactory::setAllUsingProxy (bool stngDlgisAllUsingProxy )
{
    allUseProxy=stngDlgisAllUsingProxy; //Remember the value.

    return;
} //void StbProxyFactory::setAllUsingProxy (bool stngDlgisAllUsingProxy )

/*!
 * \brief StbProxyFactory::setAutoAddProxyRule 设置是否自动添加代理规则。
 * \param stngDlgisAutoAddProxyRule 是否自动添加代理规则。
 */
void StbProxyFactory::setAutoAddProxyRule (bool stngDlgisAutoAddProxyRule)
{
    autoAddRule=stngDlgisAutoAddProxyRule; //记录。

    return;
} //void StbProxyFactory::setAutoAddProxyRule (bool stngDlgisAutoAddProxyRule)

/*!
 * \brief StbProxyFactory::removeAdblockUrl 删除该规则。
 * \param websiteDomain 要删除的规则。
 */
void StbProxyFactory::removeAdblockUrl(QString websiteDomain)
{
    adBlckUrlSt.remove(websiteDomain); //从要封锁的URL集合中删除。

    saveAdBlockUrlSet(); //保存要封锁的URL列表。


    return;
} //void StbProxyFactory::removeAdblockUrl(QString websiteDomain)

