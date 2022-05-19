
aux_source_directory(kernel DIR_LIBS_SRCS)
aux_source_directory(net DIR_LIBS_SRCS)
aux_source_directory(protobuf DIR_LIBS_SRCS)
aux_source_directory(ts DIR_LIBS_SRCS)

include_directories(
    ./include
)

add_library(ell SHARED
    ${DIR_LIBS_SRCS}    
)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

target_link_libraries(ell ${PROTOBUF_LIBRARIES})


find_package(Threads)
target_link_libraries(ell ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(ell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)



