#include "dht11_thread.h"
#include "dht11.h"
#include <QDebug>
#include <unistd.h>

DHT11Thread::DHT11Thread(QObject *parent)
    : QThread(parent)
{
}

void DHT11Thread::run()
{
    dht11_init();  // 打开 /dev/dht11

    while (1) {
        emit readyto_read_dht11data();  // 每秒通知一次主线程读取数据
        msleep(1000);
    }
}
