#include "SbPreCompile.h" //cout

// #include <QRegExpValidator> //QRegExpValidator

#include "StbProxyFactory.h"
#include "StbShadowProxyFactory.h" //StbShadowProxyFactory

/*!
 * \brief StbProxyFactory::queryProxy 查询代理规则。
 * \param query 查询对象。
 * \return 对应的代理规则列表。
 */
QList<QNetworkProxy> StbShadowProxyFactory::queryProxy(const QNetworkProxyQuery &query)
{
    QList<QNetworkProxy> result; //结果。

    result=proxyInformationSource->queryProxy(query);

    return result;
} //QList<QNetworkProxy> StbProxyFactory::queryProxy(const QNetworkProxyQuery &query = QNetworkProxyQuery()) //!<查询代理规则。

/*!
 * \brief StbShadowProxyFactory::StbShadowProxyFactory 构造函数，关联到另一个代理工厂，以那个代理工厂作为信息来源。
 * \param informationSource 真正的代理规则数据来源。
 */
StbShadowProxyFactory::StbShadowProxyFactory(QNetworkProxyFactory * informationSource)
{
    proxyInformationSource=informationSource;

} //StbShadowProxyFactory::StbShadowProxyFactory(QNetworkProxyFactory * informationSource)
