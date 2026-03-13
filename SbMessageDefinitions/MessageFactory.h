#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include "TdPreCompile.h" //QNetworkRequest

#ifndef Q_OS_MACOS
#include "CreditDataMessage.pb.h" // CreditDataMessage

using namespace com::stupidbeauty::sbrowser; // CreditDataMessage
#endif


class MessageFactory : public QObject
{
  Q_OBJECT

public:
#ifndef Q_OS_MACOS

  QByteArray constructCreditDataCbor(QMap<QString, FileCreditMessage *> fileNameCreditMap); //!< Serialize to cbor.
#endif

  QByteArray constructZoomFactorMessage(const QMap<QString, qreal> & hostNameZoomFactorMap); //!< 构造消息，缩放比例映射。陈欣
  QByteArray constructRequestAssessOneStockMessage(const QCborMap & request); //!<构造消息，要求评估一个股票

private:
  int avatarWidth;
  int avatarHeight;
  int avatarCameraX=0;
  int avatarCameraY=0;
  float cameraX=0;
  float cameraY=0;
  float cameraZ=1200;
}; //class DisplayHead : public QWidget

#endif
