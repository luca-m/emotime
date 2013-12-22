
add_executable(boost_emo_detector_cli boost_emo_detector_cli.cpp 
               emo_detector.cpp boost_emo_detector.cpp emo_detector.hpp preprocessor.hpp
               ../utils/matrix_io.cpp ../gaborbank/gaborbank.cpp 
               ../adaboost/adapredict.cpp ../facecrop/facecrop.cpp
               ../facecrop/facedetector.cpp ../facecrop/registration.cpp)
target_link_libraries(boost_emo_detector_cli ${OpenCV_LIBS} )
INSTALL(TARGETS boost_emo_detector_cli DESTINATION ${ASSETDIR} )

