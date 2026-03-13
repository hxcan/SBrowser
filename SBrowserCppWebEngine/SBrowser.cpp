#include "SbPreCompile.h" //QApplication

#include "SBrowserCppWebEngine.h" //SBrowserWindow

#include "ApplicationStateManager.h" // ApplicationStateManager

int main(int argc, char *argv[])
{
  QApplication app(argc, argv); //程序对象。
  app.setQuitOnLastWindowClosed (false); //关闭窗口的情况下还不退出。

  QTranslator XyTrns;

  QLocale::setDefault(QLocale::system());

  QLocale locale;

  XyTrns.load(":/SBrowser_"+locale.name());

  app.installTranslator (&XyTrns); //安装翻译文件。

  QApplication::setOrganizationName("StupidBeauty"); //设置组织名字。
  QApplication::setOrganizationDomain("stupidbeauty.com"); //设置组织域名。
  QApplication::setApplicationName("SBrowser"); //设置程序名字。
    
  SBrowserCppWebEngine gb; //浏览器窗口。

  QObject::connect(&app,SIGNAL(aboutToQuit()),&gb,SLOT(saveGeometry())); //程序要退出了，则保存窗口的几何属性。
  QObject::connect(&app,&QApplication::aboutToQuit,&gb,&SBrowserCppWebEngine::saveSessionStore); //程序要退出了，则保存会话数据。
  QObject::connect(&app,&QApplication::aboutToQuit,&gb,&SBrowserCppWebEngine::destroyWebPages); //程序要退出了，则销毁所有网页。

  cout  << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", url: " << QThread::currentThreadId() << ", feature: " << endl; //报告错误码。

  gb.show();

  ApplicationStateManager applicationStateManager; // Create the application state manager.

  QObject::connect (&app, &QApplication::applicationStateChanged, &applicationStateManager, &ApplicationStateManager::rememberApplicationState); // Remember application state.

  QObject::connect (&applicationStateManager, &ApplicationStateManager::shouldQuit, &gb, &SBrowserCppWebEngine::quitForIdle); // Quit when idle.
  QObject::connect (&applicationStateManager, &ApplicationStateManager::shouldPauseVideo, &gb, &SBrowserCppWebEngine::pauseVideo); // Pause video when lost focus.
  QObject::connect (&applicationStateManager, &ApplicationStateManager::shouldCheckClipboardUrl, &gb, &SBrowserCppWebEngine::checkClipboardUrl); // Check clipboard url

  int appexec=app.exec(); //进入事件循环。
    
#ifndef Q_OS_MACOS
  // logFileManager.shutdownRootLogger();
#endif

  return appexec; //进入事件循环。
} //int main(int argc, char *argv[])
