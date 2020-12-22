#include "window.h"
#include "./ui_window.h"


Window::Window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    if((model=svm_load_model("Data/test.model"))==0){
        fprintf(stderr,"Can't open model file %s\n","test.model");
        ui->lblStatus->setText("Can not detect any SVM!");
    }
    else{
        svm_type = svm_get_svm_type(model);
        nr_class = svm_get_nr_class(model);
        
        string model_path = "Data/model.pt";
        module = torch::jit::load(model_path);
        cout << "Switch to GPU mode" << endl;
        module.to(at::kCUDA);
        vector<torch::jit::IValue> inputs;
        PREDICT_RDY = true;
    }

    newfile.open("Data/label.txt",ios::in);
    if (newfile.is_open()){  
        string tp;
        while(getline(newfile, tp)) label.push_back(tp);
        newfile.close();
    }

    connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    cap.open(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 30);
    
    internalTime = new QTimer(this);
    connect(internalTime, SIGNAL(timeout()), this, SLOT(updateFrame()));
    internalTime->start(10);
}

Window::~Window()
{
    delete ui;
}

void Window::add(){
    IDName = ui->textName->text().toStdString();
    if (IDName.length() < 2 || IDName.length() > 20){
       QMessageBox::information(this, "Name Error!", "The ID Name Must Be Between 2 - 20 characters");
       return;
    }
    for(int i = 0; i < (int)IDName.length(); i++){
        if (IDName[i] < 65 || (IDName[i]>90 && IDName[i]<97) || IDName[i]>122){
            QMessageBox::information(this, "Name Error!", "ID Name cannot contain special characters!");
            return;
        }
    }
    ui->lblStatus->setText(("Adding " + IDName + " ...").c_str());
    ADD_ID = true;
}

void Window::updateFrame(){
    start = clock();
    cap.read(frame);
    if (frame.empty()){
        QMessageBox::information(this, "Frame Error!", "Can't read the webcam/video.");
        return;
    }
    flip(frame, frame, 1);

    if (ADD_ID){
        int h = frame.rows;
        int w = frame.cols;
        int min_side = h < w ? h : w;
        int radius = min_side / 2 - 20;

        cv::circle(frame, Point((int)w/2, (int)h/2), min_side, Scalar(0, 0, 0), min_side+70);
        for (int i = 0; i < NORF_LINES; i++) {
            float angle = 2 * PI * i / NORF_LINES;
            float x = (int)w/2 + sin(angle) * radius;
            float y = (int)h/2 + cos(angle) * radius;
            cv::line(frame, Point(x, y), Point(x+sin(angle)*10, y+cos(angle)*10), Scalar(255, 255, 255), 1);
        }

        double percent = (double)nrof_imgs/(double)MAX_IMGS;
        int nrof_big_lines = NORF_LINES * percent;

        for (int j = 0; j < nrof_big_lines; j++){
            float angle = 2 * PI * j / NORF_LINES;
            float x = (int)w/2 + sin(angle) * radius;
            float y = (int)h/2 + cos(angle) * radius;
            cv::line(frame, Point(x, y), Point(x+sin(angle)*15, y+cos(angle)*15), Scalar(255, 255, 255), 2);
        }

        if (nrof_imgs < MAX_IMGS){
            mkdir(SAVE_ID_DIR.c_str(), 0777);
            string dirName = SAVE_ID_DIR + IDName + "/";
            mkdir(dirName.c_str(), 0777);
            vector<struct Bbox> Boxes = detector.findFace(frame);
            for(vector<struct Bbox>::iterator it=Boxes.begin(); it!=Boxes.end();it++){
                if((*it).exist){
                    cv::Mat face = frame(Rect((*it).y1, (*it).x1, abs((*it).y1 - (*it).y2), abs((*it).x1 - (*it).x2)));
                    cv::rectangle(frame, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(0,0,255), 2,8,0);
                    cv::putText(frame, "TURN HEAD AROUND!", Point(10, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
                    if ((*it).score > 0.99) {
                        string save_file = SAVE_ID_DIR + IDName + "/" + to_string(nrof_imgs) + ".jpg";
                        bool isSuccess = imwrite(save_file, face);
                        if (isSuccess){
                            cout << "Image saved at: " << save_file << endl;
                            nrof_imgs++;
                            break;
                        }
                        else{
                            cerr << "ERROR: Failed to save the image \n";
                        }
                    }
                }
            }
            Boxes.clear();
        }
        else if (nrof_imgs == MAX_IMGS){
            cv::putText(frame, "DONE! WAIT A SEC.", Point(frame.cols/2-50, frame.rows/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
            string Dataset_dir = "../Dataset/";
            string save_dir = "Data/";
            create_txt(Dataset_dir, save_dir);
            create_train_svm(save_dir);
            create_svm(save_dir);
            QMessageBox::information(this, "Done!", ("Added " + IDName + " Successfully!").c_str());
            ADD_ID = false;
            nrof_imgs = 1;
            IDName = "";

            ui->lblStatus->setText("Predicting ...");

            if((model=svm_load_model("Data/test.model"))==0){
                fprintf(stderr,"Can't open model file %s\n","test.model");
                ui->lblStatus->setText("Can not detect any SVM!");
            }
            else{
                svm_type = svm_get_svm_type(model);
                nr_class = svm_get_nr_class(model);
                
                string model_path = "Data/model.pt";
                module = torch::jit::load(model_path);
                cout << "Switch to GPU mode" << endl;
                module.to(at::kCUDA);
                vector<torch::jit::IValue> inputs;
                PREDICT_RDY = true;
            }

            newfile.open("Data/label.txt",ios::in);
            if (newfile.is_open()){  
                string tp;
                while(getline(newfile, tp)) label.push_back(tp);
                newfile.close();
            }

            
       }
    }
    else{
	cout << "HERE1!\n";
        vector<struct Bbox> Boxes = detector.findFace(frame);
	cout << "HERE2!\n";
        for(vector<struct Bbox>::iterator it=Boxes.begin(); it!=Boxes.end();it++){
            if((*it).exist){
                cv::Mat face = frame(Rect((*it).y1, (*it).x1, abs((*it).y1 - (*it).y2), abs((*it).x1 - (*it).x2)));
                cv::rectangle(frame, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(255,0,0), 2,8,0);
                for(int num=0;num<5;num++)circle(frame,Point((int)*(it->ppoint+num), (int)*(it->ppoint+num+5)),3,Scalar(0,255,255), -1);
                
                if (PREDICT_RDY){
                    result A = predict(face);
                    ostringstream confidence_obj;
                    confidence_obj << std::fixed;
                    confidence_obj << std::setprecision(2);
                    confidence_obj << A.confidence * 100;
                    string conf = confidence_obj.str();
                    string result_text = label[(int)(A.class_name)] + " " + conf;
                    cv::putText(frame, result_text.c_str(), Point((*it).y1, (*it).x1), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, LINE_AA);
                }
            }
        }
        Boxes.clear();
    }

    fps = (1/((double)start/CLOCKS_PER_SEC));
    cv::putText(frame, to_string(fps)+" fps", Point(15, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 1, LINE_AA);
    cv::cvtColor(frame, frame, COLOR_BGR2RGB);
    QImage qFrame((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    ui->lblImg->setPixmap(QPixmap::fromImage(qFrame));
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
