
IF (MSVC)
  # Setting the xecutables as console applications
  SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
ENDIF (MSVC)

include_directories(${SRCDIR}/training)
include_directories(${SRCDIR}/facedetector)
include_directories(${SRCDIR}/gaborbank)
include_directories(${SRCDIR}/utils)
include_directories(${SRCDIR}/detector)
include_directories(${SRCDIR}/gui)

SET(EMOTIME_LIB_FILES "")
SET(EMOTIME_H_FILES "")

add_subdirectory(training)
add_subdirectory(facedetector)
add_subdirectory(gaborbank)
add_subdirectory(dataset)
add_subdirectory(utils)
add_subdirectory(detector)
add_subdirectory(gui)

find_package(OpenCV COMPONENTS opencv_core opencv_ml opencv_imgproc
  opencv_highgui opencv_objdetect REQUIRED)

# Emotime library
add_library(emotime SHARED  facedetector/FaceDetector.cpp
                            detector/EmoDetector.cpp
                            detector/BoostEmoDetector.cpp
                            detector/SVMEmoDetector.cpp
                            training/Classifier.cpp
                            training/AdaBoostClassifier.cpp
                            training/SVMClassifier.cpp
                            detector/FacePreProcessor.cpp
                            gaborbank/GaborBank.cpp
                            gaborbank/GaborKernel.h
                            utils/matrix_io.cpp
                            utils/string_utils.cpp
                                                  )

target_link_libraries(emotime ${OpenCV_LIBS})

INSTALL(TARGETS emotime DESTINATION ${ASSETDIR})

INSTALL(FILES facedetector/FaceDetector.h
               utils/matrix_io.h utils/string_utils.h
               gaborbank/GaborBank.h gaborbank/GaborKernel.h
               detector/EmoDetector.h detector/BoostEmoDetector.h detector/SVMEmoDetector.h detector/FacePreProcessor.h 
       DESTINATION "${HEADERDIR}")

