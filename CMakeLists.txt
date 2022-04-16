# 指定最低版本
cmake_minimum_required(VERSION 2.8)

# 指定项目名称
project(demo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_BUILD_TYPE Debug)

# 设置C++编译参数(CMAKE_CXX_FLAGS是全局变量)
set(CMAKE_CXX_FLAGS "-Wall --std=c++17 -g")

# 头文件查找目录
include_directories(
    ./src
)

# 库文件查找目录
# link_directories(./lib)

# sub_dir指定包含CMakeLists.txt和源码文件的子目录位置
# binary_dir是输出路径， 一般可以不指定
# add_subdirecroty(sub_dir [binary_dir])

# 把src目录下所有源文件写入变量DIR_SRCS
aux_source_directory(./ DIR_SERVER_SRCS)
aux_source_directory(server DIR_SERVER_SRCS)
# 以DIR_SRCS为源文件生成目标文件a.out
add_executable(server.out ${DIR_SERVER_SRCS})


aux_source_directory(./ DIR_CLIENT_SRCS)
aux_source_directory(client DIR_CLIENT_SRCS)
add_executable(client.out ${DIR_CLIENT_SRCS})


find_package(Threads)
target_link_libraries(client.out ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server.out ${CMAKE_THREAD_LIBS_INIT})