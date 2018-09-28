cmake_minimum_required (VERSION 2.6)
project (called_if_test)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/lib)

link_directories(${CMAKE_BINARY_DIR}lib)
link_directories(/usr/local/lib)

add_executable(called_if_test called_if_test.cc)

# do not line gtest
# https://github.com/google/googletest/issues/930
target_link_libraries(called_if_test gmock_main called pthread)

add_test(called_if_test called_if_test)
