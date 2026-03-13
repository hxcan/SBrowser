#include "SbPreCompile.h" //QDir

#include "QrCodeBackgroundImageFileChooser.h"
#include "MessageFactory.h" // Message factory.

#ifndef Q_OS_MACOS
using namespace com::stupidbeauty::sbrowser;
#endif

QrCodeBackgroundImageFileChooser::QrCodeBackgroundImageFileChooser()
{
  buildWorldView(); //建立世界观。
} //QrCodeBackgroundImageFileChooser

/*!
 * \brief QrCodeBackgroundImageFileChooser::buildWorldView 建立世界观。
 */
void QrCodeBackgroundImageFileChooser::buildWorldView()
{
  listFiles(); //列出文件列表。
    
  loadCreditDataCbor(); // 载入之前记录的表现数据。CBor

  associateCreditData(); //将之前记录的表现数据与文件列表关联起来。
    
  saveCreditData(); //将更新后的表现值数据保存。
} //void QrCodeBackgroundImageFileChooser::buildWorldView()

/*!
 * \brief QrCodeBackgroundImageFileChooser::associateCreditData 将之前记录的表现数据与文件列表关联起来。
 */
void QrCodeBackgroundImageFileChooser::associateCreditData()
{
  qint32 currentWholeCredit=0; //当前的整体表现值。
  for(auto currentFileName: fileNameList) //一个个文件地处理。
  {
#ifndef Q_OS_MACOS
    auto curentFileCredit=fileNameCreditMap[currentFileName]; //找到对应的文件表现数据指针。
        
    if (curentFileCredit) //存在对应的指针。
    {
      auto currentCreditValue=curentFileCredit->creditamount(); //获取当前文件的表现值。
            
      curentFileCredit->set_rangelow(currentWholeCredit); //设置范围低值。
      curentFileCredit->set_rangehigh(currentWholeCredit+currentCreditValue-1); //设置范围高值。
            
      currentWholeCredit+=currentCreditValue; //总表现值增加。
    } //if (curentFileCredit) //存在对应的指针。
    else //不存在对应的指针。
    {
      FileCreditMessage freshFileCredit; //创建一个新鲜的数据对象。
            
      freshFileCredit.set_filename(currentFileName.toStdString()); //设置文件名。
      freshFileCredit.set_creditamount(1); //默认的表现值都是1.
      freshFileCredit.set_rangelow(currentWholeCredit); //设置范围低值。
      freshFileCredit.set_rangehigh(currentWholeCredit); //设置范围高值。
            
      currentWholeCredit++; //总表现值增加。
            
      auto pointerToFreshFileCreditInWholeMessage=creditDataMessage.add_filecreditlist(); //加入到列表中。
            
      *pointerToFreshFileCreditInWholeMessage=freshFileCredit; //赋值。
            
      fileNameCreditMap[currentFileName]=pointerToFreshFileCreditInWholeMessage; //记录映射关系。
    } //else //不存在对应的指针。
#endif
  } //for(auto currentFileName: fileNameList) //一个个文件地处理。
    
  wholeCredit=currentWholeCredit; //更新整体的表现值。
} //void QrCodeBackgroundImageFileChooser::associateCreditData()

/*!
 * \brief QrCodeBackgroundImageFileChooser::saveCreditDataCbor Save credit data using cbor.
 */
void QrCodeBackgroundImageFileChooser::saveCreditDataCbor()
{
  QString qrBackgroundImageBaseDirUrl=QDir::homePath()+"/.sbrowser/qrBackgroundImageCreditData.132.cxsh"; // The data file of credit data.
  auto creditDataFileName=qrBackgroundImageBaseDirUrl;

//  string datagramtoStdString=creditDataMessage.SerializeAsString(); //数据包。

//  auto datagram=QByteArray::fromStdString(datagramtoStdString); //转换成字节数组。
#ifndef Q_OS_MACOS

  auto datagram=messageFactory.constructCreditDataCbor(fileNameCreditMap); // Serialize to cbor.

  QFile creditDataFile(creditDataFileName);

  creditDataFile.open(QIODevice::WriteOnly);
  creditDataFile.write(datagram);
  creditDataFile.close();
#endif
} // void QrCodeBackgroundImageFileChooser::saveCreditDataCbor()

/*!
 * \brief QrCodeBackgroundImageFileChooser::saveCreditData 将更新后的表现值数据保存。
 */
void QrCodeBackgroundImageFileChooser::saveCreditData()
{
//  saveCreditDataProtobuf(); // Save credit data using protobuf.

  saveCreditDataCbor(); // Save credit data using cbor.
} //void QrCodeBackgroundImageFileChooser::saveCreditData()

/*!
 * \brief QrCodeBackgroundImageFileChooser::loadCreditDataCbor 载入之前记录的表现数据。CBor
 */
