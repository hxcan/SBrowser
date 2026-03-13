QT       += core
QT +=gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sbrowserdownloader
TEMPLATE = app

HEADERS+=\
  SetDirectionService.h \
  SBrowserDownloader.h \
  squeezelabel.h

SOURCES+=\
  CookieExceptionsModel.cpp \
  DownloadManager.cpp \
  SetDirectionService.cpp \
  SBrowserDownloader.cpp \
  squeezelabel.cpp \
  SBrowser.cpp \
  StbProxyFactory.cpp \
  cookiejar.cpp \
  edittableview.cpp \
  autosaver.cpp \
  networkaccessmanager.cpp \
  DownloadItem.cpp \
  StbShadowProxyFactory.cpp

HEADERS  += \
  DownloadManager.h \
  SbPreCompile.h \
  StbProxyFactory.h \
  cookiejar.h \
  edittableview.h \
  autosaver.h \
  networkaccessmanager.h \
  DownloadItem.h \
  Constants.h \
  StbShadowProxyFactory.h \
  CommonHeader.h


FORMS    += \
    DownloadItem.ui \
    DownloadManager.ui \
    passworddialog.ui \
    proxy.ui \
    FindDialog.ui \
    TabManager.ui

CONFIG+=precompile_header

PRECOMPILED_HEADER+=SbPreCompile.h

QT+=network

QMAKE_CXXFLAGS+= -std=c++11

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lqrencode

#macx:QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+

CONFIG += c++11 c++17

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/Cellar/qrencode/3.4.2/lib/ -lqrencode

INCLUDEPATH += $$PWD/../../../../usr/local/Cellar/qrencode/3.4.2/include
DEPENDPATH += $$PWD/../../../../usr/local/Cellar/qrencode/3.4.2/include

#unix: LIBS += -L$$PWD/../../../../../../usr/local/Cellar/qrencode/3.4.2/lib/ -lqrencode

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Material/Code/qrcode-win32-3.1.1/dll/ -lqrcodelib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Material/Code/qrcode-win32-3.1.1/dll/ -lqrcodelibd

INCLUDEPATH += $$PWD/../../Material/Code/qrcode-win32-3.1.1/include
DEPENDPATH += $$PWD/../../Material/Code/qrcode-win32-3.1.1/include

#QT+=script

#macx: LIBS += -L$$PWD/../../../../../../usr/local/Cellar/libpng/1.5.14/lib/ -lpng

INCLUDEPATH += $$PWD/../../../../../../usr/local/Cellar/libpng/1.5.14/include
DEPENDPATH += $$PWD/../../../../../../usr/local/Cellar/libpng/1.5.14/include

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
unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -ltoast


#安装
target.path=/usr/bin

INSTALLS += target

ldSoConfDFile.path=/etc/ld.so.conf.d/
ldSoConfDFile.files=ld.so.conf.d/sbrowsercppwebengine.conf

INSTALLS+=ldSoConfDFile

win32: LIBS += -L$$PWD/../../Toast/so/ -ltoast2015

INCLUDEPATH += $$PWD/../../Toast/header
DEPENDPATH += $$PWD/../../Toast/header

win32: LIBS += -L$$PWD/../../Zlib/so/ -lzdll

DISTFILES += \
    Images/sbrowser.png \
    ld.so.conf.d/sbrowsercppwebengine.conf

#QT+=concurrent

unix:!macx: LIBS += -lParallizedAlgorithms

#unix:!macx: LIBS += -lExistQt

unix:!macx: LIBS += -lStopQt

INCLUDEPATH += $$PWD/../../../../../usr/include/Stop
DEPENDPATH += $$PWD/../../../../../usr/include/Stop

INCLUDEPATH += $$PWD/../../../../../usr/include/ExistQt
DEPENDPATH += $$PWD/../../../../../usr/include/ExistQt

INCLUDEPATH += $$PWD/../../../../../usr/include/LowMemoryQt
DEPENDPATH += $$PWD/../../../../../usr/include/LowMemoryQt

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -llowmemoryqt

CONFIG+=qtsingleapplication

QT+=remoteobjects
QT+=qml

REPC_SOURCE = ../repFile/SetDirectionService.rep

CONFIG+=qtsingleapplication

win32: LIBS += -L$$PWD/../ThirdPartyWindows/qtsingleapplication/lib/ -lQt5Solutions_SingleApplication-head

win32: INCLUDEPATH += $$PWD/../ThirdPartyWindows/qtsingleapplication/src
win32: DEPENDPATH += $$PWD/../ThirdPartyWindows/qtsingleapplication/src

win32: LIBS += -L$$PWD/../ThirdpartyWindows/ValidFileName/lib/ -lvalidfilename2017

INCLUDEPATH += $$PWD/../ThirdpartyWindows/ValidFileName/include
DEPENDPATH += $$PWD/../ThirdpartyWindows/ValidFileName/include
