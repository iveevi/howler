cmake_minimum_required(VERSION 3.20)

project(howler CXX)

if(NOT DEFINED HOWLER_FMT_EXTERNAL)
	add_subdirectory(fmt)
endif()

add_library(howler STATIC howler.cpp)
target_link_libraries(howler fmt::fmt)
target_include_directories(howler INTERFACE .)