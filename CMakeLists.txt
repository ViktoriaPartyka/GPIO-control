set(CMAKE_CXX_COMPILER "/home/kali/Project/application/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/arm-linux-g++")
cmake_minimum_required(VERSION 3.0)
project(LED)

find_library(GPIOD_LIBRARY NAMES gpiod)
find_path(GPIOD_INCLUDE_DIR NAMES gpiod.h)

include_directories(${GPIOD_INCLUDE_DIR})

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -Wall -Ofast -I/home/kali/Project/application/arm-buildroot-linux-gnueabihf_sdk-buildroot/include/")

set(CMAKE_EXE_LINKER_FLAGS "-lrt -pthread -L/home/kali/Project/application/arm-buildroot-linux-gnueabihf_sdk-buildroot/lib/")

set(CMAKE_FIND_ROOT_PATH "/home/kali/Project/application/arm-buildroot-linux-gnueabihf_sdk-buildroot/")

add_executable(LED main.cpp)

target_link_libraries(LED ${GPIOD_LIBRARY})
