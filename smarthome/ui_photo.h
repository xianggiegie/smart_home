/********************************************************************************
** Form generated from reading UI file 'photo.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHOTO_H
#define UI_PHOTO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Photo
{
public:
    QWidget *centralwidget;
    QPushButton *previous_button;
    QPushButton *next_button;
    QPushButton *back_tovideo_button;
    QLabel *photo_text_label;
    QPushButton *delete_button;

    void setupUi(QMainWindow *Photo)
    {
        if (Photo->objectName().isEmpty())
            Photo->setObjectName(QString::fromUtf8("Photo"));
        Photo->resize(800, 480);
        Photo->setMinimumSize(QSize(800, 480));
        Photo->setMaximumSize(QSize(800, 480));
        centralwidget = new QWidget(Photo);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMinimumSize(QSize(800, 480));
        centralwidget->setMaximumSize(QSize(800, 480));
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        previous_button = new QPushButton(centralwidget);
        previous_button->setObjectName(QString::fromUtf8("previous_button"));
        previous_button->setGeometry(QRect(0, 190, 81, 91));
        QFont font;
        font.setPointSize(25);
        previous_button->setFont(font);
        previous_button->setStyleSheet(QString::fromUtf8("color: rgb(85, 87, 83);\n"
"background-color: rgb(206, 92, 0);"));
        next_button = new QPushButton(centralwidget);
        next_button->setObjectName(QString::fromUtf8("next_button"));
        next_button->setGeometry(QRect(720, 190, 81, 91));
        next_button->setFont(font);
        next_button->setStyleSheet(QString::fromUtf8("color: rgb(85, 87, 83);\n"
"background-color: rgb(206, 92, 0);"));
        back_tovideo_button = new QPushButton(centralwidget);
        back_tovideo_button->setObjectName(QString::fromUtf8("back_tovideo_button"));
        back_tovideo_button->setGeometry(QRect(0, 0, 81, 51));
        QFont font1;
        font1.setPointSize(18);
        back_tovideo_button->setFont(font1);
        back_tovideo_button->setStyleSheet(QString::fromUtf8("color: rgb(85, 87, 83);\n"
"background-color: rgb(206, 92, 0);"));
        photo_text_label = new QLabel(centralwidget);
        photo_text_label->setObjectName(QString::fromUtf8("photo_text_label"));
        photo_text_label->setGeometry(QRect(80, 0, 640, 480));
        QFont font2;
        font2.setPointSize(28);
        font2.setBold(true);
        font2.setWeight(75);
        photo_text_label->setFont(font2);
        photo_text_label->setStyleSheet(QString::fromUtf8("color: rgb(204, 0, 0);"));
        photo_text_label->setTextFormat(Qt::AutoText);
        photo_text_label->setAlignment(Qt::AlignCenter);
        delete_button = new QPushButton(centralwidget);
        delete_button->setObjectName(QString::fromUtf8("delete_button"));
        delete_button->setGeometry(QRect(720, 0, 81, 51));
        delete_button->setFont(font1);
        delete_button->setStyleSheet(QString::fromUtf8("color: rgb(85, 87, 83);\n"
"background-color: rgb(206, 92, 0);"));
        Photo->setCentralWidget(centralwidget);

        retranslateUi(Photo);

        QMetaObject::connectSlotsByName(Photo);
    } // setupUi

    void retranslateUi(QMainWindow *Photo)
    {
        Photo->setWindowTitle(QApplication::translate("Photo", "MainWindow", nullptr));
        previous_button->setText(QApplication::translate("Photo", "<", nullptr));
        next_button->setText(QApplication::translate("Photo", ">", nullptr));
        back_tovideo_button->setText(QApplication::translate("Photo", "\350\277\224\345\233\236", nullptr));
        photo_text_label->setText(QApplication::translate("Photo", "\346\227\240\345\233\276\347\211\207", nullptr));
        delete_button->setText(QApplication::translate("Photo", "\345\210\240\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Photo: public Ui_Photo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHOTO_H
