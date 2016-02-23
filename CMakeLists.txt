set(CMAKE_CXX_COMPILER "/home/mecatro/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++")
link_directories("/home/mecatro/Raspberry/usr/lib")
file(GLOB HEADERS src/*/*.h)
file(GLOB SRC_LIST src/*/*.cpp)

message(STATUS "${SRC_LIST}=${${SRC_LIST}}")

set(CMAKE_CXX_FLAGS "-lwiringPi -lpthread -g -std=c++11")
project(roblind)
cmake_minimum_required(VERSION 2.8)
add_executable(${PROJECT_NAME} ${SRC_LIST})
