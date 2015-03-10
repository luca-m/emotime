cmake_minimum_required(VERSION 2.8)

if(NOT ${WIN_32})
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   # Note that Microsoft Visual C++ compiler enables C++11 by default
endif()

#TODO: command line switch for it
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMULTITHREAD")

project(emotime)

#
# Dependencies
#

# OPENCV

find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
else()
  message(FATAL "Missing OpenCV Libraries")
endif()

#
# Setup Directories
#

set(ASSETDIR "${emotime_SOURCE_DIR}/assets" )
set(HEADERDIR "${emotime_SOURCE_DIR}/include" )
set(SRCDIR "${emotime_SOURCE_DIR}/src" )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

#
# Go Ahead
#
add_subdirectory(src)


