cmake_minimum_required(VERSION 2.8)

project( emotime )

find_package( OpenCV REQUIRED )

set(ASSETDIR "${emotime_SOURCE_DIR}/assets" )

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DDEBUG" )

add_subdirectory(src)


