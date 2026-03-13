#ifndef QRBACKGROUNDIMAGENORMALIZER_H
#define QRBACKGROUNDIMAGENORMALIZER_H

//#include <QObject>

#include "SbPreCompile.h" //QRcode

#include "Constants.h" //TransparentMode

#ifdef Q_OS_UNIX

#ifndef Q_OS_MACOS
#include "RangeSortSearch.h" //RangeSortSearch
#endif

#endif

#include "LoadEverythingAsImage.h" //LoadEverythingAsImage

class QrBackgroundImageNormalizer : public QObject
{
    Q_OBJECT
private slots:
    void Delete1File(); //!<Delete timer timed out,then delete 1 file.

public:
    void quit(); //!<退出。
    void GenerateQrImage(int ImgPtrid); //!< Generate the qr image.
    QImage GenerateQrImage(QString ImgPtrid, QImage bgImage=QImage(), TransparentMode transparentMode=BlackAsTransparent, bool  shouldShowGrid=false, int targetImageWidth=512); //!<生成QR图片。
    explicit QrBackgroundImageNormalizer(QObject *parent = 0);

private:
    QImage loadImage(QString CrtFlNm); //!<尝试载入图片。
    QTimer remoteDecodeTimer; //!<远程解码定时器。
    QQueue<QString> remoteDecodeQueue ; //!< 要进行远程解码测试的文件名队列。
    void remoteDecode1File(); //!<远程解码，测试一个文件。

    bool showGrid=false; //!<是否要显示网格。
    int targetImageWidth=381; //!<生成的目标二维码宽度。
    QImage generatedQrcodeImage; //!<刚刚生成的二维码图片。
    LoadEverythingAsImage loadEverythingAsImage; //!< 这个对象，可将任意文件当成图片来载入。
    void setUrl(QString urlBrtext ); //!<设置二维码。

    QString url; //!<对其生成二维码的URL。
    TransparentMode transparentMode; //!< 透明模式。是要将黑色的点还是白色的点变成透明的。
    QString usedImageFileName; //!<曾经用过的图片文件名。
    QImage backgroundPixmap; //!<背景图片。
    void generateQr(); //!<生成二维码。
    QNetworkAccessManager * networkAccessManager; //!<网络访问管理器。
    quint16 sarCodeScannerServicePort; //!<调码扫描器的服务端口号。
    QString sarCodeScannerServiceAddress; //!<调码扫描器的服务地址。
    QString SarCodeScannerServiceName="com.stupidbeauty.sarcodescanner"; //!<调码扫描器的服务名字。

#ifndef Q_OS_MACOS
    void processDiscoveredService(QString serviceName,quint16 servicePort, const QString & serviceProtocolType,string address); //!<发现了服务，则处理。
    ServicePublisher servicePublisher; //!<服务发布器。
    QImage drawGrid(QImage qrImage, QRcode * qrcode, int targetImageWidth=512); //!<绘制网格。
    QImage qRcode2QImage(QRcode * qrcode, TransparentMode transparentMode=BlackAsTransparent, int targetImageWidget=512); //!<将结果转换成图片。
    void SaveQrCodeImage(QRcode * qrcode,QString OtptFlNm); //!<Save the qr code into a png file.
    QRcode * encode(char *intext); //!< Encode qr code.
#endif


    QTimer DltTimer; //!<Delte timer.
    QQueue<QString> oldThumbnailFiles; //!<旧的缩略图文件。
    void startDelete(); //!<搜索完毕，则开始删除。

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    RangeSortSearch<QString> rangeSortSearch; //!<范围排序搜索器。
#endif
#endif

    QTimer startWorkTimer; //!<触发开始工作动作的定时器。
    void initializeMembers(); //!<初始化成员变量。
    void connectSignals(); //!<连接信号槽。
    QString ThumbnailsDirName; //!<配置文件的目录名。
    void startWork(); //!<开始工作。
    QFutureWatcher<QStringList> subDirsFutureWatcher; //!<用于监听列出子目录过程的未来观察器。
    void scanDirsNextLevel(); //!<扫描下一个层次的子目录。
    QFutureWatcher<QStringList> fileListFutureWatcher; //!<用于监听列出目录下的文件的过程的未来观察器。
    void startRecognizeFile(); //!<开始识别一个个的文件。
    QQueue<QString> DirsQueue; //!<The directories queue.
    QList<QString> scanningDirList; //!<当前正在扫描的目录的列表。
    void scanSubDir(QList<QString> scanningDirList2Scan); //!<扫描子目录。
    void listDirFiles(); //!<开始列出各个子目录下的文件。
    QQueue<QString> FilesQueue; //!<The file paths queue.
    QImage imageOverlay(QImage result,QImage bgImage); //!<将二维码叠加到背景图片上。
}; //class QrBackgroundImageNormalizer : public QObject

#endif // QRHELPER_H
