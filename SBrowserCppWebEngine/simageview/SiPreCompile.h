#ifndef SI_PRECOMPILE_H
#define SI_PRECOMPILE_H

// #include <QNetworkCookie> //QNetworkCookie
// #include <QDesktopServices> //QDesktopServices
// #include <QDrag> //QDrag
#include <QProcess> // QProcess
//#include <QtScript/QScriptValue> //QScriptValue
// #include <QFontDialog> //QFontDialog
// #include <QFileDialog> //QFileDialog
// #include <QColorDialog> //QColorDialog
// #include <QLabel> //QLabel
#include <QGraphicsColorizeEffect> //QGraphicsColorizeEffect //字幕云。
// #include <QSystemTrayIcon> //QSystemTrayIcon
#include <QMenu> //QMenu
// #include <QPointer> //QPointer
#include <QMouseEvent> //QMouseEvent
// #include <QAuthenticator> //QAuthenticator
#include <QMessageBox> //QMessageBox
// #include <QAbstractNetworkCache> //cache
#include <QNetworkDiskCache> //QNetworkDiskCache

// #include <QSslCipher>
// #include <QNetworkProxy> //QNetworkProxy

extern "C"
{
#ifndef Q_OS_MACOS
// #include <qrencode.h> //QRcode
#endif
} //extern "C"

// #include <QRemoteObjectNode> //QRemoteObjectNode

// #include <QSvgRenderer> //QSvgRenderer

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
//#include <webp/decode.h> //decode
// #include <magic.h> //MAGIC_MIME_TYPE
//#include <libraw/libraw.h> //LibRaw
//#include <easyexif/exif.h> //EXIFInfo

//using namespace easyexif;

#endif
#endif

// #include <QtConcurrent> //QtConcurrent

#ifndef Q_OS_MACOS
// #include <log4qt/logger.h>
// #include <log4qt/propertyconfigurator.h>
// #include <log4qt/loggerrepository.h>
// #include <log4qt/consoleappender.h>
// #include <log4qt/ttcclayout.h>
// #include <log4qt/logmanager.h>
// #include <log4qt/fileappender.h>
#endif

#include <QMainWindow> //QMainWindow
#include <QScroller> //QScroller
// #include <QApplication> //QApplication
// #include <QMovie> //QMovie
// #include <QPainter> //QPainter

// using namespace std; //string

#endif // PRECOMPILE_H
