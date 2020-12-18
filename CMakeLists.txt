cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(arcface)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_PREFIX_PATH /home/tanphatnguyen/libtorch)



#OpenBlas
include_directories(/home/tanphatnguyen/OpenBLAS/include)
link_directories(/home/tanphatnguyen/OpenBLAS/lib)

#cuda
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)

#tensorrt
include_directories(/home/tanphatnguyen/TensorRT-7.2.1.6/include)
link_directories(/home/tanphatnguyen/TensorRT-7.2.1.6/lib)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)


#aux_source_directory(src DIR_SRCS)

add_executable(predict 
			src/*h 
			src/predict_main.cpp 
			src/baseEngine.cpp
			src/common.cpp 
			src/mtcnn.cpp
			src/network.cpp
			src/pnet.cpp
			src/rnet.cpp
			src/onet.cpp
			src/svm.cpp)

target_link_libraries( predict ${TORCH_LIBRARIES})
target_link_libraries( predict ${OpenCV_LIBS})
target_link_libraries( predict openblas)
target_link_libraries( predict cudart)
target_link_libraries( predict nvinfer)
target_link_libraries( predict nvparsers)





