/********************************************************************************
** Form generated from reading UI file 'window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Window
{
public:
    QLabel *lblImg;
    QLineEdit *textName;
    QLabel *label_2;
    QPushButton *btnAdd;
    QLabel *lblStatus;

    void setupUi(QWidget *Window)
    {
        if (Window->objectName().isEmpty())
            Window->setObjectName(QString::fromUtf8("Window"));
        Window->resize(800, 600);
        lblImg = new QLabel(Window);
        lblImg->setObjectName(QString::fromUtf8("lblImg"));
        lblImg->setGeometry(QRect(6, 6, 680, 480));
        lblImg->setAutoFillBackground(true);
        textName = new QLineEdit(Window);
        textName->setObjectName(QString::fromUtf8("textName"));
        textName->setGeometry(QRect(70, 520, 231, 25));
        label_2 = new QLabel(Window);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 520, 67, 17));
        btnAdd = new QPushButton(Window);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setGeometry(QRect(130, 560, 89, 25));
        lblStatus = new QLabel(Window);
        lblStatus->setObjectName(QString::fromUtf8("lblStatus"));
        lblStatus->setGeometry(QRect(410, 520, 331, 41));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        lblStatus->setFont(font);
        lblStatus->setAutoFillBackground(true);

        retranslateUi(Window);

        QMetaObject::connectSlotsByName(Window);
    } // setupUi

    void retranslateUi(QWidget *Window)
    {
        Window->setWindowTitle(QCoreApplication::translate("Window", "Window", nullptr));
        lblImg->setText(QString());
        label_2->setText(QCoreApplication::translate("Window", "Name:", nullptr));
        btnAdd->setText(QCoreApplication::translate("Window", "Add", nullptr));
        lblStatus->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Window: public Ui_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_H
