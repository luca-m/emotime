cmake_minimum_required(VERSION 2.8)
project( facegrabber )

find_package( OpenCV REQUIRED )

set(ASSETDIR ${facegrabber_SOURCE_DIR}/bin CACHE INTERNAL FORCE)

add_subdirectory(src)


