TARGET = SbErrorCodeSubSystem
TEMPLATE = lib

SOURCES += \
    ErrorCodeTranslator.cpp \
    ErrorReporter.cpp

HEADERS +=\
    RbSbPreCompile.h \
    DisableMsvcWarning.h \
    ErrorCodeSubSystemGlobal.h \
    ErrorCodeSubSystem.h \
    ErrorReporter.h \
    ErrorCodeContainer.h \
    ErrorCodeTranslator.h \
    SrbPreCompile.h \
    TdPreCompile.h

export_header_files.path=/usr/include/ErrorCodeSubSystem
export_header_files.files+=\
    OperationMenuDisplayReq.pb.h \
    ExecuteCodeReq.pb.h \
    ExecutePhenemSequenceReq.pb.h \
    SppasResultWrapper.h \
    SemanticRepsonseNotifyReply.pb.h \
    SemanticRepsonseNotifyReply.pb.h 

unix:!symbian {
        target.path = /usr/lib64

    INSTALLS += target \
export_header_files
}

TRANSLATIONS+=Xy.ts

VERSION=2019.8.12

CONFIG += c++11 c++17

unix {
QMAKE_CXXFLAGS+= -std=c++11 
}

win32 {
#QMAKE_CXXFLAGS+= /FIRpcSubSystemGlobal.h
QMAKE_CXXFLAGS+= /wd4100
}

# unix:!macx:!android: LIBS += -llog4qt

INCLUDEPATH += $$PWD

QT += widgets
QT += webchannel
QT += gui
QT += concurrent
QT += qml
QT += quick

win32: LIBS += -L$$PWD/../../../../../../ProgramFiles/vcpkg-master/packages/protobuf_x64-windows/lib/ -llibprotobuf

win32: INCLUDEPATH += $$PWD/../../../../../../ProgramFiles/vcpkg-master/packages/protobuf_x64-windows/include
win32: DEPENDPATH += $$PWD/../../../../../../ProgramFiles/vcpkg-master/packages/protobuf_x64-windows/include

win32: LIBS += -L$$PWD/../../ThirdPartyWindows/yaml-cpp/lib/ -llibyaml-cppmd

win32: INCLUDEPATH += $$PWD/../../ThirdPartyWindows/yaml-cpp/include
win32: DEPENDPATH += $$PWD/../../ThirdPartyWindows/yaml-cpp/include

# win32: LIBS += -L$$PWD/../../ThirdPartyWindows/log4qt/lib/ -llog4qt

# win32: INCLUDEPATH += $$PWD/../../ThirdPartyWindows/log4qt/include
# win32: DEPENDPATH += $$PWD/../../ThirdPartyWindows/log4qt/include

win32: LIBS += -L$$PWD/../../ThirdPartyWindows/qamqp/lib/ -lqamqp

win32: INCLUDEPATH += $$PWD/../../ThirdPartyWindows/qamqp/include
win32: DEPENDPATH += $$PWD/../../ThirdPartyWindows/qamqp/include

INCLUDEPATH += ../Microphone/SpeechRecognizerBase
DEPENDPATH += ../Microphone/SpeechRecognizerBase

DEFINES += ERROR_CODE_SUB_SYSTEM QAMQP_SHARED

android {
INCLUDEPATH += $$PWD/../../ThirdPartyAndroid/ProtobufAndroid/src
DEPENDPATH += $$PWD/../../ThirdPartyAndroid/ProtobufAndroid/src

INCLUDEPATH += $$PWD/../../ThirdPartyAndroid/yaml-cpp-yaml-cpp-0.6.2/include
DEPENDPATH += $$PWD/../../ThirdPartyAndroid/yaml-cpp-yaml-cpp-0.6.2/include

# LIBS += -L$$PWD/../../ThirdPartyAndroid/Log4Qt-1.5.0/bin/ -lLog4QtLibAndroid
# LIBS += -L$$PWD/../../ThirdPartyAndroid/Log4Qt-1.5.0/bin-v7a/ -lLog4QtLibAndroid

# INCLUDEPATH += $$PWD/../../ThirdPartyAndroid/Log4Qt-1.5.0/include
# DEPENDPATH += $$PWD/../../ThirdPartyAndroid/Log4Qt-1.5.0/include

INCLUDEPATH += $$PWD/../../ThirdPartyAndroid/qamqp-master/include
DEPENDPATH += $$PWD/../../ThirdPartyAndroid/qamqp-master/include

QMAKE_LINK += -nostdlib++
}

# QT+= multimedia
QT+=websockets
QT+=qml

#unix:!macx: LIBS += -lyaml-cpp
