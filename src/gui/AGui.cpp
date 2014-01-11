/**
 *  aGUui.cpp
 *
 * */
#include "AGui.h"

namespace emotime{

  AGui::AGui(ACapture* capt, FacePreProcessor* fp, EmoDetector* detect, int fps, string title) {
       capture = capt;
       preprocessor = fp;
       detector = detect;
       mainWinTitle = title;
       this->fps = fps;
     }
  
  bool AGui::run() {
    if(!init()) {
      return false;
    }
    while (nextFrame()) {
      int key;
      if (fps <= 0) {
        key = waitKey(0);
      } else {
        key = waitKey((int) 1000.0 / fps);
      }
      if((key & 0xFF) == 27) {
        break;
      }
    }
    return true;
  }

  bool AGui::init() {
    namedWindow(mainWinTitle.c_str(), WINDOW_NORMAL);
    return true;
  }

  bool AGui::nextFrame() {
    Mat frame;
    Mat featvector;
    if (capture->nextFrame(frame)) {
      if (preprocessor->preprocess(frame, featvector)) {
        pair<Emotion, float> prediction = detector->predict(featvector);
        if (!newFrame(frame, prediction)) {
          return false;
        }
        imshow(mainWinTitle.c_str(), frame);
      }
      return true;
    } else {
      return false;
    }
  }

}

