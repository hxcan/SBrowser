#ifndef ERRORCODETRANSLATOR_H
#define ERRORCODETRANSLATOR_H

#include "TdPreCompile.h" //QObject

#include "ErrorCodeContainer.h" //ErrorCodeContainer

class  ErrorCodeTranslator:public QObject
{
    Q_OBJECT
    
public:
    QString getSeverityLevelStringByLevel(const ErrorCodeContainer::SeverityLevel level) const; //!<获取严重级别字符串。
    QString getErrorMessageByCode(ErrorCodeContainer::ErrorCode errorCode) const; //!<获取与错误码对应的字符串。
    QString getErrorMessageByCode(const qint64 & errorCode) const; //!<获取与错误码对应的字符串。
    ErrorCodeTranslator();
    
private:
    void initializeMemberObjects(); //!<
    QMap<ErrorCodeContainer::ErrorCode, QString> errorCodeMessageMap; //!<错误码与错误描述消息之间的映射
    QMap<ErrorCodeContainer::SeverityLevel, QString> severityLevelStringMap; //!<严重级别与对应的名字字符串的映射。
}; //class ERROR_CODE_SUB_SYSTEM_EXPORT ErrorCodeTranslator:public QObject
                                                        
#endif
