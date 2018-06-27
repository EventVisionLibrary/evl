cmake_minimum_required(VERSION 3.1)
project(rodtracking_gl)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include("../CMakeLists.shared")
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

set(SOURCES
${PROJECT_SOURCE_DIR}/main.cpp
)

set(SHARED_LIBRARIES
	${SHARED_LIBRARIES}
)

set(SOURCES
${PROJECT_SOURCE_DIR}/main.cpp
)

add_executable(${PROJECT_NAME}_bin ${SOURCES} ${SHARED_SOURCES})
target_link_libraries(${PROJECT_NAME}_bin ${SHARED_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLUT_LIBRARIES} )
