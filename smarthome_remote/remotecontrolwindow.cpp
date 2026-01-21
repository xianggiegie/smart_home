#include "remotecontrolwindow.h"

#include "webapi_remote/webapi_remote.h"

#include <QPushButton>
#include <QSlider>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QInputDialog>

RemoteControlWindow::RemoteControlWindow(QWidget *parent)
    : QMainWindow(parent)
    , webapi(new WebapiRemote(this))
    , light_button(nullptr)
    , beep_button(nullptr)
    , doorlock_button(nullptr)
    , horizontalSlider(nullptr)
    , progressBar(nullptr)
    , tamp_value(nullptr)
    , humi_value(nullptr)
    , dis_value(nullptr)
    , ir_value(nullptr)
    , als_value(nullptr)
    , move_value(nullptr)
    , cloud_state_label(nullptr)
    , deviceNameLight(QStringLiteral("智能灯"))
    , deviceNameBeep(QStringLiteral("门铃"))
    , deviceNameDoorlock(QStringLiteral("门锁"))
    , deviceNameCurtain(QStringLiteral("窗帘"))
{
    initUi();

    connect(light_button, &QPushButton::toggled, this, &RemoteControlWindow::onLightToggled);
    connect(beep_button, &QPushButton::toggled, this, &RemoteControlWindow::onBeepToggled);
    connect(doorlock_button, &QPushButton::toggled, this, &RemoteControlWindow::onDoorlockToggled);
    connect(horizontalSlider, &QSlider::valueChanged, this, &RemoteControlWindow::onCurtainValueChanged);

    connect(webapi, &WebapiRemote::deviceListReady, this, &RemoteControlWindow::onDeviceListReady);
    connect(webapi, SIGNAL(deviceStateChanged(QString)), this, SLOT(onDeviceStateChanged(QString)));
    connect(webapi, SIGNAL(temperatureHumidityChanged(int,int)), this, SLOT(onTemperatureHumidityChanged(int,int)));
    connect(webapi, SIGNAL(ap3216cChanged(int,int,int)), this, SLOT(onAp3216cChanged(int,int,int)));
    connect(webapi, &WebapiRemote::moveChanged, this, [this](int move) {
        if (move_value) {
            move_value->setNum(move);
        }
    });

    webapi->setToken(QStringLiteral("256bdd22fd2b4d099762f4ace7bba5fa"));
    webapi->start();
}

void RemoteControlWindow::onDeviceListReady(QStringList deviceNames, QStringList deviceNumbers)
{
    if (deviceNames.isEmpty() || deviceNumbers.isEmpty()) {
        return;
    }

    QStringList items;
    for (int i = 0; i < deviceNames.count() && i < deviceNumbers.count(); ++i) {
        items << QString("%1(%2)").arg(deviceNames[i], deviceNumbers[i]);
    }

    bool ok = false;
    const QString chosen = QInputDialog::getItem(this, QStringLiteral("选择节点"), QStringLiteral("设备："), items, 0, false, &ok);
    if (!ok || chosen.isEmpty()) {
        return;
    }

    int idx = items.indexOf(chosen);
    if (idx < 0 || idx >= deviceNumbers.count()) {
        return;
    }

    webapi->setDeviceNumber(deviceNumbers[idx]);
    if (cloud_state_label) {
        cloud_state_label->setText(QStringLiteral("节点：%1").arg(chosen));
    }
}

RemoteControlWindow::~RemoteControlWindow()
{
}

