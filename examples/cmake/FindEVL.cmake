# - Try to find the EVL library
# Once done this will define
#
#  EVL_FOUND - system has EVL
#  EVL_INCLUDE_DIRS - the EVL include directory
#  EVL_SOURCES - the EVL source files

if(NOT EVL_FOUND)

FIND_PATH(EVL_INCLUDE_DIRS evl/evl.hpp
   ${PROJECT_SOURCE_DIR}/../src
   ${PROJECT_SOURCE_DIR}/../../src
   ${PROJECT_SOURCE_DIR}/../../../src
)

if(EVL_INCLUDE_DIRS)
   set(EVL_FOUND TRUE)
   message("found EVL")
   message("include directory of EVL is")
   message(${EVL_INCLUDE_DIRS})
   file(GLOB EVL_CORE_SOURCES
        ${EVL_INCLUDE_DIRS}/evl/core/*.h
        ${EVL_INCLUDE_DIRS}/evl/core/*.hpp
        ${EVL_INCLUDE_DIRS}/evl/core/*.cpp
   )
   file(GLOB EVL_IMGPROC_SOURCES
        ${EVL_INCLUDE_DIRS}/evl/imgproc/*.h
        ${EVL_INCLUDE_DIRS}/evl/imgproc/*.hpp
        ${EVL_INCLUDE_DIRS}/evl/imgproc/*.cpp
   )
   set(EVL_SOURCES
	${EVL_CORE_SOURCES}
	${EVL_IMGPROC_SOURCES}
    )
   message("source of EVL is")
   message(${EVL_SOURCES})
endif()

endif()
