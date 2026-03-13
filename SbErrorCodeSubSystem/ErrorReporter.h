#ifndef ERRORREPORTER_H
#define ERRORREPORTER_H


//#include "sppas_match.pb.h" //    SppasResultWrapper.h

//using namespace sppasmatch;

//Q_DECLARE_METATYPE(Result)

#include "SrbPreCompile.h" //QObject

#include "ErrorCodeContainer.h" //ErrorCodeContainer

class ERROR_CODE_SUB_SYSTEM_EXPORT ErrorReporter:public QObject
{
    Q_OBJECT
    
public:
    ErrorReporter();
    
signals:
    void errorOccurred(ErrorCodeContainer::ErrorCode errorCode); //!<发生了错误。
}; //class ERROR_CODE_SUB_SYSTEM_EXPORT ErrorReporter:public QObject

#endif // XY_GLOBAL_H
