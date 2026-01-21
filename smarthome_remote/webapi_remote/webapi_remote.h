#ifndef WEBAPI_REMOTE_H
#define WEBAPI_REMOTE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QWebSocket>
#include <QTimer>
#include <QStringList>

class QNetworkReply;

class WebapiRemote : public QObject
{
    Q_OBJECT
public:
    explicit WebapiRemote(QObject *parent = nullptr);
    ~WebapiRemote();
    // 设置和获取token
    void setToken(const QString &token);
    QString token() const;

    // 启动webapi
    void start();

    // 设置和获取设备号
    void setDeviceNumber(const QString &number);
    QString deviceNumber() const;

    // 发送命令
    void sendCommand(const QString &message);

signals:
    // 设备列表准备就绪
    void deviceListReady(QStringList deviceNames, QStringList deviceNumbers);
    // 设备状态改变
    void deviceStateChanged(QString deviceState);
    // 温湿度改变
    void temperatureHumidityChanged(int temperature, int humidity);
    // AP3216C传感器改变
    void ap3216cChanged(int distance, int ir, int als);
    // 移动检测
    void moveChanged(int move);
    // 原始设备消息接收
    void rawDeviceMessageReceived(QString message);

private:
    // 睡眠函数
    void sleep(double second);

    // 获取orgURL、groupListUrl、devOfGroupUrl
    void getOrgURL();
    // 获取组列表URL
    void getGroupListUrl();
    // 获取设备列表URL
    void getDevOfGroupUrl();

    // 从web获取数据
    void getDataFromWeb(const QUrl &url);

    // 解析数据
    void parseOrgId(const QString &data);
    // 解析组ID
    void parseGroupId(const QString &data);
    // 解析设备列表
    void parseDeviceList(const QString &data);

    // 订阅单个设备
    void subscribeSingleDevice();

    // 发送命令
    void sendCmd(QString number, QByteArray cmd);
    // 发送命令消息
    void sendCmdMessage(QString number, QByteArray cmd, const QString &message);

private slots:
    // 网络回复完成
    void replyFinished();
    // 数据就绪
    void readyReadData();
    // WebSocket连接
    void webSocketConnected();
    // 二进制消息接收
    void onBinaryMessageReceived(const QByteArray &payload);

private:
    // 网络访问管理器
    QNetworkAccessManager *networkAccessManager;
    // WebSocket
    QWebSocket *webSocket;
    // 定时器
    QTimer *timer;

    // API token
    QString api_token;
    // 随机token
    QString random_token;

    // 数据字符串
    QString dataString;

    // 组ID
    QString org_id;
    // 组ID
    QString group_id;

    // 设备号
    QString device_number;

    // 设备名称和编号列表
    QStringList device_names;
    QStringList device_numbers;

    // URL
    QString orgURL;
    // 组列表URL
    QString groupListUrl;
    // 设备列表URL
    QString devOfGroupUrl;

    // 标志位
    bool started;
    bool subscribed;
    bool socket_connected;
};

#endif // WEBAPI_REMOTE_H
