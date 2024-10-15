cmake_minimum_required(VERSION 3.12)

project(Matrix_fma VERSION 1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

option(use_unoptimized "Use unoptimized code" OFF)

if(CMAKE_HOST_LINUX OR CMAKE_HOST_UNIX)
	if(use_unoptimized)
		set(CMAKE_CXX_FLAGS " -O3 -Dun_optimized ")
	else()
		set(CMAKE_CXX_FLAGS " -O3 -mavx -mfma -mavx2 -mtune=native ")
	endif()
endif()

if(CMAKE_HOST_WIN32)
  set(CMAKE_CXX_FLAGS " -O2 -arch:AVX2 ")
endif()

add_executable(performance_test src/performance_test.cpp src/Matrix.cpp src/MatrixView.cpp src/frobenius_norm.cpp src/Matrix.hpp src/MatrixView.hpp src/frobenius_norm.hpp )

add_executable(unit_test src/unit_test.cpp src/Matrix.cpp src/MatrixView.cpp src/frobenius_norm.cpp src/Matrix.hpp src/MatrixView.hpp src/frobenius_norm.hpp)
target_link_libraries(unit_test GTest::GTest)
