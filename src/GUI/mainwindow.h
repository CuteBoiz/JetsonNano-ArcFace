#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "predict.h"
#include "mtcnn.h"
#include "add_id.h"

#include <ctime>
#include <memory>
#include <stdio.h>

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <QCloseEvent>

#include "add_id_form.h"

using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void updateFrame();

private slots:
    void add_id();
    void showDatabase();
    void capture();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    mtcnn detector;
    
    QTimer* fps;
    VideoCapture cap;
    Mat frame;
    QImage qFrame;

    add_id_form *addIDForm;
    

    bool ADD_ID = false;
    string NameID = "Phat";

};
#endif // MAINWINDOW_H
