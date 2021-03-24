# JetsonNano-ArcFace
A high-performance face recognition application on Jetson Nano(6-9fps) / PC 

## I. Required:
- Libtorch for C++ ([PC](https://pytorch.org/cppdocs/installing.html)/[Jetson](https://forums.developer.nvidia.com/t/pytorch-for-jetson-version-1-8-0-now-available/72048))
- [OpenCV 3.4.x](https://github.com/CuteBoiz/Ubuntu_Installation/blob/master/opencv.md)
- [Cuda/CuDNN/TensorRT](https://github.com/CuteBoiz/Ubuntu_Installation/blob/master/cuda.md)
- [Qt]
  ```sh
  sudo apt -y install cmake cmake-gui cmake-qt-gui
  sudo apt -y install qtbase5-dev
  sudo apt-get -y install qtdeclarative5-dev
  
  sudo apt -y install libcrypto++-dev 
  sudo apt -y install libgoogle-glog-dev 
  sudo apt -y install libgflags-dev
  sudo apt -y install --no-install-recommends libboost-all-dev
  ```

If you are using Jetson you don't have to install Cuda/CuDNN/TensorRT.

## II. Run
*Clone project:*
```sh
git clone https://github.com/CuteBoiz/JetsonNano-ArcFace
cd JetsonNano-ArcFace
```

*If you are using Jetson Nano:*
```sh
rm CMakeLists.txt
mv CMakeLists.txt.jetson-nano  CMakeLists.txt
gedit CMakeLists.txt #Then change my username (tanphatnguyen) in the file to your username then start compile
```

*Compile & Run:*
```sh
cmake .
make
./main
```

## References
- [MTCNN](https://github.com/kpzhang93/MTCNN_face_detection_alignment)
- [Arcface](https://arxiv.org/abs/1801.07698)
- [MobileFaceNets](https://arxiv.org/abs/1804.07573)
- [libSVM](https://www.csie.ntu.edu.tw/~cjlin/libsvm/)
