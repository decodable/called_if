cmake_minimum_required (VERSION 2.6)
project (lib_called)

add_library(called called_function.cc)

target_link_libraries(called)
