# 指定最低版本
cmake_minimum_required(VERSION 2.8)

# 指定项目名称
project(demo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# 设置C++编译参数(CMAKE_CXX_FLAGS是全局变量)
set(CMAKE_CXX_FLAGS "-Wall --std=c++20 -O0 -g")



add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(src)
