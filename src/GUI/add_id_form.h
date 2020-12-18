#ifndef ADD_ID_FORM_H
#define ADD_ID_FORM_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include <fstream>
#include <sys/stat.h>
#include <iostream>
using namespace std;

namespace Ui {
class add_id_form;
}

class add_id_form : public QDialog
{
    Q_OBJECT

public:
    explicit add_id_form(QWidget *parent = nullptr);
    ~add_id_form();
protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
private slots:
    void on_btnAdd_clicked();
    void changeAvatar();

private:
    Ui::add_id_form *ui;

    
    QImage qAvatar;
    string IDName;

};

#endif // ADD_ID_FROM_H
