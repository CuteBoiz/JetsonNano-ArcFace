#include "add_id_form.h"
#include "ui_add_id_form.h"

// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

// using namespace cv;

Mat avatar;



add_id_form::add_id_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_id_form)
{

    ui->setupUi(this);
    avatar = imread("avatar.png");
    cv::resize(avatar, avatar, Size(136, 96));
    QImage qAvatar((uchar*)avatar.data, avatar.cols, avatar.rows, QImage::Format_BGR888);
    ui->lblAvatar->setPixmap(QPixmap::fromImage(qAvatar));
    connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(on_btnAdd_clicked()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnChange, SIGNAL(clicked()), this, SLOT(changeAvatar()));

}

add_id_form::~add_id_form()
{
    delete ui;
}

void add_id_form::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    avatar = imread("tempAvatar.jpg");
    if (avatar.empty()){
        avatar = imread("avatar.png");
    }
    cv::resize(avatar, avatar, Size(136, 96));
    QImage qAvatar((uchar*)avatar.data, avatar.cols, avatar.rows, QImage::Format_BGR888);
    ui->lblAvatar->setPixmap(QPixmap::fromImage(qAvatar));
}


void add_id_form::closeEvent(QCloseEvent *event){
    if (QMessageBox::question(this, "Confirm", "Do you want to cancel adding an ID") == QMessageBox::No) {
        event->ignore();
    }
    remove( "tempAvatar.jpg" );
}

void add_id_form::on_btnAdd_clicked()
{
    IDName = ui->lineName->text().toStdString();
    if (IDName.length() < 2 || IDName.length() > 20){
        QMessageBox::information(this, "Name Error!", "The ID Name Must Be Between 2 - 20 characters");
        return;
    }
    for(int i = 0; i < (int)IDName.length(); i++){
        if ((IDName[i]>=65 && IDName[i]<=90)||
            (IDName[i]>=97 && IDName[i]<=122))
            {
              continue;
            }
        else
        {
            QMessageBox::information(this, "Name Error!", "ID Name cannot contain special characters!");
            return;
        }
    }
    avatar = imread("tempAvatar.jpg");
    if (avatar.empty()){
       QMessageBox::information(this, "Missing Avatar", "Press Capture button to take a avatar!");
       return;
    }
    this->hide();
    mkdir("../Dataset/", 0777);
    string dirName = "../Dataset/" + IDName + "/";
    mkdir(dirName.c_str(), 0777);
    imwrite(dirName + "avatar.jpg", avatar);
    string msg = "Added "+ IDName +" Successfully!";
    QMessageBox::information(this, "Success!", msg.c_str());
}

void add_id_form::changeAvatar()
{
    this->hide();
}
