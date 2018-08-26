cmake_minimum_required(VERSION 3.1)
project(image_reconstruction)

include("../CMakeLists.shared")

set(SOURCES
  ${PROJECT_SOURCE_DIR}/main.cpp
  ${PROJECT_SOURCE_DIR}/common.cpp
  ${PROJECT_SOURCE_DIR}/denoise.cpp
)

set(HEADERS
  ${PROJECT_SOURCE_DIR}/event.h
  ${PROJECT_SOURCE_DIR}/common.h
  ${PROJECT_SOURCE_DIR}/denoise.h
)
add_executable(${PROJECT_NAME}_bin ${SOURCES} ${SHARED_SOURCES})
target_link_libraries(${PROJECT_NAME}_bin ${SHARED_LIBRARIES})
