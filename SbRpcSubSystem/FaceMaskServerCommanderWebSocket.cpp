#include "FaceMaskServerCommanderWebSocket.h" //FaceMaskServerCommanderWebSocket

/*!
 * \brief FaceMaskServerCommanderWebSocket::FaceMaskServerCommanderWebSocket 默认构造函数
 */
FaceMaskServerCommanderWebSocket::FaceMaskServerCommanderWebSocket()
{
//    assessStockServerIp=configurationHolder.getAssessStockServerIp(); //获取服务器的IP

    initializeMembers(); //

    connectSignals(); //连接信号槽。


//    webSocketClient.bind (); //绑定

    connect(&webSocketClient, &QWebSocket::binaryMessageReceived, this, &FaceMaskServerCommanderWebSocket::processMessage); //Process message。


} //FaceMaskServerCommanderWebSocket::FaceMaskServerCommanderWebSocket()

/*!
 * \brief Worker::processMessage
 */
void FaceMaskServerCommanderWebSocket::processMessage(const QByteArray &message)
{

//    auto datagram = webSocketClient.receiveDatagram(); //Read datagram.
    QByteArray messageContent=message; //Read all the content.

    auto decodedMessage=QCborValue::fromCbor (messageContent); //解码

    auto functionName=decodedMessage.toMap ()[QStringLiteral("function")].toString (); //获取功能名字

//    logger()->debug() << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", function: " << functionName; //Debug.


//    AmqpFunctionName functionName=amqpMessage.functionname(); //

    if (functionName=="FaceResponse") //sell out price 结果
    {
        reportFaceMaskResponse(decodedMessage); //报告 sell out price 结果
    } //if (functionName=="StockAssessResponse") //股票评估结果
} //void Worker::processMessage()


/*!
 * \brief VoiceRecognizeReportCommanderWebSocket::initializeMembers 初始化成员变量。
 */
void FaceMaskServerCommanderWebSocket::initializeMembers()
{
    reconnectTimer.setSingleShot(true); //单发
    reconnectTimer.setInterval(100); //延迟尝试

    connectWebSocket(); //连接网页套接字。
} //void SImageView::initializeMembers()


/*!
 * \brief VoiceRecognizeReportCommanderWebSocket::connectWebSocket 套接字断开了，则尝试重新连接。
 */
void FaceMaskServerCommanderWebSocket::connectWebSocket()
{
    QUrl yunZhiShengIp; //要连接的网址。

    yunZhiShengIp=constructWebSocketUrl(); //构造要连接的网址。

    webSocketClient.open((yunZhiShengIp)); //
} //void VoiceRecognizeReportCommanderWebSocket::connectWebSocket()

/*!
 * \brief MicrophoneCommanderWebSocket::constructWebSocketUrl 构造要连接的网址。
 * \return 构造得到的网址。
 */
QUrl FaceMaskServerCommanderWebSocket::constructWebSocketUrl() const
{
    QString searchEnginePrefix=QString("ws://%1:14250").arg(assessStockServerIp); //要连接的网址。

    QString searchUrlString=searchEnginePrefix; //
    QUrl searchUrl(searchUrlString); //

//    logger()->trace()  << __FILE__ << ", " << __LINE__ << ", " << __func__  <<  ", url: " << searchUrl.toString(); //Debug.

    return searchUrl;
} //QUrl ShengHanSpeechRecognizer::constructWebSocketUrl()


/*!
 * \brief AssessStockServerCommander::reportCalculateSellOutPriceResponse 报告 sell out price 结果
 * \param decodedMessage Decoded message
 */
