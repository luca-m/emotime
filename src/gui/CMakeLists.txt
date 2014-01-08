
add_executable(debuggui_boost_cli boost_debug_gui.cpp capture.hpp debuggui.hpp agui.hpp 
                            ../detector/emo_detector.hpp ../detector/emo_detector.cpp 
                            ../detector/boost_emo_detector.cpp ../detector/preprocessor.hpp 
                            ../gaborbank/gaborbank.cpp 
                            ../facecrop/facedetector.cpp
                            ../utils/matrix_io.cpp )
target_link_libraries(debuggui_boost_cli ${OpenCV_LIBS} )
INSTALL(TARGETS debuggui_boost_cli DESTINATION ${ASSETDIR} )

add_executable(gaborgui_cli gaborgui_cli.cpp  capture.hpp debuggui.hpp agui.hpp 
                            ../detector/emo_detector.hpp ../detector/emo_detector.cpp 
                            ../detector/boost_emo_detector.cpp ../detector/preprocessor.hpp 
                            ../gaborbank/gaborbank.cpp 
                            ../facecrop/facedetector.cpp
                            ../utils/matrix_io.cpp )
target_link_libraries(gaborgui_cli ${OpenCV_LIBS} )
INSTALL(TARGETS gaborgui_cli DESTINATION ${ASSETDIR} )

add_executable(emotimegui_cli emotimegui_cli.cpp
                            ../detector/emo_detector.cpp
                            ../detector/boost_emo_detector.cpp
                            ../detector/svm_emo_detector.cpp
                            ../gaborbank/gaborbank.cpp
                            ../facecrop/facedetector.cpp
                            ../utils/matrix_io.cpp
                            ../utils/string_utils.cpp )
target_link_libraries(emotimegui_cli ${OpenCV_LIBS} )
INSTALL(TARGETS emotimegui_cli DESTINATION ${ASSETDIR} )

add_executable(emotimevideo_cli emotimevideo_cli.cpp capture.hpp debuggui.hpp agui.hpp 
                            ../detector/emo_detector.hpp ../detector/emo_detector.cpp 
                            ../detector/boost_emo_detector.cpp ../detector/svm_emo_detector.cpp 
                            ../detector/preprocessor.hpp 
                            ../gaborbank/gaborbank.cpp 
                            ../facecrop/facedetector.cpp
                            ../utils/matrix_io.cpp 
                            ../utils/string_utils.cpp )
target_link_libraries(emotimevideo_cli ${OpenCV_LIBS} )
INSTALL(TARGETS emotimevideo_cli DESTINATION ${ASSETDIR} )
