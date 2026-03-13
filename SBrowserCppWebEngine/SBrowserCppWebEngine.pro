QT += core
QT += gui

QT += widgets
QT += websockets

TARGET = sbrowsercppwebengine
TEMPLATE = app

QT += webenginewidgets

unix:!macx {
SOURCES+= \
QrHelper.cpp

SOURCES+=     FileTimeComparator.cpp

HEADERS+=     FileTimeComparator.h
}

macx {
SOURCES +=\
    MacOsPermissionRequestor.mm

HEADERS+= \
    MacOsPermissionRequestor.h

LIBS += -framework AVFoundation
}

SOURCES +=\
    ApplicationStateManager.cpp \
    DownloadManager.cpp \
    IconUrlCallback.cpp \
    SBrowser.cpp \
    SBrowserCppWebEngine.cpp \
    SWebPageWidget.cpp \
    StbWebView.cpp \
    StbSettingDialog.cpp \
    StbProxyFactory.cpp \
    VideoUrlCallback.cpp \
    cookiejar.cpp \
    autosaver.cpp \
    QrCodeDialog.cpp \
    FindDialog.cpp \
    SbTtsClient.cpp \
    AboutDialog.cpp \
    StartSlideGoBackTop.cpp \
    TabManager.cpp \
    StbShadowProxyFactory.cpp \
    QrBackgroundImageNormalizer.cpp \
    downloadmessagehandler.cpp \
    mapReduceFunctions.cpp

SOURCES+=StbWebPage.cpp \
    SbAdblockInterceptor.cpp \
    ContainerInterceptor.cpp \
    UnknownSchemeInterceptor.cpp \
    QrRemoteDecoder.cpp \
    LoadEverythingAsImage.cpp \
    CookieExceptionsModel.cpp \
    SearchEngineManager.cpp \
    Client.cpp \
    QrCodeBackgroundImageFileChooser.cpp \
    TunnelInfoDialog.cpp \
    UserScriptManager.cpp \
    simageview/SImageView.cpp


HEADERS+=StbWebPage.h \
    ApplicationStateManager.h \
    DownloadManager.h \
    DownloaderType.h \
    HostnameZoomFactorEntryMessage.pb.h \
    IconUrlCallback.h \
    SbAdblockInterceptor.h \
    ContainerInterceptor.h \
    SvLogSubSystem/LgPreCompile.h \
    SvLogSubSystem/SvLogSubSystemGlobal.h \
    UnknownSchemeInterceptor.h \
    QrRemoteDecoder.h \
    LoadEverythingAsImage.h \
    CookieExceptionsModel.h \
    SearchEngineManager.h \
    Client.h \
    UserScriptManager.h \
    VideoUrlCallback.h \
    downloadmessagehandler.h \
    simageview/SImageView.h \
    simageview/SiPreCompile.h \
    QrCodeBackgroundImageFileChooser.h \
    TunnelInfoDialog.h


HEADERS  += \
  SBrowserCppWebEngine.h \
  SbPreCompile.h \
  SWebPageWidget.h \
  StbWebView.h \
  StbSettingDialog.h \
  StbProxyFactory.h \
  cookiejar.h \
  autosaver.h \
  QrCodeDialog.h \
  QrHelper.h \
  FindDialog.h \
  SbTtsClient.h \
  AboutDialog.h \
  Constants.h \
  StartSlideGoBackTop.h \
  TabManager.h \
  StbShadowProxyFactory.h \
  QrBackgroundImageNormalizer.h \
  mapReduceFunctions.h

FORMS    += \
  SBrowserCppWebEngine.ui \
  StbSettingDialog.ui \
  passworddialog.ui \
  QrCodeDialog.ui \
  FindDialog.ui \
  AboutDialog.ui \
  TabManager.ui \
  ThumbnailsDeleter.ui \
  TunnelInfoDialog.ui \
  simageview/SImageView.ui

CONFIG+=precompile_header

PRECOMPILED_HEADER+=SbPreCompile.h

QT += network

QMAKE_CXXFLAGS+= -std=c++14
CONFIG += c++14 c++17

unix:!macx: LIBS += -lqrencode

macx:CONFIG +=c++11

RESOURCES += \
    SBrowser.qrc \
    qdarkstyle/style.qrc

TRANSLATIONS+=SBrowser_ru.ts
TRANSLATIONS+=SBrowser_zh_CN.ts  SBrowser_zh_TW.ts

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -lvalidfilename


INCLUDEPATH += $$PWD/../../ValidFileName/header
DEPENDPATH += $$PWD/../../ValidFileName/header