void FaceMaskServerCommanderWebSocket::reportFaceMaskResponse(const QCborValue & decodedMessage) const
{
    auto messageObject=decodedMessage.toMap (); //转换成映射

//    averageHoldDateAmount=messageObject['averageHoldDateAmount']
//    averageHoldProfit=messageObject['averageHoldProfit']
//    averageBuyInPrice=messageObject['averageBuyInPrice']

    auto averageHoldDateAmount=messageObject[QStringLiteral("quitFixPrice")].toDouble(); //平均持有天数

    auto averageHoldProfit=messageObject[QStringLiteral("quitSellPrice")].toDouble (); //平均收益

    auto averageBuyInPrice=messageObject[QStringLiteral("fixPrice")].toDouble (); //平均购买价格


    auto errorCode=messageObject[QStringLiteral("errorCode")].toInteger(); //获取错误码

    auto averageSellPrice=messageObject[QStringLiteral("sellPrice")].toDouble (); //平均卖出价格

//    QString languageId=QString::fromStdString(setLanguageRequest.languageid()); //

//    logger()->debug() << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", profit: " << averageHoldProfit << ", price: " << averageBuyInPrice; //Debug.

    averageHoldProfit=((float)(qRound (averageHoldProfit*100.0)))/100.0;
    averageBuyInPrice=((float)(qRound (averageBuyInPrice*100.0)))/100.0;
    averageSellPrice=((float)(qRound (averageSellPrice*100.0)))/100.0;
    averageHoldDateAmount=((float)(qRound (averageHoldDateAmount*100.0)))/100.0; //Quit fix price


//    logger()->debug() << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", profit: " << averageHoldProfit << ", price: " << averageBuyInPrice; //Debug.

    if (errorCode) //出现了错误
    {
    } //if (errorCode) //出现了错误
    else //未出现错误
    {
        emit shouldReportCalculateSellOutPriceResponse(averageHoldDateAmount, averageHoldProfit, averageBuyInPrice, averageSellPrice); //发射信号，应当报告 calculate sell out price 结果
    }
} //void AssessStockServerCommander::reportCalculateSellOutPriceResponse(const QCborValue & decodedMessage) const

/*!
 * \brief AssessStockServerCommander::requestAssessOneStock request to assess one stock
 * \param request 请求信息对象
 */
void FaceMaskServerCommanderWebSocket::requestFaceMaskServer(const QCborMap & request)
{
//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__  << ", web socket status: " << webSocketClient.state() ; //Debug.

    QByteArray imageFileNameString=messageFactory.constructRequestAssessOneStockMessage(request); //

//    logger()->debug() << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", content length: " << imageFileNameString.length() << ", this: " << this; //Debug.


//    worker.replyMessage(imageFileNameString, queueName); //

    webSocketClient.sendBinaryMessage(imageFileNameString); //发送数据包。

} //AssessStockServerCommander::requestAssessOneStock(const QCborMap & request)

/*!
 * \brief VoiceRecognizeReportCommanderWebSocket::connectSignals 连接信号槽。
 */
void FaceMaskServerCommanderWebSocket::connectSignals()
{
//    connect(&webSocketClient, &QWebSocket::disconnected, this, &FaceMaskServerCommanderWebSocket::connectWebSocket); //套接字断开了，则尝试重新连接。
    connect(&webSocketClient, &QWebSocket::disconnected, &reconnectTimer, QOverload<>::of(&QTimer::start)); //套接字断开了，则尝试重新连接。

//    connect (rotateAct, &QAction::triggered, this, QOverload<>::of(&SImageView::rotate)); //按旋转按钮，则旋转。


    connect(&webSocketClient, &QWebSocket::disconnected, this, &FaceMaskServerCommanderWebSocket::reportWebSocketDisconnected); //套接字断开了，则报告套接字断开了。

} //void VoiceRecognizeReportCommanderWebSocket::connectSignals()


/*!
 * \brief AssessStockServerCommanderWebSocket::reportWebSocketDisconnected  套接字断开了，则报告套接字断开了。
 */
void FaceMaskServerCommanderWebSocket::reportWebSocketDisconnected()
{
    emit webSocketDisconnected(); //发射信号，网页套接字断开了
} //void AssessStockServerCommanderWebSocket::reportWebSocketDisconnected()

