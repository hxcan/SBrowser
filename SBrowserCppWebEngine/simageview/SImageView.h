#ifndef SIMAGEVIEW_H
#define SIMAGEVIEW_H

#include "SiPreCompile.h" //QNetworkRequest

#include "StbSettingDialog.h" //StbSettingDialog
#include "rep_SetDirectionService_replica.h" //SetDirectionServiceReplica
#include "Client.h" //Client

namespace Ui {
class SImageView;
}

class SImageView : public QMainWindow
{
    Q_OBJECT

public:
    explicit SImageView(QWidget *parent = 0);
    ~SImageView();
    void setPixmap (const QPixmap & backgroundPixmap); //!<设置图片。
    
protected:
    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent * event); //!<尺寸改变。
    
private:
    QNetworkAccessManager networkAccessManager; //!<网络请求管理器。
    QImage rotate0Image;
    QScopedPointer<QImage> rotate90Image; //!<旋转90度后的缓存图片。
    QScopedPointer<QImage> rotate180Image; //!<旋转180度后的缓存图片。
    QScopedPointer<QImage> rotate270Image; //!<旋转270度后的缓存图片。
    QRemoteObjectNode repNode;
    quint32 rotateDegree=0; //!<当前的旋转角度。
    void showPixmapToImageLabel(QPixmap newSizePixmap); //!<显示位图。
    QString clientId; //!<本机的客户端编号。
    bool useLocalPredict=false; //!<是否要使用本地旋转预测。
    bool provideRemotePredictService=false; //!<是否要提供远程旋转预测服务。
    QString privacyDirectoryList; //!<隐私目录列表。
    void autoFitImageSize(); //!<按自适应尺寸按钮，则显示自适应尺寸。
    void initializeMembers(); //!<初始化成员变量。
    QString AptCmd="simageviewtrainer"; //!<apt-get命令行。
    QString RotatePredictorAptCmd="simageviewrotatepredictor"; //!<旋转预测程序命令行。
    QString SImageNormalizerCmd="SImageNormalizer"; //!<图片规范化进程命令行。
    QString RemoteRotatePredictServerCommand="simageviewremotepredictserver"; //!<旋转预测程序命令行。
    QProcess  currentVersionNumber; //!<当前的版本号。
    QProcess  rotatePredictorProcess; //!<旋转预测器进程。
    QProcess  SImageNormalizerProcess; //!<图片规范化进程。
    QProcess  remotePredictServerProcess; //!<远程预测服务器进程。
    QProcess shutdownat2100remotePredictServerProcess; //!<21点关机的进程。
    const QString GsCmd="gs"; //!<gs命令。
    QAction * trashAction; //!<回收站动作。
    QString lastTimeSavedQuickMoveTargetDir; //!<上次保存的快速移动目录。
    QString quickMoveTargetDir; //!<Quick move target diresctory.
    void createWidgets(); //!<创建各个部件。
    QTimer initScaleTimer; //!<初始缩放定时器。
    QString imageFileName; //!<要显示的图片文件名。
    QImage staticImage; //!<静态图片。用于显示。
    QImage originalImage; //!<原始图片。用于计算旋转缓存。
    QString printerSubSystemIp="0.0.0.0"; //!<
    QUrl LeftCenterWebUrl; //!<左边中部网页的网址。
    QUrl LeftTopWebUrl; //!<左上角网页的网址。
    QUrl MetadataUrl; //!<Metadata文件的URL。
    void connectSignals(); //!<连接信号槽。
    Ui::SImageView *ui; //!<界面。

private slots:
    void scalePixmapToNewSize(); //!<将位图缩放到新的尺寸。
    void Quit(); //!<按退出按钮，则退出。

signals:
    void queueNumberPrintResultNotify(QString queueNumber, bool isSuccessful); //!<
    void BroadcastReaded(); //!<信号，已经读取广播消息。
    void GameInfoListReaded(); //!<信号，已经读取游戏信息列表。
};

#endif // XYRUNNER_H
