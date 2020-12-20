cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(arcface)

include_directories(/home/tanphatnguyen/.local/lib/python3.6/site-packages/torch/include/torch/csrc/api/include)
include_directories(/home/tanphatnguyen/.local/lib/python3.6/site-packages/torch/include)

link_directories(/home/tanphatnguyen/.local/lib)
link_directories(/home/tanphatnguyen/.local/lib/python3.6/site-packages/torch/lib)

set(CMAKE_PREFIX_PATH /home/tanphatnguyen/.local/lib/python3.6/site-packages/torch)
set(Torch_DIR /home/tanphatnguyen/.local/lib/python3.6/dist-packages/torch)

#OpenBlas
include_directories(/home/tanphatnguyen/OpenBLAS/include)
link_directories(/home/tanphatnguyen/OpenBLAS/lib)

#cuda
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)

find_path(TENSORRT_INCLUDE_DIR NvInfer.h
  HINTS ${TENSORRT_ROOT} ${CUDA_TOOLKIT_ROOT_DIR} /usr/include
  PATH_SUFFIXES include)
find_path(TENSORRT_INCLUDE_DIR NvInferPlugin.h
  HINTS ${TENSORRT_ROOT} ${CUDA_TOOLKIT_ROOT_DIR}
  PATH_SUFFIXES include)
MESSAGE(STATUS "Found TensorRT headers at ${TENSORRT_INCLUDE_DIR}")
find_library(TENSORRT_LIBRARY_INFER nvinfer
  HINTS ${TENSORRT_ROOT} ${TENSORRT_BUILD} ${CUDA_TOOLKIT_ROOT_DIR}
  PATH_SUFFIXES lib lib64 lib/x64 lib/aarch64-linux-gnu)
find_library(TENSORRT_LIBRARY_INFER_PLUGIN nvinfer_plugin
  HINTS  ${TENSORRT_ROOT} ${TENSORRT_BUILD} ${CUDA_TOOLKIT_ROOT_DIR}
  PATH_SUFFIXES lib lib64 lib/x64 lib/aarch64-linux-gnu)
  find_library(TENSORRT_LIBRARY_PARSER nvparsers
  HINTS  ${TENSORRT_ROOT} ${TENSORRT_BUILD} ${CUDA_TOOLKIT_ROOT_DIR}
  PATH_SUFFIXES lib lib64 lib/x64 lib/aarch64-linux-gnu)
set(TENSORRT_LIBRARY ${TENSORRT_LIBRARY_INFER} ${TENSORRT_LIBRARY_INFER_PLUGIN} ${TENSORRT_LIBRARY_PARSER})
message(STATUS "Find TensorRT libs at ${TENSORRT_LIBRARY}")
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  TENSORRT DEFAULT_MSG TENSORRT_INCLUDE_DIR TENSORRT_LIBRARY)
if(NOT TENSORRT_FOUND)
  message(ERROR
    "Cannot find TensorRT library.")
endif()


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

target_link_libraries( addID ${TORCH_LIBRARIES})
target_link_libraries( addID ${OpenCV_LIBS})
target_link_libraries( addID openblas)
target_link_libraries( addID cudart)

target_link_libraries(predict ${TENSORRT_LIBRARY})
target_link_libraries(addID ${TENSORRT_LIBRARY})


set_property(TARGET predict PROPERTY CXX_STANDARD 14)
set_property(TARGET addID PROPERTY CXX_STANDARD 14)







