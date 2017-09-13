cmake_minimum_required (VERSION 2.8.11)
project (MATRIKE)
set (CMAKE_CXX_FLAGS "-O3 -march=native -DNDEBUG -DBOOST_UBLAS_NDEBUG")
file(GLOB SOURCES "*.cpp")

find_package( Boost REQUIRED COMPONENTS program_options )
include_directories( ${Boost_INCLUDE_DIRS} )

#add_executable(main newtests.cpp matrix.h)
add_executable(main ${SOURCES})
target_link_libraries(main ${Boost_PROGRAM_OPTIONS_LIBRARY})




