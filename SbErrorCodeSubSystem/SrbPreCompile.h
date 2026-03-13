#ifndef SRB_PRECOMPILE_H
#define SRB_PRECOMPILE_H

//#include <grpcpp/grpcpp.h> //ServerBuilder

// #include <QUdpSocket> //QUdpSocket

#ifdef Q_OS_UNIX
// #include <unistd.h> //usleep
#else
//#include <qtsinglecoreapplication.h> //QtSingleCoreApplication
#endif

#ifndef Q_OS_MACOS

// #include <log4qt/logger.h>
// #include <log4qt/propertyconfigurator.h>
// #include <log4qt/loggerrepository.h>
// #include <log4qt/consoleappender.h>
// #include <log4qt/logmanager.h>
// #include <log4qt/fileappender.h>

#endif

// #include <QTcpSocket> //QTcpSocket
// #include <QDateTime> //QDateTime

// #include <QStandardPaths> //QStandardPaths
// #include <QJsonDocument> //QJsonDocument
// #include <QJsonObject> //QJsonObject
// #include <QWebSocket> //QWebSocket
// #include <QtMath> //qSqrt
//#include <yaml-cpp/yaml.h> //YAML

#include <QDir> //QDir
// #include <QAudioInput> //QAudioInput
//#include <google/protobuf/util/json_util.h> //MessageToJsonString
// #include <QNetworkAccessManager> //QNetworkAccessManager
// #include <QNetworkReply> //QNetworkReply
// #include <memory> //shared_ptr.

// using namespace std; //string
//using namespace google::protobuf::util; //MessageToJsonString

#endif
