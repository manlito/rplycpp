cmake_minimum_required(VERSION 2.6)
project(rply)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}" RPLY_SOURCES_LIST)

add_library(${PROJECT_NAME} ${RPLY_SOURCES_LIST})
