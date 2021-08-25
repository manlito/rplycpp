cmake_minimum_required(VERSION 3.5)
project(rplycpp VERSION 1.1.0)

# Options
option(RPLYCPP_BUILD_TESTS "Build tests (requires GTest)" ON)
option(RPLYCPP_BUILD_SAMPLES "Build samples" ON)
set(RPLYCPP_TEST_OUTPUT_PATH "" CACHE STRING "Folder where to put results from write tests")

# Default build type: Release
if (NOT CMAKE_BUILD_TYPE AND NOT MSVC)
  set(CMAKE_BUILD_TYPE "Release")
endif ()

add_library(rplycpp)
target_include_directories(rplycpp
	PUBLIC
		$<INSTALL_INTERFACE:./src>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<INSTALL_INTERFACE:./third_party/rply>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/rply>
)

target_sources(rplycpp
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/third_party/rply/rply.h
		${CMAKE_CURRENT_SOURCE_DIR}/third_party/rply/rply.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/rplycpp.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/rplycpp_reader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/rplycpp_writer.cpp
)

target_compile_features(rplycpp
	PUBLIC
		cxx_std_17
)

add_library(RPLYCPP::RPLYCPP ALIAS rplycpp)

# When tests are enabled, GTest must be installed
if(RPLYCPP_BUILD_TESTS)
	enable_testing()
	find_package(GTest QUIET)
	if(GTEST_FOUND)
		message(STATUS "GTestIncludes: ${GTEST_INCLUDE_DIRS}")
		message(STATUS "GTestLibraries: ${GTEST_LIBRARIES}")
		add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
	else()
	    message(STATUS "GTest could not be found")
	endif()
endif()

if(RPLYCPP_BUILD_SAMPLES)
  message(STATUS "Including Samples")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/samples")
endif()

install(
	TARGETS
		rplycpp
	EXPORT
		PointcloudRasterTargets
	LIBRARY DESTINATION
		${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION
		${CMAKE_INSTALL_LIBDIR}
)

export(
	TARGETS
		rplycpp
	NAMESPACE
		RPLYCPP::
	FILE
		${CMAKE_CURRENT_BINARY_DIR}/RPLYCPPTargets.cmake
)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE rplycpp)