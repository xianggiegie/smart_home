#include "smarthomewindow.h"
#include <sys/signal.h>
#include <QThread>
#include <QDebug>
#include <fstream>
#include <iostream>

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSizePolicy>

static QLabel *sr501_move_value = nullptr;

SmartHomeWindow::SmartHomeWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUi();

    /* 视频页 */
   // v4l2 = new V4l2(this);
    //v4l2->setCursor(Qt::BlankCursor); // 隐藏鼠标指针
    //connect(v4l2,&V4l2::backto_main_page,this,&SmartHomeWindow::show_main_page);

    /* 设置滑动条控制范围 */
    horizontalSlider->setRange(0, 100);
    /* 设置滑动条控制步长 */
    horizontalSlider->setSingleStep(0);
    /* 关闭滑块跟踪：滑块仅在用户释放滑块时才发出valueChanged()信号 */
    horizontalSlider->setTracking(false);


    led = new Led(this);
    light_button->setCheckable(true);

    beep = new Beep(this);
    beep_button->setCheckable(true);

    jdq = new Jdq(this);
    doorlock_button->setCheckable(true);

    sg90 = new Sg90(this);

    esp8266 = new Esp82266(this);

    /**
      开启sr501并且注册信号回调函数
      这里为什么要用信号驱动io呢？
      因为你不知道它什么时候才会有数据，它的数据只有发生中断才会有
      但是你不知道它的中断什么时候有阿，所以就要用这种等着它通知的方式来查询
      中断是核心
      */
    sr501 = new Sr501(this);
    signal(SIGIO,sr501_handler);

    //蓝牙
    // bluetoothUart = new BluetoothUart(this);
    // connect(bluetoothUart,&BluetoothUart::uart_recv,this,&SmartHomeWindow::uart_handler);
    //bluetoothUart->start();
    dht11_thread = new DHT11Thread(this);
    connect(dht11_thread, &DHT11Thread::readyto_read_dht11data, this, &SmartHomeWindow::dht11_handler);
    dht11_thread->start();

   // mq2ADC =new Mq2ADC(this);
    //connect(mq2ADC,&Mq2ADC::readyto_read_mq2data,this,&SmartHomeWindow::mq2_handler);

    ap3216c =new Ap3216c(this);
    connect(ap3216c,&Ap3216c::readyto_read_ap3216c_data,this,&SmartHomeWindow::ap3216c_handler);

    connect(light_button, &QPushButton::toggled, this, &SmartHomeWindow::on_light_button_toggled);
    connect(beep_button, &QPushButton::toggled, this, &SmartHomeWindow::on_beep_button_toggled);
    connect(doorlock_button, &QPushButton::toggled, this, &SmartHomeWindow::on_doorlock_button_toggled);

    connect(horizontalSlider, &QSlider::sliderMoved, this, &SmartHomeWindow::on_horizontalSlider_sliderMoved);
    connect(horizontalSlider, &QSlider::valueChanged, this, &SmartHomeWindow::on_horizontalSlider_valueChanged);
    connect(horizontalSlider, &QSlider::sliderReleased, this, &SmartHomeWindow::on_horizontalSlider_sliderReleased);



}

SmartHomeWindow::~SmartHomeWindow()
{
}

