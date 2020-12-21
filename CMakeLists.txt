cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(arcface)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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
find_package(Qt5 COMPONENTS Quick Core Widgets  REQUIRED)
qt5_add_resources(RESOURCES qml.qrc)
if(Qt5Widgets_FOUND)
	message("-- Found QT5: !")
endif()

#aux_source_directory(src DIR_SRCS)

link_libraries(stdc++fs)


add_executable(main 
			src/main.cpp 
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
			src/pBox.h
			src/window.cpp
			src/window.h
			src/window.ui
			src/ui_window.h
			src/train_svm.h
			src/train_svm.cpp)

target_link_libraries( main ${TORCH_LIBRARIES})
target_link_libraries( main ${OpenCV_LIBS})
target_link_libraries( main openblas)
target_link_libraries( main cudart)
target_link_libraries( main nvinfer)
target_link_libraries( main nvparsers)
target_link_libraries( main Qt5::Core Qt5::Quick Qt5::Widgets)
target_link_libraries( main stdc++fs)







