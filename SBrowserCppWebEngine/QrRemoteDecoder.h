#ifndef QRREMOTEDECODER_H
#define QRREMOTEDECODER_H

//#include <QObject>

#include "SbPreCompile.h" //QRcode

#include "Constants.h" //TransparentMode


#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
#include "RangeSortSearch.h" //RangeSortSearch
#endif
#endif

class QrRemoteDecoder : public QObject
{
    Q_OBJECT
private slots:
    void Delete1File(); //!<Delete timer timed out,then delete 1 file.

public:
    explicit QrRemoteDecoder( QObject *parent = 0);
    void setMaterialFileName(const QString &value);
    
    void setSarCodeScannerServicePort(const quint16 &value);
    
    void setSarCodeScannerServiceAddress(const QString &value);
    
private:
    void modifyTimestampByAppendingBytes(); //!<追加字节，“黄映如”。
    QString materialFileName; //!<素材文件名。
    void parseSendAvatarReply(); //!<请求结束，则处理结果。
    bool showGrid=false; //!<是否要显示网格。
    int targetImageWidth=412; //!<生成的目标二维码宽度。
    QImage generatedQrcodeImage; //!<刚刚生成的二维码图片。
    void toggleForegroundTransparent(); //!<模式切换为前景色透明。
    
    void setUrl(QString urlBrtext ); //!<设置二维码。
    void sendAvatar(); //!<向服务器发送头像文件。
    QString url; //!<对其生成二维码的URL。
    TransparentMode transparentMode; //!<透明模式。是要将黑色的点还是白色的点变成透明的。
    QString usedImageFileName; //!<曾经用过的图片文件名。
    QImage backgroundPixmap; //!<背景图片。
    void generateQr(); //!<生成二维码。
    QNetworkAccessManager * networkAccessManager; //!<网络访问管理器。
    QNetworkProxy dummyProxy; //!<不使用代理的代理。
    
    quint16 sarCodeScannerServicePort; //!<调码扫描器的服务端口号。
    QString sarCodeScannerServiceAddress; //!<调码扫描器的服务地址。
    QString SarCodeScannerServiceName="com.stupidbeauty.sarcodescanner"; //!<调码扫描器的服务名字。
    
#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
    RangeSortSearch<QString> rangeSortSearch; //!<范围排序搜索器。
#endif
#endif

    QTimer startWorkTimer; //!<触发开始工作动作的定时器。
    void initializeMembers(); //!<初始化成员变量。
    void connectSignals(); //!<连接信号槽。
    void startWork(); //!<开始工作。
    QFutureWatcher<QStringList> subDirsFutureWatcher; //!<用于监听列出子目录过程的未来观察器。
    
signals:
    void finished(); //!<工作完毕。
};

#endif // QRHELPER_H
