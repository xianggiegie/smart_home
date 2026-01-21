#ifndef REMOTECONTROLWINDOW_H
#define REMOTECONTROLWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QPushButton;
class QSlider;
class QProgressBar;
class QLabel;
class WebapiRemote;

class RemoteControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RemoteControlWindow(QWidget *parent = nullptr);
    ~RemoteControlWindow();

private slots:
    void onLightToggled(bool checked);
    void onBeepToggled(bool checked);
    void onDoorlockToggled(bool checked);
    void onCurtainValueChanged(int value);
    void onDeviceStateChanged(const QString &deviceState);
    void onTemperatureHumidityChanged(int temperature, int humidity);
    void onAp3216cChanged(int distance, int ir, int als);
    void onDeviceListReady(QStringList deviceNames, QStringList deviceNumbers);

private:
    void initUi();

    WebapiRemote *webapi;

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

    QLabel *cloud_state_label;

    QString deviceNameLight;
    QString deviceNameBeep;
    QString deviceNameDoorlock;
    QString deviceNameCurtain;
};

#endif // REMOTECONTROLWINDOW_H
