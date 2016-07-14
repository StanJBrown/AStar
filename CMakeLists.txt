CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
PROJECT(Graph)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_BUILD_TYPE Debug)
SET(TEST_EXECUTABLE_NAME "graph_test")

INCLUDE_DIRECTORIES(
    ./
    /test/
)

LINK_DIRECTORIES(
    /usr/lib/
)

ADD_LIBRARY(graph graph.cpp)

ADD_EXECUTABLE(graph_test test/graph_test.cpp)
TARGET_LINK_LIBRARIES(
    graph_test
    graph
)

