cmake_minimum_required(VERSION 3.5)
project(parminel)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(minelement main.cpp minelement.h clock.h random_int.h)
target_link_libraries(minelement  Threads::Threads)

