#include "SbPreCompile.h" //qDebug

#include "QrHelper.h"
#include "QrBackgroundImageNormalizer.h" //QrBackgroundImageNormalizer

#include "mapReduceFunctions.h" //addToSubDirList

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
#include "FileTimeComparator.h" //FileTimeComparator
#endif
#endif

#include "QrRemoteDecoder.h" //QrRemoteDecoder


QrRemoteDecoder::QrRemoteDecoder(QObject *parent) : QObject(parent)
{
    initializeMembers(); //初始化成员变量。

    startWorkTimer.start(); //启动enet轮询定时器。

    connectSignals(); //连接信号槽。
}

/*!
 * \brief ThumbnailsDeleter::initializeMembers 初始化成员变量。
 */
void QrRemoteDecoder::initializeMembers()
{
    startWorkTimer.setInterval (30); //30ms轮询一次。
    startWorkTimer.setSingleShot (true); //单发。

    networkAccessManager=new QNetworkAccessManager(this); //重新创建网络访问管理器。
    
    dummyProxy.setType(QNetworkProxy::NoProxy); //不使用代理。
    
    networkAccessManager->setProxy(dummyProxy); //设置直接连接的代理。
} //void ThumbnailsDeleter::initializeMembers()

/*!
 * \brief FakeImageEliminator::Delete1File Delete timer timed out,then delete 1 file.
 */
void QrRemoteDecoder::Delete1File()
{
    QString qrCodeContent="else //尺寸不大，则尝试生成二维码，并且让调码扫描器尝试识别。"; //生成二维码的内容。
                        
    setUrl(qrCodeContent); //生成二维码。
                        
    QString fileName;
    sendAvatar(); //发送二维码，由调码扫描器尝试解码。
} //void FakeImageEliminator::Delete1File() //!<Delete timer timed out,then delete 1 file.

/*!
 * \brief SWebPageWidget::sendAvatar 向服务器发送头像文件。
 * \param fileName 头像文件名。
 */
void QrRemoteDecoder::sendAvatar()
{
    QString httpPrefix="http://"; //http前缀。瑞哥此刻的婚恋状态是什么？
    QString serverIpPort=QString("%1:%2").arg(sarCodeScannerServiceAddress).arg(sarCodeScannerServicePort); //服务器IP及端口号。蔡火胜。
    QString commitTextCommand="/decodeQr/"; //发送头像文件的命令。
    QString wholeUrl=httpPrefix+serverIpPort+commitTextCommand; //构造URL。
    
    QUrlQuery srchQry; //查询对象。
    
    
    QByteArray imageContent;
    QBuffer buffer(&imageContent);
    buffer.open(QIODevice::WriteOnly);
    generatedQrcodeImage.save(&buffer, "PNG") ; //生成文件内容。
     
    buffer.close();
     
     
    QByteArray srchKyWrdPcntEcd=imageContent.toBase64(); //做base64编码。
    
    srchQry.addQueryItem ("image",srchKyWrdPcntEcd); //设置查询条件。


    QUrl searchUrl(wholeUrl); //构造URL。
    searchUrl.setQuery (srchQry); //设置查询对象。

    QNetworkRequest imageUploadRequest(searchUrl); //图片上传请求。
    
    

    QNetworkReply * commitTextReply= networkAccessManager->get ( imageUploadRequest ); //发起请求。

    connect(commitTextReply,&QNetworkReply::finished,this,&QrRemoteDecoder::parseSendAvatarReply); //请求结束，则处理结果。
    connect(commitTextReply, &QNetworkReply::finished, commitTextReply, &QNetworkReply::deleteLater); //请求结束，则延迟释放资源。
    
} //void SWebPageWidget::sendAvatar(QString fileName)

/*!
 * \brief QrRemoteDecoder::modifyTimestampByAppendingBytes 追加字节，“黄映如”。
 */
void QrRemoteDecoder::modifyTimestampByAppendingBytes()
{
  QFile imageFile(materialFileName);
    
  imageFile.open(QIODevice::Append);

  QTextStream imageStream(&imageFile);

  imageStream << tr("HuangYingRu");

  imageFile.close();
} //void QrRemoteDecoder::modifyTimestampByAppendingBytes()

/*!
 * \brief SWebPageWidget::parseSendAvatarReply 请求结束，则处理结果。
 */