win32: LIBS += -L$$PWD/../../Material/GnuWin32/lib/ -llibpng

win32:INCLUDEPATH += $$PWD/../../Toast/header
unix:!macx: LIBS += -ltoast

#安装
target.path = /usr/bin

INSTALLS += target

ldSoConfDFile.path=/etc/ld.so.conf.d/
ldSoConfDFile.files=ld.so.conf.d/sbrowsercppwebengine.conf

INSTALLS+=ldSoConfDFile

win32: LIBS += -L$$PWD/../../Toast/so/ -ltoast2015

INCLUDEPATH += $$PWD/../../Toast/header
DEPENDPATH += $$PWD/../../Toast/header

win32: LIBS += -L$$PWD/../../Zlib/so/ -lzdll

INCLUDEPATH += $$PWD/../../Zlib/include
DEPENDPATH += $$PWD/../../Zlib/include

QMAKE_INFO_PLIST = Info.plist

DISTFILES += \
    Images/sbrowser.png \
    Info.plist \
    ld.so.conf.d/sbrowsercppwebengine.conf \
    simageview/proto_files/CollectImageFileService.proto \
    simageview/proto_files/CommonTypes.proto \
    simageview/proto_files/ImageFile.proto \
    simageview/proto_files/NodeInfoService.proto \
    simageview/proto_files/RemoteRotatePredictRequest.proto \
    simageview/proto_files/regexptestrequestmessage.proto \
    simageview/proto_files/regexptestresponsemessage.proto

QT+=concurrent

unix:!macx: LIBS += -lParallizedAlgorithms

unix:!macx: LIBS += -lExistQt

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -lStopQt

INCLUDEPATH += $$PWD/../../../../../usr/include/Stop
DEPENDPATH += $$PWD/../../../../../usr/include/Stop

INCLUDEPATH += /usr/include/ExistQt
DEPENDPATH += /usr/include/ExistQt

INCLUDEPATH += /usr/include/qt6/QtSolutions
DEPENDPATH += /usr/include/qt6/QtSolutions

INCLUDEPATH += $$PWD/simageview
DEPENDPATH += $$PWD/simageview

INCLUDEPATH += $$PWD/SvRpcSubSystem
DEPENDPATH += $$PWD/SvRpcSubSystem

INCLUDEPATH += $$PWD/SvLogSubSystem
DEPENDPATH += $$PWD/SvLogSubSystem

unix:!macx: LIBS += -llowmemoryqt

QT+=remoteobjects
QT += svg

REPC_REPLICA = ../repFile/SetDirectionService.rep

win32: LIBS += -L$$PWD/../ThirdpartyWindows/ExistQt/lib/ -lExistQt2019

win32 {
    INCLUDEPATH += $$PWD/../ThirdpartyWindows/ExistQt/include
    DEPENDPATH += $$PWD/../ThirdpartyWindows/ExistQt/include
}

win32: LIBS += -L$$PWD/../ThirdpartyWindows/Protobuf/lib/ -llibprotobuf

INCLUDEPATH += $$PWD/../ThirdpartyWindows/Protobuf/include
DEPENDPATH += $$PWD/../ThirdpartyWindows/Protobuf/include

win32: LIBS += -L$$PWD/../ThirdpartyWindows/ValidFileName/lib/ -lvalidfilename2017

INCLUDEPATH += $$PWD/../ThirdpartyWindows/ValidFileName/include
DEPENDPATH += $$PWD/../ThirdpartyWindows/ValidFileName/include

INCLUDEPATH += $$PWD/cppGenerated
DEPENDPATH += $$PWD/cppGenerated

INCLUDEPATH += ../SbRpcSubSystem
DEPENDPATH += ../SbRpcSubSystem

macx: LIBS += -L$$PWD/../../ThirdPartyMac/SbRpcSubSystem -lSbRpcSubSystem

unix:!macx: LIBS += -lSbRpcSubSystem

unix:!macx: LIBS += -lSbMessageDefinitions

INCLUDEPATH += $$PWD/../SbMessageDefinitions
DEPENDPATH += $$PWD/../SbMessageDefinitions

INCLUDEPATH += $$PWD/../SbMessageDefinitions/cppGenerated
DEPENDPATH += $$PWD/../SbMessageDefinitions/cppGenerated

macx: LIBS += -L$$PWD/../../ThirdPartyMac/SbMessageDefinitions -lSbMessageDefinitions

QT += location

greaterThan(QT_MAJOR_VERSION, 5): QT -= location

QT += core5compat

SUBDIRS += \
  SvLogSubSystem/SvLogSubSystem.pro