bool QrCodeBackgroundImageFileChooser::loadCreditDataCbor()
{
  auto rsult=false; // Result

  QString qrBackgroundImageBaseDirUrl=QDir::homePath()+"/.sbrowser/qrBackgroundImageCreditData.132.cxsh"; // data file
  auto creditDataFileName=qrBackgroundImageBaseDirUrl;

  QFile creditDataFile(creditDataFileName);

  creditDataFile.open(QIODevice::ReadOnly);
  auto datagram=creditDataFile.readAll();
  creditDataFile.close();

  auto decodedMessage=QCborValue::fromCbor (datagram); //解码

  auto creditDataMessage=decodedMessage.toArray(); // 转换成数组.

  if (creditDataMessage.size()) // Loaded something.
  {
    rsult=true;
  } // if (creditDataMessage.size()) // Loaded something.

//  string datagramtoStdString=datagram.toStdString(); //数据包。

#ifndef Q_OS_MACOS
//  creditDataMessage.ParseFromString (datagramtoStdString); //解析数据包。

//  auto fileCreditAmount=creditDataMessage.filecreditlist_size(); //获取文件表现数据个数。

  for(auto currentFileCredit: creditDataMessage) // Add one by one.
  {
    auto currentFileName=currentFileCredit.toMap()[QStringLiteral("fileName")].toString(); // 获取主机名。陈欣

//    auto currentFileName=QString::fromStdString(currentFileCredit->filename()); //获取文件名。

    auto currentFileCreditMessage=new FileCreditMessage; // Create file credit message.

    auto rangeLow=currentFileCredit.toMap()[QStringLiteral("rangeLow")].toInteger(); // 获取主机名。陈欣
    auto rangeHigh=currentFileCredit.toMap()[QStringLiteral("rangeHigh")].toInteger(); // 获取主机名。陈欣
    auto creditAmount=currentFileCredit.toMap()[QStringLiteral("creditAmount")].toInteger(); // 获取主机名。陈欣

    currentFileCreditMessage->set_rangelow(rangeLow); // Set field.
    currentFileCreditMessage->set_rangehigh(rangeHigh); // Set field.
    currentFileCreditMessage->set_creditamount(creditAmount); // Set field.
    currentFileCreditMessage->set_filename(currentFileName.toStdString()); // Set field.

    auto pointerToFileCreditInList=currentFileCreditMessage; // 记录指针。

    if (currentFileCreditMessage->creditamount()) //表现值有效。
    {
      fileNameCreditMap[currentFileName]=pointerToFileCreditInList; //记录映射关系。

    }


//    auto currentFileName=currentFileCredit.toMap()[QStringLiteral("hostName")].toString(); // 获取主机名。陈欣

//    auto currentzoomFactor=currentFileCredit.toMap()[QStringLiteral("zoomFactor")].toDouble(); // 获取缩放比例。

//    hostNameZoomFactorMap[currentFileName]=currentzoomFactor; //记录映射关系。
  } //for(auto currentFileCredit: creditDataMessage.zoomfactormapentry()) //Add one by one.

//  for(auto fileCreditCounter=0; fileCreditCounter<fileCreditAmount; fileCreditCounter++) //一个个地加入到映射中。
//  {
//    auto currentFileCredit=creditDataMessage.mutable_filecreditlist(fileCreditCounter); //获取当前数据条目的指针。

//    if (currentFileCredit->creditamount()) //表现值有效。
//    {
//      auto currentFileName=QString::fromStdString(currentFileCredit->filename()); //获取文件名。

//      auto pointerToFileCreditInList=currentFileCredit; //记录指针。

//      fileNameCreditMap[currentFileName]=pointerToFileCreditInList; //记录映射关系。
//    } //if (currentFileCredit->creditamount()) //表现值有效。
//    else //表现值无效。
//    {
//      creditDataMessage.mutable_filecreditlist()->DeleteSubrange(fileCreditCounter, 1); //删除这一个。

//      fileCreditAmount--; //总个数少一个了。
//    } //else //表现值无效。
//  } //for(auto fileCreditCounter=0; fileCreditCounter<fileCreditAmount; fileCreditCounter++) //一个个地加入到映射中。
#endif

  return rsult;
} // void QrCodeBackgroundImageFileChooser::loadCreditDataCbor()

/*!
 * \brief QrCodeBackgroundImageFileChooser::listFiles 列出目录。
 * \param qrBackgroundImageBaseDirUrl 要列出的目录的路径。
 */
void QrCodeBackgroundImageFileChooser::listFiles(QString qrBackgroundImageBaseDirUrl)
{
    QString result;
    
    QDir candidateImagesDir(qrBackgroundImageBaseDirUrl); //包含着候选图片的目录。

    candidateImagesDir.setFilter(QDir::NoDotAndDotDot  | QDir::Files | QDir::Dirs ); //不要列出点和点点，列出文件和目录。

    QFileInfoList imageFileList=candidateImagesDir.entryInfoList (); //图片文件列表。

    QString flNm=""; //当前要用作背景的图片文件名。
    
    for(auto currentFileInfo: imageFileList) //一个个地处理。
    {
        QFileInfo fileInfo(currentFileInfo); //构造文件信息对象。

        flNm=currentFileInfo.absoluteFilePath(); //获取绝对路径。
        
        if (fileInfo.isDir ()) //是个目录。
        {
            listFiles(flNm); //递归列出。
        } //if (fileInfo.isDir ()) //是个目录。
        else //是个文件。
        {
            fileNameList << flNm; //加到文件列表。
        } //else //是个文件。
    } //for(auto currentFileInfo: imageFileList) //一个个地处理。
} //void QrCodeBackgroundImageFileChooser::listFiles(QString qrBackgroundImageBaseDirUrl)

