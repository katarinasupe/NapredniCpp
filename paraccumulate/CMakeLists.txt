cmake_minimum_required(VERSION 3.12.0)
project(paraccumulate VERSION 0.1.0)

find_package(Threads REQUIRED)
#find_package(TBB REQUIRED)


add_executable(paraccumulate main.cpp paraccumulate.h clock.h)
target_link_libraries(paraccumulate  Threads::Threads)

set_target_properties(paraccumulate
    PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )
