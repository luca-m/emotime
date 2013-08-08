add_executable( gaborbank gaborbank.cpp gabor.h gabor.cpp )
target_link_libraries( gaborbank ${OpenCV_LIBS} )

INSTALL(TARGETS gaborbank DESTINATION ${ASSETDIR})