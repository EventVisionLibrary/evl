project(EventVisionLibrary)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_MACOSX_RPATH 1)

# set c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall")

# cmake can locate ${CMAKE_PREFIX_PATH}/lib
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)

include("CMakeLists.shared")

add_subdirectory("shutdown")
add_subdirectory("multithread")
add_subdirectory("buffer_davis")
add_subdirectory("buffer_csv")
add_subdirectory("rodtracking")
add_subdirectory("rodtracking_gl")