/*!
 * \brief QrCodeBackgroundImageFileChooser::listFiles 列出文件列表。
 */
void QrCodeBackgroundImageFileChooser::listFiles()
{
    fileNameList.clear(); //清空。
    
    QString qrBackgroundImageBaseDirUrl=QDir::homePath()+"/.sbrowser/qrBackgroundImage/"; //背景图片的基准目录。
    
    listFiles(qrBackgroundImageBaseDirUrl); //列出目录。
} //void QrCodeBackgroundImageFileChooser::listFiles()

/*!
 * \brief QrCodeDialog::chooseRandomImageFile 在目录及其子目录中随机选中一张图片。
 * \param qrBackgroundImageBaseDirUrl 顶级目录。
 * \return 选中的图片文件的路径。
 */
QString QrCodeBackgroundImageFileChooser::chooseRandomImageFile(QString qrBackgroundImageBaseDirUrl)
{
  QString result;
    
  QString flNm=""; //当前要用作背景的图片文件名。

  if (wholeCredit) //There is actually credit info.
  {
//    int chosenImageIndex=qrand ()%wholeCredit; //随机选中一个表现值，看落在哪个文件的表现值范围。
    auto chosenImageIndex=QRandomGenerator::global ()->generate ()%wholeCredit; //随机选中一个表现值，看落在哪个文件的表现值范围。

#ifndef Q_OS_MACOS
    auto fileCreditList=fileNameCreditMap.values(); // gEt creidt list
    for(auto currentFileCredit: fileCreditList) // 一个个地检查。
//      for(auto currentFileCredit: creditDataMessage.filecreditlist()) //一个个地检查。
    {
      if ((currentFileCredit->rangelow()<= chosenImageIndex) && (currentFileCredit->rangehigh()>=chosenImageIndex)) //在范围内。
      {
        flNm=(currentFileCredit->filename()); //获取文件名。
      } //if ((currentFileCredit.rangelow()<= chosenImageIndex) && (currentFileCredit.rangehigh()>=chosenImageIndex)) //在范围内。
    } //for(auto currentFileCredit: creditDataMessage.filecreditlist()) //一个个地检查。
#endif

    QFileInfo fileInfo(flNm); //构造文件信息对象。

    if (fileInfo.exists()) //存在。
    {
    } //if (fileInfo.isDir ()) //是个目录。
    else //不存在。
    {
      flNm=chooseRandomImageFile(qrBackgroundImageBaseDirUrl); //递归继续寻找。
    } //else //不存在。
  } //if (wholeCredit) //There is actually credit info.
    
  result=flNm;
    
  chosenImageFileName=flNm; //记录，当前选中的背景图片文件名。
    
  return result;
} //QString QrCodeDialog::chooseRandomImageFile(QString qrBackgroundImageBaseDirUrl)

/*!
 * \brief QrCodeBackgroundImageFileChooser::removeCurrentImageFile 要求删除当前图片相关的记录。
 */
void QrCodeBackgroundImageFileChooser::removeCurrentImageFile()
{
#ifndef Q_OS_MACOS
    auto curentFileCredit=fileNameCreditMap[chosenImageFileName]; //找到对应的文件表现数据指针。
    
//    creditDataMessage.filecreditlist().Delete

    curentFileCredit->set_creditamount(0); //表现值归零。
    curentFileCredit->set_rangelow(-1); //设置无效的范围值。
    curentFileCredit->set_rangehigh(-1); //设置无效的范围值。
    
//    currentCreditValue++; //增加。
    
    // logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", url: " << chosenImageFileName << ", feature: "; //报告错误码。
    
    
//    curentFileCredit->set_creditamount(currentCreditValue); //设置新的表现值。
    
    saveCreditData(); //保存表现值数据。
#endif
} //void QrCodeBackgroundImageFileChooser::removeCurrentImageFile()

/*!
 * \brief QrCodeBackgroundImageFileChooser::creditCurrentImageFile 为当前图片文件增加表现值。
 */
void QrCodeBackgroundImageFileChooser::creditCurrentImageFile()
{
#ifndef Q_OS_MACOS
    auto curentFileCredit=fileNameCreditMap[chosenImageFileName]; //找到对应的文件表现数据指针。

    auto currentCreditValue=curentFileCredit->creditamount(); //获取表现值。
    
    currentCreditValue++; //增加。
    
    // logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", url: " << chosenImageFileName << ", feature: " << currentCreditValue ; //报告错误码。
    
    
    curentFileCredit->set_creditamount(currentCreditValue); //设置新的表现值。
    
    saveCreditData(); //保存表现值数据。
#endif
} //void QrCodeBackgroundImageFileChooser::creditCurrentImageFile()
