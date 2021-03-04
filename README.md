# JetsonNano-ArcFace
C++ Arcface with TensorRT + GUI  on Jetson Nano 

## Required:
- [Pytorch]()
- [OpenCV 3.x.x]()
- [Cuda/CuDNN]()
- [TensorRT]()
- [OpenBlas]()
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
