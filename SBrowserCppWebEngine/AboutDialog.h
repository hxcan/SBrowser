#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#include "SBrowserCppWebEngine.h" //SBrowserWindow

class  SBrowserCppWebEngine; //!<SBrowserWindow的前向声明。

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    void setPackgeUrl(QString packageUrl); //!<设置新版本安装包的下载URL。
    void setNewVersionNumber(QString newVersion); //!<设置新版本的版本号。
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog(); //!<换个屏幕。男 读音仙仙满腹经纶。下班了。妳需祈祷，抢到票机会不大，30%左右。

private:
    SBrowserCppWebEngine * sBrowserWindow; //!<浏览器主窗口。
    QString mPackageUrl; //!<新版本安装包的下载URL。必须的，用软件抢。
    Ui::AboutDialog *ui; //!<大家都用软件抢，票只有那么多，就看哪个的网速快。

private slots:
    void downloadNewVersion(); //!<下载新版本安装包。到哪个站？
};

#endif // ABOUTDIALOG_H
