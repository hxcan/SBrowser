#include "RbSbPreCompile.h" //QNetworkDatagram

#include "WebSocketServiceManager.h" //WebSocketServiceManager

//using namespace com::stupidbeauty::stockshort;

/*!
 * \brief WebSocketServiceManager::start 开始工作，接收消息队列中的消息。
 * \param portList 要监听的端口列表。
 */
void WebSocketServiceManager::start(const QList<quint16> portList)
{
    for(auto currentPort: portList) //一个个地监听。
    {
        start(currentPort); //监听。
    } //for(auto currentPort: portList) //一个个地监听。
} //void WebSocketServiceManager::start(const QList<quint16> portList)

/*!
 * \brief GrpcServiceThread::start 
 */
void WebSocketServiceManager::start(const quint16 aliyunUdpPort)
{
//    auto aliyunUdpPort=13491; //Aliyun process udp port.
    
    QWebSocketServer * server=nullptr; //网页套接字服务器。
    
    server=new QWebSocketServer(QStringLiteral("IKnowNumber WebSocket Server"), QWebSocketServer::NonSecureMode); //
    
    serverlist << server; //加入列表中

    QHostAddress webSocketListenAddress; //
    
    webSocketListenAddress=QHostAddress::Any; //同时监听IPV4和IPV6的地址。
//    webSocketListenAddress=QHostAddress::AnyIPv4; //只监听IPV4地址。
//    logger()->trace() << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", target port: " << aliyunUdpPort << ", this: " << this; //Debug.

    if (!server->listen(webSocketListenAddress, aliyunUdpPort)) //监听地址。
    {
//        logger()->fatal() << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", failed to open web socket server."; //报告错误。
        emit listenFailed(); //Listen failed
    } //if (!server->listen(webSocketListenAddress, 11013)) //
    
    connect(server, &QWebSocketServer::newConnection, this, &WebSocketServiceManager::acceptNewConnection); //有新连接到达，则接受新连接。
} //void GrpcServiceThread::start()

/*!
 * \brief WebSocketServiceManager::stop 停止
 */
void WebSocketServiceManager::stop()
{
    for(auto curetnServer: serverlist) //一个个地停止
    {
        curetnServer->close(); //停止
    } //for(auto curetnServer: serverlist) //一个个地停止
} //void WebSocketServiceManager::stop()

/*!
 * \brief WebSocketServiceManager::acceptNewConnection 有新连接到达，则接受新连接。
 */
void WebSocketServiceManager::acceptNewConnection()
{
    
    QWebSocketServer * server=nullptr; //网页套接字服务器。

    server=qobject_cast<QWebSocketServer*>(sender()); //获取服务器对象的指针。
    
    auto webSocket=server->nextPendingConnection(); //获取连接。

//    logger()->debug() << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", server port: " << server->serverPort(); //Debug.
//    logger()->debug() << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", server port: " << server->serverPort() << ", this: " << this; //Debug.

    connect(webSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServiceManager::processMessage); //收到二进制消息，则处理。


    emit newConnection(webSocket); //发射信号，有新连接到来。
} //void WebSocketServiceManager::acceptNewConnection()

/*!
 * \brief SystemManagementServiceImpl::quit 退出。
 * \param response timeStamp 退出指令发出的时间戳。
 */
void WebSocketServiceManager::quit(const qint64 &timeStamp) const
{
    emit shouldQuit(timeStamp); //发射信号，应当退出。
} //Status HumanBodyDetectReportServiceImpl::reportHumanBodyDetectStatus(grpc::ServerContext *context, const ReportHumanBodyDetectStatusReq *request, DummyReply *response)

/*!
 * \brief WebSocketServiceManager::requestFaceMask 要求做人脸掩盖
 * \param decodedMessage 人脸掩盖消息体
 */
void WebSocketServiceManager::requestFaceMask(const QCborValue & decodedMessage) const
{
    auto messageObject=decodedMessage.toMap (); //转换成映射

    auto averageHoldDateAmount=messageObject[QStringLiteral("imageFileName")].toString(); //图片文件名
    auto imageContent=messageObject[QStringLiteral("imageContent")].toByteArray(); //获取图片内容

    emit shouldViewImage(averageHoldDateAmount, imageContent); //发射信号，应当做人脸掩盖
} //void WebSocketServiceManager::requestFaceMask(const QCborValue & decodedMessage) const

/*!
 * \brief Worker::processMessage
 */
void WebSocketServiceManager::processMessage(const QByteArray &messageContent)
{
    auto decodedMessage=QCborValue::fromCbor (messageContent); //解码

    auto functionName=decodedMessage.toMap ()[QStringLiteral("function")].toString (); //获取功能名字

    if (functionName=="Quit") //quit
    {
        quit(); //quit
    } //if (functionName=="FaceMask") //人脸掩盖
} //void Worker::processMessage()
