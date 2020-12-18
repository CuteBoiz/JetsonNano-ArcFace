cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(main)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_PREFIX_PATH /home/vinh/libtorch)



#OpenBlas
include_directories(/home/vinh/OpenBLAS/include)
link_directories(/home/vinh/OpenBLAS/lib)

#cuda
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)

#tensorrt
include_directories(/home/vinh/TensorRT-7.2.1.6/include)
link_directories(/home/vinh/TensorRT-7.2.1.6/lib)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)


aux_source_directory(src DIR_SRCS)

add_executable(main ${DIR_SRCS} )

target_link_libraries( main ${TORCH_LIBRARIES})
target_link_libraries( main ${OpenCV_LIBS})
target_link_libraries( main openblas)
target_link_libraries( main cudart)
target_link_libraries( main nvinfer)
target_link_libraries( main nvparsers)





