extern "C"
{
#ifndef Q_OS_MACOS
#include <qrencode.h> //QRcode
#endif
} //extern "C"

//#include <png.h> //png_structp

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

QrBackgroundImageNormalizer::QrBackgroundImageNormalizer(QObject *parent) : QObject(parent)
{
  initializeMembers(); //初始化成员变量。

  startWorkTimer.start(); //启动enet轮询定时器。

  connectSignals(); //连接信号槽。
}

/*!
 * \brief ThumbnailsDeleter::initializeMembers 初始化成员变量。
 */
void QrBackgroundImageNormalizer::initializeMembers()
{
  startWorkTimer.setInterval (30); //30ms轮询一次。
  startWorkTimer.setSingleShot (true); //单发。

  DltTimer.setInterval(1000/60); //Not to delete too quick.

  ThumbnailsDirName=QDir::homePath ()+"/.sbrowser/qrBackgroundImage/"; //缩略图目录名。
    
  networkAccessManager=new QNetworkAccessManager(this); //重新创建网络访问管理器。
    
  remoteDecodeTimer.setInterval(2343); //进行远程解码的延迟。
  remoteDecodeTimer.setSingleShot(true); //单发。
} //void ThumbnailsDeleter::initializeMembers()

/*!
 * \brief QrBackgroundImageNormalizer::remoteDecode1File 远程解码，测试一个文件。
 */
void QrBackgroundImageNormalizer::remoteDecode1File()
{
  if (remoteDecodeQueue.isEmpty()) //队列为空。
  {
  } //if (remoteDecodeQueue.isEmpty()) //队列为空。
  else //队列不为空。
  {
    QString CrtFlNm=remoteDecodeQueue.dequeue(); //Get the current file name.
        
    QrRemoteDecoder * qrRemoteDecoder=new QrRemoteDecoder(); //创建一个远程解码器，用于尝试对这张图片做二维码编码再解码。
                        
    qrRemoteDecoder->setMaterialFileName(CrtFlNm); //设置素材图片文件名。
    qrRemoteDecoder->setSarCodeScannerServicePort(sarCodeScannerServicePort); //设置扫描器的服务端口号。
    qrRemoteDecoder->setSarCodeScannerServiceAddress(sarCodeScannerServiceAddress); //设置扫描器的服务地址。
        
    connect(qrRemoteDecoder,&QrRemoteDecoder::finished,&remoteDecodeTimer,static_cast<void (QTimer::*)()>(&QTimer::start)); //处理完毕，则延时处理下一个。
  } //else //队列不为空。
} //void QrBackgroundImageNormalizer::remoteDecode1File()

/*!
 * \brief QrBackgroundImageNormalizer::loadImage 尝试载入图片。
 * \param CrtFlNm 图片文件名。
 * \return 载入的图片对象。
 */
QImage QrBackgroundImageNormalizer::loadImage(QString CrtFlNm)
{
    QImage result;
    
    bool loadResult=result.load(CrtFlNm);
    
    if (loadResult) //载入成功。
    {
        
    } //if (loadResult) //载入成功。
    else //载入失败。
    {
        result=loadEverythingAsImage.parseImage(CrtFlNm); //强行解析成图片。
        
    } //else //载入失败。
    
    
    return result;
} //QImage QrBackgroundImageNormalizer::loadImage(QString CrtFlNm)

/*!
 * \brief FakeImageEliminator::Delete1File Delete timer timed out,then delete 1 file.
 */
