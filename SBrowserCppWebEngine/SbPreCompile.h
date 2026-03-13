#ifndef SB_PRECOMPILE_H
#define SB_PRECOMPILE_H

#include <QTimer> // QTimer
#include <QElapsedTimer> // QElapsedTimer
#include <QDialog> // QDialog
#include <QTcpSocket> // QTcpSocket
#include <QWebEngineScript> // QWebEngineScript
#include <QNetworkAccessManager> // QNetworkAccessManager
#include <QWebEnginePage> // QWebEnginePage
#include <QPointer> // QPointer
#include <QVBoxLayout> // QVBoxLayout
#include <QWebEngineFullScreenRequest> // QWebEngineFullScreenRequest
#include <QDir> //QDir
#include <QFileDialog> // QFileDialog
#include <QDesktopServices> // QDesktopServices
#include <QAuthenticator> // QAuthenticator
#include <QWebEngineView> //QWebEngineView
#include <QWebEngineSettings> // QWebEngineSettings
#include <QWebEngineProfile> //QWebEngineProfile
#include <QWebEngineCookieStore> // QWebEngineCookieStore
#include <QRandomGenerator> // QRandomGenerator
#include <QPainter> // QPainter
#include <QSystemTrayIcon> // QSystemTrayIcon
#include <QStandardPaths> // QStandardPaths
#include <QWebEngineDownloadRequest> // QWebEngineDownloadRequest
#include <QTextCodec> // QTextCodec
#include <QMenuBar> // QMenuBar
#include <QWebChannel> // QWebChannel

#ifndef Q_OS_MACOS
#include <ExistQt/ServicePublisher.h> // ServicePublisher
#endif

#include <QWebEngineUrlRequestInterceptor> //QWebEngineUrlRequestInterceptor
#include <QNetworkProxy> // QNetworkProxy

#ifdef Q_OS_UNIX
#ifndef Q_OS_MACOS
#include <LowMemoryQt/MemoryMonitor.h> //MemoryMonitor
#include <Stop/StopListener.h> // StopListener
#endif
#endif

extern "C"
{
#ifndef Q_OS_MACOS
#include <qrencode.h> // QRcode
#endif
} // extern "C"

#include <QToolBar> // QToolBar
#include <QToolButton> // QToolButton
#include <ValidFileName.h> // ValidFileName
#include <QInputDialog> // QInputDialog
#include <QClipboard> //QClipboard
#include <QStatusBar> // QStatusBar
#include <QJsonObject> // QJsonObject
#include <QJsonDocument> //QJsonDocument
#include <QRegularExpressionValidator> // QRegularExpressionValidator
#include <QSettings> // QSettings
#include <QWebEngineCertificateError> //QWebEngineCertificateError
#include <QApplication> //QApplication
#include <QWebEngineScriptCollection> //QWebEngineScriptCollection

#endif // PRECOMPILE_H
