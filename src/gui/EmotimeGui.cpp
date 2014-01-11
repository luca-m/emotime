/**
 * EmotimeGui.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "EmotimeGui.h"

namespace emotime{

  EmotimeGui::~EmotimeGui() {
    //delete this->capture;
  }

  bool EmotimeGui::newFrame(Mat& frame, pair<Emotion, float> prediction) {
    Mat copy;
    frame.copyTo(copy);
    stringstream ss, ss2;
    ss << "Emotion: " << emotionStrings(prediction.first);
    ss2 << "Score: " << prediction.second;
    string osd = ss.str();
    string osd2 = ss2.str();

    cv::putText(frame, osd.c_str(), Point(20,60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar::all(255));
    cv::putText(frame, osd2.c_str(), Point(20,100), FONT_HERSHEY_SIMPLEX, 0.7, Scalar::all(255));
    // QT only
    //displayOverlay(mainWinTitle.c_str(), osd.c_str(), 2000);
    return true;
  }


}