void QrBackgroundImageNormalizer::Delete1File()
{
  if (oldThumbnailFiles.isEmpty()) //The queue is empty.
  {
    DltTimer.stop(); //Stop the timer.
        
    if (sarCodeScannerServiceAddress.isNull()) //尚未寻找到调码扫描器。
    {
    } //if (sarCodeScannerServiceAddress.isNull()) //尚未寻找到调码扫描器。
    else //已经寻找到调码扫描器。
    {
      remoteDecode1File(); //远程解码，测试一个文件。
    } //else //已经寻找到调码扫描器。
  } //if (NonImgQueue.isEmpty()) //The queue is empty.
  else //The queue is not empty.
  {
    QString CrtFlNm=oldThumbnailFiles.dequeue(); //Get the current file name.

    QImage image; //尝试载入图片。
        
    image=loadImage(CrtFlNm); //尝试载入图片。

    if (image.isNull ()) //载入失败。
    {
      QFile CrtFl(CrtFlNm); //Create the file object.

      CrtFl.remove(); //Remove the file.
    } //if (image.isNull ()) //载入失败。
    else //载入成功。
    {
//            int targetImageWidth=412; //生成的目标二维码宽度。
//            int tooBigImageWidth=3634; //太大的图片宽度。

      if ((image.width ()>targetImageWidth) && (image.height ()>targetImageWidth)) //尺寸较大。
      {
        image=image.scaled ( targetImageWidth, targetImageWidth, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation ); //缩放。
                    
        //裁剪：
        int cropX=0;
        int cropY=0; //裁剪的起点。
                    
        if (image.width()>image.height()) //宽度较大。
        {
          auto randomValue=QRandomGenerator::global()->generate(); // 生成随机值。

          cropX=randomValue%(image.width()-targetImageWidth+1); //随机选择裁剪的X坐标。
        } //if (image.width()>image.height()) //宽度较大。
        else if (image.width()<image.height()) //高度较大。
        {
          auto randomValue=QRandomGenerator::global()->generate(); // 生成随机值。

          cropY=randomValue%(image.height()-targetImageWidth+1); //随机选择裁剪的X坐标。
                        
        } //else if (image.width()<image.height()) //高度较大。
                    
        image=image.copy(cropX,cropY,targetImageWidth,targetImageWidth); //复制局部。
    
        image.save (CrtFlNm+".jpg","JPG"); //写入图片。
                    
//                    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", saved scaled image: " << QString(CrtFlNm+".jpg").toStdString() << endl; //Debug.
                    

        QFile CrtFl(CrtFlNm); //Create the file object.

        CrtFl.remove(); //Remove the file.
      } //if ((image.width ()>targetImageWidth) && (image.height ()>targetImageWidth)) //尺寸较大。
      else //尺寸不大，则尝试生成二维码，并且让调码扫描器尝试识别。
      {
        remoteDecodeQueue << CrtFlNm; //加入到远程解码队列中。
      } //else //尺寸不大，则尝试生成二维码，并且让调码扫描器尝试识别。
    } //else //载入成功。
  } //else //The queue is not empty.
} //void FakeImageEliminator::Delete1File() //!<Delete timer timed out,then delete 1 file.

/*!
 * \brief QrCodeDialog::setUrl 设置二维码。
 * \param urlBrtext 要设置的二维码对应的网址。
 */
void QrBackgroundImageNormalizer::setUrl(QString urlBrtext )
{
    url=urlBrtext; //记录URL。
    transparentMode=BlackAsTransparent; //黑色的点作为透明颜色使用。


    QImage CrtPxmp; //The pixmap object.用于尝试载入图片文件的图片对象。

    QString ssnStrDir=QDir::homePath()+"/.sbrowser/qrBackgroundImage/"; //保存背景图片文件的目录。

    QDir candidateImagesDir(ssnStrDir); //包含着候选图片的目录。
    candidateImagesDir.setFilter(QDir::NoDotAndDotDot  | QDir::Files); //不要列出点和点点，列出文件。

    QStringList imageFileList=candidateImagesDir.entryList (); //图片文件列表。

    int imageFileListSize=imageFileList.size (); //文件列表的长度。

    QString flNm=""; //当前要用作背景的图片文件名。

    if (imageFileListSize>0) //存在着图片。
    {
        int chosenImageIndex= QRandomGenerator::system ()->generate () % imageFileListSize; //随机选中一张图片。

        flNm=QDir::homePath()+"/.sbrowser/qrBackgroundImage/"+imageFileList[chosenImageIndex]; //获取其文件名。

        usedImageFileName=flNm; //记录曾经用过的图片文件名。
    } //if (imageFileListSize>0) //存在着图片。

    CrtPxmp.load(flNm); //尝试载入。

    backgroundPixmap=CrtPxmp; //记录背景图片。

    generateQr (); //生成二维码。
} //void QrCodeDialog::setUrl(QString urlBrtext ) //!<设置二维码。

