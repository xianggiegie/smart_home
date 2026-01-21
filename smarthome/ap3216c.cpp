//#include "ap3216c.h"
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <string.h>
//#include <QDebug>
//#include <string.h>
//#include <stdint.h>
//#include <sys/ioctl.h>

//Ap3216c::Ap3216c(QObject *parent) : QObject(parent)
//{
//    ap3216c_fd = open(ap3216c_device.toStdString().c_str(),O_RDWR);
//    if(ap3216c_fd < 0){
//        qDebug() << "fail to open ap3216c_device\n";
//        return;
//    }

//    ap3216c_timer = new QTimer(this);
//    ap3216c_timer->setInterval(1000);//周期1s
//    connect(ap3216c_timer,&QTimer::timeout,this,&Ap3216c::timeto_read_ap3216cdata);
//    ap3216c_timer->start();
//}

//void Ap3216c::ap3216c_read(char *buf)
//{
//    ioctl(ap3216c_fd,AP3216C_GETIR,&buf[0]);
//    ioctl(ap3216c_fd,AP3216C_GETLIGHT,&buf[1]);
//    ioctl(ap3216c_fd,AP3216C_GETPS,&buf[2]);
//}

//void Ap3216c::timeto_read_ap3216cdata()
//{
//    emit readyto_read_ap3216cdata();
//}
#include "ap3216c.h"
#include <stdio.h>
#include <stdlib.h>
#include <QString>
#include <QDebug>

Ap3216c::Ap3216c(QObject *parent) : QObject(parent)
{
    // 初始化定时器
    ap3216c_timer = new QTimer(this);
    ap3216c_timer->setInterval(1000);  // 设置定时器时间间隔为1.5秒
    connect(ap3216c_timer, &QTimer::timeout, this, &Ap3216c::timeto_read_ap3216c_data);
    ap3216c_timer->start();  // 启动定时器
}

// 读取 PS 数据
void Ap3216c::read_ps(char *buf)
{
    char data[20];
    FILE *ps_fd = fopen(ps_device.toStdString().c_str(), "r");
    if (ps_fd == NULL) {
        qDebug() << "fail to fopen ap3216c_ps_device";
        return;
    }

    // 读取文件内容
    rewind(ps_fd);  // 确保文件指针在开头
    fscanf(ps_fd, "%s", data);
    *buf = atoi(data);  // 将读取的字符串转换为整型并存储

    fclose(ps_fd);  // 关闭文件
}

// 读取 IR 数据
void Ap3216c::read_ir(char *buf)
{
    char data[20];
    FILE *ir_fd = fopen(ir_device.toStdString().c_str(), "r");
    if (ir_fd == NULL) {
        qDebug() << "fail to fopen ap3216c_ir_device";
        return;
    }

    // 读取文件内容
    rewind(ir_fd);
    fscanf(ir_fd, "%s", data);
    *buf = atoi(data);

    fclose(ir_fd);
}

// 读取 ALS 数据
void Ap3216c::read_als(char *buf)
{
    char data[20];
    FILE *als_fd = fopen(als_device.toStdString().c_str(), "r");
    if (als_fd == NULL) {
        qDebug() << "fail to fopen ap3216c_als_device";
        return;
    }

    // 读取文件内容
    rewind(als_fd);
    fscanf(als_fd, "%s", data);
    *buf = atoi(data);

    fclose(als_fd);
}

// 定时读取 ap3216c 数据
void Ap3216c::timeto_read_ap3216c_data()
{
    char ps_data, ir_data, als_data;

    // 读取 PS 数据
    read_ps(&ps_data);

    // 读取 IR 数据
    read_ir(&ir_data);

    // 读取 ALS 数据
    read_als(&als_data);


    // 如果需要，你可以在此发射信号，或者处理数据
    emit readyto_read_ap3216c_data();
}
