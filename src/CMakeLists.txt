project(rplycpp)

include_directories(.)
aux_source_directory(. RPLYCPP_SOURCES)
add_library(${PROJECT_NAME} ${RPLYCPP_SOURCES})

# DensePoints::Core
target_link_libraries(${PROJECT_NAME} densepoints_core)

# Densepoints common definitions and libraries
target_link_libraries(${PROJECT_NAME} ${DENSEPOINTS_LIBS})
add_definitions(${DENSEPOINTS_DEFINITIONS})
