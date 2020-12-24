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
```

*Compile & Run:*
```sh
cmake .
make
./main
```
