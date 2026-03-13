TEMPLATE = subdirs

SUBDIRS += SBrowserLauncher
SUBDIRS += SBrowserCppWebEngine
SUBDIRS +=  SBrowserDownloader
SUBDIRS += SbRpcSubSystem
#SUBDIRS += SbConfigurationSubSystem
SUBDIRS += SbMessageDefinitions
SUBDIRS += SbErrorCodeSubSystem
# SUBDIRS += SvLogSubSystem

SBrowserLauncher.depends += SbRpcSubSystem
SbRpcSubSystem.depends += SbMessageDefinitions

SBrowserCppWebEngine.depends += SbMessageDefinitions
