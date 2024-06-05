cmake_minimum_required(VERSION 3.5)

project(assignment6_5__1_QC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(/home/max/libpqxx-7.8.1 pqxx-build)
add_executable(assignment6_5__1_QC main.cpp
    table_class.h
    table_class.cpp)

target_link_libraries(assignment6_5__1_QC PRIVATE pqxx)



