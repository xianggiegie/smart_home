#include "webapi_remote.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QUuid>
#include <QUrl>

#include <unistd.h>

WebapiRemote::WebapiRemote(QObject *parent)
    : QObject(parent)
    , networkAccessManager(new QNetworkAccessManager(this))
    , webSocket(new QWebSocket())
    , timer(new QTimer(this))
    , orgURL(QStringLiteral("https://cloud.alientek.com/api/orgs"))
    , started(false)
    , subscribed(false)
    , socket_connected(false)
{   // 生成随机token
    QUuid uuid = QUuid::createUuid();
    random_token = uuid.toString();
    // 连接定时器
    connect(timer, &QTimer::timeout, this, &WebapiRemote::getDevOfGroupUrl);

    // 配置SSL
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    webSocket->setSslConfiguration(config);

    // 连接WebSocket信号
    connect(webSocket, &QWebSocket::connected, this, &WebapiRemote::webSocketConnected);
    // 处理二进制消息
    connect(webSocket, &QWebSocket::binaryMessageReceived, this, [this](const QByteArray &msg) {
        onBinaryMessageReceived(msg);
    });
}
// 设置设备号
void WebapiRemote::setDeviceNumber(const QString &number)
{
    device_number = number;
    subscribed = false;
    if (socket_connected) {
        subscribeSingleDevice();
    }
}
// 获取设备号
QString WebapiRemote::deviceNumber() const
{
    return device_number;
}

WebapiRemote::~WebapiRemote()
{
    if (timer) {
        timer->stop();
    }
    if (webSocket) {
        webSocket->close();
        delete webSocket;
        webSocket = nullptr;
    }
}
// 设置token
void WebapiRemote::setToken(const QString &token)
{
    api_token = token;
}
// 获取token
QString WebapiRemote::token() const
{
    return api_token;
}
// 启动
void WebapiRemote::start()
{
    if (started) {
        return;
    }
    started = true;

    getOrgURL();
}
// 睡眠
void WebapiRemote::sleep(double second)
{
    usleep(static_cast<useconds_t>(second * 1000000));
}
// 获取组织URL
void WebapiRemote::getOrgURL()
{
    getDataFromWeb(QUrl(orgURL));
}
// 获取组列表URL
void WebapiRemote::getGroupListUrl()
{
    if (org_id.isEmpty()) {
        return;
    }
    groupListUrl = QStringLiteral("https://cloud.alientek.com/api/orgs/%1/grouplist").arg(org_id);
    getDataFromWeb(QUrl(groupListUrl));
}
// 获取设备组URL
void WebapiRemote::getDevOfGroupUrl()
{
    if (org_id.isEmpty() || group_id.isEmpty()) {
        return;
    }
    devOfGroupUrl = QStringLiteral("https://cloud.alientek.com/api/orgs/%1/groups/%2/devices").arg(org_id, group_id);
    getDataFromWeb(QUrl(devOfGroupUrl));
}

// 从Web获取数据
void WebapiRemote::getDataFromWeb(const QUrl &url)
{
    if (api_token.isEmpty()) {
        return;
    }

    QNetworkRequest req;

    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    req.setSslConfiguration(config);

    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json;charset=UTF-8"));
    req.setRawHeader("token", api_token.toLatin1());

    QNetworkReply *reply = networkAccessManager->get(req);
    connect(reply, &QNetworkReply::finished, this, &WebapiRemote::replyFinished);
    connect(reply, &QNetworkReply::readyRead, this, &WebapiRemote::readyReadData);
}

// 读取数据
void WebapiRemote::readyReadData()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        return;
    }
    dataString = QString::fromUtf8(reply->readAll());
}

// 回复完成
void WebapiRemote::replyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        return;
    }

    const QUrl url = reply->url();
    reply->deleteLater();

    if (url == QUrl(orgURL)) {
        parseOrgId(dataString);
        getGroupListUrl();

        if (!api_token.isEmpty() && !org_id.isEmpty()) {
            webSocket->open(QUrl(QStringLiteral("wss://cloud.alientek.com/connection/%1/org/%2?token=%3")
                                     .arg(api_token, org_id, random_token)));
        }
        return;
    }

    if (url == QUrl(groupListUrl)) {
        parseGroupId(dataString);
        if (!group_id.isEmpty()) {
            if (!timer->isActive()) {
                timer->start(500);
            }
        }
        return;
    }

    if (url == QUrl(devOfGroupUrl)) {
        parseDeviceList(dataString);
        if (timer->isActive()) {
            timer->stop();
        }
        if (!device_numbers.isEmpty()) {
            emit deviceListReady(device_names, device_numbers);
        }
        return;
    }
}

