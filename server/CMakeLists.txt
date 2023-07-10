cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
find_library(jemalloc_lib jemalloc) # jemalloc 5.0
find_package(gflags)
find_package(glog)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(database Server.cpp)
set_target_properties(database PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
target_link_libraries(database ${jemalloc_lib} glog::glog services)


