add_executable( facecrop_cli facecrop_cli.cpp facecrop.cpp facedetector.cpp registration.cpp ../utils/matrix_io.cpp )
target_link_libraries( facecrop_cli ${OpenCV_LIBS} )

INSTALL(TARGETS facecrop_cli DESTINATION ${ASSETDIR} )

