#include <iostream>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <string>
#include "svm.h"
using namespace std;

static char *line = NULL;
static int max_line_len;

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

static struct svm_parameter param;     // set by parse_command_line
static struct svm_problem prob;        // set by read_problem
static struct svm_model *SVM_MODEL;
static struct svm_node *x_space;
static int cross_validation;
static int nr_fold;

void init(){
    void (*print_func)(const char*) = NULL; // default printing to stdout

    // default values
    param.svm_type = C_SVC;
    param.kernel_type = LINEAR;
    param.degree = 3;
    param.gamma = 0;    // 1/num_features
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 1;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    cross_validation = 0;
}

void exit_input_error(int line_num){
	fprintf(stderr,"Wrong input format at line %d\n", line_num);
    exit(0);
}

static char* readline(FILE *input){
	int len;
    if(fgets(line,max_line_len,input) == NULL)
        return NULL;
    while(strrchr(line,'\n') == NULL){
        max_line_len *= 2;
        line = (char *) realloc(line,max_line_len);
        len = (int) strlen(line);
        if(fgets(line+len,max_line_len-len,input) == NULL) break;
    }
    return line;
} 

void read_problem(const char *filename){
	int max_index, inst_max_index, i;
    size_t elements, j;
    FILE *fp = fopen(filename,"r");
    char *endptr;
    char *idx, *val, *label;

    if(fp == NULL)
    {
        fprintf(stderr,"Can't open input file %s\n",filename);
        exit(0);
    }

    prob.l = 0;
    elements = 0;

    max_line_len = 1024;
    line = Malloc(char,max_line_len);
    while(readline(fp)!=NULL)
    {
        char *p = strtok(line," \t"); // label

        // features
        while(1)
        {
            p = strtok(NULL," \t");
            if(p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
                break;
            ++elements;
        }
        ++elements;
        ++prob.l;
    }
    rewind(fp);

    prob.y = Malloc(double,prob.l);
    prob.x = Malloc(struct svm_node *,prob.l);
    x_space = Malloc(struct svm_node,elements);

    max_index = 0;
    j=0;
    for(i=0;i<prob.l;i++)
    {
        inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0
        readline(fp);
        prob.x[i] = &x_space[j];
        label = strtok(line," \t\n");
        if(label == NULL){ // empty line
            exit_input_error(i+1);
        }

        prob.y[i] = strtod(label,&endptr);
        if(endptr == label || *endptr != '\0'){
            exit_input_error(i+1);
        }

        while(1)
        {
            idx = strtok(NULL,":");
            val = strtok(NULL," \t");

            if(val == NULL) break;

            errno = 0;
            x_space[j].index = (int) strtol(idx,&endptr,10);
            if(endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index){
                exit_input_error(i+1);
            }
            else{
                inst_max_index = x_space[j].index;
            }

            errno = 0;
            x_space[j].value = strtod(val,&endptr);
            if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr))){
                exit_input_error(i+1);
            }
            ++j;
        }

        if(inst_max_index > max_index)
            max_index = inst_max_index;
        x_space[j++].index = -1;
    }

    if(param.gamma == 0 && max_index > 0)
        param.gamma = 1.0/max_index;

    if(param.kernel_type == PRECOMPUTED)
        for(i=0;i<prob.l;i++)
        {
            if (prob.x[i][0].index != 0)
            {
                fprintf(stderr,"Wrong input format: first column must be 0:sample_serial_number\n");
                exit(0);
            }
            if ((int)prob.x[i][0].value <= 0 || (int)prob.x[i][0].value > max_index)
            {
                fprintf(stderr,"Wrong input format: sample_serial_number out of range\n");
                exit(0);
            }
        }

    fclose(fp);
}


void create_svm(string files_path){
    cout << "Generating SVM model at " << files_path << " ...\n";
	init();
    const char *error_msg;
    read_problem("Data/train");
    error_msg = svm_check_parameter(&prob,&param);
    if(error_msg){
        fprintf(stderr,"ERROR: %s\n",error_msg);
        exit(0);
    }
    SVM_MODEL = svm_train(&prob,&param);
    if(svm_save_model((files_path + "svm.model").c_str(), SVM_MODEL)){
        fprintf(stderr, "Can't save model to file %s\n", "test.model");
        exit(0);

    }
    else{
    	cout << "SVM model created!" << endl;
    }
    svm_free_and_destroy_model(&SVM_MODEL);
    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    free(x_space);
    free(line);
    cout << " Done!" << endl;
}

int main(){
    create_svm("Data/");
    return 1;
}