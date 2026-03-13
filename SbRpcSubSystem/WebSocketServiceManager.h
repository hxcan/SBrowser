#ifndef WEBSOCKETSERVICEMANAGER_H
#define WEBSOCKETSERVICEMANAGER_H

#include "RbSbPreCompile.h" //QObject

class WebSocketServiceManager : public QObject
{
    Q_OBJECT

public:
    void start(const quint16 aliyunUdpPort); //!<开始工作。
    void start(const QList<quint16> portList); //!<开始工作，接收消息队列中的消息。
    void stop(); //!<停止

private:
    QList<QWebSocketServer*> serverlist; //!<服务器对象列表
    void acceptNewConnection(); //!<有新连接到达，则接受新连接。
    void quit(const qint64 & timeStamp =0) const; //!<退出。
    void processMessage(const QByteArray &datagram); //!<处理消息。
    void requestFaceMask(const QCborValue & decodedMessage) const; //!<要求做人脸掩盖

signals:
    void shouldViewImage(const QString & averageHoldDateAmount, const QByteArray & imageContent) const; //!<信号，应当显示图片
    void newConnection(QWebSocket * webSocket); //!<信号，有新连接到来。
    void shouldQuit(qint64 timeStamp) const; //!<信号，应当退出。
    void listenFailed() const; //!<Signal, Listen failed
}; //class RPC_SUB_SYSTEM_EXPORT WebSocketServiceManager : public QObject


#endif
