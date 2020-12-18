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
			src/main_predict.cpp 
			src/baseEngine.h
			src/baseEngine.cpp
			src/common.h
			src/common.cpp 
			src/mtcnn.h
			src/mtcnn.cpp
			src/network.h
			src/network.cpp
			src/pnet_rt.h
			src/pnet_rt.cpp
			src/rnet_rt.h
			src/rnet_rt.cpp
			src/onet_rt.h
			src/onet_rt.cpp
			src/svm.h
			src/svm.cpp
			src/pBox.h)

add_executable(addID 
			src/main_addID.cpp 
			src/baseEngine.h
			src/baseEngine.cpp
			src/common.h
			src/common.cpp 
			src/mtcnn.h
			src/mtcnn.cpp
			src/network.h
			src/network.cpp
			src/pnet_rt.h
			src/pnet_rt.cpp
			src/rnet_rt.h
			src/rnet_rt.cpp
			src/onet_rt.h
			src/onet_rt.cpp
			src/svm.h
			src/svm.cpp
			src/pBox.h)

target_link_libraries( predict ${TORCH_LIBRARIES})
target_link_libraries( predict ${OpenCV_LIBS})
target_link_libraries( predict openblas)
target_link_libraries( predict cudart)
target_link_libraries( predict nvinfer)
target_link_libraries( predict nvparsers)

target_link_libraries( addID ${TORCH_LIBRARIES})
target_link_libraries( addID ${OpenCV_LIBS})
target_link_libraries( addID openblas)
target_link_libraries( addID cudart)
target_link_libraries( addID nvinfer)
target_link_libraries( addID nvparsers)






