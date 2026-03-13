#ifndef TUNNELINFODIALOG_H
#define TUNNELINFODIALOG_H

#include <QDialog>

namespace Ui {
    class TunnelInfoDialog;
}

class TunnelInfoDialog : public QDialog
{
    Q_OBJECT
    
public:
    void setAutoCheckTunnelStatus(bool autoCheckTunnelStatus); //!<设置是否自动检查隧道状态。
    void setAutoStart(bool autoStart); //!<设置是否要自动启动。
    bool isAutoStart(); //!<是否要自动启动。
    void setAutoRestart(bool autoRestart); //!<设置是否自动重启。
    bool isAutoRestart(); //!<是否自动重启。
    bool isAutoCheckTunnelStatus(); //!<是否自动检查隧道状态。
    QString getUserName(); //!<获取用户名。
    void setUserName(QString userName2Set); //!<设置用户名。
    void SetLocalPort(quint16 CrtLclPrt); //!<设置本地端口号。
    void SetServerPort(quint16 CrtSrvrPrt); //!<设置服务器端口号。
    void SetServerIP(QString CrtSrvrIp); //!<设置服务器IP。
    quint16 GetLocalPort(); //!<获取本地监听的端口号。
    quint16 GetServerPort(); //!<获取服务器端口号。
    QString GetServerIp(); //!<获取服务器IP。
    explicit TunnelInfoDialog(QWidget *parent = 0);
    ~TunnelInfoDialog();
    
    QString getPassword() const;
    void setPassword(const QString &value);

private:
    Ui::TunnelInfoDialog *ui; //!<Ui hierachy.
}; //class TunnelInfoDialog : public QDialog

#endif // TUNNELINFODIALOG_H
