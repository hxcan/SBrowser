#ifndef STBPROXYFACTORY_H
#define STBPROXYFACTORY_H

#include <QNetworkProxyFactory>

class StbProxyFactory : public QNetworkProxyFactory
{
private:
    quint16 smartProxyPortHttp; //!<智能代理的HTTP端口号。
    bool useProxyForDns=false; //!<是否通过代理解析DNS。
    bool _enableAdBlock=true; //!<是否启用广告禁用功能。
    bool autoAddRule; //!<是否应当自动添加规则。蔡火胜
    bool allUseProxy; //!<Whether we should use proxy for all the hosts.
    bool shouldBlockUrl(QString peerUrl); //!<该URL是否要封锁。
    void loadAdBlockUrlSet(); //!<载入需要封锁的URL集合。
    void saveAdBlockUrlSet(); //!<保存要封锁的URL列表。
    QSet <QString> adBlckUrlSt; //!<要封锁的URL集合。
    void loadExternalProxyHostSet(); //!<载入外部需代理主机名列表。
    void saveExternalProxyHostSet(); //!<保存内部需代理的主机名列表。
    QSet <QString> extnlPrxHstSt; //!<外部的需代理主机名集合。
    QSet<QString> intnlPrxyHstNmSt; //!<内部的需代理主机名集合。
    bool usngSmtPrxy; //!<是否要使用智能代理。
    QString smartProxyHost="127.0.0.1"; //!<智能代理主机名。
    quint16 usnSmtPrxyPrt; //!<要使用的智能代理的端口号。

public:
    void setSmartProxyHost(QString stngDlggetSmartProxyHost); //!<设置智能代理的主机名。
    void setEnableAdBlock(bool enableAdBlock); //!<是否启用广告禁用功能。
    void removeAdblockUrl(QString websiteDomain); //!<删除该规则。
    void setAutoAddProxyRule (bool stngDlgisAutoAddProxyRule); //!<设置是否自动添加代理规则。
    void setAllUsingProxy (bool stngDlgisAllUsingProxy ); //!<设置是否要all使用代理。
    QStringList getAdBlockUrls(); //!<获取需要封锁的URL列表。
    void addAdBlockUrl (QString CrtPt); //!<加入一个要封锁的URL。
    QStringList getWebsiteListNeedsProxy(); //!<获取需要通过代理访问的网站列表。
    bool isUsingProxyForHostname(QString hstNm); //!<是否在为此主机使用代理。
    void removeHostname(QString hstNm); //!<从代理规则中删除。
    void addHostname(QString hstNm); //!<加入到代理规则中。
    void setSmartProxyPort(quint16 smtPrxyPrt); //!<设置智能代理的端口号。
    void setUsingSmartProxy(bool crtSz); //!<设置是否要使用智能代理。
    QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query = QNetworkProxyQuery()); //!<查询代理规则。
    bool isBlockingUrl(QUrl inforequestUrl); //!<是否应当阻止此网址。
    explicit StbProxyFactory(); //!<构造函数。
    bool getUseProxyForDns() const;
    void setUseProxyForDns(bool value);
    quint16 getSmartProxyPortHttp() const;
    void setSmartProxyPortHttp(const quint16 &value);
};

#endif // STBPROXYFACTORY_H
