QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simageview
TEMPLATE = app


SOURCES += main.cpp \
    SImageView.cpp \
    LastOperation.cpp \
    StbSettingDialog.cpp \
    Client.cpp \
    PrinterWorker.cpp

HEADERS  += \
    SImageView.h \
    LastOperation.h \
    OperationType.h \
    StbSettingDialog.h \
    SbPreCompile.h \
    Client.h \
    TextDisplayReq.grpc.pb.h \
    PrinterWorker.h \
    DisplaySubSystemWorker.h

FORMS    += \
    SImageView.ui \
    StbSettingDialog.ui

CONFIG+=precompile_header

PRECOMPILED_HEADER+=\
SbPreCompile.h

RESOURCES += Images.qrc


TRANSLATIONS+=XyRunner.ts

#安装
installIconFiles.path=/usr/share/icons/hicolor/512x512/apps/
installIconFiles.files+=Images/simageview.png

installDesktopFiles.path=/usr/share/applications/
installDesktopFiles.files+=com.stupidbeauty.simageview.desktop


target.path=/usr/bin

INSTALLS += target \
installIconFiles \
installDesktopFiles

unix:!macx: LIBS += -lwebp
unix:!macx: LIBS += -lmagic

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib64/ -lKF5KIOCore

INCLUDEPATH += $$PWD/../../../../usr/lib64
DEPENDPATH += $$PWD/../../../../usr/lib64

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib64/ -lKF5KIOCore -lKF5CoreAddons

INCLUDEPATH += $$PWD/../../../../usr/include/KF5/KIOCore
DEPENDPATH += $$PWD/../../../../usr/include/KF5/KIOCore

INCLUDEPATH += $$PWD/../../../../usr/include/KF5/KIOCore/kio
DEPENDPATH += $$PWD/../../../../usr/include/KF5/KIOCore/kio

INCLUDEPATH += $$PWD/../../../../usr/include/KF5/KCoreAddons
DEPENDPATH += $$PWD/../../../../usr/include/KF5/KCoreAddons

DEFINES+=DOXYGEN_SHOULD_SKIP_THIS

QMAKE_CXXFLAGS+= -std=c++14
CONFIG +=c++14

#kf5-kio-devel.x86_64
QT += KIOCore KIOFileWidgets KIOWidgets KNTLM
QT+=network

unix:!macx: LIBS += -lraw -lraw_r

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += ImageMagick

unix: PKGCONFIG += ImageMagick++

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -lKF5KIOWidgets

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

QT+=remoteobjects concurrent

REPC_REPLICA = ../repFile/SetDirectionService.rep

unix:!macx: LIBS += -lgrpc++

unix:!macx: LIBS += -lprotobuf-lite

unix:!macx: LIBS += -lprotobuf

DISTFILES += \
    protobuf.pri \
    proto_files/CollectImageFileService.proto \
    proto_files/CommonTypes.proto \
    proto_files/ImageFile.proto \
    proto_files/NodeInfoService.proto \
    proto_files/regexptestrequestmessage.proto \
    proto_files/regexptestresponsemessage.proto \
    proto_files/RemoteRotatePredictRequest.proto

PROTOS =             proto_files/ImageFile.proto \
proto_files/CommonTypes.proto \
proto_files/CollectImageFileService.proto \
    proto_files/NodeInfoService.proto \
    proto_files/regexptestrequestmessage.proto \
    proto_files/regexptestresponsemessage.proto \
    proto_files/RemoteRotatePredictRequest.proto



DEFINES+="MAGICKCORE_HDRI_ENABLE=0" \
"MAGICKCORE_QUANTUM_DEPTH=16"

unix:!macx: LIBS += -llog4qt

unix:!macx: LIBS += -L$$OUT_PWD/../SvRpcSubSystem/ -lSvRpcSubSystem

INCLUDEPATH += $$PWD/../SvRpcSubSystem
DEPENDPATH += $$PWD/../SvRpcSubSystem

unix:!macx: LIBS += -L$$OUT_PWD/../SvLogSubSystem/ -lSvLogSubSystem

INCLUDEPATH += $$PWD/../SvLogSubSystem
DEPENDPATH += $$PWD/../SvLogSubSystem

unix:!macx: LIBS += -lqamqp
unix:!macx: LIBS += -leasyexif
