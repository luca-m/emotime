cmake_minimum_required(VERSION 2.8)

project( emotime )

find_package( OpenCV REQUIRED )

set(ASSETDIR "${emotime_SOURCE_DIR}/assets" )

add_subdirectory(src)


