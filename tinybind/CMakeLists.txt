cmake_minimum_required(VERSION 2.8)

project(tinybind)

set(CMAKE_CXX_FLAGS -std=c++0x)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


aux_source_directory(. SRC_LIST)

add_library(tinybind SHARED ${SRC_LIST})
