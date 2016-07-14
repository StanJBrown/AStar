CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
PROJECT(Graph)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_BUILD_TYPE Debug)
SET(TEST_EXECUTABLE_NAME "graph_test")
ADD_SUBDIRECTORY(dependencies/gtest)


INCLUDE_DIRECTORIES(
    ./
    /test/
    /dependencies/gtest/include/
)

LINK_DIRECTORIES(
    /usr/lib/
)

ADD_LIBRARY(graph graph.cpp)


# TESTS
ADD_EXECUTABLE(graph_test test/graph_test.cpp ${gtest_SOURCE_DIR}/src/gtest_main.cc)
TARGET_LINK_LIBRARIES(
    graph_test
    graph
    gtest
)

enable_testing()
add_test(test graph_test)