/*!
 * \brief QrCodeDialog::generateQr 生成二维码。
 */
void QrBackgroundImageNormalizer::generateQr()
{
#ifndef Q_OS_MACOS
    //生成二维码：
    QrHelper QrHelper1;
    QImage qrImg=QrHelper1.GenerateQrImage(url,backgroundPixmap,transparentMode,showGrid,targetImageWidth);//参数为URL字符串。生成二维码图片。生成普通的二维码图片。

    generatedQrcodeImage=qrImg; //记录刚才生成的二维码图片。
#endif
} //void QrCodeDialog::generateQr()

#ifndef Q_OS_MACOS
/*!
 * \brief PhoneWidget::processDiscoveredService 发现了服务，则处理。
 * \param serviceName 服务名字。
 * \param servicePort 服务端口。
 * \param serviceProtocolType 服务协议类型。
 */
void QrBackgroundImageNormalizer::processDiscoveredService(QString serviceName, quint16 servicePort, const QString &serviceProtocolType, string address)
{
    Q_UNUSED(serviceProtocolType); //不使用勒个参数。
    Q_UNUSED(serviceName);

    
    if (serviceName==SarCodeScannerServiceName) //正是要找的服务。
    {
        sarCodeScannerServicePort=servicePort; //记录服务端口号。
        sarCodeScannerServiceAddress=QString::fromStdString(address); //记录服务地址。
        
        remoteDecode1File(); //尝试做一次远程解码。
    } //if (serviceName==SarCodeScannerServiceName) //正是要找的服务。
}
#endif

/*!
 * \brief ThumbnailsDeleter::connectSignals 连接信号槽。
 */
void QrBackgroundImageNormalizer::connectSignals()
{
    connect (&startWorkTimer,&QTimer::timeout,this,&QrBackgroundImageNormalizer::startWork); //定时器超时，则开始工作。
    connect(&subDirsFutureWatcher,&QFutureWatcher<QStringList>::finished,this,&QrBackgroundImageNormalizer::scanDirsNextLevel); //扫描下一个层次的子目录。
    connect(&fileListFutureWatcher,&QFutureWatcher<QStringList>::finished,this,&QrBackgroundImageNormalizer::startRecognizeFile); //开始识别一个个的文件。

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    connect (rangeSortSearch.getSignalProxy (),&PaSignalProxy::finished,this,&QrBackgroundImageNormalizer::startDelete); //搜索完毕，则开始删除。
    connect(&servicePublisher,&ServicePublisher::serviceDiscovered,this,&QrBackgroundImageNormalizer::processDiscoveredService); //发现了服务，则处理。
#endif
#endif

    connect(&DltTimer, SIGNAL(timeout()), this, SLOT(Delete1File())); //Delete timer timed out,then delete 1 file.
    
    connect(&remoteDecodeTimer,&QTimer::timeout,this,&QrBackgroundImageNormalizer::remoteDecode1File); //解码定时器超时，则远程解码一个文件。

} //void ThumbnailsDeleter::connectSignals()

/*!
 * \brief SBrowserWindow::quit 退出。
 */
void QrBackgroundImageNormalizer::quit()
{
#ifndef Q_OS_MACOS
    servicePublisher.quit(); //退出。
#endif
} //void SBrowserWindow::quit()

/*!
 * \brief ThumbnailsDeleter::startDelete 搜索完毕，则开始删除。
 */
void QrBackgroundImageNormalizer::startDelete()
{
#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    QStringList oldFiles=rangeSortSearch.getResult (); //获取结果。

    oldThumbnailFiles.append(oldFiles);
#endif
#endif

    DltTimer.start(); //Start the delete timer.
} //void ThumbnailsDeleter::startDelete()