void SmartHomeWindow::initUi()
{
    const int rootMarginLeft = 30;      //根布局左边距
    const int rootMarginTop = 20;       //根布局上边距
    const int rootMarginRight = 30;     //根布局右边距
    const int rootMarginBottom = 20;    //根布局下边距
    const int rootSpacing = 18;         //根布局间距

    const int headerSpacing = 6;        //头部间距
    const int titleFontPx = 25;         //标题字体大小
    const int subtitleFontPx = 15;      //副标题字体大小
    const int sectionTitleFontPx = 20;  //章节标题字体大小
    const int buttonFontPx = 15;        //按钮字体大小

    const int panelMarginLeft = 30;     //面板左边距
    const int panelMarginTop = 18;      //面板上边距
    const int panelMarginRight = 30;    //面板右边距
    const int panelMarginBottom = 18;   //面板下边距

    const int buttonWidth = 120;        //按钮宽度
    const int buttonHeight = 36;        //按钮高度

    const int curtainMaxHeight = 0;     //窗帘最大高度
    const int sensorMaxHeight = 160;    //传感器最大高度

    const int curtainStretch = 1;       //窗帘拉伸
    const int sensorStretch = 0;        //传感器拉伸

    setGeometry(0, 0, 800, 480);
    setMinimumWidth(700);
    setMinimumHeight(450);
    setObjectName("smarthome");

    QWidget *central = new QWidget(this);
    central->setObjectName("widget0");
    setCentralWidget(central);

    setStyleSheet(
        "QWidget#smarthome { background:#1f1f20; }"
        "QWidget#widget3 { background:#3c3c3c; border-radius:20px; }"
        "QWidget#widget5 { background:#3c3c3c; border-radius:20px; }"
        "QWidget#widget4 { background:#3c3c3c; border-radius:20px; }"
        + QString("QLabel#label0 { color:white; font-size:%1px; }").arg(titleFontPx)
        + QString("QLabel#label1 { color:#88eeeeee; font-size:%1px; }").arg(subtitleFontPx)
        + QString("QLabel#label2 { color:white; font-size:%1px; }").arg(sectionTitleFontPx)
        + QString("QPushButton { color:white; font-size:%1px; background:#1f1f20; border-radius:5px; padding:6px 12px; }").arg(buttonFontPx)
        + "QPushButton:checked { background:#f39800; }"
        "QProgressBar { border:none; background:transparent; color:white; }"
        "QProgressBar::chunk { background:#f39800; border-radius:4px; }"
        "QSlider::groove:horizontal { height:6px; background:#1f1f20; border-radius:3px; }"
        "QSlider::handle:horizontal { width:14px; margin:-5px 0; border-radius:7px; background:#f39800; }"
    );

    QVBoxLayout *root = new QVBoxLayout(central);
    root->setContentsMargins(rootMarginLeft, rootMarginTop, rootMarginRight, rootMarginBottom); //外边距
    root->setSpacing(rootSpacing); //间距

    QWidget *header = new QWidget(central);
    header->setObjectName("widget1");
    QVBoxLayout *headerLayout = new QVBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(headerSpacing);

    QLabel *title = new QLabel(header);
    title->setObjectName("label0");
    title->setText("Smart Home");
    QLabel *subtitle = new QLabel(header);
    subtitle->setObjectName("label1");
    subtitle->setText("设备控制与传感器数据");
    headerLayout->addWidget(title);
    headerLayout->addWidget(subtitle);
    root->addWidget(header);

    QWidget *controlPanel = new QWidget(central);
    controlPanel->setObjectName("widget3");
    controlPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QVBoxLayout *controlPanelLayout = new QVBoxLayout(controlPanel);
    controlPanelLayout->setContentsMargins(panelMarginLeft, panelMarginTop, panelMarginRight, panelMarginBottom);
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
    light_button->setFocusPolicy(Qt::NoFocus);
    beep_button->setFocusPolicy(Qt::NoFocus);
    doorlock_button->setFocusPolicy(Qt::NoFocus);
    light_button->setFixedSize(buttonWidth, buttonHeight);
    beep_button->setFixedSize(buttonWidth, buttonHeight);
    doorlock_button->setFixedSize(buttonWidth, buttonHeight);
    light_button->setText("灯关");
    beep_button->setText(" 门铃关");
    doorlock_button->setText(" 门关");
    controlLayout->addWidget(light_button);
    controlLayout->addWidget(beep_button);
    controlLayout->addWidget(doorlock_button);
    controlLayout->addStretch(1);
    controlPanelLayout->addLayout(controlLayout);
    root->addWidget(controlPanel, 0);

    QWidget *curtainPanel = new QWidget(central);
    curtainPanel->setObjectName("widget4");
    curtainPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    if (curtainMaxHeight > 0) {
        curtainPanel->setMaximumHeight(curtainMaxHeight);
    }
    QVBoxLayout *curtainLayout = new QVBoxLayout(curtainPanel);
    curtainLayout->setContentsMargins(panelMarginLeft, panelMarginTop, panelMarginRight, panelMarginBottom);
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

    curtainPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    root->addWidget(curtainPanel, curtainStretch);

    QWidget *sensorPanel = new QWidget(central);
    sensorPanel->setObjectName("widget5");
    sensorPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    if (sensorMaxHeight > 0) {
        sensorPanel->setMaximumHeight(sensorMaxHeight);
    }
    QVBoxLayout *sensorPanelLayout = new QVBoxLayout(sensorPanel);
    sensorPanelLayout->setContentsMargins(panelMarginLeft, panelMarginTop, panelMarginRight, panelMarginBottom);
    sensorPanelLayout->setSpacing(12);

    QLabel *sensorTitle = new QLabel(sensorPanel);
    sensorTitle->setObjectName("label2");
    sensorTitle->setText("传感器数据");
    sensorPanelLayout->addWidget(sensorTitle);

    QHBoxLayout *row1 = new QHBoxLayout();          //第一行
    row1->setContentsMargins(30, 0, 0, 0);           //左上右下
    row1->setSpacing(16);                           //间距

    QHBoxLayout *row2 = new QHBoxLayout();          //第二行
    row2->setContentsMargins(30, 0, 0, 0);          //左上右下
    row2->setSpacing(16);                           //间距

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

    tamp_value = new QLabel("0", sensorPanel);      //温度值
    humi_value = new QLabel("0", sensorPanel);      //湿度值
    dis_value = new QLabel("0", sensorPanel);       //距离值
    ir_value = new QLabel("0", sensorPanel);        //红外值
    als_value = new QLabel("0", sensorPanel);       //光照值
    move_value = new QLabel("0", sensorPanel);      //人体值

    auto setupValueLabel = [](QLabel *lbl) {
        lbl->setMinimumWidth(40);
        lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        lbl->setStyleSheet("color:#88ffffff; font-size:14px;");
    };
    setupValueLabel(tamp_value);    //温度值
    setupValueLabel(humi_value);    //湿度值
    setupValueLabel(dis_value);     //距离值
    setupValueLabel(ir_value);      //红外值
    setupValueLabel(als_value);     //光照值
    setupValueLabel(move_value);    //人体值

    row1->addWidget(tLabel);        //温度标签
    row1->addWidget(tamp_value);    //温度值
    row1->addSpacing(10);           //间距
    row1->addWidget(hLabel);        //湿度标签
    row1->addWidget(humi_value);    //湿度值
    row1->addStretch(1);            //拉伸

    row2->addWidget(dLabel);        //距离标签
    row2->addWidget(dis_value);     //距离值
    row2->addSpacing(10);           //间距
    row2->addWidget(irLabel);       //红外标签
    row2->addWidget(ir_value);      //红外值
    row2->addSpacing(10);           //间距
    row2->addWidget(alsLabel);      //光照标签
    row2->addWidget(als_value);     //光照值
    row2->addSpacing(10);           //间距
    row2->addWidget(mvLabel);       //人体标签
    row2->addWidget(move_value);    //人体值
    row2->addStretch(1);            //拉伸

    sensorPanelLayout->addLayout(row1);
    sensorPanelLayout->addLayout(row2);
    sensorPanelLayout->addStretch(1);
    root->addWidget(sensorPanel, sensorStretch);

    sr501_move_value = move_value;
}