// 解析组织ID
void WebapiRemote::parseOrgId(const QString &data)
{
    QRegularExpression re(QStringLiteral("\\\"id\\\":(\\d+)"));
    QRegularExpressionMatch m = re.match(data);
    if (m.hasMatch()) {
        org_id = m.captured(1);
    }
}

// 解析组ID
void WebapiRemote::parseGroupId(const QString &data)
{
    QRegularExpression re(QStringLiteral("\\\"id\\\":(\\d+)"));
    QRegularExpressionMatch m = re.match(data);
    if (m.hasMatch()) {
        group_id = m.captured(1);
    }
}

// 解析设备列表
void WebapiRemote::parseDeviceList(const QString &data)
{
    device_names.clear();
    device_numbers.clear();

    QRegularExpression reName(QStringLiteral("\\\"name\\\":\\\"([^\\\"]+)\\\""));
    QRegularExpression reNumber(QStringLiteral("\\\"number\\\":\\\"(\\d+)\\\""));

    QRegularExpressionMatchIterator itName = reName.globalMatch(data);
    while (itName.hasNext()) {
        QRegularExpressionMatch m = itName.next();
        if (m.hasMatch()) {
            device_names.append(m.captured(1));
        }
    }

    QRegularExpressionMatchIterator itNumber = reNumber.globalMatch(data);
    while (itNumber.hasNext()) {
        QRegularExpressionMatch m = itNumber.next();
        if (m.hasMatch()) {
            device_numbers.append(m.captured(1));
        }
    }

    while (device_names.count() > device_numbers.count()) {
        device_names.removeLast();
    }
    while (device_numbers.count() > device_names.count()) {
        device_numbers.removeLast();
    }
}

// Websocket连接
void WebapiRemote::webSocketConnected()
{
    socket_connected = true;
    emit deviceStateChanged(QStringLiteral("Cloud: connected"));
    subscribeSingleDevice();
}

// 订阅单个设备
void WebapiRemote::subscribeSingleDevice()
{
    if (subscribed) {
        return;
    }
    if (device_number.isEmpty()) {
        return;
    }

    QByteArray cmd;
    cmd.append((char)0x01);
    sendCmd(device_number, cmd);
    subscribed = true;
}

// 发送命令
void WebapiRemote::sendCmd(QString number, QByteArray cmd)
{
    const QStringList list = number.split("");
    for (int i = 0; i < list.count(); i++) {
        if (!list[i].isEmpty()) {
            cmd.append(list[i].toLatin1());
        }
    }

    webSocket->sendBinaryMessage(cmd);
}

// 发送命令消息
void WebapiRemote::sendCmdMessage(QString number, QByteArray cmd, const QString &message)
{
    const QStringList list = number.split("");
    for (int i = 0; i < list.count(); i++) {
        if (!list[i].isEmpty()) {
            cmd.append(list[i].toLatin1());
        }
    }

    cmd.append(message.toUtf8());
    webSocket->sendBinaryMessage(cmd);
}

// 发送命令
void WebapiRemote::sendCommand(const QString &message)
{
    if (device_number.isEmpty()) {
        return;
    }

    if (!socket_connected) {
        return;
    }

    QByteArray cmd;
    cmd.append((char)0x03);
    sendCmdMessage(device_number, cmd, message);
}
// 接收二进制消息
void WebapiRemote::onBinaryMessageReceived(const QByteArray &payload)
{
    const QString msg = QString::fromUtf8(payload);
    emit rawDeviceMessageReceived(msg);

    if (msg.contains(QStringLiteral("online"))) {
        if (!device_number.isEmpty() && msg.contains(device_number)) {
            emit deviceStateChanged(QStringLiteral("节点|在线"));
        }
    }

    {
        QRegularExpression re(QStringLiteral("Temperature:(\\d+),Humidity:(\\d+)"));
        QRegularExpressionMatch m = re.match(msg);
        if (m.hasMatch()) {
            emit temperatureHumidityChanged(m.captured(1).toInt(), m.captured(2).toInt());
        }
    }

    {
        QRegularExpression re(QStringLiteral("Distance:(\\d+),IR:(\\d+),ALS:(\\d+)"));
        QRegularExpressionMatch m = re.match(msg);
        if (m.hasMatch()) {
            emit ap3216cChanged(m.captured(1).toInt(), m.captured(2).toInt(), m.captured(3).toInt());
        }
    }

    {
        QRegularExpression re(QStringLiteral("Move:(\\d+)"));
        QRegularExpressionMatch m = re.match(msg);
        if (m.hasMatch()) {
            emit moveChanged(m.captured(1).toInt());
        }
    }
}
