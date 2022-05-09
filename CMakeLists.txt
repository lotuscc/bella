# 指定最低版本
cmake_minimum_required(VERSION 2.8)

# 指定项目名称
project(demo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# 设置C++编译参数(CMAKE_CXX_FLAGS是全局变量)
set(CMAKE_CXX_FLAGS "-Wall --std=c++20 -g")


find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})


# 头文件查找目录
include_directories(
    ./src
    ./src/ts
    ./protobuf
)


aux_source_directory(src DIR_LIBS_SRCS)
aux_source_directory(src/ts DIR_LIBS_SRCS)
aux_source_directory(protobuf DIR_LIBS_SRCS)



# 把src目录下所有源文件写入变量DIR_SRCS
aux_source_directory(server DIR_SERVER_SRCS)
# 以DIR_SRCS为源文件生成目标文件a.out
add_executable(server.out ${DIR_SERVER_SRCS} ${DIR_LIBS_SRCS})


aux_source_directory(client DIR_CLIENT_SRCS)
add_executable(client.out ${DIR_CLIENT_SRCS} ${DIR_LIBS_SRCS})


aux_source_directory(test DIR_TEST_SRCS)
add_executable(test.out ${DIR_TEST_SRCS}  ${DIR_LIBS_SRCS})


find_package(Threads)
target_link_libraries(client.out ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server.out ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(test.out ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(client.out ${PROTOBUF_LIBRARIES})
target_link_libraries(server.out ${PROTOBUF_LIBRARIES})
target_link_libraries(test.out  ${PROTOBUF_LIBRARIES})