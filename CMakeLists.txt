cmake_minimum_required(VERSION 3.2)
project(context_weighting_tree)

set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(SOURCE_FILES
        cwt.cpp
        cwt.h
        cwt_node.cpp
        cwt_node.h
        fastmath.cpp
        fastmath.hpp
        icsilog.h
        icsilogw.hpp
        icsilog.cpp
        jacoblog.hpp
        PowFast.hpp
        PowFast.cpp)

add_executable(context_weighting_tree ${SOURCE_FILES} test/image_test.cpp)

add_executable(construct_destruct_test ${SOURCE_FILES} test/construct_destruct_test.cpp)

add_executable(paper_example_test ${SOURCE_FILES} test/paper_example_test.cpp)

add_executable(image_test ${SOURCE_FILES} test/image_test.cpp)