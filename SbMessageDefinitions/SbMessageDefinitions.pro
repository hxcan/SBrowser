TARGET = SbMessageDefinitions
TEMPLATE = lib

SOURCES += \
    CreditDataMessage.pb.cc \
    FileCreditMessage.pb.cc \
    MessageFactory.cpp

HEADERS +=\
    CreditDataMessage.pb.h \
    FileCreditMessage.pb.h \
    cppGenerated/AvatarMacroReq.pb.h \
    cppGenerated/ConfigurationMessage.pb.h \
    MessageFactory.h \
    MdSbPreCompile.h

unix:!symbian 
{
        target.path = /usr/lib64

    INSTALLS += target
}

VERSION=2022.10.22

CONFIG += c++14 c++17

unix {
QMAKE_CXXFLAGS+= -std=c++14
}

QT+=concurrent network

# unix:!macx:!android: LIBS += -llog4qt

QT += gui
QT-=widgets

win32: LIBS += -L$$PWD/../../../ThirdPartyWindows/qamqp/lib/ -lqamqp

INCLUDEPATH += $$PWD/../SbRpcSubSystem
DEPENDPATH += $$PWD/../SbRpcSubSystem

INCLUDEPATH += $$PWD/cppGenerated
DEPENDPATH += $$PWD/cppGenerated

# QT+=scxml

INCLUDEPATH += ../../Microphone/SpeechRecognizerBase
DEPENDPATH += ../../Microphone/SpeechRecognizerBase

QT += widgets

DISTFILES += \
  SbMessageDefinitions.pro.user