/*!
 * \brief ThumbnailsDeleter::startWork 开始工作。
 */
void QrBackgroundImageNormalizer::startWork()
{
    DirsQueue.enqueue(ThumbnailsDirName); //Add the target directory into the directories queue.

    scanningDirList.append(ThumbnailsDirName); //加入到当前正在扫描的目录列表中。

    scanSubDir(scanningDirList); //扫描子目录。
} //void ThumbnailsDeleter::startWork()

/*!
 * \brief FakeImageEliminator::scanSubDir 扫描子目录。
 * \param scanningDirList2Scan 要扫描其子目录的目录名列表。
 */
void QrBackgroundImageNormalizer::scanSubDir(QList<QString> scanningDirList2Scan)
{
    QFuture<QStringList> subDirs=QtConcurrent::mappedReduced(scanningDirList2Scan,getSubDirs,addToSubDirList); //启动一个并行映射归并过程。
    subDirsFutureWatcher.setFuture(subDirs); //设置未来对象。
} //void FakeImageEliminator::scanSubDir(QList<QString> scanningDirList2Scan)


/*!
 * \brief FakeImageEliminator::scanDirsNextLevel 扫描下一个层次的子目录。
 */
void QrBackgroundImageNormalizer::scanDirsNextLevel()
{
    QFuture<QStringList> subDirFutr=subDirsFutureWatcher.future (); //获取未来对象。

    QStringList subDirs=subDirFutr.result(); //获取扫描到的结果。

    if (subDirs.size ()) //有新结果。
    {
        DirsQueue.append(subDirs); //将当前扫描到的子目录列表追加到整个要扫描的目录列表中。

        scanningDirList.clear (); //清空当前正在扫描的目录列表。
        scanningDirList.append(subDirs); //加入到当前正在扫描的目录列表中。

        scanSubDir(scanningDirList); //扫描子目录。
    } //if (subDirs.size ()) //有新结果。
    else //没有新结果，则应当进行下一步骤的扫描了。扫描各个目录中的文件。
    {
        listDirFiles(); //开始列出各个子目录下的文件。
    } //else //没有新结果，则应当进行下一步骤的扫描了。扫描各个目录中的文件。
} //void FakeImageEliminator::scanDirsNextLevel()

/*!
 * \brief FakeImageEliminator::listDirFiles 开始列出各个子目录下的文件。
 */
void QrBackgroundImageNormalizer::listDirFiles()
{
    QFuture<QStringList> fileList=QtConcurrent::mappedReduced(DirsQueue,getSubDirFileList,addToFileList); //启动一个并行映射归并过程。
    fileListFutureWatcher.setFuture(fileList); //设置未来对象。
} //void FakeImageEliminator::listDirFiles()


/*!
 * \brief FakeImageEliminator::startRecognizeFile 开始识别一个个的文件。
 */
void QrBackgroundImageNormalizer::startRecognizeFile()
{
    QFuture<QStringList> subDirFutr=fileListFutureWatcher.future (); //获取未来对象。

    QStringList subDirs=subDirFutr.result(); //获取扫描到的结果。

    FilesQueue.append(subDirs); //将当前扫描到的文件列表追加到整个要扫描的文件列表中。

    DirsQueue.clear (); //清空当前正在扫描的目录列表。


    int targetAmount=FilesQueue.size ()*30/100; //目标个数。

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    FileTimeComparator * fileTimeComparator=new FileTimeComparator(); //创建比较器。

//    int targetAmount=FilesQueue.size ()*3/100; //目标个数。

    rangeSortSearch.startSearch(FilesQueue,fileTimeComparator,targetAmount); //开始排序并寻找指定个数的元素。
#endif
#endif
} //void FakeImageEliminator::startRecognizeFile()


