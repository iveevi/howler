cmake_minimum_required(VERSION 3.20)

project(howler CXX)

if(NOT DEFINED HOWLER_FMT_EXTERNAL)
	add_subdirectory(fmt)
endif()

if(NOT DEFINED HOWLER_PREFIX)
	set(HOWLER_PREFIX "default")

	message(WARNING "HOWLER_PREFIX not set, using 'default'")
endif()

add_compile_definitions(HOWLER_PREFIX="${HOWLER_PREFIX}")

set(HOWLER_ARTIFACT_NAME "howler-${HOWLER_PREFIX}")

message(STATUS "Howler library artifact set to ${HOWLER_ARTIFACT_NAME}")

add_library(${HOWLER_ARTIFACT_NAME} STATIC howler.cpp)
target_link_libraries(${HOWLER_ARTIFACT_NAME} fmt)
target_include_directories(${HOWLER_ARTIFACT_NAME} INTERFACE .)