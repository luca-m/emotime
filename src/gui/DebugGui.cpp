/**
 *
 * @file    DebugGui.cpp
 * @brief   Contains the implementation of DebugGui
 *
 */

#include "DebugGui.h"

namespace emotime{

  DebugGui::DebugGui(ACapture* capt, FacePreProcessor* fp, EmoDetector* detect, int fps) : AGui(capt, fp, detect, fps, "Emotime Video Debug Gui") {
    faceWinTitle = string("AGui: Face");
    featsWinTitle = string("AGui: Features");
  }
  
  bool DebugGui::init() {
    if (!AGui::init()) {
      return false;
    }
    //namedWindow(faceWinTitle.c_str(), CV_WINDOW_AUTOSIZE);
    namedWindow(featsWinTitle.c_str(), CV_WINDOW_AUTOSIZE);
    return true;
  }

  bool DebugGui::newFrame(Mat& frame, pair<Emotion, float> prediction) {
        Mat copy;
        frame.copyTo(copy);
        stringstream ss;
        ss << "Emotion=" << emotionStrings(prediction.first) << ", Score=" << prediction.second;
        string osd = ss.str();
        cv::putText(frame, osd.c_str(), Point(80,60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar::all(255));
        // QT only
        //displayOverlay(mainWinTitle.c_str(), osd.c_str(), 2000);
        Mat face;
        if (AGui::preprocessor->extractFace(copy, face)) {
          Mat gabor;
          if (AGui::preprocessor->filterImage(face,gabor)){
            double min;
            double max;
            cv::minMaxIdx(gabor, &min, &max);
            cv::Mat adjMap;
            cv::convertScaleAbs(gabor, adjMap, 255 / max);
            Mat bigger;
            resize(adjMap,bigger,Size(adjMap.size().width*3,adjMap.size().height*3), 0, 0, CV_INTER_LINEAR);
            equalizeHist(bigger,bigger);  
            imshow(featsWinTitle.c_str(), bigger);
          }
        }
        return true;
      }

}