void QrRemoteDecoder::parseSendAvatarReply()
{
    QNetworkReply * reply=(QNetworkReply*)sender(); //获取回复对象。

    QByteArray wholeContent=reply->readAll (); //读取全部内容。
    
    QJsonDocument loadDoc(QJsonDocument::fromJson(wholeContent)); //创建JSON文档。

    QJsonObject objVrt=loadDoc.object(); //将载入的JSON文档转换成JSON对象。

    QVariantMap obj=objVrt.toVariantMap (); //转换成映射。

    qDebug() << __FILE__ << __LINE__ << __func__ << tr("whole map:") << obj; //Debug.

    
    if (reply->error()) //出现过错误。
    {
        emit finished();
    } //if (reply->error()) //出现过错误。
    else //请求成功完成。
    {
        bool success=obj["success"].toBool (); //获取是否成功。
    
        if (success) //成功。
        {
            //更新该文件的时间戳，向末尾追加若干个字节：
            modifyTimestampByAppendingBytes(); //追加字节，“黄映如”。
            
            emit finished();
        } //if (success) //成功。
        else //失败。
        {
            if (transparentMode==BlackAsTransparent) //是黑点透明模式。失败。则还需要尝试白点透明模式。
            {
                toggleForegroundTransparent(); //切换前景透明。
                
                sendAvatar(); //再次发送请求，尝试解码。
            } //if (transparentMode==BlackAsTransparent) //是黑点透明模式。失败。则还需要尝试白点透明模式。
            else //白点透明模式也失败，则删除文件。
            {
                QFile CrtFl(materialFileName); //Create the file object.
    
                CrtFl.remove(); //Remove the file.
                
                emit finished();
            } //else //白点透明模式也失败，则删除文件。
        } //else //失败。
    } //else //请求成功完成。
} //void SWebPageWidget::parseSendAvatarReply()

/*!
 * \brief QrCodeDialog::toggleForegroundTransparent 模式切换为前景色透明。
 */
void QrRemoteDecoder::toggleForegroundTransparent()
{
    transparentMode=WhiteAsTransparent; //透明模式切换为，前景透明。

    generateQr (); //重新生成。
} //void QrCodeDialog::toggleForegroundTransparent()

/*!
 * \brief QrCodeDialog::setUrl 设置二维码。
 * \param urlBrtext 要设置的二维码对应的网址。
 */
void QrRemoteDecoder::setUrl(QString urlBrtext )
{
    url=urlBrtext; //记录URL。
    transparentMode=BlackAsTransparent; //黑色的点作为透明颜色使用。


    QImage CrtPxmp; //The pixmap object.用于尝试载入图片文件的图片对象。

    QString flNm=materialFileName; //当前要用作背景的图片文件名。

    usedImageFileName=flNm; //记录曾经用过的图片文件名。

    CrtPxmp.load(flNm); //尝试载入。

    backgroundPixmap=CrtPxmp; //记录背景图片。

    generateQr (); //生成二维码。
} //void QrCodeDialog::setUrl(QString urlBrtext ) //!<设置二维码。

/*!
 * \brief QrCodeDialog::generateQr 生成二维码。
 */
void QrRemoteDecoder::generateQr()
{
#ifndef Q_OS_MACOS
    //生成二维码：
    QrHelper QrHelper1;
    QImage qrImg=QrHelper1.GenerateQrImage(url,backgroundPixmap,transparentMode,showGrid,targetImageWidth);//参数为URL字符串。生成二维码图片。生成普通的二维码图片。

    generatedQrcodeImage=qrImg; //记录刚才生成的二维码图片。
    
    QFileInfo imageFile(materialFileName);
    QString baseName=imageFile.baseName();
    
    generatedQrcodeImage.save(QString("/root/Temp/debug_7c562b9fa53c76e0f7abf1eebc68c69facdf92f5/%1.%2.png").arg(transparentMode).arg(baseName)); //Debug.
#endif
}

void QrRemoteDecoder::setSarCodeScannerServiceAddress(const QString &value)
{
    sarCodeScannerServiceAddress = value;
}

void QrRemoteDecoder::setSarCodeScannerServicePort(const quint16 &value)
{
    sarCodeScannerServicePort = value;
} //void QrCodeDialog::generateQr()

/*!
 * \brief ThumbnailsDeleter::connectSignals 连接信号槽。
 */
void QrRemoteDecoder::connectSignals()
{
    connect (&startWorkTimer,&QTimer::timeout,this,&QrRemoteDecoder::startWork); //定时器超时，则开始工作。
} //void ThumbnailsDeleter::connectSignals()

/*!
 * \brief ThumbnailsDeleter::startWork 开始工作。
 */
void QrRemoteDecoder::startWork()
{
    Delete1File(); //处理。
} //void ThumbnailsDeleter::startWork()

void QrRemoteDecoder::setMaterialFileName(const QString &value)
{
    materialFileName = value;
}
