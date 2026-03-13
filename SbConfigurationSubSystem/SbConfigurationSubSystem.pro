QT += core
QT-=widgets

TARGET = SbConfigurationSubSystem
TEMPLATE = lib

SOURCES +=

HEADERS  +=

CONFIG +=  c++11

DISTFILES += \
    logiccontrolsubsystem.yml \
    speakersubsystem.yml

installEtcFiles.files+=logiccontrolsubsystem.yml \
wvipcdisplay.yml

target.path = /usr/lib64

INSTALLS += installEtcFiles \
target 

VERSION=2021.4.10

unix:!macx: LIBS += -lyaml-cpp
