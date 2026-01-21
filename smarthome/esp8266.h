/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   esp8266
* @brief         esp8266.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-27
*******************************************************************/
#ifndef ESP8266_H
#define ESP8266_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include "smarthomewindow.h"


class SmartHomeWindow;  // 前向声明，节省编译时间

class Esp82266 : public QObject
{
    Q_OBJECT

public:
    explicit Esp82266(SmartHomeWindow *window, QObject *parent = nullptr);
    ~Esp82266();
    void sendTextMessage(QString);
private:
    /* led对象 */

    /* 定时器，发送心跳包 */
    QTimer *timer;

    /* 串口对象 */
    QSerialPort *serialPort;

    SmartHomeWindow *mainWindow;

    /* 发送命令到esp8266模块 */
    void sendCmdToEsp8266(QString cmd);

    /* 接收到的数据 */
    QString readData = nullptr;

    /* 延时函数 */
    void sleep(int);

    /* 连接到云服务器 */
    void connectToClound();


private slots:
    /* 串口读消息 */
    void serialPortReadyRead();

    /* 串口发送消息 */


    /* 定时器时间到，发送心跳包 */
    void onTimerTimeOut();
};
#endif // ESP8266_H
