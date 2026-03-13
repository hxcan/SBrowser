extern "C"
{
#ifndef Q_OS_MACOS
#include <qrencode.h> //QRcode
#endif
} //extern "C"

//#include <png.h> //png_structp

#include "SbPreCompile.h" //qDebug

#include "QrHelper.h"

QrHelper::QrHelper(QObject *parent) :
    QObject(parent)
{
}

void QrHelper::GenerateQrImage(int ImgPtrid) //!<Generate the qr image.
{
    QString Str2Ecd; //The string to encode.
    Str2Ecd.setNum(ImgPtrid); //Set the number.

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    QRcode * qrcode; //The qrcode comtent.

    qrcode=encode(Str2Ecd.toUtf8().data()); //Encode.

    QString OtptFlNm=QString("QrCodeImages/%1.png").arg(ImgPtrid); //Output file name.

    SaveQrCodeImage(qrcode,OtptFlNm); //Save the qr code into a png file.

    QRcode_free(qrcode); //Release the memory.
#endif
#endif
} //void QrHelper::GenerateQrImage(int ImgPtrid) //!<Generate the qr image.

/*!
 * \brief QrHelper::GenerateQrImage  生成二维码图片。
 * \param ImgPtrid 要为之生成图片的字符串。
 */
QImage QrHelper::GenerateQrImage(QString ImgPtrid, QImage bgImage, TransparentMode transparentMode, bool shouldShowGrid,int targetImageWidth)
{
  auto bgImageInUse=bgImage; //The bg image in use.
  QImage result; //结果。

  QString Str2Ecd; //The string to encode.
  Str2Ecd=ImgPtrid; //给字符串赋值。

#ifndef Q_OS_MACOS
  QRcode * qrcode; //The qrcode comtent.

  QUrl url2Encode=QUrl(Str2Ecd); //构造URL。

  auto encodedUrl=url2Encode.toEncoded (); //编码之后的网址。
    
  encodedUrl=encodedUrl.left(MaxUrlLength); //截断过长的内容。

#ifdef Q_OS_UNIX
  qrcode=encode(encodedUrl.data()); //Encode.

  if (qrcode) //生成成功。
  {
    result=qRcode2QImage(qrcode,transparentMode,targetImageWidth); //将结果转换成图片。

    if (bgImageInUse.isNull ()) //未传入背景图片。则将纯黑色图片作为背景图片。
    {
      bgImageInUse=QImage(targetImageWidth,targetImageWidth,QImage::Format_RGB32); //创建背景图片。
      bgImageInUse.fill (qRgb(0,0,0)); //以纯黑色填充。
    } //if (bgImage.isNull ()) //未传入背景图片。
    else //传入背景图片，则叠加二维码到背景图片上。
    {
      const auto TooBigWidth=1740; //太大的图片不缩放

      if (bgImageInUse.width()>=TooBigWidth) //图片过大
      {
        bgImageInUse=bgImageInUse.copy(0, 0, targetImageWidth, targetImageWidth); //Copy the square area.
      } //if (bgImageInUse.width()>=TooBigWidth) //图片过大
      else //图片不大
      {
        bgImageInUse=bgImageInUse.scaled (targetImageWidth,targetImageWidth, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); //缩放到合适的尺寸。
      } //else //图片不大

      if (bgImageInUse.size()!=QSize(targetImageWidth, targetImageWidth)) //Size not match.
      {
        bgImageInUse=bgImageInUse.copy(0, 0, targetImageWidth, targetImageWidth); //Copy the square area.
      } //if (bgImage.size()!=QSize(targetImageWidth, targetImageWidth)) //Size not match.
    } //else //传入背景图片，则叠加二维码到背景图片上。

    result=imageOverlay(result, bgImageInUse); //将二维码叠加到背景图片上。

    if (shouldShowGrid) //应当显示网格。
    {
      result=drawGrid(result,qrcode,targetImageWidth); //绘制网格。
    } //if (shouldShowGrid) //应当显示网格。

    QRcode_free(qrcode); //Release the memory.
  } //if (qrcode) //生成成功。
  else //生成失败。
  {
    result=QImage(256,256,QImage::Format_RGB32); //创建图片。
    result.fill (Qt::green); //填充绿色。
    QPainter errPntr(&result); //创建绘图器。

    errPntr.drawText (20,20,tr("QR code generating failed")); //绘制，生成失败。

    errPntr.end (); //绘制完成。
  } //else //生成失败。
#endif

#endif

  return result;
} //void QrHelper::GenerateQrImage(int ImgPtrid) //!<Generate the qr image.

