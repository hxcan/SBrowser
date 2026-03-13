#include "SbPreCompile.h" //QColorDialog

#include "QrCodeDialog.h"
#include "ui_QrCodeDialog.h"
#include "QrHelper.h" //QrHelper
#include "ValidFileName.h" //ValidFileName

QrCodeDialog::QrCodeDialog(QWidget *parent) : QDialog(parent), ui(new Ui::QrCodeDialog)
{
    ui->setupUi(this);
} //QrCodeDialog::QrCodeDialog(QWidget *parent) : QDialog(parent), ui(new Ui::QrCodeDialog)

/*!
 * \brief QrCodeDialog::~QrCodeDialog 析构函数。
 */
QrCodeDialog::~QrCodeDialog()
{
    delete ui;

    if (backgroundImageLabel) //背景图片文字标签存在。
    {
        delete backgroundImageLabel.data (); //删除背景图片文字标签。
    } //if (backgroundImageLabel) //背景图片文字标签存在。
} //QrCodeDialog::~QrCodeDialog()

void QrCodeDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
 * \brief QrCodeDialog::resizeEvent Resize event. 陈欣
 * \param e The event object
 */
void QrCodeDialog::resizeEvent(QResizeEvent * e)
{
    Q_UNUSED(e)

    elideImageFileName(); //Show image file name elide
} //void QrCodeDialog::resizeEvent(QResizeEvent * e)

/*!
 * \brief QrCodeDialog::elideImageFileName Show image file name elide
 */
void QrCodeDialog::elideImageFileName()
{
    QFont font=ui->imagefileNamelabel->font(); //Get the font. 陈欣
    QFontMetrics fm(font);
    auto elidedText=fm.elidedText(usedImageFileName, Qt::ElideMiddle, ui->imagefileNamelabel->width());
//    int pixelsHigh = fm.height();


    ui->imagefileNamelabel->setText (elidedText); //显示文件名。

} //void QrCodeDialog::elideImageFileName()

/*!
 * \brief QrCodeDialog::setUrl 设置二维码。
 * \param urlBrtext 要设置的二维码对应的网址。
 */
void QrCodeDialog::setUrl(QString urlBrtext )
{
    url=urlBrtext; //记录URL。
    transparentMode=BlackAsTransparent; //黑色的点作为透明颜色使用。

    QImage CrtPxmp; //The pixmap object.用于尝试载入图片文件的图片对象。

    QString qrBackgroundImageBaseDirUrl=QDir::homePath()+"/.sbrowser/qrBackgroundImage/"; //背景图片的基准目录。

    QString flNm=""; //当前要用作背景的图片文件名。

    flNm=chooseRandomImageFile(qrBackgroundImageBaseDirUrl); //随机选中一张图片。

    usedImageFileName=flNm; //记录曾经用过的图片文件名。

    elideImageFileName(); //elide image file name. 陈欣
//    ui->imagefileNamelabel->setText (usedImageFileName); //显示文件名。

    bool loadResult=CrtPxmp.load(flNm); //尝试载入。
    
    if (loadResult) //载入成功。
    {
    } //if (loadResult) //载入失败。
    else //载入失败。
    {
        CrtPxmp=loadEverythingAsImage.parseImageRaw(flNm); //强行解析成图片。
//        CrtPxmp=loadEverythingAsImage.parseImage(flNm); //强行解析成图片。
    } //else //载入失败。

    backgroundPixmap=CrtPxmp; //记录背景图片。

    generateQr (); //生成二维码。
} //void QrCodeDialog::setUrl(QString urlBrtext ) //!<设置二维码。

/*!
 * \brief QrCodeDialog::chooseRandomImageFile 在目录及其子目录中随机选中一张图片。
 * \param qrBackgroundImageBaseDirUrl 顶级目录。
 * \return 选中的图片文件的路径。
 */
QString QrCodeDialog::chooseRandomImageFile(QString qrBackgroundImageBaseDirUrl)
{
    QString result;
    
    result=qrCodeBackgroundImageFileChooser.chooseRandomImageFile(qrBackgroundImageBaseDirUrl); //使用随机选择器来选择。

    return result;
} //QString QrCodeDialog::chooseRandomImageFile(QString qrBackgroundImageBaseDirUrl)

/*!
 * \brief QrCodeDialog::regenerate 重新生成二维码。
 */
void QrCodeDialog::regenerate()
{
  backgroundPixmap=QImage(); //背景图片重置为空图片。
  transparentMode=BlackAsTransparent; //透明模式重置，背景色透明。

  qrCodeBackgroundImageFileChooser.removeCurrentImageFile(); //要求删除当前图片相关的记录。
    
  QFile usedImageFile(usedImageFileName); //刚刚使用的图片文件。
  usedImageFile.remove (); //删除该图片文件。
    
  generateQr(); //生成二维码。
} //void QrCodeDialog::regenerate()

/*!
 * \brief QrCodeDialog::generateQr 生成二维码。
 */
