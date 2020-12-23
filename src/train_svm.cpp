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

    idx = 0;
    ofstream myfile, labels;

    myfile.open (destination_folder + "paths.txt");
    labels.open(destination_folder + "labels.txt");

    for(int temp = 0; temp < len_of_folders_in_directory; temp++){
        int len_folder = find_length(directory[temp]);
        for(int temp1 = 0; temp1 < len_folder; temp1++){
            myfile << directory[temp][temp1] << endl;
            labels << temp << endl;
            idx++;
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
    cout << "Done!" << endl;
}

void exit_input_error(int line_num)
{
    fprintf(stderr,"Wrong input format at line %d\n", line_num);
    exit(1);
}



// void init()
// {
//     void (*print_func)(const char*) = NULL; // default printing to stdout

//     // default values
//     param.svm_type = C_SVC;
//     param.kernel_type = LINEAR;
//     param.degree = 3;
//     param.gamma = 0;    // 1/num_features
//     param.coef0 = 0;
//     param.nu = 0.5;
//     param.cache_size = 100;
//     param.C = 1;
//     param.eps = 1e-3;
//     param.p = 0.1;
//     param.shrinking = 1;
//     param.probability = 1;
//     param.nr_weight = 0;
//     param.weight_label = NULL;
//     param.weight = NULL;
//     cross_validation = 0;
// }

int create_train_svm(string files_path) {

    // init();
    const char *error_msg;

    //////////////////////////////////////////////////////////////////////////////////////////
    cout << "Creating train_file at " << files_path << " ... \n";
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


    //////////////////////////////////////////////////////////////////////////////////////////
    struct svm_model *SVM_MODEL;
    struct svm_node *x_space;
    struct svm_problem prob;  

    size_t elements, j;
    int i;

    prob.l = idx;

    elements = 513*idx;

    int cross_validation;

    prob.y = Malloc(double,prob.l);
    prob.x = Malloc(struct svm_node *,prob.l);
    x_space = Malloc(struct svm_node,elements);
    struct svm_parameter param;

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


    j = 0;

    string model_path = files_path + "model.pt";

    torch::jit::script::Module module = torch::jit::load(model_path);
    module.to(at::kCUDA);

    std::vector<torch::jit::IValue> inputs;
    cv::Mat image;

    for(i=0; i < paths.size(); i++)
    {
        prob.x[i] = &x_space[j];
        prob.y[i] = (double)(labels[i]);

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
        int count = 0;
        for(int id = 0; id < 512; id++)
        {
            x_space[j].index = id;
            x_space[j].value = out_tensor[0][id].item<double>();
            count++;
            ++j;
        }

        x_space[j++].index = -1;
    }

    param.gamma = 1.0/511;

    error_msg = svm_check_parameter(&prob,&param);

    if(error_msg)
    {  
        fprintf(stderr,"ERROR: %s\n",error_msg);
        exit(1);
    }

    SVM_MODEL = svm_train(&prob,&param);

    if(svm_save_model("Data/svm.model",SVM_MODEL))
    {
        fprintf(stderr, "can't save model to file %s\n", "test.model");
        exit(1);

    }
    svm_free_and_destroy_model(&SVM_MODEL);

    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    free(x_space);

    return 1;
}