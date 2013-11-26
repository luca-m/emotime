
add_executable(boost_emo_detector_cli boost_emo_detector_cli.cpp boost_emo_detector.hpp emo_detector.hpp ../utils/matrix_io.cpp)
target_link_libraries(boost_emo_detector_cli ${OpenCV_LIBS} )
INSTALL(TARGETS aboost_emo_detector_cli DESTINATION ${ASSETDIR} )
