
add_executable(debuggui_boost_cli boost_debug_gui.cpp capture.hpp debuggui.hpp agui.hpp 
                            ../detector/emo_detector.hpp ../detector/emo_detector.cpp 
                            ../detector/boost_emo_detector.cpp ../detector/preprocessor.hpp 
                            ../gaborbank/gaborbank.cpp 
                            ../facecrop/facecrop.cpp ../facecrop/registration.cpp ../facecrop/facedetector.cpp
                            ../utils/matrix_io.cpp )
target_link_libraries(debuggui_boost_cli ${OpenCV_LIBS} )
INSTALL(TARGETS debuggui_boost_cli DESTINATION ${ASSETDIR} )

