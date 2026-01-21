#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <QDebug>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/types.h>

static int fd;

void dht11_init(void)
{
    fd = open("/dev/dht11_device", O_RDWR);
    if (fd < 0)
    {
        qDebug()<<"open /dev/dht11 failed";
    }
}

int dht11_read(char *buf)
{
    int len;
    len = read(fd, buf, 5);
    return len;
}