void SmartHomeWindow::setLightButtonText(const QString &text)
{
    if (light_button) {
        light_button->setText(text);
    }
}

void SmartHomeWindow::setBeepButtonText(const QString &text)
{
    if (beep_button) {
        beep_button->setText(text);
    }
}

void SmartHomeWindow::setDoorlockButtonText(const QString &text)
{
    if (doorlock_button) {
        doorlock_button->setText(text);
    }
}

void SmartHomeWindow::setCurtainValue(int value)
{
    if (horizontalSlider) {
        horizontalSlider->setValue(value);
    }
    if (progressBar) {
        progressBar->setValue(value);
    }
}

/**
  接收到蓝牙数据之后控制相应的模块
 * @brief SmartHomeWindow::uart_handler
 * @param module
 * @param value
 */
//void SmartHomeWindow::uart_handler(char *module, char *value)
//{
//    char val;
//    int int_value = -1;
//    if(strcmp(module,"curtain") == 0){
//        int_value = atoi(value);
//        if(int_value >= 0 && int_value <= 180){//判断数据是否有效,防止蓝牙乱传
//            val = 100.0 / 180 * int_value;
//            ui->horizontalSlider->setValue(val);
//        }

//    }else if(strcmp(module,"door") == 0){
//        int_value = atoi(value);
//        if(int_value >= 0 && int_value <= 1){
//            //val = int_value;
//            //jdq->jdq_write(&val);

