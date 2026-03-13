#include "SbPreCompile.h" //QApplication

#include "SBrowserDownloader.h" //SBrowserWindow

int main(int argc, char *argv[])
{
    int runResult=0; //退出代码。


#ifndef Q_OS_MACOS
//    QtSingleApplication app( "com.stupidbeauty.sbrowserdownloader" ,argc, argv); //程序对象。
    QApplication app(argc, argv); //程序对象。

//    if (app.isRunning()) //正在运行。
//    {

//    } //if (TheApplication.isRunning()) //正在运行。
//    else //尚未运行。
    {
#else
//    {
//    QApplication app(argc, argv); // 程序对象。 陈欣

    #endif


        app.setQuitOnLastWindowClosed (false); //关闭窗口的情况下还不退出。
        
        QTranslator XyTrns;
        
        QLocale::setDefault(QLocale::system());
        
        QLocale locale;
        
        auto loadREsult = XyTrns.load(":/SBrowser_"+locale.name());
        (void)(loadREsult);
        
        app.installTranslator (&XyTrns); //安装翻译文件。
        
        QApplication::setOrganizationName("StupidBeauty"); //设置组织名字。
        QApplication::setOrganizationDomain("stupidbeauty.com"); //设置组织域名。
        QApplication::setApplicationName("SBrowser"); //设置程序名字。

        SBrowserDownloader gb; //浏览器窗口。
        
        QObject::connect(&app,SIGNAL(aboutToQuit()),&gb,SLOT(saveGeometry())); //程序要退出了，则保存窗口的几何属性。
        
        // gb.show();
        
        runResult= app.exec();
        
    #ifndef Q_OS_MACOS
    }
    #endif

    return runResult; //进入事件循环。
} //int main(int argc, char *argv[])
