#ifndef TRAIN_SVM_H
#define TRAIN_SVM_H

#include <torch/script.h>
#include <torch/torch.h>

#include <iostream>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <string>
#include <fstream>
#include <experimental/filesystem>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "svm.h"

static int nr_folder = 0;
static int len_of_folders_in_directory;
using namespace std;

int find_length(string* paths);
void read_files_inside_one_directory(string*& paths, string read_path);
string** read_directory(string read_path);
int findNthOccur(string str, char ch, int N);
void write_file_txt(string** directory,  string destination_folder);
void write_file_name_only(string* directory, string input,  string destination_folder);
void create_txt(string path, string destination_folder);
int create_train_svm(string files_path);


#endif //TRAIN_SVM_H