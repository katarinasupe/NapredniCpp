cmake_minimum_required(VERSION 3.5)
project(hash)

list(APPEND CMAKE_MODULE_PATH "cmake")
include(CTest)

enable_testing()

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(external/googletest)


