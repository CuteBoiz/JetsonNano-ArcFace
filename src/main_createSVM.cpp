#include "train_svm.h"
#include <iostream>
using namespace std;

int main(){
	string Dataset_dir = "../Dataset/";
    string save_dir = "Data/";
    create_txt(Dataset_dir, save_dir);
    create_train_svm(save_dir);
    create_svm(save_dir);
    return 1;
}