# - Try to find the libcaer library
# Once done this will define
#
#  LIBCAER_FOUND - system has LIBCAER
#  LIBCAER_INCLUDE_DIRS - the LIBCAER include directory
#  LIBCAER_LIBRARY_DIRS - the LIBCAER library directory
#  LIBCAER_LIBRARIES - the LIBCAER libraries

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBCAER REQUIRED libcaer)

if(LIBCAER_INCLUDE_DIRS)
    set(LIBCAER_FOUND TRUE)
    message("found libcaer")
endif()

if(NOT LIBCAER_FOUND)
    message("could not find libcaer")
endif()