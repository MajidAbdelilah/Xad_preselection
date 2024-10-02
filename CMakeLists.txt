cmake_minimum_required(VERSION 3.12)

project(Matrix_fma VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)

if(CMAKE_HOST_LINUX OR CMAKE_HOST_UNIX)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS " -g -Ofast -mavx -mfma -mavx2 -mtune=native -fsanitize=address")
    endif()
    if(CMAKE_BUILD_TYPE MATCHES Release)
        set(CMAKE_CXX_FLAGS " -Ofast -mavx -mfma -mavx2 -mtune=native ")
    endif()
endif()

add_executable(performance_test src/performance_test.cpp src/Matrix.cpp src/MatrixView.cpp src/frobenius_norm.cpp)

