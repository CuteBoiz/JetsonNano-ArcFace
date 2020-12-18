#ifndef PREDICT_H
#define PREDICT_H

#include <torch/script.h>
#include <torch/torch.h>

#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;


string predict(const Mat faces);

#endif //PREDICT_H