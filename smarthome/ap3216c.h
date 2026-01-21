//#ifndef AP3216C_H
//#define AP3216C_H

//#include <QObject>
//#include <QTimer>

//#define AP3216C_IOCTL_BASE    'L'
//#define AP3216C_GETPS       _IOR(AP3216C_IOCTL_BASE, 1, int)
//#define AP3216C_GETLIGHT    _IOR(AP3216C_IOCTL_BASE, 2, int)
//#define AP3216C_GETIR    	_IOR(AP3216C_IOCTL_BASE, 3, int)

//class Ap3216c : public QObject
//{
//    Q_OBJECT
//public:
//    explicit Ap3216c(QObject *parent = nullptr);
//    void ap3216c_read(char *buf);

//public slots:
//     void timeto_read_ap3216cdata(void);

//signals:
//    void readyto_read_ap3216cdata(void);

//private:
//    QString ap3216c_device = "/dev/ap3216c";
//    int ap3216c_fd;
//    QTimer * ap3216c_timer;

//};

//#endif // AP3216C_H
#ifndef AP3216C_H
#define AP3216C_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDebug>

class Ap3216c : public QObject
{
    Q_OBJECT
public:
    explicit Ap3216c(QObject *parent = nullptr);

    // 读取感应器数据
    void read_ps(char *buf);
    void read_ir(char *buf);
    void read_als(char *buf);

public slots:
    void timeto_read_ap3216c_data(void);

signals:
    void readyto_read_ap3216c_data(void);  // 发送信号表示可以处理数据

private:
    QString ps_device = "/sys/class/misc/ap3216c/ps";
    QString ir_device = "/sys/class/misc/ap3216c/ir";
    QString als_device = "/sys/class/misc/ap3216c/als";

    QTimer *ap3216c_timer;  // 定时器
};

#endif
