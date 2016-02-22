cmake_minimum_required(VERSION 2.6)
project(rplycpp)
add_definitions(-std=c++11)

# Options
option(RPLYCPP_BUILD_TESTS "Build tests (requires GTest)" ON)
set(RPLYCPP_TEST_OUTPUT_PATH "" CACHE STRING "Folder where to put results from write tests")

# Default build type: Release
if (NOT CMAKE_BUILD_TYPE AND NOT MSVC)
  set(CMAKE_BUILD_TYPE "Release")
endif ()

# Original RPLy library (http://w3.impa.br/~diego/software/rply/)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/rply-1.1.4")

# Build project
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" RPLYCPP_SOURCES_LIST)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/rply-1.1.4" RPLY_SOURCES_LIST)
add_library(rplycpp ${RPLYCPP_SOURCES_LIST} ${RPLY_SOURCES_LIST})
set(RPLYCPP_LIBS rplycpp)

# When tests are enabled, GTest must be installed
if(RPLYCPP_BUILD_TESTS)
	enable_testing()
	find_package(GTest QUIET)
	if(GTEST_FOUND)
		message(STATUS "GTestIncludes: ${GTEST_INCLUDE_DIRS}")
		message(STATUS "GTestLibraries: ${GTEST_LIBRARIES}")
		add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
	else()
		message(FATAL_ERROR "GTest could not be found")
	endif()
endif()

