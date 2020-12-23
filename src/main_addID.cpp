#include "mtcnn.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PI  3.141592653589793238463
#define NORF_LINES 256
#define MAX_IMGS 100
string SAVE_DIR = "../Dataset/";
int nrof_imgs = 1;
string IDName = "Phat";

int main(){
	Mat frame;
    VideoCapture cap(0);
    if(!cap.isOpened()){
        cout<<"Fail to open video!"<<endl;
        return -1;
    }
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

        int h = frame.rows;
		int w = frame.cols;
		int min_side = h < w ? h : w;
		int radius = min_side / 2 - 20;
		circle(frame, Point((int)w/2, (int)h/2), min_side, Scalar(0, 0, 0), min_side+70);

		for (int i = 0; i < NORF_LINES; i++) {
			float angle = 2 * PI * i / NORF_LINES;
			float x = (int)w/2 + sin(angle) * radius;
			float y = (int)h/2 + cos(angle) * radius;
			line(frame, Point(x, y), Point(x+sin(angle)*10, y+cos(angle)*10), Scalar(255, 255, 255), 1);
		}

		double percent = (double)nrof_imgs/(double)MAX_IMGS;
		int nrof_big_lines = NORF_LINES * percent;

		for (int j = 0; j < nrof_big_lines; j++){
			float angle = 2 * PI * j / NORF_LINES;
			float x = (int)w/2 + sin(angle) * radius;
			float y = (int)h/2 + cos(angle) * radius;
			line(frame, Point(x, y), Point(x+sin(angle)*15, y+cos(angle)*15), Scalar(255, 255, 255), 2);
		}

    	if (nrof_imgs < MAX_IMGS){
    		mkdir(SAVE_DIR.c_str(), 0777);
		    string dirName = SAVE_DIR + IDName + "/";
		    mkdir(dirName.c_str(), 0777);
            vector<struct Bbox> Boxes = detector.findFace(frame);
            for(vector<struct Bbox>::iterator it=Boxes.begin(); it!=Boxes.end();it++){
                if((*it).exist){
                    Mat face = frame(Rect((*it).y1, (*it).x1, abs((*it).y1 - (*it).y2), abs((*it).x1 - (*it).x2)));
                    rectangle(frame, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(0,0,255), 2,8,0);
                    putText(frame, "TURN HEAD AROUND!", Point(10, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
                    if ((*it).score > 0.99) {
                        string save_file = SAVE_DIR + IDName + "/" + to_string(nrof_imgs) + ".jpg";
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
            putText(frame, "DONE! WAIT A SEC.", Point(frame.cols/2-50, frame.rows/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
            
            cout << "Added " << IDName << " Successfully!";
            return -1;
        }
        imshow("result", frame);
        start = clock() - start;
        cout << "FPS: " << (int)(1/((double)start/CLOCKS_PER_SEC))<<endl;
        if( waitKey(1)>=0 ) break;
    }
    cap.release();
    destroyAllWindows();
	return 0;
}