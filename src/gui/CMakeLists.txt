# Emotime Video Debug Gui Tool
add_executable(emotimevideodebug_cli emotimevideodebug_cli.cpp 
        ACapture.cpp VideoCapture.cpp AGui.cpp DebugGui.cpp
        ../detector/EmoDetector.cpp ../detector/BoostEmoDetector.cpp ../detector/FacePreProcessor.cpp ../detector/SVMEmoDetector.cpp
        ../training/Classifier.cpp ../training/AdaBoostClassifier.cpp ../training/SVMClassifier.cpp
        ../facedetector/FaceDetector.cpp
        ../gaborbank/GaborBank.cpp
        ../utils/string_utils.cpp ../utils/matrix_io.cpp )
target_link_libraries(emotimevideodebug_cli  ${OpenCV_LIBS} )
INSTALL(TARGETS emotimevideodebug_cli DESTINATION ${ASSETDIR} )

# Emotime Video Gui Tool
add_executable(emotimevideo_cli emotimevideo_cli.cpp 
        ACapture.cpp VideoCapture.cpp AGui.cpp EmotimeGui.cpp
        ../detector/EmoDetector.cpp ../detector/BoostEmoDetector.cpp ../detector/FacePreProcessor.cpp ../detector/SVMEmoDetector.cpp
        ../training/Classifier.cpp ../training/AdaBoostClassifier.cpp ../training/SVMClassifier.cpp
        ../facedetector/FaceDetector.cpp
        ../gaborbank/GaborBank.cpp
        ../utils/string_utils.cpp ../utils/matrix_io.cpp )
target_link_libraries(emotimevideo_cli ${OpenCV_LIBS} )
INSTALL(TARGETS emotimevideo_cli DESTINATION ${ASSETDIR} )

# Emotime Webcam Gui Tool
add_executable(emotimegui_cli emotimegui_cli.cpp 
        ACapture.cpp  VideoCapture.cpp AGui.cpp EmotimeGui.cpp
        ../detector/EmoDetector.cpp ../detector/BoostEmoDetector.cpp ../detector/FacePreProcessor.cpp ../detector/SVMEmoDetector.cpp
        ../training/Classifier.cpp ../training/AdaBoostClassifier.cpp ../training/SVMClassifier.cpp
        ../facedetector/FaceDetector.cpp
        ../gaborbank/GaborBank.cpp
        ../utils/string_utils.cpp ../utils/matrix_io.cpp )
target_link_libraries(emotimegui_cli ${OpenCV_LIBS} )
INSTALL(TARGETS emotimegui_cli DESTINATION ${ASSETDIR} )

# Gabor Parameter Tuning Gui Tool
add_executable(gaborgui_cli gaborgui_cli.cpp  
        ImageCapture.cpp ACapture.cpp GaborGui.hpp
        ../gaborbank/GaborBank.cpp
        ../utils/string_utils.cpp ../utils/matrix_io.cpp )
target_link_libraries(gaborgui_cli ${OpenCV_LIBS} )
INSTALL(TARGETS gaborgui_cli DESTINATION ${ASSETDIR} )

