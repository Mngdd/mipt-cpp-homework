cmake_minimum_required(VERSION 3.26)
project(huita)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra")

include_directories(.)

add_executable(huita
        VECTOR2.hpp
        VECTOR2.cpp)