#ifdef Q_OS_UNIX
QRcode * QrHelper::encode(char *intext)
{
    QRcode *code;

    int version=0; //Version of qr code.
    QRecLevel level=QR_ECLEVEL_H; //The level.
    QRencodeMode hint=QR_MODE_8; //The hint
    int casesensitive = 1;

    code = QRcode_encodeString(intext, version, level, hint, casesensitive);

    return code;
}
#endif

/*!
 * \brief StoreListWidget::SaveQrCodeImage Save the qr code into a png file.
 * \param qrcode The qr code to save.
 * \param OtptFlNm The output file name.
 */
void QrHelper::SaveQrCodeImage(QRcode * qrcode,QString OtptFlNm)
{


    int RwCtr=0; //Row counter.
    int Wdth=qrcode->width; //The width of qr code.
    int ClCtr=0; //Column counter.
    QImage CrtImg=QImage(Wdth+2,Wdth+2,QImage::Format_RGB32); //The image.

    CrtImg.setDotsPerMeterX(800*100/2.54); //Set the dpi.
    CrtImg.setDotsPerMeterY(800*100/2.54); //Set the dpi.
    QRgb CrtClr1; //Current color.
    CrtClr1=qRgb(255,255,255); //White.
    CrtImg.fill(CrtClr1); //Fill with white.

    for(RwCtr=0;RwCtr<Wdth;RwCtr++) //Row by row.
    {
        for (ClCtr=0;ClCtr<Wdth;ClCtr++) //Column by column.
        {
            QRgb CrtClr; //Current color.

            if (( (qrcode->data)[RwCtr*Wdth+ClCtr] & 1 ) == 1) //Black.
            {
                CrtClr=qRgb(0,0,0); //Black.
            } //if (( (qrcode->data)[RwCtr*Wdth+ClCtr] & 1 ) == 1) //Black.
            else //White.
            {
                CrtClr=qRgb(255,255,255); //White.
            } //else //White.



            CrtImg.setPixel(ClCtr+1,RwCtr+1,CrtClr); //Set the color of current point.
        } //for (ClCtr=0;ClCtr<Wdth;ClCtr++) //Column by column.

    } //for(RwCtr=0;RwCtr<Wdth;RwCtr++) //Row by row.

    CrtImg=CrtImg.scaledToWidth(512); //scale.


    CrtImg.save(OtptFlNm); //Save the image.

    return;
} //void StoreListWidget::SaveQrCodeImage(QRcode * qrcode,QString OtptFlNm) //!<Save the qr code into a png file.

/*!
 * \brief QrHelper::qRcode2QImage 将结果转换成图片。
 * \param qrcode 要转换的二维码数据。
 * 将二维码数据中的1绘制成背景色，将二维码数据中的0绘制成前景色。
 * \return 转换得到的图片。
 */