void RemoteControlWindow::initUi()
{
    setGeometry(0, 0, 800, 480);
    setMinimumWidth(700);
    setMinimumHeight(450);
    setObjectName("smarthome");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setStyleSheet(
        "QWidget#smarthome { background:#1f1f20; }"
        "QWidget#widget3 { background:#3c3c3c; border-radius:20px; }"
        "QWidget#widget5 { background:#3c3c3c; border-radius:20px; }"
        "QWidget#widget4 { background:#3c3c3c; border-radius:20px; }"
        "QLabel#label0 { color:white; font-size:25px; }"
        "QLabel#label1 { color:#88eeeeee; font-size:15px; }"
        "QLabel#label2 { color:white; font-size:20px; }"
        "QPushButton { color:white; font-size:15px; background:#1f1f20; border-radius:5px; padding:6px 12px; }"
        "QPushButton:checked { background:#f39800; }"
        "QProgressBar { border:none; background:transparent; color:white; }"
        "QProgressBar::chunk { background:#f39800; border-radius:4px; }"
        "QSlider::groove:horizontal { height:6px; background:#1f1f20; border-radius:3px; }"
        "QSlider::handle:horizontal { width:14px; margin:-5px 0; border-radius:7px; background:#f39800; }"
    );

    QVBoxLayout *root = new QVBoxLayout(central);
    root->setContentsMargins(30, 20, 30, 20);
    root->setSpacing(18);

    QWidget *header = new QWidget(central);
    QVBoxLayout *headerLayout = new QVBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(6);

    QLabel *title = new QLabel(header);
    title->setObjectName("label0");
    title->setText("Smart Home");

    QLabel *subtitle = new QLabel(header);
    subtitle->setObjectName("label1");
    subtitle->setText("远程控制端");

    cloud_state_label = new QLabel(header);
    cloud_state_label->setObjectName("label1");
    cloud_state_label->setText("Cloud: connecting...");

    headerLayout->addWidget(title);
    headerLayout->addWidget(subtitle);
    headerLayout->addWidget(cloud_state_label);
    root->addWidget(header);

    QWidget *controlPanel = new QWidget(central);
    controlPanel->setObjectName("widget3");
    QVBoxLayout *controlPanelLayout = new QVBoxLayout(controlPanel);
    controlPanelLayout->setContentsMargins(30, 18, 30, 18);
    controlPanelLayout->setSpacing(12);

    QLabel *controlTitle = new QLabel(controlPanel);
    controlTitle->setObjectName("label2");
    controlTitle->setText("常用控制面板");
    controlPanelLayout->addWidget(controlTitle);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->setSpacing(18);

    light_button = new QPushButton(controlPanel);
    beep_button = new QPushButton(controlPanel);
    doorlock_button = new QPushButton(controlPanel);

    light_button->setCheckable(true);
    beep_button->setCheckable(true);
    doorlock_button->setCheckable(true);

    light_button->setText("灯关");
    beep_button->setText("门铃关");
    doorlock_button->setText("门关");

    controlLayout->addWidget(light_button);
    controlLayout->addWidget(beep_button);
    controlLayout->addWidget(doorlock_button);
    controlLayout->addStretch(1);

    controlPanelLayout->addLayout(controlLayout);
    root->addWidget(controlPanel);

    QWidget *curtainPanel = new QWidget(central);
    curtainPanel->setObjectName("widget4");
    QVBoxLayout *curtainLayout = new QVBoxLayout(curtainPanel);
    curtainLayout->setContentsMargins(30, 18, 30, 18);
    curtainLayout->setSpacing(10);

    QLabel *curtainTitle = new QLabel(curtainPanel);
    curtainTitle->setObjectName("label2");
    curtainTitle->setText("窗帘 / 舵机");
    curtainLayout->addWidget(curtainTitle);

    horizontalSlider = new QSlider(Qt::Horizontal, curtainPanel);
    progressBar = new QProgressBar(curtainPanel);

    horizontalSlider->setRange(0, 100);
    horizontalSlider->setTracking(false);
    progressBar->setRange(0, 100);
    progressBar->setTextVisible(false);

    curtainLayout->addWidget(horizontalSlider);
    curtainLayout->addWidget(progressBar);
    root->addWidget(curtainPanel);

    QWidget *sensorPanel = new QWidget(central);
    sensorPanel->setObjectName("widget5");
    QVBoxLayout *sensorPanelLayout = new QVBoxLayout(sensorPanel);
    sensorPanelLayout->setContentsMargins(30, 18, 30, 18);
    sensorPanelLayout->setSpacing(12);

    QLabel *sensorTitle = new QLabel(sensorPanel);
    sensorTitle->setObjectName("label2");
    sensorTitle->setText("传感器数据");
    sensorPanelLayout->addWidget(sensorTitle);

    QHBoxLayout *row1 = new QHBoxLayout();
    row1->setContentsMargins(30, 0, 0, 0);
    row1->setSpacing(16);

    QHBoxLayout *row2 = new QHBoxLayout();
    row2->setContentsMargins(30, 0, 0, 0);
    row2->setSpacing(16);

    QLabel *tLabel = new QLabel("温度:", sensorPanel);
    QLabel *hLabel = new QLabel("湿度:", sensorPanel);
    QLabel *dLabel = new QLabel("距离:", sensorPanel);
    QLabel *irLabel = new QLabel("红外:", sensorPanel);
    QLabel *alsLabel = new QLabel("光照:", sensorPanel);
    QLabel *mvLabel = new QLabel("人体:", sensorPanel);

    tLabel->setStyleSheet("color:white");
    hLabel->setStyleSheet("color:white");
    dLabel->setStyleSheet("color:white");
    irLabel->setStyleSheet("color:white");
    alsLabel->setStyleSheet("color:white");
    mvLabel->setStyleSheet("color:white");

    tamp_value = new QLabel("0", sensorPanel);
    humi_value = new QLabel("0", sensorPanel);
    dis_value = new QLabel("0", sensorPanel);
    ir_value = new QLabel("0", sensorPanel);
    als_value = new QLabel("0", sensorPanel);
    move_value = new QLabel("0", sensorPanel);

    auto setupValueLabel = [](QLabel *lbl) {
        lbl->setMinimumWidth(40);
        lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        lbl->setStyleSheet("color:#88ffffff; font-size:14px;");
    };

    setupValueLabel(tamp_value);
    setupValueLabel(humi_value);
    setupValueLabel(dis_value);
    setupValueLabel(ir_value);
    setupValueLabel(als_value);
    setupValueLabel(move_value);

    row1->addWidget(tLabel);
    row1->addWidget(tamp_value);
    row1->addSpacing(10);
    row1->addWidget(hLabel);
    row1->addWidget(humi_value);
    row1->addStretch(1);

    row2->addWidget(dLabel);
    row2->addWidget(dis_value);
    row2->addSpacing(10);
    row2->addWidget(irLabel);
    row2->addWidget(ir_value);
    row2->addSpacing(10);
    row2->addWidget(alsLabel);
    row2->addWidget(als_value);
    row2->addSpacing(10);
    row2->addWidget(mvLabel);
    row2->addWidget(move_value);
    row2->addStretch(1);

    sensorPanelLayout->addLayout(row1);
    sensorPanelLayout->addLayout(row2);
    sensorPanelLayout->addStretch(1);
    root->addWidget(sensorPanel);
}

