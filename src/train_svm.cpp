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