void QrCodeDialog::generateQr()
{
#ifndef Q_OS_MACOS
  //生成二维码：
  QrHelper QrHelper1;
  QImage qrImg=QrHelper1.GenerateQrImage(url, backgroundPixmap, transparentMode, showGrid, targetImageWidth);//参数为URL字符串。生成二维码图片。生成普通的二维码图片。

    generatedQrcodeImage=qrImg; //记录刚才生成的二维码图片。

    ui->qrlabel->setPixmap (QPixmap::fromImage (qrImg)); //显示二维码。

    setWindowTitle (tr("Qr code:")+url); //标题栏中显示出当前编码为二维码的内容。
#endif
} //void QrCodeDialog::generateQr()

/*!
 * \brief QrCodeDialog::toggleForegroundTransparent 模式切换为前景色透明。
 */
void QrCodeDialog::toggleForegroundTransparent()
{
    transparentMode=WhiteAsTransparent; //透明模式切换为，前景透明。

    generateQr (); //重新生成。
} //void QrCodeDialog::toggleForegroundTransparent()

/*!
 * \brief QrCodeDialog::showBackgroundImage 显示背景图片。
 */
void QrCodeDialog::showBackgroundImage()
{
    if (backgroundImageLabel) //背景图片文字标签存在。
    {

    } //if (backgroundImageLabel) //背景图片文字标签存在。
    else //背景图片文字标签不存在。
    {
        backgroundImageLabel=new SImageView; //创建一个文字标签。
    } //else //背景图片文字标签不存在。

    backgroundImageLabel->hide (); //隐藏。

//    backgroundImageLabel->setScaledContents (true); //缩放内容。
    backgroundImageLabel->setPixmap (QPixmap::fromImage ((backgroundPixmap))); //设置图片。

    backgroundImageLabel->show (); //显示。
    backgroundImageLabel->stackUnder (this); //放到栈下面。
    backgroundImageLabel->raise (); //升到最前面。
    backgroundImageLabel->activateWindow (); //激活窗口。
} //void QrCodeDialog::showBackgroundImage()

/*!
 * \brief QrCodeDialog::exportQrcodeImage 导出生成的二维码图片。
 */
void QrCodeDialog::exportQrcodeImage()
{
    QUrl urlObject(url); //构造网址对象。
    QString urlFileBaseName; //文件基本名字。

    urlFileBaseName=webPageTitle; //设置 web page title as 基本名字。

    if (urlFileBaseName.isEmpty ()) //文件基本名字为空。
    {
        urlFileBaseName=urlObject.fileName (); //Set url base file name as 文件基本名字。
    } //if (urlFileBaseName.isEmpty ()) //文件基本名字为空。

    if (urlFileBaseName.isEmpty ()) //文件基本名字为空。
    {
      urlFileBaseName=tr("QrCode.SBrowser"); //设置默认的基本名字。
    } //if (urlFileBaseName.isEmpty ()) //文件基本名字为空。

#ifndef Q_OS_MACOS
    urlFileBaseName=ValidFileName::filterForFileName (urlFileBaseName,ValidFileName::GenericFileName); //过滤文件名前缀。
#endif

    auto randomNumber=QRandomGenerator::global ()->generate (); //获取随机数。

    QString saveFileName=QFileDialog::getSaveFileName (this,tr("Save qrcode image"),tr("%1.%2.png").arg(urlFileBaseName).arg (randomNumber)); //获取保存文件名。

    if (saveFileName.isEmpty ()) //文件名为空，取消了。
    {
    } //if (saveFileName.isEmpty ()) //文件名为空，取消了。
    else //文件名不为空，则要保存。
    {
      generatedQrcodeImage.save(saveFileName); //保存文件。
        
      qrCodeBackgroundImageFileChooser.creditCurrentImageFile(); //为当前图片文件增加表现值。
    } //else //文件名不为空，则要保存。
} //void QrCodeDialog::exportQrcodeImage()

/*!
 * \brief QrCodeDialog::toggleGrid 切换，是否显示网格。
 * \param showGrid 是否要显示网格。
 */
void QrCodeDialog::toggleGrid(bool showGrid2Set)
{
  showGrid=showGrid2Set; //记录。

    generateQr (); //生成二维码。

    return;
} //void QrCodeDialog::toggleGrid(bool showGrid)

/*!
 * \brief QrCodeDialog::hideEvent 隐藏事件。
 * \param ev 事件对象。
 */
void QrCodeDialog::hideEvent(QHideEvent * ev)
{
    Q_UNUSED(ev); //不使用勒个参数。

    if (backgroundImageLabel) //背景图片存在。
    {
        backgroundImageLabel->hide (); //隐藏图片。
    } //if (backgroundImageLabel) //背景图片存在。



    return;
}

void QrCodeDialog::setWebPageTitle(const QString &value)
{
    webPageTitle = value;
} //void QrCodeDialog::hideEvent(QHideEvent * ev)
