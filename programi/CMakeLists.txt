cmake_minimum_required (VERSION 2.8.11)
project (MATRIKE)
set (CMAKE_CXX_FLAGS "-O3 -march=native")
file(GLOB SOURCES "*.cpp")
add_executable(main ${SOURCES})

