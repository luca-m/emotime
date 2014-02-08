
SET(EMOTIME_LIB_FILES "${EMOTIME_LIB_FILES} training/Classifier.cpp training/SVMClassifier.cpp training/AdaBoostClassifier.cpp")
SET(EMOTIME_H_FILES "${EMOTIME_H_FILES} training/Classifier.h training/SVMClassifier.h training/AdaBoostClassifier.h")


ADD_EXECUTABLE(train_cli train_cli.cpp Classifier.cpp SVMClassifier.cpp
  AdaBoostClassifier.cpp ../utils/matrix_io.cpp)

FIND_PACKAGE(OpenCV COMPONENTS opencv_core opencv_ml opencv_contrib
  opencv_imgproc opencv_highgui REQUIRED)

TARGET_LINK_LIBRARIES(train_cli ${OpenCV_LIBS})

INSTALL(TARGETS train_cli DESTINATION ${ASSETDIR})