QImage QrHelper::qRcode2QImage(QRcode * qrcode, TransparentMode transparentMode,int targetImageWidget)
{
    QRgb bgColor; //背景色。
    QRgb fgColor; //前景色。

    if (transparentMode==BlackAsTransparent) //黑色变成透明，即使用透明色作为背景色。
    {
        bgColor=qRgba(0,0,0,0); //背景色设为透明色。
        fgColor=qRgba(255,255,255,255); //前景色设为白色。
    } //if (transparentMode==BlackAsTransparent) //黑色变成透明，即使用透明色作为背景色。
    else //白色变成透明，即使用透明色作为前景色。
    {
        fgColor=qRgba(0,0,0,0); //前景色设为透明色。
        bgColor=qRgba(0,0,0,255); //背景色设为黑色。
    } //else //白色变成透明，即使用透明色作为前景色。

    int RwCtr=0; //Row counter.
    int Wdth=qrcode->width; //The width of qr code.
    int ClCtr=0; //Column counter.
    QImage CrtImg=QImage(Wdth+2,Wdth+2,QImage::Format_ARGB32); //The image.

    CrtImg.setDotsPerMeterX(800*100/2.54); //Set the dpi.
    CrtImg.setDotsPerMeterY(800*100/2.54); //Set the dpi.
    QRgb CrtClr1; //Current color.
    CrtClr1=fgColor; //前景色。
    CrtImg.fill(CrtClr1); //Fill with white.


    QVariantList qrPointList; //二维码点列表。
    QVariantMap currentWhitePoint; //当前白色点。
    QVariantMap currentBlackPoint; //当前黑色点。

    currentBlackPoint["color"]="#000000"; //黑色。

    //写入一行白色点：
    for(int whitePointCounter=0;whitePointCounter<(Wdth+2);whitePointCounter++) //一个个点地输出。
    {

        currentWhitePoint["x"]=whitePointCounter; //X坐标。
        currentWhitePoint["y"]=0; //y坐标。
        currentWhitePoint["color"]="#ffffff"; //白色。


        qrPointList << currentWhitePoint; //加入当前白色点。
    } //for(int whitePointCounter=0;whitePointCounter<(Wdth+2);whitePointCounter++) //一个个点地输出。


    for(RwCtr=0;RwCtr<Wdth;RwCtr++) //Row by row.
    {
        currentWhitePoint["x"]=0; //X坐标。
        currentWhitePoint["y"]=RwCtr+1; //y坐标。
        qrPointList << currentWhitePoint; //加入当前白色点。


        for (ClCtr=0;ClCtr<Wdth;ClCtr++) //Column by column.
        {
            QRgb CrtClr; //Current color.

            if (( (qrcode->data)[RwCtr*Wdth+ClCtr] & 1 ) == 1) //Black.
            {
                CrtClr=bgColor; //背景色。

                currentBlackPoint["x"]=ClCtr+1; //X坐标。
                currentBlackPoint["y"]=RwCtr+1; //Y坐标。

                qrPointList << currentBlackPoint; //加入当前黑色点。
            } //if (( (qrcode->data)[RwCtr*Wdth+ClCtr] & 1 ) == 1) //Black.
            else //White.
            {
                CrtClr=fgColor; //前景色。

                currentWhitePoint["x"]=ClCtr+1; //X坐标。
                currentWhitePoint["y"]=RwCtr+1; //Y坐标。

                qrPointList << currentWhitePoint; //加入当前白色点。
            } //else //White.



            CrtImg.setPixel(ClCtr+1,RwCtr+1,CrtClr); //Set the color of current point.
        } //for (ClCtr=0;ClCtr<Wdth;ClCtr++) //Column by column.

        currentWhitePoint["x"]=Wdth+1; //X坐标。

        qrPointList << currentWhitePoint; //加入当前白色点。

    } //for(RwCtr=0;RwCtr<Wdth;RwCtr++) //Row by row.

    //写入一行白色点：
    for(int whitePointCounter=0;whitePointCounter<(Wdth+2);whitePointCounter++) //一个个点地输出。
    {
        QVariantMap currentWhitePoint; //当前白色点。

        currentWhitePoint["x"]=whitePointCounter; //X坐标。
        currentWhitePoint["y"]=Wdth+1; //y坐标。
        currentWhitePoint["color"]="#ffffff"; //白色。


        qrPointList << currentWhitePoint; //加入当前白色点。
    } //for(int whitePointCounter=0;whitePointCounter<(Wdth+2);whitePointCounter++) //一个个点地输出。


    QString ssnStrJs=QDir::homePath()+"/.sbrowser/qrcodearray.json"; //会话记录的JSON文件名。
    QFile ssnStrFl(ssnStrJs); //JSON文件。
    ssnStrFl.open(QIODevice::WriteOnly); //打开文件。

    QVariantMap qrPointListObject; //二维码的映射对象。
    qrPointListObject["qrcodearray"]=qrPointList; // 设置数组对象。

    QJsonObject gameObject=QJsonObject::fromVariantMap (qrPointListObject); //转换成QJSON对象。
    QJsonDocument saveDoc(gameObject); //JSON文档。


    ssnStrFl.write (saveDoc.toJson ()); //输出内容。

    ssnStrFl.close(); //关闭文件。



    CrtImg=CrtImg.scaledToWidth(targetImageWidget); //scale.

    return CrtImg;
} //QImage QrHelper::qRcode2QImage(QRcode * qrcode) //!<将结果转换成图片。

