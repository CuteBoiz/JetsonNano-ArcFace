#include "window.h"
#include "./ui_window.h"
#include <unistd.h>


Window::Window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    if(!(model=svm_load_model("Data/svm.model"))){
        fprintf(stderr,"Can't open model file %s\n","svm.model");
        ui->lblStatus->setText("Add an ID to start predicting!");
        PREDICT_RDY = false;
    }
    else{
        svm_type = svm_get_svm_type(model);
        nr_class = svm_get_nr_class(model);
		
		scale = 0.8*(100/double(nr_class));
		step = 0;

        string model_path = "Data/model.pt";
        module = torch::jit::load(model_path);
        cout << "Switch to GPU mode" << endl;
        module.to(at::kCUDA);

        fstream newfile;
        newfile.open("Data/names.txt",ios::in);

        if (newfile.is_open()){
            label.clear();
            string tp;
            while(getline(newfile, tp)) label.push_back(tp);
            newfile.close();
        }
        PREDICT_RDY = true;
    }

    connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    cap.open(1);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 30);
    
    internalTime = new QTimer(this);
    connect(internalTime, SIGNAL(timeout()), this, SLOT(updateFrame()));
    internalTime->start(10);
}

void Window::closeEvent(QCloseEvent *event){
    if (QMessageBox::question(this, "Confirm", "Do you want to close ?") == QMessageBox::No) {
        event->ignore();
    }
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
    
    start = std::clock();
    cap.read(frame);
    if (frame.empty()){
        QMessageBox::information(this, "Frame Error!", "Can't read the webcam/video.");
        return;
    }
    flip(frame, frame, 1);

    if (ADD_ID){

        if (nrof_imgs < MAX_IMGS) {
            mkdir(SAVE_ID_DIR.c_str(), 0777);
            string dirName = SAVE_ID_DIR + IDName + "/";
            mkdir(dirName.c_str(), 0777);
            vector<struct Bbox> Boxes = detector.findFace(frame);
            for(vector<struct Bbox>::iterator it=Boxes.begin(); it!=Boxes.end();it++){
                if(((*it).exist) && ((*it).y1 > 25) && ((*it).x1 > 25) && ((abs((*it).y1 - (*it).y2) + 25) < frame.cols) && ((abs((*it).x1 - (*it).x2)+ 25) < frame.rows)){
                    cv::Mat face = frame(Rect((*it).y1 - 25, (*it).x1 - 25, abs((*it).y1 - (*it).y2) + 25, abs((*it).x1 - (*it).x2) + 25));
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
                    cv::rectangle(frame, Point((*it).y1 - 25, (*it).x1 - 25), Point((*it).y2 + 25, (*it).x2 + 25), Scalar(255,0,0), 2,8,0);
                    cv::putText(frame, "TURN HEAD AROUND!", Point(10, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
                }
            }
            Boxes.clear();
        }
        else if (nrof_imgs == MAX_IMGS) {
            ui->lblStatus->setText("Training SVM model. Please wait ...");
            string Dataset_dir = "../Dataset/";
            string save_dir = "Data/";
            create_txt(Dataset_dir, save_dir);
            create_train_svm(save_dir);
            QMessageBox::information(this, "Done!", ("Added " + IDName + " Successfully!").c_str());
            ADD_ID = false;
            nrof_imgs = 1;
            IDName = "";

            ui->lblStatus->setText("Predicting ...");

            if(!(model=svm_load_model("Data/svm.model"))){
                fprintf(stderr,"Can't open model file %s\n","svm.model");
                ui->lblStatus->setText("Add an ID to start predicting!");
                PREDICT_RDY = false;
            }
            else{
                svm_type = svm_get_svm_type(model);
                nr_class = svm_get_nr_class(model);
                string model_path = "Data/model.pt";

				scale = 0.8*(100/double(nr_class));
				step = 0;

                module = torch::jit::load(model_path);
                cout << "Switch to GPU mode" << endl;
                module.to(at::kCUDA);

                fstream newfile;
                newfile.open("Data/names.txt",ios::in);
                if (newfile.is_open()){  
                    label.clear();
                    string tp;
                    while(getline(newfile, tp)) label.push_back(tp);
                    newfile.close();
                }            
                PREDICT_RDY = true;
            }
        }
        int h = frame.rows;
        int w = frame.cols;
        int min_side = h < w ? h : w;
        int radius = min_side / 2 - 20;

        // cv::circle(frame, Point((int)w/2, (int)h/2), min_side, Scalar(0, 0, 0), min_side+70);
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
    }
    else{
        vector<struct Bbox> Boxes = detector.findFace(frame);
        for(vector<struct Bbox>::iterator it=Boxes.begin(); it!=Boxes.end();it++){
            if((*it).exist){

                cv::Mat face;

                if (((*it).y1 > 25) && ((*it).x1 > 25) && ((abs((*it).y1 - (*it).y2) + 25) < frame.cols) && ((abs((*it).x1 - (*it).x2)+ 25) < frame.rows))
                {
                    face = frame(Rect((*it).y1 - 25, (*it).x1 - 25, abs((*it).y1 - (*it).y2) + 25, abs((*it).x1 - (*it).x2) + 25));
                    cv::rectangle(frame, Point((*it).y1 - 25, (*it).x1 - 25), Point((*it).y2 + 25, (*it).x2 + 25), Scalar(255,0,0), 2,8,0);
                }
                else
                {
                    face = frame(Rect((*it).y1, (*it).x1, abs((*it).y1 - (*it).y2), abs((*it).x1 - (*it).x2)));
                    cv::rectangle(frame, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(255,0,0), 2,8,0);
                }

                //for(int num=0;num<5;num++)circle(frame,Point((int)*(it->ppoint+num), (int)*(it->ppoint+num+5)),3,Scalar(0,255,255), -1);
                
                if (PREDICT_RDY){
                    result A = predict(face);
                    ostringstream confidence_obj;
                    confidence_obj << std::fixed;
                    confidence_obj << std::setprecision(2);
                    confidence_obj << A.confidence * 100;
                    string conf = confidence_obj.str();
					string result_text;
					if (int(A.class_name) != -1){
                    	result_text = label[(int)(A.class_name)] + " " + conf;
						if(step == 0 || (step%5) != 0)
							step++;

						else if (step != 0 && (step%5) == 0)
						{
							step = 0;
							//puttext
							open_str = "Open for " + label[(int)(A.class_name)] + ", Please wait for 5s!";
							cout << open_str << endl;
							open = true;
							cv::putText(frame, open_str, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1, LINE_AA);
							temp = 0;
						}
					}
					else{
						result_text = "Unknown";
						step = 0;
					}
                    cv::putText(frame, result_text.c_str(), Point((*it).y1, (*it).x1), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, LINE_AA);
                }
            }
        }
        Boxes.clear();
    }
	
	fps = (1 / ((std::clock() - start ) / (double)CLOCKS_PER_SEC));
	cv::putText(frame, to_string(fps)+" fps", Point(15, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1, LINE_AA);
	cv::cvtColor(frame, frame, COLOR_BGR2RGB);
	
    QImage qFrame((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    ui->lblImg->setPixmap(QPixmap::fromImage(qFrame));
	temp++;
	if (open && temp == 2){
		sleep(5);
		open = false;
	}
}


result predict(Mat faces){
    if (faces.empty()){
        return {};
    }
    result A;
    double *prob_estimates=NULL;
    double predict_label;
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
    double arr[nr_class];

    for (int i = 0; i < nr_class; i++)
        arr[i] = prob_estimates[i];

    int n = sizeof(arr)/sizeof(arr[0]);

    std::sort(arr, arr + n);

    if (nr_class > 25)
    {
        if (arr[nr_class - 1]/arr[nr_class - 2] < 3)
        {
            A.class_name = -1;
            A.confidence = 0;
        }

        else
        {
            A.class_name = predict_label;
            A.confidence = prob_estimates[(int) predict_label];
        }
    }

    else if (nr_class < 25)
    {

        if (arr[nr_class - 1]/arr[nr_class - 2] < scale)
        {
            A.class_name = -1;
            A.confidence = 0;
        }

        else
        {
            A.class_name = predict_label;
            A.confidence = prob_estimates[(int) predict_label];
        }
    }

    return A;
}
