cmake_minimum_required(VERSION 3.23)
project(cinsky)

set(CMAKE_CXX_STANDARD 20)

add_executable(cinsky main.cpp)
add_compile_options(-Wall -pedantic -Wno-long-long -O2 -c)
