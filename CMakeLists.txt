cmake_minimum_required(VERSION 3.24)

project(thinkcell CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib/)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/lib/)


### Target main ####

set(SOURCES
${CMAKE_SOURCE_DIR}/main.cpp
${CMAKE_SOURCE_DIR}/interval_map.hpp
)

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/)


