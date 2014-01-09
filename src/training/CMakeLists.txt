

ADD_EXECUTABLE(train_cli train_cli.cpp Classifier.cpp SVMClassifier.cpp
  AdaBoostClassifier.cpp ../utils/matrix_io.cpp)
TARGET_LINK_LIBRARIES(train_cli ${OpenCV_LIBS})
INSTALL(TARGETS train_cli DESTINATION ${ASSETDIR})

