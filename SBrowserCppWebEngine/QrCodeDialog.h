#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include "SbPreCompile.h" //QLabel

#include "Constants.h" //TransparentMode
#include "LoadEverythingAsImage.h" //LoadEverythingAsImage
#include "QrCodeBackgroundImageFileChooser.h" //QrCodeBackgroundImageFileChooser
#include "SImageView.h"

namespace Ui {
class QrCodeDialog;
}

class QrCodeDialog : public QDialog
{
    Q_OBJECT

public:
    void setUrl(QString urlBrtext ); //!<设置二维码。
    explicit QrCodeDialog(QWidget *parent = 0);
    ~QrCodeDialog(); //!<析构函数。

    void setWebPageTitle(const QString &value);

protected:
    void changeEvent(QEvent *e) override;
    void hideEvent(QHideEvent * ev) override; //!<隐藏事件。
    void resizeEvent(QResizeEvent * e) override; //!<Resize event. 陈欣

private:
    void elideImageFileName(); //!<Show image file name elide
    QrCodeBackgroundImageFileChooser qrCodeBackgroundImageFileChooser; //!<随机背景图片选择器。
    LoadEverythingAsImage loadEverythingAsImage; //!<这个对象，可将任意文件当成图片来载入。
    QString chooseRandomImageFile(QString qrBackgroundImageBaseDirUrl); //!<在目录及其子目录中随机选中一张图片。
    QString webPageTitle; //!<The web page title.
    int targetImageWidth=381; //!<生成的目标二维码宽度。
    QPointer<SImageView> backgroundImageLabel; //!<背景图片文字标签。
    bool showGrid=false; //!<是否要显示网格。
    QImage generatedQrcodeImage; //!<刚刚生成的二维码图片。
    TransparentMode transparentMode; //!<透明模式。是要将黑色的点还是白色的点变成透明的。
    void generateQr(); //!<生成二维码。
    QImage backgroundPixmap; //!<背景图片。
    QString usedImageFileName; //!<曾经用过的图片文件名。
    QString url; //!<对其生成二维码的URL。
    Ui::QrCodeDialog *ui; //!<  用戶界面對象。

private slots:
    void toggleGrid(bool showGrid2Set); //!<切换，是否显示网格。
    void exportQrcodeImage(); //!<导出生成的二维码图片。
    void showBackgroundImage(); //!<显示背景图片。
    void toggleForegroundTransparent(); //!<模式切换为前景色透明。
    void regenerate(); //!<重新生成二维码。
};

#endif // QRCODEDIALOG_H
