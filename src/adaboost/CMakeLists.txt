add_executable( adatrain adatrain.cpp )
target_link_libraries( adatrain ${OpenCV_LIBS} )

INSTALL(TARGETS adatrain DESTINATION ${ASSETDIR} )