//            bool checked = int_value == 1? true:false;
//            ui->doorlock_button->setChecked(checked);
//            on_doorlock_button_toggled(checked);
//        }

//    }else if(strcmp(module,"beep") == 0){
//        int_value = atoi(value);
//        if(int_value >= 0 && int_value <= 1){
//            //val = int_value;
//            //beep->beep_write(&val);

//            bool checked = int_value == 1? true:false;
//            ui->beep_button->setChecked(checked);
//            on_beep_button_toggled(checked);
//        }

//    }else if(strcmp(module,"led") == 0){
//        int_value = atoi(value);
//        if(int_value >= 0 && int_value <= 1){
//            //val = int_value;
//            //led->led_write(&val);

//            bool checked = int_value == 1? true:false;
//            ui->light_button->setChecked(checked);
//            on_light_button_toggled(checked);
//        }
//    }
//}

/**
  读取温湿度
 * @brief SmartHomeWindow::dht11_handler
 */
void SmartHomeWindow::dht11_handler()
{
    char data[5] = {0};
    dht11_read(data);
    qDebug() << "[dht11] data:" << (int)data[0] << (int)data[1] << (int)data[2] << (int)data[3] << (int)data[4];
    tamp_value->setNum(data[2]);
    humi_value->setNum(data[0]);

    QString msg = QString("Temperature:%1,Humidity:%2\n").arg((int)data[2]).arg((int)data[0]);
    esp8266->sendTextMessage(msg);

}

/**
  读取空气
 * @brief SmartHomeWindow::mq2_handler
 */
//void SmartHomeWindow::mq2_handler()
//{
//    char data[1] = {0};
//    mq2ADC->mq2_read(data);
//    ui->air_value->setNum(data[0]);
//}

/**
  读取红外,光照,距离
 * @brief SmartHomeWindow::ap3216c_handler
 */
void SmartHomeWindow::ap3216c_handler()
{
    char dis_data = 0;
    char ir_data = 0;
    char als_data = 0;

    // 读取 PS 数据
    ap3216c->read_ps(&dis_data);
    // 读取 IR 数据
    ap3216c->read_ir(&ir_data);
    // 读取 ALS 数据
    ap3216c->read_als(&als_data);

    // 更新 UI 中显示的值
    qDebug() << "[ap3216c] dis/ir/als:" << (int)dis_data << (int)ir_data << (int)als_data;
    dis_value->setNum(dis_data);
    ir_value->setNum(ir_data);
    als_value->setNum(als_data);

    QString msg = QString("Distance:%1,IR:%2,ALS:%3\n")
                           .arg((int)dis_data)
                           .arg((int)ir_data)
                           .arg((int)als_data);
    esp8266->sendTextMessage(msg);
}


/**
  控制灯
 * @brief SmartHomeWindow::on_light_button_toggled
 * @param checked
 */
