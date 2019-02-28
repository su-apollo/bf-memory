cmake_minimum_required(VERSION 3.4.1)
project(bf-memory)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fno-rtti -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

set(OUTPUT test.out)
set(LIB bf-memory)
set(INCLUDE ../bf-memory/header)
aux_source_directory(../bf-memory/source SOURCE)
set(MAIN main.cpp)

add_library(${LIB} ${SOURCE})
target_include_directories(${LIB} PUBLIC ${INCLUDE})

add_executable(${OUTPUT} ${MAIN})
add_dependencies(${OUTPUT} ${LIB})
target_link_libraries(${OUTPUT} ${LIB})
