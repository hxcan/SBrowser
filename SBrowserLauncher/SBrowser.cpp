#include "SbPreCompile.h" //QApplication

#include "SBrowserWindow.h" //SBrowserWindow

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv); //程序对象。
//    app.setQuitOnLastWindowClosed (false); //关闭窗口的情况下还不退出。

    QCoreApplication::setOrganizationName("StupidBeauty"); //设置组织名字。
    QCoreApplication::setOrganizationDomain("stupidbeauty.com"); //设置组织域名。
    QCoreApplication::setApplicationName("SBrowserLauncher"); //设置程序名字。

    SBrowserWindow gb; //浏览器窗口。

    return app.exec(); //进入事件循环。
} //int main(int argc, char *argv[])
