TARGET = SbRpcSubSystem
TEMPLATE = lib

SOURCES += \
    FaceMaskServerCommanderWebSocket.cpp \
    WebSocketServiceManager.cpp

HEADERS +=\
    FaceMaskServerCommanderWebSocket.h \
    RbSbPreCompile.h \
    RpSbPreCompile.h \
    TdPreCompile.h \
    WebSocketServiceManager.h

unix:!symbian {
        target.path = /usr/lib64

    INSTALLS += target
}

VERSION=2022.3.12

CONFIG += c++11 c++17

QT+=network
QT+=websockets

# unix:!macx: LIBS += -llog4qt

macx: LIBS += -L$$PWD/../../ThirdPartyMac/SbMessageDefinitions -lSbMessageDefinitions

INCLUDEPATH += $$PWD/../SbMessageDefinitions
DEPENDPATH += $$PWD/../SbMessageDefinitions

INCLUDEPATH += $$PWD/../SbMessageDefinitions/cppGenerated
DEPENDPATH += $$PWD/../SbMessageDefinitions/cppGenerated

QT -= gui
QT -= widgets
