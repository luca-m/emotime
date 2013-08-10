add_executable( facecrop facecrop.cpp facedetector.h facedetector.cpp registration.h registration.cpp)
target_link_libraries( facecrop ${OpenCV_LIBS} )

INSTALL(TARGETS facecrop DESTINATION ${ASSETDIR} )

