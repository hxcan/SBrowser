#include "MacOsPermissionRequestor.h" // MacOsPermissionRequestor

#include <QTimer> //QTimer
#include <QThread> //QThread
//#include <QProcess> //QProcess
#include <QFileInfo> //QFileInfo
#include <QUrlQuery> //QUrlQuery
//#include <yaml-cpp/node/node.h> //YAML
//#include <yaml-cpp/yaml.h> //LoadFile
#include <QTranslator> //QTranslator
#include <QJsonDocument> //QJsonDocument
//#include <QJsonObject> //QJsonObject
//#include <log4qt/logger.h>
//#include <log4qt/propertyconfigurator.h>
//#include <log4qt/loggerrepository.h>
//#include <log4qt/consoleappender.h>
//#include <log4qt/ttcclayout.h>
//#include <log4qt/logmanager.h>
//#include <log4qt/fileappender.h>

#include <QStandardPaths> //QStandardPaths
//#include <QFile> //QFile
#include <QSettings> //QSettings
//#include <QDir> //QDir
//#include <QStandardPaths> //QStandardPaths
//#include <log4qt/logmanager.h>

//#import <AVFoundation/AVCaptureDevice.h>

//import AVFoundation

#include <AVFoundation/AVCaptureDevice.h>

/*!
 * \brief MacOsPermissionRequestor::approvePermission  Approve permission.
 * \param mediaType The media type.
 */
void MacOsPermissionRequestor::approvePermission(NSString * mediaType)
{
  QWebEnginePage::PermissionPolicy policy=QWebEnginePage::PermissionGrantedByUser;

  auto feature=QWebEnginePage::MediaAudioCapture;

  if (mediaType==AVMediaTypeAudio)
  {

  } // if (mediaType==AVMediaTypeAudio)
  else
  {
    feature=QWebEnginePage::MediaVideoCapture;
  }

  customPage->setFeaturePermission(securityOrigin, feature, policy);

} // void MacOsPermissionRequestor::approvePermission(NSString * mediaType)

/*!
 * \brief MacOsPermissionRequestor::checkAndRequestPermission  Check and request permission.
 * \param mediaType The media type.
 */
void MacOsPermissionRequestor::checkAndRequestPermission(NSString * mediaType)
{
  AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:mediaType];


  if(authStatus == AVAuthorizationStatusAuthorized) {
    // do your logic
    NSLog(@"Access granted");


    approvePermission(mediaType);

  } else if(authStatus == AVAuthorizationStatusDenied){
    NSLog(@"Access denied");
    // denied
  } else if(authStatus == AVAuthorizationStatusRestricted){
    // restricted, normally won't happen
    NSLog(@"Access restricted");

  } else if(authStatus == AVAuthorizationStatusNotDetermined){
    // not determined?!
    NSLog(@"Access not determined");

    [AVCaptureDevice requestAccessForMediaType:mediaType completionHandler:^(BOOL granted) {
      if(granted){
        NSLog(@"Granted access to %@", mediaType);

        approvePermission(mediaType);
      } else {
        NSLog(@"Not granted access to %@", mediaType);
      }
    }];
  }

} // void MacOsPermissionRequestor::checkAndRequestPermission(NSString * mediaType)

/*!
 * \brief MacOsPermissionRequestor::requestPermission  Request permission.
 * \param feature The feature.
 */
void MacOsPermissionRequestor::requestPermission(const QWebEnginePage::Feature & feature, QPointer<QWebEnginePage > customPage, const QUrl & securityOrigin)
{
  this->customPage=customPage;
  this->securityOrigin=securityOrigin;

//  m_videoInput = [AVCaptureDeviceInput
//                 deviceInputWithDevice:videoDevice
//                 error:&error];

//  [AVCaptureDevice
//      requestAccess:.audio
//                     ]

//  AVCaptureDevice *defaultDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];

//  AVCaptureDevice.requestAccess(for: .audio) { granted in
//              if granted {
////                  self.setupCaptureSession()
//              }
//          }

//  NSString *mediaType = AVMediaTypeVideo;

  NSString *mediaType = AVMediaTypeAudio;

  if (feature==QWebEnginePage::MediaAudioVideoCapture)
  {
    NSString *mediaTypeVideo = AVMediaTypeVideo;

    checkAndRequestPermission(mediaTypeVideo); // Check and request permission.
    checkAndRequestPermission(mediaType); // Check and request permission.
  } // QPointer<QWebEnginePage >
  else
  {
    if ((feature==QWebEnginePage::MediaAudioCapture) )
    {
    }
    else //
    {
      mediaType=AVMediaTypeVideo;
    }

    checkAndRequestPermission(mediaType); // Check and request permission.
  }



//  NSLog(@"Auth status: %@", authStatus);


//  [AVCaptureDevice requestAccessForMediaType:mediaType completionHandler:^(BOOL granted) {
//      if(granted){
//        NSLog(@"Granted access to %@", mediaType);
//      } else {
//        NSLog(@"Not granted access to %@", mediaType);
//      }
//    }];
} // void MacOsPermissionRequestor::requestPermission(const QWebEnginePage::Feature & feature)
