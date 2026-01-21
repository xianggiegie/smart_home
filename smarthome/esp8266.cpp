/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   esp8266
* @brief         esp8266.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-27
*******************************************************************/
#include "esp8266.h"
#include <unistd.h>
#include <QDebug>


Esp82266::Esp82266(SmartHomeWindow *window, QObject *parent)
    : QObject(parent), mainWindow(window)
{
    Q_UNUSED(parent)
    /* 串口对象，用于与Esp8266模块通信 */
    serialPort = new QSerialPort(this);

    /* 定时器对象，用于定时发送设备在线的心跳包 */
    timer = new QTimer();

    /* led对象，用于串口接收到云发过来的数据，然后控制板子的LED */

    /* 设置串口名 */
#if __arm__
    serialPort->setPortName("ttymxc2");
#else
    serialPort->setPortName("ttyUSB0");
#endif

    /* 设置波特率 */
    serialPort->setBaudRate(115200);

    /* 设置数据位数 */
    serialPort->setDataBits(QSerialPort::Data8);

    /* 设置奇偶校验 */
    serialPort->setParity(QSerialPort::NoParity);

    /* 设置停止位 */
    serialPort->setStopBits(QSerialPort::OneStop);

    /* 设置流控制 */
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite))
        qDebug()<<"串口无法打开！可能正在被使用！"<<endl;
    else {
        qDebug()<<"串口打开成功！"<<endl;
    }

    /* 开始连接云 */
    connectToClound();

    connect(serialPort, SIGNAL(readyRead()),
            this, SLOT(serialPortReadyRead()));

    connect(timer, SIGNAL(timeout()),
            this, SLOT(onTimerTimeOut()));
}

void Esp82266::serialPortReadyRead()
{
    /* 接收缓冲区中读取数据 */
    QByteArray buf = serialPort->readAll();
    QString temp = QString(buf);
    char buf1[1];
    readData.append(temp);
    qDebug()<<temp<<endl;

    if  (readData.contains("ready")) {
        /* 如果复位成功 */
        sendCmdToEsp8266("AT+CWMODE=1");
        readData.clear();
    }

    if (readData.contains("OK") && readData.contains("AT+CWMODE")) {
        qDebug()<<"设置STA模式成功"<<endl;
        sendCmdToEsp8266("AT+CWJAP=\"Xiaomi_44DC\",\"mima123456\"");
        qDebug()<<"开始连接WIFI"<<endl;
        readData.clear();
    }

    if  (temp.contains("WIFI GOT IP")) {
        qDebug()<<"连接WIFI成功"<<endl;
        sleep(2);
        /* 原子云的设备号及密码 */
        sendCmdToEsp8266("AT+ATKCLDSTA=\"42113345401276863637\",\"12345678\"");
        qDebug()<<"开始连接原子云请等待"<<endl;
    }

    if (temp.contains("CLOUD CONNECTED")) {
        qDebug()<<"连接原子云成功"<<endl;
        sleep(2);
        /* 15s就发送一次心跳包 */
        timer->start(10000);
    }

    if (temp == "开") {
        buf1[0] = '1';
        mainWindow->getLed()->led_write(buf1);
        mainWindow->setLightButtonText("灯开");
    }
    if (temp == "关") {
        qDebug() << "[DEBUG] 收到 'guan' 指令，准备关灯";
        buf1[0] = '0';
        mainWindow->getLed()->led_write(buf1);
        mainWindow->setLightButtonText("灯关");
    }
    if (temp == "kailing") {
        buf1[0] = '1';
        mainWindow->getBeep()->beep_write(buf1);
        mainWindow->setBeepButtonText("门铃开");
    }
    if (temp == "guanling") {
        buf1[0] = '0';
        mainWindow->getBeep()->beep_write(buf1);
        mainWindow->setBeepButtonText("门铃关");
    }
    if (temp == "kaimen") {
        buf1[0] = 1;
        mainWindow->getSJdq()->jdq_write(buf1);
        mainWindow->setDoorlockButtonText("门开");
    }
    if (temp == "guanmen") {
        buf1[0] = 0;
        mainWindow->getSJdq()->jdq_write(buf1);
        mainWindow->setDoorlockButtonText("门关");
    }
    if (temp.startsWith("sg90:")) {
        bool ok;
        int angle = temp.mid(5).toInt(&ok); // 提取角度部分
        if (ok && angle >= 0 && angle <= 180) {
            char buf[1];
            buf[0] = static_cast<char>(angle);
            mainWindow->getSg90()->sg90_write(buf);
            mainWindow->setCurtainValue(angle);
            qDebug() << "[DEBUG] 云端控制 SG90 到 " << angle << " 度";
        } else {
            qDebug() << "[WARN] 收到无效的 sg90 命令：" << temp;
        }
    }

}

Esp82266::~Esp82266()
{
    serialPort->close();
    delete timer;
    timer = nullptr;
}

void Esp82266::sendCmdToEsp8266(QString cmd)
{
    cmd = cmd + "\r\n";

    QByteArray data = cmd.toUtf8();
    serialPort->write(data);
}

void Esp82266::connectToClound()
{
    /* 重启模块，注意若已经连接上原子云，
     * 需要重新上电或者短接RST脚来复位模块 */
    sendCmdToEsp8266("AT+RST");
}

void Esp82266::sleep(int second)
{
    usleep(second * 1000000);
}

void Esp82266::sendTextMessage(QString message)
{
    serialPort->write(message.toLatin1());
}

void Esp82266::onTimerTimeOut()
{

}
