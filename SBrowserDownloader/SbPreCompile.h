#ifndef SB_PRECOMPILE_H
#define SB_PRECOMPILE_H

#include <QTranslator> //QTranslator

#ifndef Q_OS_MACOS
//#include <QtSingleApplication> //QtSingleApplication
#endif

#include <QSettings> //QSettings
#include <QClipboard> //QClipboard
#include <QDir> //QDir
#include <QJsonObject> //QJsonObject
#include <QJsonDocument> //QJsonDocument
#include <QSystemTrayIcon> // QSystemTrayIcon
#include <QNetworkCookie> //QNetworkCookie
#include <QMetaEnum> //QMetaEnum
#include <QNetworkReply> //QNetworkReply
#include <QTimer> //QTimer
#include <QApplication> //QApplication
#include <QSslCipher> //QSslCipher
#include <QNetworkAccessManager> //QNetworkAccessManager
#include <ValidFileName.h> //ValidFileName
#include <QDesktopServices> //QDesktopServices
#include <cmath> //floor
#include <QPropertyAnimation> //QPropertyAnimation
#include <QRegularExpressionValidator> // QRegularExpressionValidator
#include <QStandardPaths> // QStandardPaths

#ifdef Q_OS_UNIX

#ifndef Q_OS_MACOS
#include <LowMemoryQt/MemoryMonitor.h> //MemoryMonitor
#include <Stop/StopListener.h> //StopListener
#endif

#endif

using namespace std; //string

#endif // PRECOMPILE_H