void QrBackgroundImageNormalizer::GenerateQrImage(int ImgPtrid) //!<Generate the qr image.
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
QImage QrBackgroundImageNormalizer::GenerateQrImage(QString ImgPtrid, QImage bgImage, TransparentMode transparentMode, bool shouldShowGrid,int targetImageWidth)
{
    QImage result; //结果。

    QString Str2Ecd; //The string to encode.
    Str2Ecd=ImgPtrid; //给字符串赋值。

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    QRcode * qrcode; //The qrcode comtent.

    QUrl url2Encode=QUrl(Str2Ecd); //构造URL。



    qrcode=encode(url2Encode.toEncoded ().data()); //Encode.


    if (qrcode) //生成成功。
    {
        result=qRcode2QImage(qrcode,transparentMode,targetImageWidth); //将结果转换成图片。

        if (bgImage.isNull ()) //未传入背景图片。则将纯黑色图片作为背景图片。
        {
            bgImage=QImage(targetImageWidth,targetImageWidth,QImage::Format_RGB32); //创建背景图片。
            bgImage.fill (qRgb(0,0,0)); //以纯黑色填充。
        } //if (bgImage.isNull ()) //未传入背景图片。
        else //传入背景图片，则叠加二维码到背景图片上。
        {
            bgImage=bgImage.scaled (targetImageWidth,targetImageWidth, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); //缩放到合适的尺寸。

//            image=image.scaled ( targetImageWidth, targetImageWidth, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation ); //缩放。

        } //else //传入背景图片，则叠加二维码到背景图片上。

        result=imageOverlay(result,bgImage); //将二维码叠加到背景图片上。

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
#ifndef Q_OS_MACOS
QRcode * QrBackgroundImageNormalizer::encode(char *intext)
{
    QRcode *code;

    int version=0; //Version of qr code.
    QRecLevel level=QR_ECLEVEL_H; //The level.
    QRencodeMode hint=QR_MODE_8; //The hint
    int casesensitive = 1;

    code = QRcode_encodeString((char *)intext, version, level, hint, casesensitive);

    return code;
}
#endif
#endif

#ifndef Q_OS_MACOS
/*!
 * \brief StoreListWidget::SaveQrCodeImage Save the qr code into a png file.
 * \param qrcode The qr code to save.
 * \param OtptFlNm The output file name.
 */
void QrBackgroundImageNormalizer::SaveQrCodeImage(QRcode * qrcode,QString OtptFlNm)
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
#endif

#ifndef Q_OS_MACOS
/*!
 * \brief QrHelper::qRcode2QImage 将结果转换成图片。
 * \param qrcode 要转换的二维码数据。
 * 将二维码数据中的1绘制成背景色，将二维码数据中的0绘制成前景色。
 * \return 转换得到的图片。
 */
QImage QrBackgroundImageNormalizer::qRcode2QImage(QRcode * qrcode, TransparentMode transparentMode,int targetImageWidget)
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
    qrPointListObject["qrcodearray"]=qrPointList; //设置数组对象。

    QJsonObject gameObject=QJsonObject::fromVariantMap (qrPointListObject); //转换成QJSON对象。
    QJsonDocument saveDoc(gameObject); //JSON文档。


    ssnStrFl.write (saveDoc.toJson ()); //输出内容。

    ssnStrFl.close(); //关闭文件。



    CrtImg=CrtImg.scaledToWidth(targetImageWidget); //scale.

    return CrtImg;
} //QImage QrHelper::qRcode2QImage(QRcode * qrcode) //!<将结果转换成图片。
#endif

/*!
 * \brief QrHelper::imageOverlay 将二维码叠加到背景图片上。
 * \param result 二维码图片。
 * \param bgImage 背景图片。
 * \return 叠加之后的图片。
 */
QImage QrBackgroundImageNormalizer::imageOverlay(QImage result,QImage bgImage)
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

#ifndef Q_OS_MACOS
/*!
 * \brief QrHelper::drawGrid 绘制网格。
 * \param qrImage 要在其上绘制网格的二维码图片。
 * \param qrcode 二维码数据。
 * \return 绘制之后的图片。
 */
QImage QrBackgroundImageNormalizer::drawGrid(QImage qrImage,QRcode * qrcode,int targetImageWidth)
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
#endif
