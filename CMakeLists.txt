cmake_minimum_required(VERSION 3.16)
project(bl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS NO)

file(GLOB SOURCES "./src/*.cpp")
add_executable(bl ${SOURCES})
add_compile_options(bl -Wall -Wextra)

target_include_directories(bl PRIVATE "./include")
