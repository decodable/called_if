cmake_minimum_required (VERSION 2.6)
project (called_if_libclang)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
FIND_PACKAGE(Boost 1.65 COMPONENTS log REQUIRED)
FIND_PACKAGE(Threads)

include_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(/usr/local/lib)
add_executable(called_if called_if.cc main.cc)

target_link_libraries(called_if called clang ${Boost_LOG_LIBRARY} ${Boost_LIBRARIES} pthread)
