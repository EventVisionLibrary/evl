cmake_minimum_required(VERSION 3.1)
project(save_csv)

include("../CMakeLists.shared")

set(SOURCES
${PROJECT_SOURCE_DIR}/main.cpp
)

add_executable(${PROJECT_NAME}_bin ${SOURCES} ${SHARED_SOURCES})
target_link_libraries(${PROJECT_NAME}_bin ${SHARED_LIBRARIES})
