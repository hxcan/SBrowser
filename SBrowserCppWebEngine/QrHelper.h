#ifndef QRHELPER_H
#define QRHELPER_H

//#include <QObject>

#include "SbPreCompile.h" //QRcode

#include "Constants.h" //TransparentMode

class QrHelper : public QObject
{
  Q_OBJECT

public:
  void GenerateQrImage(int ImgPtrid); //!<Generate the qr image.
  QImage GenerateQrImage(QString ImgPtrid, QImage bgImage=QImage(), TransparentMode transparentMode=BlackAsTransparent, bool  shouldShowGrid=false, int targetImageWidth=512); //!<生成QR图片。
  explicit QrHelper(QObject *parent = nullptr); //!<构造函数。

private:
  qint32 MaxUrlLength=276; //!<截断过长的内容。

#ifndef Q_OS_MACOS
  QImage drawGrid(QImage qrImage, QRcode * qrcode, int targetImageWidth=512); //!<绘制网格。
  QImage qRcode2QImage(QRcode * qrcode, TransparentMode transparentMode=BlackAsTransparent, int targetImageWidget=512); //!<将结果转换成图片。
  void SaveQrCodeImage(QRcode * qrcode,QString OtptFlNm); //!<Save the qr code into a png file.
  QRcode * encode(char *intext); //!<Encode qr code.
#endif

  QImage imageOverlay(QImage result,QImage bgImage); //!<将二维码叠加到背景图片上。
}; //class QrHelper : public QObject

#endif // QRHELPER_H
