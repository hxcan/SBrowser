#ifndef ERRORCODECONTAINER_INCLUDED
#define ERRORCODECONTAINER_INCLUDED


#include "TdPreCompile.h" //QObject

#include "ErrorCodeSubSystemGlobal.h" //ERROR_CODE_SUB_SYSTEM_EXPORT

//#include "cardreader.pb.h"

class ERROR_CODE_SUB_SYSTEM_EXPORT ErrorCodeContainer:public QObject
{
    Q_OBJECT    
    
public:
    enum ErrorCode
    {
        StockCodeInvalid=95059, //!<股票代码不正确
        StockDataNotEnough=10362, //!<股票数据不足。
        StockPriceTooHigh=10285, //!<股票过贵，买不起。
        
        XunfeiSdkVoiceRecognizeAppIdNotRight=400001, //!<讯飞语音识别SDK的APPID不对。
        
        ConversationalAvatarDirectoryNotExist=500001, //!<ConversationalAvatar资源目录不存在。
    }; //enum ErrorCode
    
    enum SeverityLevel
    {
        SeverityFatal=0, //!<致命级别。
        SeverityWarn=1, //!<警告级别。
    };
    
    ErrorCodeContainer();
}; //class ERROR_CODE_SUB_SYSTEM_EXPORT ErrorCodeContainer:public QObject

      
#endif  // GRPC_cardreader_2eproto__INCLUDED
