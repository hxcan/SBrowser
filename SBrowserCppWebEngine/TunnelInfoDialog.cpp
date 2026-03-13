#include "TunnelInfoDialog.h"
#include "ui_TunnelInfoDialog.h" //.h"

TunnelInfoDialog::TunnelInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TunnelInfoDialog)
{
    ui->setupUi(this);
}

TunnelInfoDialog::~TunnelInfoDialog()
{
    delete ui;
}

QString TunnelInfoDialog::getPassword() const
{
    return ui->passwordlineEdit->text();
}

void TunnelInfoDialog::setPassword(const QString &value)
{
    ui->passwordlineEdit->setText(value);
}

/*!
 * \brief TunnelInfoDialog::GetServerIp 获取服务器IP。
 * \return 服务器IP。
 */
QString TunnelInfoDialog::GetServerIp()
{
    return (ui->SrvrIplineEdit->text()); //返回服务器IP。
} //QString TunnelInfoDialog::GetServerIp() //!<获取服务器IP。

/*!
 * \brief TunnelInfoDialog::GetServerPort 获取服务器端口号。
 * \return 服务器端口号。
 */
quint16 TunnelInfoDialog::GetServerPort()
{
    return (ui->SrvrPrtspinBox->value()); //返回服务器端口号。
} //quint16 TunnelInfoDialog::GetServerPort() //!<获取服务器端口号。

/*!
 * \brief TunnelInfoDialog::GetLocalPort 获取本地监听的端口号。
 * \return 本地监听的端口号。
 */
quint16 TunnelInfoDialog::GetLocalPort()
{
    return (ui->LclspinBox->value()); //返回本地监听端口号。
} //quint16 TunnelInfoDialog::GetLocalPort() //!<获取本地监听的端口号。

/*!
 * \brief TunnelInfoDialog::SetServerIP 设置服务器IP。
 * \param CrtSrvrIp 要设置的服务器IP。
 */
void TunnelInfoDialog::SetServerIP(QString CrtSrvrIp)
{
    ui->SrvrIplineEdit->setText(CrtSrvrIp); //显示。

} //void TunnelInfoDialog::SetServerIP(QString CrtSrvrIp) //!<设置服务器IP。

/*!
 * \brief TunnelInfoDialog::SetServerPort 设置服务器端口号。
 * \param CrtSrvrPrt 要设置的服务器端口号。
 */
void TunnelInfoDialog::SetServerPort(quint16 CrtSrvrPrt)
{
    ui->SrvrPrtspinBox->setValue(CrtSrvrPrt); //显示。

    return;
} //void TunnelInfoDialog::SetServerPort(quint16 CrtSrvrPrt) //!<设置服务器端口号。

/*!
 * \brief TunnelInfoDialog::SetLocalPort 设置本地端口号。
 * \param CrtLclPrt 要设置的本地端口号。
 */
void TunnelInfoDialog::SetLocalPort(quint16 CrtLclPrt)
{
    ui->LclspinBox->setValue(CrtLclPrt); //显示。

    return;
} //void TunnelInfoDialog::SetLocalPort(quint16 CrtLclPrt) //!<设置本地端口号。

/*!
 * \brief TunnelInfoDialog::setUserName 设置用户名。
 * \param userName2Set 要设置的用户名。
 */
void TunnelInfoDialog::setUserName(QString userName2Set)
{
    ui->userlineEdit->setText (userName2Set); //显示。

    return;
} //void TunnelInfoDialog::setUserName(QString userName2Set)

/*!
 * \brief TunnelInfoDialog::getUserName 获取用户名。
 * \return 用户名。
 */
QString TunnelInfoDialog::getUserName()
{
    return ui->userlineEdit->text ();
} //QString TunnelInfoDialog::getUserName()

/*!
 * \brief TunnelInfoDialog::setAutoCheckTunnelStatus 设置是否自动检查隧道状态。
 * \param autoCheckTunnelStatus 是否自动检查隧道状态。
 */
void TunnelInfoDialog::setAutoCheckTunnelStatus(bool autoCheckTunnelStatus)
{
    ui->autoCheckTunnelStatuscheckBox->setChecked(autoCheckTunnelStatus); //设置界面部件的状态。
} //void TunnelInfoDialog::setAutoCheckTunnelStatus(bool autoCheckTunnelStatus)

/*!
 * \brief TunnelInfoDialog::isAutoCheckTunnelStatus 是否自动检查隧道状态。
 * \return 是否自动检查隧道状态。
 */
bool TunnelInfoDialog::isAutoCheckTunnelStatus()
{
    return ui->autoCheckTunnelStatuscheckBox->isChecked();
} //bool TunnelInfoDialog::isAutoCheckTunnelStatus()

/*!
 * \brief TunnelInfoDialog::isAutoStart 是否要自动启动。
 * \return 是否要自动启动。
 */
bool TunnelInfoDialog::isAutoStart()
{
    return ui->autoStartcheckBox->isChecked();
} //bool TunnelInfoDialog::isAutoStart()

/*!
 * \brief TunnelInfoDialog::isAutoRestart 是否自动重启。
 * \return 是否自动重启。
 */
bool TunnelInfoDialog::isAutoRestart()
{
    qDebug() << __FILE__ << __LINE__ << __func__ << tr("auto restart check box is checked:") << ui->autoRestartcheckBox->isChecked (); //Debug.

    return ui->autoRestartcheckBox->isChecked ();
} //bool TunnelInfoDialog::isAutoRestart()

/*!
 * \brief TunnelInfoDialog::setAutoStart 设置是否要自动启动。
 * \param autoStart 是否要自动启动。
 */
void TunnelInfoDialog::setAutoStart(bool autoStart)
{
    ui->autoStartcheckBox->setChecked(autoStart); //设置界面部件的状态。

} //void TunnelInfoDialog::setAutoStart(bool autoStart)

/*!
 * \brief TunnelInfoDialog::setAutoRestart 设置是否自动重启。
 * \param autoRestart 是否自动重启。
 */
void TunnelInfoDialog::setAutoRestart(bool autoRestart)
{
    ui->autoRestartcheckBox->setChecked (autoRestart); //设置界面元素的状态。
} //void TunnelInfoDialog::setAutoRestart(bool autoRestart)
