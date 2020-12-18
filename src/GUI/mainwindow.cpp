#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    detector(112, 112);
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnAdd_ID, SIGNAL(clicked()),this, SLOT(add_id()));
    connect(ui->btnDatabase, SIGNAL(clicked()), this, SLOT(showDatabase()));
    connect(ui->btnCapture, SIGNAL(clicked()), this, SLOT(capture()));

    cap.open(0);
    if(!cap.isOpened()){
        QMessageBox::information(this, "Failed To Open Webcam", "Please Check Your Pluged-Webcam!");
        return;
    }
    fps = new QTimer(this);
    addIDForm = new add_id_form(this);
    addIDForm->setModal(true);
    connect(fps, SIGNAL(timeout()), this, SLOT(updateFrame()));
    fps->start(10);

}  
        
void MainWindow::updateFrame(){
    clock_t start;
    start = clock();
    string SAVE_DIR = "Dataset/";

    if (ADD_ID){
        add_id(frame, NROF_IMGS);
        if (NROF_IMGS < MAX_IMGS){
            vector<struct Bbox> Boxes = detector.findFace(frame);
        
            for(vector<struct Bbox>::iterator it=Boxes.begin(); it!=Boxes.end();it++){
                if((*it).exist){
                    Mat face = frame(Rect((*it).y1, (*it).x1, abs((*it).y1 - (*it).y2), abs((*it).x1 - (*it).x2)));
                    rectangle(frame, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(0,0,255), 2,8,0);
                    putText(frame, "TURN HEAD AROUND!", Point(10, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
                    if ((*it).score > 0.99) {
                        string save_file = SAVE_DIR + IDName + to_string(NROF_IMGS) + ".jpg";
                        bool isSuccess = imwrite(save_file, face);
                        if (isSuccess){
                            cout << "Image saved at: " << save_file << endl;
                            NROF_IMGS++;
                            break;
                        }
                        else{
                            cerr << "ERROR: Failed to save the image \n";
                            NROF_IMGS--;
                        }
                    }
                }
            }
            Boxes.clear();
        }
        else if (NROF_IMGS == MAX_IMGS){
            putText(frame, "DONE! WAIT A SEC.", Point(frame.cols/2-50, frame.rows/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
            //LOAD & TRAIN NEW SVMs
            //Remove File
            
        }
        
    }
    else{
        vector<struct Bbox> Boxes = detector.findFace(frame);
        for(vector<struct Bbox>::iterator it=Boxes.begin(); it!=Boxes.end();it++){
            if((*it).exist){
                Mat face = frame(Rect((*it).y1, (*it).x1, abs((*it).y1 - (*it).y2), abs((*it).x1 - (*it).x2)));
                rectangle(frame, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(255,0,0), 2,8,0);
                for(int num=0;num<5;num++)circle(frame,Point((int)*(it->ppoint+num), (int)*(it->ppoint+num+5)),3,Scalar(0,255,255), -1);
                //string class_name = predict(face);
                //putText(frame, class_name.c_str(), Point((*it).y1, (*it).x1), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, LINE_AA);
            }
        }
        Boxes.clear();
    }
    start = clock() - start;
    cout << "FPS: " << (int)(1/((double)start/CLOCKS_PER_SEC))<<endl;
    imshow("result", frame);
    QImage qFrame((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    ui->labelImg->setPixmap(QPixmap::fromImage(qFrame));
}

void MainWindow::add_id(){
    ui->lbl->setText("Adding ID");
    addIDForm->show();
    ui->lbl->setText(" ");
}

void MainWindow::showDatabase(){

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if (QMessageBox::question(this, "Confirm", "Do you want to close exit") == QMessageBox::No) {
        event->ignore();
    }
}

void MainWindow::capture()
{
    cap.read(frame);
    if (frame.empty()) return;
    Vec3f circ((int)frame.cols/2, (int)frame.rows/2, 200);
    Mat1b mask(frame.size(), uchar(0));
    circle(mask, Point(circ[0], circ[1]), circ[2], Scalar(255), CV_FILLED);
    Rect bbox(circ[0] - circ[2], circ[1] - circ[2], 2 * circ[2], 2 * circ[2]);
    Mat3b res(frame.size(), Vec3b(0,0,0));
    frame.copyTo(res, mask);
    res = res(bbox);
    imwrite("tempAvatar.jpg", res);
    addIDForm->show();
}