void RemoteControlWindow::onLightToggled(bool checked)
{
    light_button->setText(checked ? "灯开" : "灯关");
    if (webapi) {
        webapi->sendCommand(checked ? "开" : "关");
    }
}

void RemoteControlWindow::onBeepToggled(bool checked)
{
    beep_button->setText(checked ? "门铃开" : "门铃关");
    if (webapi) {
        webapi->sendCommand(checked ? "kailing" : "guanling");
    }
}

void RemoteControlWindow::onDoorlockToggled(bool checked)
{
    doorlock_button->setText(checked ? "门开" : "门关");
    if (webapi) {
        webapi->sendCommand(checked ? "kaimen" : "guanmen");
    }
}

void RemoteControlWindow::onCurtainValueChanged(int value)
{
    progressBar->setValue(value);
    if (webapi) {
        webapi->sendCommand(QString("sg90:%1").arg(value));
    }
}

void RemoteControlWindow::onDeviceStateChanged(const QString &deviceState)
{
    if (cloud_state_label) {
        cloud_state_label->setText(deviceState);
    }
}

void RemoteControlWindow::onTemperatureHumidityChanged(int temperature, int humidity)
{
    if (tamp_value) {
        tamp_value->setNum(temperature);
    }
    if (humi_value) {
        humi_value->setNum(humidity);
    }
}

void RemoteControlWindow::onAp3216cChanged(int distance, int ir, int als)
{
    if (dis_value) {
        dis_value->setNum(distance);
    }
    if (ir_value) {
        ir_value->setNum(ir);
    }
    if (als_value) {
        als_value->setNum(als);
    }
}
