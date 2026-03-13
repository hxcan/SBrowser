#include "ErrorCodeTranslator.h"

ErrorCodeTranslator::ErrorCodeTranslator()
{
    initializeMemberObjects(); //
    
} //ErrorCodeTranslator::ErrorCodeTranslator()

/*!
 * \brief ErrorCodeTranslator::getSeverityLevelStringByLevel 获取严重级别字符串。
 * \param level 严重级别值。
 * \return 对应的字符串。
 */
QString ErrorCodeTranslator::getSeverityLevelStringByLevel(const ErrorCodeContainer::SeverityLevel level) const
{
    QString result; //
    
    result=severityLevelStringMap[level]; //从映射中查询结果。
    
    return result;
    
} //QString ErrorCodeTranslator::getSeverityLevelStringByLevel(ErrorCodeContainer::SeverityLevel level) 

/*!
 * \brief ErrorCodeTranslator::getErrorMessageByCode 获取与错误码对应的字符串。
 * \param errorCode 错误码
 * \return 对应的字符串
 */
QString ErrorCodeTranslator::getErrorMessageByCode(const qint64 & errorCode) const
{
    auto errorCodeEnum=(ErrorCodeContainer::ErrorCode)(errorCode); //转换成枚举

    auto result=getErrorMessageByCode(errorCodeEnum);

    return result;
} //QString ErrorCodeTranslator::getErrorMessageByCode(const qint64 & errorCode) const

/*!
 * \brief ErrorCodeTranslator::getErrorMessageByCode 
 * \param errorCode 
 * \return 
 */
QString ErrorCodeTranslator::getErrorMessageByCode(ErrorCodeContainer::ErrorCode errorCode) const
{
    QString result; //
    
    result=errorCodeMessageMap[errorCode]; //从映射中查询结果。
    
    return result;
} //QString ErrorCodeTranslator::getErrorMessageByCode(ErrorCodeContainer::ErrorCode errorCode)

/*!
 * \brief ErrorCodeTranslator::initializeMemberObjects 
 */
void ErrorCodeTranslator::initializeMemberObjects()
{
    errorCodeMessageMap=
    {
        { ErrorCodeContainer::StockCodeInvalid , tr("Stock code is invalid") },
        { ErrorCodeContainer::StockDataNotEnough , tr("Stock data is not enough") },
        { ErrorCodeContainer::StockPriceTooHigh, tr("Stock price is too high")},
        
        { ErrorCodeContainer::XunfeiSdkVoiceRecognizeAppIdNotRight , tr("Xunfei recognize sdk appid is not right") },
        
        { ErrorCodeContainer::ConversationalAvatarDirectoryNotExist, tr("ConversationalAvatar resources directory does not exist")}, //报告错误码。
        
    };
    
    severityLevelStringMap=
    {
        {ErrorCodeContainer::SeverityFatal, "fatal"}, //致命。
        {ErrorCodeContainer::SeverityWarn, "warn"}, //警告。
    };
} //void ErrorCodeTranslator::initializeMemberObjects()
