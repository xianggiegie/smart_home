#ifndef DHT11_THREAD_H
#define DHT11_THREAD_H

#include <QThread>
class DHT11Thread : public QThread
{
    Q_OBJECT
public:
    explicit DHT11Thread(QObject *parent = nullptr);
    void run() override;

signals:
    void readyto_read_dht11data();  // 无参数
};

#endif // DHT11_THREAD_H
