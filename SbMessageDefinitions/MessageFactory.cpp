#include "TdPreCompile.h" //QTimer

#include "MessageFactory.h" //MessageFactory

/*!
 * \brief MessageFactory::constructRequestAssessOneStockMessage 构造消息，要求评估一个股票
 * \param request 请求信息对象
 * \return 构造得到的消息体
 */
QByteArray MessageFactory::constructRequestAssessOneStockMessage(const QCborMap & request)
{
    QByteArray result; //

    result=request.toCborValue ().toCbor (); //编码

    return result;
} //QByteArray MessageFactory::constructRequestAssessOneStockMessage(const QCborMap & request)

#ifndef Q_OS_MACOS

/*!
 * \brief MessageFactory::constructCreditDataCbor Serialize to cbor.
 * \param creditDataMessage Credit data as a whole.
 * \return Constructed byte array.
 */
QByteArray MessageFactory::constructCreditDataCbor(QMap<QString, FileCreditMessage *> fileNameCreditMap)
{
  QCborArray creditDataMessage; // 整个映射消息.

  QMapIterator<QString, FileCreditMessage *> i(fileNameCreditMap); // The map iterator.
  while (i.hasNext())
  {
    i.next();

    QCborMap currentMapEntry; // 单个映射条目。

    currentMapEntry.insert(QStringLiteral("fileName"), i.key()); // 加入 file 名。
    currentMapEntry.insert(QStringLiteral("rangeLow"), i.value()->rangelow()); // 加入 range low.
    currentMapEntry.insert(QStringLiteral("rangeHigh"), i.value()->rangehigh()); // 加入 range low.
    currentMapEntry.insert(QStringLiteral("creditAmount"), i.value()->creditamount()); // 加入 range low.

    creditDataMessage << currentMapEntry; // 加入列表中.
  }

  auto datagram=creditDataMessage.toCborValue().toCbor(); // 序列化.

  return datagram;
} // QByteArray MessageFactory::constructCreditDataCbor(CreditDataMessage creditDataMessage)
#endif

/*!
 * \brief MessageFactory::constructZoomFactorMessage 构造消息，缩放比例映射。陈欣
 * \param hostNameZoomFactorMap 缩放比例映射对象。
 * \return 构造得到的消息体
 */
QByteArray MessageFactory::constructZoomFactorMessage(const QMap<QString, qreal> & hostNameZoomFactorMap)
{
  QCborArray creditDataMessage; // 整个映射消息.

  QMapIterator<QString, qreal> i(hostNameZoomFactorMap);
  while (i.hasNext())
  {
    i.next();

    QCborMap currentMapEntry; // 单个映射条目。

    currentMapEntry.insert(QStringLiteral("hostName"), i.key()); // 加入主机名。
    currentMapEntry.insert(QStringLiteral("zoomFactor"), i.value()); // 加入缩放比例

    creditDataMessage << currentMapEntry; // 加入列表中.
  }

  auto datagram=creditDataMessage.toCborValue().toCbor(); // 序列化.

  return datagram;
} //QByteArray MessageFactory::constructZoomFactorMessage(const QMap<QString, qreal> & hostNameZoomFactorMap)
