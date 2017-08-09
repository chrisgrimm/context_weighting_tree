cmake_minimum_required(VERSION 3.2)
project(context_weighting_tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES
        common.h
        cwt.cpp cwt.h cwt_node.cpp cwt_node.h
        cts.cpp cts.h cts_node.cpp cts_node.h
        context_tree.h
        location_dependent_cwt.cpp location_dependent_cwt.h
        python_api.cpp
        fastmath.cpp fastmath.hpp
        icsilog.h icsilogw.hpp
        icsilog.cpp jacoblog.hpp
        PowFast.hpp PowFast.cpp)

add_executable(construct_destruct_test ${SOURCE_FILES} test/construct_destruct_test.cpp)

add_executable(paper_example_test ${SOURCE_FILES} test/paper_example_test.cpp)

add_executable(image_test ${SOURCE_FILES} test/image_test.cpp)

add_library(cwt SHARED ${SOURCE_FILES})