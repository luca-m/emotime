
add_executable( facecrop_cli facecrop_cli.cpp FaceDetector.cpp ../utils/matrix_io.cpp )
target_link_libraries( facecrop_cli ${OpenCV_LIBS} )

INSTALL(TARGETS facecrop_cli DESTINATION ${ASSETDIR} )
