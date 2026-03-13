#ifndef MACOSPERMISSIONREQUESTOR_H
#define MACOSPERMISSIONREQUESTOR_H

#include "SbPreCompile.h" //QObject

class MacOsPermissionRequestor : public QObject
{
  Q_OBJECT

public:
  void requestPermissions(); //!<Request permissions.
  void requestPermission(const QWebEnginePage::Feature & feature, QPointer<QWebEnginePage> customPage, const QUrl &securityOrigin); //!< Request permission.
    
private:
  QPointer<QWebEnginePage > customPage; //!< Custom page.
  void approvePermission(NSString * mediaType); //!< Approve permission.
//    static void permissionResultCallback(const PermissionResultMap & permissionResultMap); //!<Permissions result callback.
  QUrl securityOrigin; //!< Security origin.
  void checkAndRequestPermission(NSString * mediaType); //!< Check and request permission.
};
 
  
#endif
