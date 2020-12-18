/********************************************************************************
** Form generated from reading UI file 'add_id_form.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_ID_FORM_H
#define UI_ADD_ID_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_add_id_from
{
public:
    QPushButton *btnAdd;
    QPushButton *btnCancel;
    QLineEdit *lineEdit;
    QGraphicsView *avatar;

    void setupUi(QDialog *add_id_from)
    {
        if (add_id_from->objectName().isEmpty())
            add_id_from->setObjectName(QString::fromUtf8("add_id_from"));
        add_id_from->resize(240, 214);
        btnAdd = new QPushButton(add_id_from);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setGeometry(QRect(170, 180, 61, 25));
        btnCancel = new QPushButton(add_id_from);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(70, 180, 89, 25));
        lineEdit = new QLineEdit(add_id_from);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(20, 140, 191, 31));
        avatar = new QGraphicsView(add_id_from);
        avatar->setObjectName(QString::fromUtf8("avatar"));
        avatar->setGeometry(QRect(60, 10, 121, 111));

        retranslateUi(add_id_from);

        QMetaObject::connectSlotsByName(add_id_from);
    } // setupUi

    void retranslateUi(QDialog *add_id_from)
    {
        add_id_from->setWindowTitle(QApplication::translate("add_id_from", "Dialog", nullptr));
        btnAdd->setText(QApplication::translate("add_id_from", "Add", nullptr));
        btnCancel->setText(QApplication::translate("add_id_from", "Cancel", nullptr));
        lineEdit->setText(QApplication::translate("add_id_from", "Name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class add_id_from: public Ui_add_id_from {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_ID_FORM_H
