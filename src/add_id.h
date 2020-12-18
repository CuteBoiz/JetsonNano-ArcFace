#ifndef ADD_ID_H
#define ADD_ID_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cmath>
#include <iostream>
using namespace std;
using namespace cv;

#define NORF_LINES 256
#define MAX_IMGS 100
const double PI  = 3.141592653589793238463;


void add_id(Mat &image, int nrof_images);


#endif //ADD_ID_H