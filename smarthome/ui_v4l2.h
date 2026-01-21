/********************************************************************************
** Form generated from reading UI file 'v4l2.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_V4L2_H
#define UI_V4L2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_V4l2
{
public:
    QWidget *centralwidget;
    QLabel *video_text_label;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *open_button;
    QSpacerItem *verticalSpacer_2;
    QPushButton *photo_button;
    QSpacerItem *verticalSpacer_3;
    QPushButton *takephoto_button;
    QSpacerItem *verticalSpacer_4;
    QPushButton *back_tomain_button;

    void setupUi(QMainWindow *V4l2)
    {
        if (V4l2->objectName().isEmpty())
            V4l2->setObjectName(QString::fromUtf8("V4l2"));
        V4l2->resize(800, 480);
        V4l2->setMinimumSize(QSize(800, 480));
        V4l2->setMaximumSize(QSize(800, 480));
        centralwidget = new QWidget(V4l2);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMinimumSize(QSize(800, 480));
        centralwidget->setMaximumSize(QSize(800, 480));
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        video_text_label = new QLabel(centralwidget);
        video_text_label->setObjectName(QString::fromUtf8("video_text_label"));
        video_text_label->setGeometry(QRect(0, 0, 711, 480));
        QFont font;
        font.setPointSize(28);
        font.setBold(true);
        font.setWeight(75);
        video_text_label->setFont(font);
        video_text_label->setStyleSheet(QString::fromUtf8("color: rgb(245, 121, 0);"));
        video_text_label->setAlignment(Qt::AlignCenter);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(691, 0, 111, 481));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        open_button = new QPushButton(layoutWidget);
        open_button->setObjectName(QString::fromUtf8("open_button"));
        open_button->setMinimumSize(QSize(101, 61));
        QFont font1;
        font1.setPointSize(18);
        open_button->setFont(font1);
        open_button->setStyleSheet(QString::fromUtf8("background-color: rgb(206, 92, 0);"));

        verticalLayout->addWidget(open_button);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        photo_button = new QPushButton(layoutWidget);
        photo_button->setObjectName(QString::fromUtf8("photo_button"));
        photo_button->setMinimumSize(QSize(101, 61));
        photo_button->setFont(font1);
        photo_button->setStyleSheet(QString::fromUtf8("background-color: rgb(206, 92, 0);"));

        verticalLayout->addWidget(photo_button);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        takephoto_button = new QPushButton(layoutWidget);
        takephoto_button->setObjectName(QString::fromUtf8("takephoto_button"));
        takephoto_button->setMinimumSize(QSize(101, 61));
        takephoto_button->setFont(font1);
        takephoto_button->setStyleSheet(QString::fromUtf8("background-color: rgb(206, 92, 0);"));

        verticalLayout->addWidget(takephoto_button);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        back_tomain_button = new QPushButton(layoutWidget);
        back_tomain_button->setObjectName(QString::fromUtf8("back_tomain_button"));
        back_tomain_button->setMinimumSize(QSize(101, 61));
        back_tomain_button->setFont(font1);
        back_tomain_button->setStyleSheet(QString::fromUtf8("background-color: rgb(206, 92, 0);"));

        verticalLayout->addWidget(back_tomain_button);

        verticalLayout->setStretch(0, 3);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 3);
        verticalLayout->setStretch(3, 1);
        verticalLayout->setStretch(4, 3);
        verticalLayout->setStretch(5, 1);
        verticalLayout->setStretch(6, 3);
        V4l2->setCentralWidget(centralwidget);

        retranslateUi(V4l2);

        QMetaObject::connectSlotsByName(V4l2);
    } // setupUi

    void retranslateUi(QMainWindow *V4l2)
    {
        V4l2->setWindowTitle(QApplication::translate("V4l2", "MainWindow", nullptr));
        video_text_label->setText(QApplication::translate("V4l2", "\345\233\276\345\203\217\345\214\272\345\237\237", nullptr));
        open_button->setText(QApplication::translate("V4l2", "\345\274\200\345\247\213", nullptr));
        photo_button->setText(QApplication::translate("V4l2", "\347\233\270\345\206\214", nullptr));
        takephoto_button->setText(QApplication::translate("V4l2", "\346\213\215\347\205\247", nullptr));
        back_tomain_button->setText(QApplication::translate("V4l2", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class V4l2: public Ui_V4l2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_V4L2_H
