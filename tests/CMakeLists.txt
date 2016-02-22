project(rplycpp_tests)
add_definitions(-std=c++11)

# Files used by tests
set(DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")

if("${RPLYCPP_TEST_OUTPUT_PATH}" STREQUAL "")
  set(RPLYCPP_TEST_OUTPUT_PATH ".")
endif()

add_definitions(
  -DRPLYCPP_TEST_OUTPUT_SIMPLE_PLY="${RPLYCPP_TEST_OUTPUT_PATH}/simple_ply_out.ply"
  -DRPLYCPP_TEST_OUTPUT_SURFACE_PLY="${RPLYCPP_TEST_OUTPUT_PATH}/surface_ply_out.ply"
  -DRPLYCPP_TEST_SIMPLE_PLY_XYZRGBNormal="${DATA_PATH}/simple_ply_xyz_rgb_normal.ply"
  -DRPLYCPP_TEST_SIMPLE_PLY_SURFACE="${DATA_PATH}/simple_ply_surface.ply"
)

# Rule: Each file will contain a single test
file(GLOB RPLYCPP_TEST_SOURCES *.cpp)

# Create an executable for each test
foreach(TEST_SOURCE_FILE ${RPLYCPP_TEST_SOURCES})
	# Get the filename part (no path)
	get_filename_component(TEST_SOURCE_FILENAME ${TEST_SOURCE_FILE} NAME)
	# Remove the extension
	string(REPLACE ".cpp" "" TEST_NAME ${TEST_SOURCE_FILENAME})
	add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
  target_link_libraries(${TEST_NAME} ${RPLYCPP_LIBS} ${GTEST_BOTH_LIBRARIES})
	# Add to GTest
	GTEST_ADD_TESTS(${TEST_NAME} "${TEST_ARGS}" AUTO)
endforeach()
