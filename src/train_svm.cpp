#include "train_svm.h"

void read_files_inside_one_directory(string*& paths, string read_path){
	nr_folder = 0;
	for(const auto & entry : experimental::filesystem::directory_iterator(read_path)){
		paths[nr_folder] = entry.path();
		nr_folder += 1;
	}	
}

int find_length(string* paths){
	int temp = 0;
	while(!paths[temp].empty()){
		++temp;
	}
	return temp;
}

string** read_directory(string read_path){
	string* directory = new string[100];
	read_files_inside_one_directory(directory, read_path);

	len_of_folders_in_directory = find_length(directory);

	string** folders = new string*[len_of_folders_in_directory];
	cout << "len_of_folders_in_directory: " << len_of_folders_in_directory << endl;
	for (int temp = 0; temp < len_of_folders_in_directory; temp++){
		folders[temp] = new string[1000];
	}

	for(int temp = 0; temp < len_of_folders_in_directory; temp++){
		cout << "Reading this folder: " << directory[temp] << endl;
		read_files_inside_one_directory(folders[temp], directory[temp]);
	}

	return folders;
}

void write_file_txt(string** directory, string destination_folder){
	ofstream myfile, labels;

	myfile.open (destination_folder + "paths.txt");
	labels.open(destination_folder + "labels.txt");

	for(int temp = 0; temp < len_of_folders_in_directory; temp++){
		int len_folder = find_length(directory[temp]);
		for(int temp1 = 0; temp1 < len_folder; temp1++){
			myfile << directory[temp][temp1] << endl;
			labels << temp << endl;
		}
	}

	myfile.close();
	labels.close();
}

int findNthOccur(string str, char ch, int N) {
	int occur = 0; 
    // Loop to find the Nth 
    // occurence of the character 
    for (int i = 0; i < str.length(); i++) { 
        if (str[i] == ch) { 
            occur += 1; 
        } 
        if (occur == N) 
            return i; 
    } 
    return -1; 
}

void write_file_name_only(string* directory, string input, string destination_folder){
	ofstream myfile;
	myfile.open (destination_folder + "names.txt");

	string* name_only = new string[len_of_folders_in_directory];
	for(int temp = 0; temp < len_of_folders_in_directory; temp++){
		int pos1 = findNthOccur(directory[temp], '/', 2);
		name_only[temp] = (directory[temp]).substr(pos1 + 1);
		myfile << name_only[temp] << endl;
	}

	myfile.close();
}

void create_txt(string data_path, string destination_folder){
    cout << "Generating label.txt + paths.txt + names.txt at " << destination_folder << " ...";
	string** paths = new string*[100];
	for(int temp = 0; temp < 3; temp++){
		paths[temp] = new string[1000];
	}
	paths = read_directory(data_path);
	write_file_txt(paths, destination_folder);
	string* directory = new string[100];
	read_files_inside_one_directory(directory, data_path);
	write_file_name_only(directory, data_path, destination_folder);
	cout << " Done!" << endl;
}

int create_train_svm(string files_path) {
    cout << "Creating train_file at " << files_path << " ...";
	ifstream path,label;
    ofstream train;
    string line_path, line_label;
    vector <string> paths;
    vector <int> labels;

    path.open(files_path + "paths.txt");
    label.open(files_path + "labels.txt");

    while(getline(path, line_path))
        paths.push_back(line_path);


    while(getline(label, line_label))
        labels.push_back(stoi(line_label));

    if(labels.size() != paths.size())
    {
        cout << "Wrong input!" << endl;
        return -1;
    }

    /////////////////////////////////Create Data for SVM///////////////////////////////////

    string model_path = files_path + "model.pt";

    torch::jit::script::Module module = torch::jit::load(model_path);
    module.to(at::kCUDA);

    std::vector<torch::jit::IValue> inputs;
    cv::Mat image;
    train.open(files_path + "train");

    for(int i = 0; i < paths.size(); i++)
    {
        image = cv::imread(paths[i]);
        cv::resize(image, image, cv::Size(112,112));
        image.convertTo(image, CV_32FC3, 1.0f / 255.0f);
        auto input_tensor = torch::from_blob(image.data, {1, 112, 112, 3});
        input_tensor = input_tensor.permute({0, 3, 1, 2});
        input_tensor[0][0] = input_tensor[0][0].sub_(0.5).div_(0.5);
        input_tensor[0][1] = input_tensor[0][1].sub_(0.5).div_(0.5);
        input_tensor[0][2] = input_tensor[0][2].sub_(0.5).div_(0.5);
        input_tensor = input_tensor.to(at::kCUDA);
        torch::Tensor out_tensor = module.forward({input_tensor}).toTensor();
        train << labels[i] << " ";
        for(int j =  0; j < 512; j++)
            train << j << ":" << out_tensor[0][j].item<float>() << " ";
        train << endl;
    }
    cout << " Done!" << endl;
    return 1;
}

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
    exit(1);
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
        exit(1);
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
        if(label == NULL) // empty line
            exit_input_error(i+1);

        prob.y[i] = strtod(label,&endptr);
        if(endptr == label || *endptr != '\0')
            exit_input_error(i+1);

        while(1)
        {
            idx = strtok(NULL,":");
            val = strtok(NULL," \t");

            if(val == NULL)
                break;

            errno = 0;
            x_space[j].index = (int) strtol(idx,&endptr,10);
            if(endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
                exit_input_error(i+1);
            else
                inst_max_index = x_space[j].index;

            errno = 0;
            x_space[j].value = strtod(val,&endptr);
            if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
                exit_input_error(i+1);

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
                exit(1);
            }
            if ((int)prob.x[i][0].value <= 0 || (int)prob.x[i][0].value > max_index)
            {
                fprintf(stderr,"Wrong input format: sample_serial_number out of range\n");
                exit(1);
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
        exit(1);
    }
    SVM_MODEL = svm_train(&prob,&param);
    if(svm_save_model((files_path + "svm.model").c_str(), SVM_MODEL)){
        fprintf(stderr, "Can't save model to file %s\n", "test.model");
        exit(1);

    }
    else{
    	cout << "SVM model created!";
    }
    svm_free_and_destroy_model(&SVM_MODEL);
    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    free(x_space);
    free(line);
    cout << " Done!";
}