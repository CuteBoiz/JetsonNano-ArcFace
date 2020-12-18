cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(main)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_PREFIX_PATH /home/tanphatnguyen/libtorch)

find_package(Torch REQUIRED)

#OpenBlas
include_directories(/home/tanphatnguyen/OpenBLAS)
link_directories(/home/tanphatnguyen/OpenBLAS)

#cuda
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)


#tensorrt
include_directories(/home/tanphatnguyen/TensorRT-7.2.1.6/include)
link_directories(/home/tanphatnguyen/TensorRT-7.2.1.6/lib)


find_package(OpenCV REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

AUX_SOURCE_DIRECTORY(./src DIR_SRCS)
add_executable(main ${DIR_SRCS})

target_link_libraries( main ${TORCH_LIBRARIES})
target_link_libraries( main ${OpenCV_LIBS})
target_link_libraries( main openblas)
target_link_libraries( main cudart)
target_link_libraries( main nvinfer)
target_link_libraries( main nvparsers)
target_link_libraries( main Qt${QT_VERSION_MAJOR}::Widgets)




