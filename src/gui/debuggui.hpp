/*
 * debuggui.hpp
 */

#ifndef DEBUGGUI_HPP
#define DEBUGGUI_HPP

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "agui.hpp"
#include "preprocessor.hpp"
#include "emo_detector.hpp"
#include "boost_emo_detector.h"

using namespace std;
using namespace cv;
using namespace emotime;

namespace emotime{

  class DebugGuiBoost: public ADebugGui<CvBoost>{
    public:
      DebugGuiBoost(ACapture * capt, FacePreProcessor * fp, EmoDetector<CvBoost> * detect, int fps): ADebugGui<CvBoost>(capt, fp, detect, fps){
      }
  };
  /*
  class DebugGuiSvm: ADebugGui<CvSvm>{
      DebugGuiBoost(ACapture * capt, FacePreprocessor * fp, EmoDetector<CvSvm> * detect, int fps): ADebugGui<CvBoost>(capt, fp, detect, fps){
      }
  };
  */
}

#endif /* !DEBUGGUI_HPP */

