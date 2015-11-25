cmake_minimum_required(VERSION 2.8)

project(xml_parse)

set(CMAKE_CXX_FLAGS -std=c++0x)

set(TOOLKITS_DIR $ENV{TOOLKITS})

include_directories( ./ ..)

#link_directories(${TOOLKITS_DIR}/lib ../lib)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory(. SRC_LIST)

add_library(xml_parse SHARED ${SRC_LIST})

target_link_libraries(xml_parse tinybind)
