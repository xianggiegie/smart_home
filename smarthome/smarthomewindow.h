#ifndef SMARTHOMEWINDOW_H
#define SMARTHOMEWINDOW_H

#include <QMainWindow>
#include "led.h"
#include "beep.h"
#include "jdq.h"
#include "sg90.h"
#include "sr501.h"
//#include "bluetoothuart.h"
#include "dht11.h"
#include "mq2adc.h"
#include "ap3216c.h"
//#include "v4l2.h"
#include "dht11_thread.h"
#include "esp8266.h"

class QPushButton;
class QSlider;
class QProgressBar;
class QLabel;
class QString;

class Esp82266;

class SmartHomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    SmartHomeWindow(QWidget *parent = nullptr);
    ~SmartHomeWindow();
    Led* getLed() const { return led; }
    Beep* getBeep() const { return beep; }
    Sg90* getSg90() const { return sg90; }
    Jdq* getSJdq() const { return jdq; }

    void setLightButtonText(const QString &text);
    void setBeepButtonText(const QString &text);
    void setDoorlockButtonText(const QString &text);
    void setCurtainValue(int value);



public slots:
//    void uart_handler( char* module,  char*  value);
    void dht11_handler(void);
//    void mq2_handler(void);
    void ap3216c_handler(void);
//    void show_main_page(void);

signals:


private slots:
    void on_light_button_toggled(bool checked);

    void on_beep_button_toggled(bool checked);

    void on_doorlock_button_toggled(bool checked);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_sliderReleased();

//    void on_camera_button_clicked();


private:
    void initUi();

    QPushButton *light_button;
    QPushButton *beep_button;
    QPushButton *doorlock_button;
    QSlider *horizontalSlider;
    QProgressBar *progressBar;

    QLabel *tamp_value;
    QLabel *humi_value;
    QLabel *dis_value;
    QLabel *ir_value;
    QLabel *als_value;
    QLabel *move_value;

    Led *led;
    Beep * beep;
    Jdq * jdq;
    Sg90 * sg90;
    Sr501 * sr501;
//    BluetoothUart * bluetoothUart;
    Mq2ADC * mq2ADC;
    Ap3216c * ap3216c;
    DHT11Thread *dht11_thread;
//    V4l2 *v4l2;

    static void sr501_handler(int signum);
    Esp82266 *esp8266;

};
#endif // SMARTHOMEWINDOW_H
