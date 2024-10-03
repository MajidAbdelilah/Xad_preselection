cmake_minimum_required(VERSION 3.12)

project(Matrix_fma VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)

if(CMAKE_HOST_LINUX OR CMAKE_HOST_UNIX)
  set(CMAKE_CXX_FLAGS " -Ofast -mavx -mfma -mavx2 -mtune=native ")
endif()

if(CMAKE_HOST_WIN32)
  set(CMAKE_CXX_FLAGS " -O2 -arch:AVX2 ")
endif()

add_executable(performance_test src/performance_test.cpp src/Matrix.cpp src/MatrixView.cpp src/frobenius_norm.cpp)

