project(rplycpp_samples)

# C++11 is required
add_definitions(-std=c++11)

# Setup our file paths. You won't need it
set(DATA_PATH "${CMAKE_SOURCE_DIR}/tests/data")

if("${RPLYCPP_TEST_OUTPUT_PATH}" STREQUAL "")
  set(RPLYCPP_TEST_OUTPUT_PATH ".")
endif()

add_definitions(
  -DRPLYCPP_SAMPLE_OUTPUT_SIMPLE_PLY="${RPLYCPP_TEST_OUTPUT_PATH}/example_ply_simple_out.ply"
  -DRPLYCPP_SAMPLE_OUTPUT_SURFACE_PLY="${RPLYCPP_TEST_OUTPUT_PATH}/example_ply_surface_out.ply"
  -DRPLYCPP_SAMPLE_SIMPLE_PLY="${DATA_PATH}/simple_ply_xyz_rgb_normal.ply"
  -DRPLYCPP_SAMPLE_SURFACE_PLY="${DATA_PATH}/simple_ply_surface.ply"
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# Rule: Each file will contain a single sample
file(GLOB RPLYCPP_SAMPLE_SOURCES *.cpp)

# Create an executable for each sample
foreach(SAMPLE_SOURCE_FILE ${RPLYCPP_SAMPLE_SOURCES})
	# Get the filename part (no path)
	get_filename_component(SAMPLE_SOURCE_FILENAME ${SAMPLE_SOURCE_FILE} NAME)
	# Remove the extension
	string(REPLACE ".cpp" "" SAMPLE_NAME ${SAMPLE_SOURCE_FILENAME})
	add_executable(${SAMPLE_NAME} ${SAMPLE_SOURCE_FILE})
	target_link_libraries(${SAMPLE_NAME} ${RPLYCPP_LIBS} )
endforeach()
