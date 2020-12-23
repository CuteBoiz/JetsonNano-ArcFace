#include <torch/script.h>
#include <torch/torch.h>
#include "mtcnn.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iomanip>
#include "svm.h"

using namespace cv;
using namespace std;

struct result{
	double class_name;
	double confidence;
};

struct svm_node *x;
struct svm_model *model;

torch::jit::script::Module module;

result predict(Mat faces);
int svm_type;
int nr_class;


int main()
{
    if((model=svm_load_model("Data/test.model"))==0){
        fprintf(stderr,"Can't open model file %s\n","test.model");
        exit(1);
    }
    svm_type=svm_get_svm_type(model);
    nr_class=svm_get_nr_class(model);
    x = (struct svm_node *) malloc(513*sizeof(struct svm_node));


    string model_path = "Data/model.pt";
    module = torch::jit::load(model_path);
    cout << "Switch to GPU mode" << endl;
    module.to(at::kCUDA);
    fstream newfile;
    vector<string> label;
    newfile.open("Data/names.txt",ios::in);
    if (newfile.is_open()){  
        string tp;
        while(getline(newfile, tp)) label.push_back(tp);
        newfile.close();
    }

    Mat frame;
    VideoCapture cap(0);
    clock_t start;
    cap>>frame;
    if(!frame.data) {
        cout << "unable to open camera" << endl;
        return -1;
    }
    mtcnn detector(frame.rows, frame.cols);

    while(1){
        start = clock();
        cap >> frame;
        if (frame.empty()) break;
        
        vector<struct Bbox> Boxes = detector.findFace(frame);
        for(vector<struct Bbox>::iterator it=Boxes.begin(); it!=Boxes.end();it++){
            if((*it).exist){
                Mat face = frame(Rect((*it).y1, (*it).x1, abs((*it).y1 - (*it).y2), abs((*it).x1 - (*it).x2)));
                rectangle(frame, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(255,0,0), 2,8,0);
                for(int num=0;num<5;num++)circle(frame,Point((int)*(it->ppoint+num), (int)*(it->ppoint+num+5)),3,Scalar(0,255,255), -1);
                
                result A = predict(face);
                ostringstream confidence_obj;
                confidence_obj << std::fixed;
                confidence_obj << std::setprecision(2);
                confidence_obj << A.confidence * 100;
                string conf = confidence_obj.str();
                string result_text = label[(int)(A.class_name)] + " " + conf;
                putText(frame, result_text.c_str(), Point((*it).y1, (*it).x1), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, LINE_AA);
            }
        }
        Boxes.clear();
        imshow("result", frame);
        start = clock() - start;
        cout << "FPS: " << (int)(1/(((clock() - start)/(double)CLOCKS_PER_SEC)))<<endl;
        if( waitKey(1)>=0 ) break;
    }
    cap.release();
    destroyAllWindows();
    return 1;
}

result predict(Mat faces){
    if (faces.empty()){
        return {};
    }
    result A;
    double *prob_estimates=NULL;
    double predict_label;
    vector<torch::jit::IValue> inputs;
    cv::resize(faces, faces, Size(112,112));
    faces.convertTo(faces, CV_32FC3, 1.0f / 255.0f);

    auto input_tensor = torch::from_blob(faces.data, {1, 112, 112, 3});
    input_tensor = input_tensor.permute({0, 3, 1, 2});
    input_tensor[0][0] = input_tensor[0][0].sub_(0.5).div_(0.5);
    input_tensor[0][1] = input_tensor[0][1].sub_(0.5).div_(0.5);
    input_tensor[0][2] = input_tensor[0][2].sub_(0.5).div_(0.5);
    input_tensor = input_tensor.to(at::kCUDA);
    torch::Tensor out_tensor = module.forward({input_tensor}).toTensor();


    for(int i = 0; i < 512; i++){
        x[i].index = i;
        x[i].value = out_tensor[0][i].item<double>();
    }
    x[512].index = -1;
    prob_estimates = (double *) malloc(nr_class*sizeof(double));
	predict_label = svm_predict_probability(model,x,prob_estimates);

	A.class_name = predict_label;
	A.confidence = *(prob_estimates);
    return A;
}