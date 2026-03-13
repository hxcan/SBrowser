#include "SbPreCompile.h" //QTimer

#include "MessageFactory.h" // MessageFactory

#ifndef Q_OS_MACOS
#include "FileCreditMessage.pb.h" //FileCreditMessage
#include "CreditDataMessage.pb.h" //CreditDataMessage

using namespace com::stupidbeauty::sbrowser;
#endif

class QrCodeBackgroundImageFileChooser : public QObject
{
  Q_OBJECT

public:
  QrCodeBackgroundImageFileChooser();
  void creditCurrentImageFile(); //!<为当前图片文件增加表现值。
  void removeCurrentImageFile(); //!<要求删除当前图片相关的记录。
  QString chooseRandomImageFile(QString qrBackgroundImageBaseDirUrl); //!<在目录及其子目录中随机选中一张图片。
    
private:
  MessageFactory messageFactory; //!< 消息构造工厂.陈欣
  QString chosenImageFileName; //!<记录，当前选中的背景图片文件名。
  qint32 wholeCredit=0; //!<整体的表现值。
  void buildWorldView(); //!<建立世界观。
  void listFiles(); //!<列出文件列表。
  void listFiles(QString qrBackgroundImageBaseDirUrl); //!<列出目录。
  QStringList fileNameList; //!<候选文件列表。

#ifndef Q_OS_MACOS
  CreditDataMessage creditDataMessage; //!<历史表现数据记录。
  QMap<QString, FileCreditMessage *> fileNameCreditMap; //!<文件名与对应的单条历史表现消息之间的映射。
#endif

  void saveCreditDataCbor(); //!< Save credit data using cbor.
  void saveCreditData(); //!< 将更新后的表现值数据保存。
  bool loadCreditDataCbor(); //!< 载入之前记录的表现数据。CBor
  void associateCreditData(); //!<将之前记录的表现数据与文件列表关联起来。
}; //class QrCodeBackgroundImageFileChooser : public QObject
