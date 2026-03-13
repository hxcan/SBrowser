QT       += core
QT += network
QT += concurrent
QT += websockets
QT -= gui

QT -= widgets

TARGET = sbrowser
TEMPLATE = app

SOURCES +=\
    SBrowser.cpp \
    SBrowserWindow.cpp

HEADERS  += \
    SbPreCompile.h \
    SBrowserWindow.h

unix {
CONFIG+=precompile_header

PRECOMPILED_HEADER+=SbPreCompile.h
}

QMAKE_CXXFLAGS+= -std=c++11

unix:!macx: LIBS += -lqrencode

#macx:QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+

CONFIG += c++11 c++17

macx: LIBS += -L$$PWD/../../ThirdPartyMac/SbRpcSubSystem -lSbRpcSubSystem

unix:!macx: LIBS += -lSbRpcSubSystem

unix:!macx: LIBS += -lSbMessageDefinitions

INCLUDEPATH += ../SbRpcSubSystem
DEPENDPATH += ../SbRpcSubSystem

INCLUDEPATH += $$PWD/../../../../usr/local/Cellar/qrencode/3.4.2/include
DEPENDPATH += $$PWD/../../../../usr/local/Cellar/qrencode/3.4.2/include

#macx: LIBS += -L$$PWD/../../../../../../usr/local/Cellar/libpng/1.5.14/lib/ -lpng

RESOURCES += \
    SBrowser.qrc

TRANSLATIONS+=SBrowser_ru.ts
TRANSLATIONS+=SBrowser_zh_CN.ts

target.path=/usr/bin

INSTALLS += target

desktopFile.path=/usr/share/applications
desktopFile.files=com.stupidbeauty.sbrowser.desktop

INSTALLS+=desktopFile

iconFile.path=/usr/share/icons/hicolor/512x512/apps/
iconFile.files=Images/sbrowser.png

INSTALLS+=iconFile

DISTFILES += \
    com.stupidbeauty.sbrowser.desktop \
    Images/sbrowser.png
