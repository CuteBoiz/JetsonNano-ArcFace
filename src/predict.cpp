#include "predict.h"

string predict(const Mat faces){

    string model_path = "model.pt";
    torch::jit::script::Module module = torch::jit::load(model_path);

    cout << "Switch to GPU mode" << endl;

    module.to(at::kCUDA);

    vector<torch::jit::IValue> inputs;

    resize(faces, faces, Size(112,112));

    faces.convertTo(faces, CV_32FC3, 1.0f / 255.0f);

    auto input_tensor = torch::from_blob(faces.data, {1, 112, 112, 3});
    input_tensor = input_tensor.permute({0, 3, 1, 2});

    input_tensor[0][0] = input_tensor[0][0].sub_(0.5).div_(0.5);
    input_tensor[0][1] = input_tensor[0][1].sub_(0.5).div_(0.5);
    input_tensor[0][2] = input_tensor[0][2].sub_(0.5).div_(0.5);

    input_tensor = input_tensor.to(at::kCUDA);

    torch::Tensor out_tensor = module.forward({input_tensor}).toTensor();
    float prob = out_tensor[0][0].item<float>();
    cout << prob << endl;

    string name;

    //LOAD TRAINED SVM MODEL
            
    //PREDICT
    
    return name;
}