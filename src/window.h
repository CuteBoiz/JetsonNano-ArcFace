#ifndef WINDOW_H
#define WINDOW_H

#include <torch/script.h>
#include <torch/torch.h>

#include "svm.h"
#include "train_svm.h"
#include "mtcnn.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <sys/stat.h>
#include <iomanip>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QCloseEvent>

using namespace std;

#define PI  3.141592653589793238463
#define NORF_LINES 256
#define MAX_IMGS 100

struct result{
    double class_name;
    double confidence;
};

static struct svm_node *x = (struct svm_node *) malloc(513*sizeof(struct svm_node));;
static struct svm_model *model;
static torch::jit::script::Module module;
static int svm_type;
static int nr_class;
static double scale;
static int step;
static bool open = false;
static string open_str;
static string SAVE_ID_DIR = "../Dataset/";
static mtcnn detector(480, 640);
static int temp = 0;

result predict(Mat faces);

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();
public slots:
    void updateFrame();
private slots:
    void add();
    
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::Window *ui;

    QTimer *internalTime;
    cv::VideoCapture cap;
    cv::Mat frame;
    QImage qFrame;

    bool PREDICT_RDY = false;
    bool ADD_ID = false;
    string IDName;
    int nrof_imgs = 1;

    std::clock_t start;
    int fps;

    vector<string> label;
};
#endif // WINDOW_H