void SmartHomeWindow::on_light_button_toggled(bool checked)
{
    char buf[1];
    if(checked){
        buf[0] = '1';//这里写入1，逻辑有效1
        led->led_write(buf);
        light_button->setText("灯开");
    }else{
        buf[0] = '0';
        led->led_write(buf);
        light_button->setText("灯关");
    }
}

/**
  控制门铃
 * @brief SmartHomeWindow::on_beep_button_toggled
 * @param checked
 */
void SmartHomeWindow::on_beep_button_toggled(bool checked)
{
    char buf[1];
    if(checked){
        buf[0] = '1';//这里写入1，逻辑有效1
        beep->beep_write(buf);
        beep_button->setText(" 门铃开");
    }else{
        buf[0] = '0';
        beep->beep_write(buf);
        beep_button->setText(" 门铃关");
    }
}

/**
  控制门锁
 * @brief SmartHomeWindow::on_doorlock_button_toggled
 * @param checked
 */
void SmartHomeWindow::on_doorlock_button_toggled(bool checked)
{
    char buf[1];
    if(checked){
        buf[0] = 1;//这里写入1，逻辑有效1
        jdq->jdq_write(buf);
        doorlock_button->setText(" 门开");
    }else{
        buf[0] = 0;
       jdq->jdq_write(buf);
       doorlock_button->setText(" 门关");
    }
}

///**
// 窗帘进度条
// * @brief SmartHomeWindow::on_horizontalSlider_sliderMoved
// * @param position
// */
void SmartHomeWindow::on_horizontalSlider_sliderMoved(int position)
{
    progressBar->setValue(position);
}

void SmartHomeWindow::on_horizontalSlider_valueChanged(int value)
{
    char buf[1] = {0};
    progressBar->setValue(value);
    buf[0] = progressBar->value();
    buf[0] = buf[0] / 100.0 * buf[0];
    sg90->sg90_write(buf);
}



//void SmartHomeWindow::on_horizontalSlider_valueChanged(int value)
//{
//    // 1. 定义 unsigned char 以支持 0-255 的数值 (char 默认可能是有符号的 -128~127)
//    unsigned char buf[1] = {0};

//    // 2. 更新 UI 进度条
//    ui->progressBar->setValue(value);

//    // 3. 数值映射逻辑 (假设 Slider 范围是 0-100，舵机需要 0-180)
//    // 如果 Slider 已经是 0-180，则直接 buf[0] = value;
//    int angle = (int)((value / 100.0) * 180.0);

//    // 4. 边界安全检查
//    if(angle > 180) angle = 180;
//    if(angle < 0) angle = 0;

//    buf[0] = (unsigned char)angle;

//    // 5. 打印调试信息 (关键：看看到底发了什么给驱动)
//    qDebug() << "Slider Value:" << value << " -> Angle Sent:" << angle;

//    // 6. 发送给驱动
//    // 确保 sg90 类内部确实打开了文件描述符 fd
//    sg90->sg90_write((char*)buf);
//}

void SmartHomeWindow::on_horizontalSlider_sliderReleased()
{

}

/**
 sr501人体感应
 这个handler需要弄成静态的所以sr501_ui是全局的
 * @brief SmartHomeWindow::sr501_handler
 * @param signum
 */
void SmartHomeWindow::sr501_handler(int signum)
{
    char buf[1] = {0};
    Sr501::sr501_read(buf);
    if (sr501_move_value) {
        sr501_move_value->setNum(buf[0]);
    }
}

/**
 * @brief SmartHomeWindow::on_camera_button_clicked
 * 主页隐藏视频页显示
 */
//void SmartHomeWindow::on_camera_button_clicked()
//{
//    this->hide();//主页隐藏,不释放窗口
//    v4l2->show();
//}

///**
// * @brief SmartHomeWindow::show_main_page
// * 主页显示视频页隐藏
// */
//void SmartHomeWindow::show_main_page()
//{
//    this->show();
//    v4l2->hide();
//}
