#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "SBrowserCppWebEngine.h" //SBrowserWindow

/*!
 * \brief AboutDialog::AboutDialog 构造函数。
 * \param parent 亲代对象的指针。
 */
AboutDialog::AboutDialog(QWidget *parent) :QDialog(parent),ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->nwVersionpushButton->hide (); //隐藏新版本按钮。
} //AboutDialog::AboutDialog(QWidget *parent) :QDialog(parent),ui(new Ui::AboutDialog)

AboutDialog::~AboutDialog()
{
    delete ui;
} // AboutDialog::~AboutDialog()

/*!
 * @brief AboutDialog::setNewVersionNumber 设置新版本的版本号。
 * @param newVersion 新版本的版本号。
 */
void AboutDialog::setNewVersionNumber(QString newVersion)
{
    ui->nwVersionpushButton->show (); //显示出按钮，下载新版本。
    ui->nwVersionpushButton->setText (tr("Get new version:")+newVersion); //设置按钮上的文字内容。

    return;
} //void AboutDialog::setNewVersionNumber(QString newVersion)

/*!
 * \brief AboutDialog::setPackgeUrl 设置新版本安装包的下载URL。
 * \param packageUrl 新版本安装包的下载URL。
 */
void AboutDialog::setPackgeUrl(QString packageUrl)
{
    mPackageUrl=packageUrl; //记录。

    return;
} //void AboutDialog::setPackgeUrl(QString packageUrl) //!<设置新版本安装包的下载URL。

/*!
 * \brief AboutDialog::downloadNewVersion 下载新版本安装包。
 */
void AboutDialog::downloadNewVersion()
{
    QNetworkRequest request=QNetworkRequest(mPackageUrl); //要下载的目标URL对应的请求对象。

    sBrowserWindow->downloadManager()->download(request); //由下载管理器来处理。

    return;
} //void AboutDialog::downloadNewVersion() //!<下载新版本安装包。
