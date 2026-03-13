#ifndef FACEMASKSERVERCOMMANDERWEBSOCKET_H
#define FACEMASKSERVERCOMMANDERWEBSOCKET_H

#include "RbSbPreCompile.h" //QObject

#include "MessageFactory.h" //MessageFactory

class FaceMaskServerCommanderWebSocket : public QObject
{
    Q_OBJECT

public:
    void requestFaceMaskServer(const QCborMap & request); //!<向人脸掩盖服务器发送消息

    FaceMaskServerCommanderWebSocket();

private:
    MessageFactory messageFactory; //!<The message constructing factory.
    QString assessStockServerIp="127.0.0.1"; //!<服务器的IP
    void initializeMembers(); //!<初始化各个成员。
    void connectSignals(); //!<连接信号槽。
    QWebSocket webSocketClient; //!<数据发送器。
    QTimer reconnectTimer; //!<套接字断开了，则尝试重新连接。
    void processMessage(const QByteArray &message); //!<处理消息。
    void reportFaceMaskResponse(const QCborValue & decodedMessage) const; //!<报告 人脸掩盖结果
    void connectWebSocket(); //!<套接字断开了，则尝试重新连接。
    QUrl constructWebSocketUrl() const; //!<构造要连接的网址。
    void reportWebSocketDisconnected(); //!<套接字断开了，则报告套接字断开了。

signals:
    void webSocketDisconnected() const; //!<信号，网页套接字断开了
    void shouldReportStockAssessResponse(const qint32 & averageHoldDateAmount, const float & averageHoldProfit, const float & averageBuyInPrice, const float & averageSellPrice, const qint32 & averageBuyAmount) const; //!<信号，应当报告股票评估结果
    void shouldReportCalculateSellOutPriceResponse(const float & quitFixPrice, const float & quitSellPrice, const float & fixPrice, const float & sellPrice) const; //!<信号，应当报告 calculate sell out price 结果
    void shouldReportCalculateFixEffectResponse(const QString & fixEffectResponseJson) const; //!<信号，应当报告，补仓效果计算结果
    void shouldReportRandomStockCodeResponse(const QString & stockCode) const; //!<信号，收到随机股票代码生成结果
    void shouldReportErrorCode(const qint64 & errorCode, const QString & errorMessage) const; //!<信号，应当报告错误

};

#endif
