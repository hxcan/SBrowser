TARGET = SvLogSubSystem
TEMPLATE = lib

SOURCES += \
    LogFileManager.cpp

HEADERS +=\
    LgPreCompile.h \
    LogFileManager.h \
    SvLogSubSystemGlobal.h

unix:!symbian {
        target.path = /usr/lib64

    INSTALLS += target 
}

TRANSLATIONS+=Xy.ts

VERSION=2018.11.23

CONFIG+=c++11

QMAKE_CXXFLAGS+= -std=c++11

unix:!macx: LIBS += -llog4qt

QT -= gui widgets

win32: LIBS += -L$$PWD/../../ThirdPartyWindows/log4qt/lib/ -llog4qt

win32: INCLUDEPATH += $$PWD/../../ThirdPartyWindows/log4qt/include
win32: DEPENDPATH += $$PWD/../../ThirdPartyWindows/log4qt/include

DEFINES += SV_LOG_SUB_SYSTEM 
