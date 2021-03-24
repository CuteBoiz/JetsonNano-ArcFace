# JetsonNano-ArcFace
A high-performance face recognition application on Jetson Nano / PC 

## Required:
- [Libtorch]()
- [OpenCV 3.4.x]()
- [Cuda/CuDNN/TensorRT]()
- [Qt]()

If you are using Jetson you don't have to install Cuda/CuDNN/TensorRT.

## Compile &  Run
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
