#ifndef BEEP_H
#define BEEP_H

#include <QObject>
//门铃
class Beep : public QObject
{
    Q_OBJECT
public:
    explicit Beep(QObject *parent = nullptr);
    int beep_write(char *buf);

signals:
private:
    QString beep_device = "/sys/devices/platform/leds/leds/beep/brightness";
    int beep_fd;
};

#endif // BEEP_H