/*!
 * \brief QrHelper::imageOverlay 将二维码叠加到背景图片上。
 * \param result 二维码图片。
 * \param bgImage 背景图片。
 * \return 叠加之后的图片。
 */
QImage QrHelper::imageOverlay(QImage result,QImage bgImage)
{
    QImage result2Ret=bgImage; //结果。

    if (result2Ret.format ()==QImage::Format_Indexed8) //颜色为索引颜色，则不支持绘图操作，需要转换成红绿蓝颜色。
    {
        result2Ret=result2Ret.convertToFormat (QImage::Format_RGB32); //转换颜色格式。
    } //if (result2Ret.format ()==QImage::Format_Indexed8) //颜色为索引颜色，则不支持绘图操作，需要转换成红绿蓝颜色。

    QPainter errPntr(&result2Ret); //创建绘图器。

    errPntr.drawImage (0,0,result); //叠加图片。

    errPntr.end (); //绘制完成。


    return result2Ret;
} //QImage QrHelper::imageOverlay(QImage result,QImage bgImage)

/*!
 * \brief QrHelper::drawGrid 绘制网格。
 * \param qrImage 要在其上绘制网格的二维码图片。
 * \param qrcode 二维码数据。
 * \return 绘制之后的图片。
 */
QImage QrHelper::drawGrid(QImage qrImage,QRcode * qrcode,int targetImageWidth)
{
    QImage result(qrImage); //结果。

    int pointAmount=qrcode->width; //每一行的点数。
    QPainter linePainter(&result); //创建绘图器。

    float imageWidth=(float)(targetImageWidth); //图片宽度。

    float pointWidth=imageWidth/(  (float)(pointAmount+2) ); //计算出点的宽度。

    QPen darkAndThickPen; //加粗加深的画笔。
    darkAndThickPen.setColor (Qt::darkGreen); //深绿色。
    darkAndThickPen.setWidth (3); //加粗。

    QPen normalGreenPen(Qt::green); //普通的绿色画笔。


    for(int columnCounter=0;columnCounter<(pointAmount+3);columnCounter++) //一条条地绘制竖线。
    {
        if ((columnCounter % 10 ) == 0) //每10条线，加粗加深。
        {
            linePainter.setPen (darkAndThickPen); //加粗加深。
        } //if ((columnCounter % 10 ) == 0) //每10条线，加粗加深。
        else //普通画笔。
        {
            linePainter.setPen (normalGreenPen); //普通的绿色画笔。
        } //else //普通画笔。


        QPointF lineTop( (float)(columnCounter) * pointWidth , 0.0  ); //线的顶端。
        QPointF lineBottom( (float)(columnCounter) * pointWidth , imageWidth-1.0  ); //线的底端。
        linePainter.drawLine (lineTop,lineBottom); //画线。
    } //for(columnCounter=0;columnCounter<(pointAmount+2);columnCounter++) //一条条地绘制竖线。

    //绘制横线：
    for(int rowCounter=0;rowCounter<(pointAmount+3);rowCounter++) //一条条地绘制横线。
    {
        if ((rowCounter % 10 ) == 0) //每10条线，加粗加深。
        {
            linePainter.setPen (darkAndThickPen); //加粗加深。
        } //if ((columnCounter % 10 ) == 0) //每10条线，加粗加深。
        else //普通画笔。
        {
            linePainter.setPen (normalGreenPen); //普通的绿色画笔。
        } //else //普通画笔。

        QPointF lineLeft(  0.0, (float)(rowCounter) * pointWidth  ); //线的左端。
        QPointF lineRight(   imageWidth-1.0 ,(float)(rowCounter) * pointWidth ); //线的右端。
        linePainter.drawLine (lineLeft,lineRight); //画线。
    } //for(columnCounter=0;columnCounter<(pointAmount+2);columnCounter++) //一条条地绘制竖线。



    linePainter.end (); //绘制完成。

    return result;
} //QImage QrHelper::drawGrid(QImage qrImage,QRcode * qrcode)
