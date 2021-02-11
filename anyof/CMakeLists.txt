cmake_minimum_required(VERSION 3.5)
project(anyof)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(anyof main.cpp par_any_of.h clock.h)
target_link_libraries(anyof  Threads::Threads)

