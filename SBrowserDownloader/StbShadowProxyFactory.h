#ifndef STBSHADOWPROXYFACTORY_H
#define STBSHADOWPROXYFACTORY_H

//#include <QNetworkProxyFactory>

class StbShadowProxyFactory : public QNetworkProxyFactory
{
private:
  QNetworkProxyFactory * proxyInformationSource; //!<代理规则的数据来源。

public:
    QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query = QNetworkProxyQuery()); //!<查询代理规则。
    StbShadowProxyFactory() = delete; //!< Forbid the default constructor.
    StbShadowProxyFactory(QNetworkProxyFactory * informationSource); //!<构造函数，关联到另一个代理工厂，以那个代理工厂作为信息来源。
}; // class StbShadowProxyFactory : public QNetworkProxyFactory

#endif // STBPROXYFACTORY_H
