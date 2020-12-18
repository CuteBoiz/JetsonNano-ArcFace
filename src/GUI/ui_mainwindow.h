/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lbl;
    QFrame *frame;
    QPushButton *btnAdd_ID;
    QPushButton *btnDatabase;
    QPushButton *btnExit;
    QMenuBar *menubar;
    QMenu *menuFace_Recognition_In_Embeddeed_System;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        lbl = new QLabel(centralwidget);
        lbl->setObjectName(QString::fromUtf8("lbl"));
        lbl->setGeometry(QRect(100, 530, 311, 17));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 651, 511));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(5);
        frame->setMidLineWidth(2);
        btnAdd_ID = new QPushButton(centralwidget);
        btnAdd_ID->setObjectName(QString::fromUtf8("btnAdd_ID"));
        btnAdd_ID->setGeometry(QRect(690, 10, 89, 25));
        btnDatabase = new QPushButton(centralwidget);
        btnDatabase->setObjectName(QString::fromUtf8("btnDatabase"));
        btnDatabase->setGeometry(QRect(690, 60, 89, 25));
        btnExit = new QPushButton(centralwidget);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        btnExit->setGeometry(QRect(700, 520, 89, 25));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuFace_Recognition_In_Embeddeed_System = new QMenu(menubar);
        menuFace_Recognition_In_Embeddeed_System->setObjectName(QString::fromUtf8("menuFace_Recognition_In_Embeddeed_System"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFace_Recognition_In_Embeddeed_System->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        lbl->setText(QString());
        btnAdd_ID->setText(QApplication::translate("MainWindow", "Add ID", nullptr));
        btnDatabase->setText(QApplication::translate("MainWindow", "Database", nullptr));
        btnExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
        menuFace_Recognition_In_Embeddeed_System->setTitle(QApplication::translate("MainWindow", "Face Recognition In Embeddeed System", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
