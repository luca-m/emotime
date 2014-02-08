
find_package(OpenCV COMPONENTS opencv_core opencv_ml opencv_imgproc opencv_highgui opencv_objdetect REQUIRED)

add_executable(emo_detector_cli emo_detector_cli.cpp 
                                EmoDetector.cpp
                                BoostEmoDetector.cpp
                                SVMEmoDetector.cpp
                                FacePreProcessor.cpp
                                ../training/Classifier.cpp
                                ../training/SVMClassifier.cpp
                                ../training/AdaBoostClassifier.cpp
                                ../utils/matrix_io.cpp
                                ../gaborbank/GaborBank.cpp
                                ../utils/string_utils.cpp
                                ../facedetector/FaceDetector.cpp)

target_link_libraries(emo_detector_cli ${OpenCV_LIBS} )

INSTALL(TARGETS emo_detector_cli DESTINATION ${ASSETDIR} )
