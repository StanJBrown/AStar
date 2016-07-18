CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
PROJECT(AStar)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_BUILD_TYPE Debug)
SET(TEST_EXECUTABLE_NAME "AStar_test")
ADD_SUBDIRECTORY(dependencies/gtest)


INCLUDE_DIRECTORIES(
    ./
    /test/
    /dependencies/gtest/include/
)

LINK_DIRECTORIES(
    /usr/lib/
)

ADD_LIBRARY(Astar AStar.cpp)


# TESTS
ADD_EXECUTABLE(AStar_test test/AStar_test.cpp ${gtest_SOURCE_DIR}/src/gtest_main.cc)
TARGET_LINK_LIBRARIES(
    AStar_test
    Astar
    gtest
)

enable_testing()
add_test(test AStar_test)

