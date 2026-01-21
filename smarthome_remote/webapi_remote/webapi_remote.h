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

    void setToken(const QString &token);
    QString token() const;

    void start();

    void setDeviceNumber(const QString &number);
    QString deviceNumber() const;

    void sendCommand(const QString &message);

signals:
    void deviceListReady(QStringList deviceNames, QStringList deviceNumbers);
    void deviceStateChanged(QString deviceState);
    void temperatureHumidityChanged(int temperature, int humidity);
    void ap3216cChanged(int distance, int ir, int als);
    void moveChanged(int move);
    void rawDeviceMessageReceived(QString message);

private:
    void sleep(double second);

    void getOrgURL();
    void getGroupListUrl();
    void getDevOfGroupUrl();

    void getDataFromWeb(const QUrl &url);

    void parseOrgId(const QString &data);
    void parseGroupId(const QString &data);
    void parseDeviceList(const QString &data);

    void subscribeSingleDevice();

    void sendCmd(QString number, QByteArray cmd);
    void sendCmdMessage(QString number, QByteArray cmd, const QString &message);

private slots:
    void replyFinished();
    void readyReadData();
    void webSocketConnected();
    void onBinaryMessageReceived(const QByteArray &payload);

private:
    QNetworkAccessManager *networkAccessManager;
    QWebSocket *webSocket;
    QTimer *timer;

    QString api_token;
    QString random_token;

    QString dataString;

    QString org_id;
    QString group_id;

    QString device_number;

    QStringList device_names;
    QStringList device_numbers;

    QString orgURL;
    QString groupListUrl;
    QString devOfGroupUrl;

    bool started;
    bool subscribed;
    bool socket_connected;
};

#endif // WEBAPI_REMOTE_H
