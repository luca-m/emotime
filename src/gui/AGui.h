/**
 * aGui.h
 * Copyright (C) 2014 luca.mella@studio.unibo.it
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef AGUI_H
#define AGUI_H

#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>

#include "ACapture.h"
#include "EmoDetector.h"
#include "FacePreProcessor.h"

using std::stringstream;
using std::string;
using std::pair;

using namespace emotime;

namespace emotime {

  /**
  * @class    AGui
  * @date    12/31/2013 10:53:55 AM
  *
  * @brief   Generic GUI
  *
  * @details
  *
  */
  class AGui {

    public:

      /**
       *  @brief          Creates an AGui
       *
       *  @param[in]      capt  A capture instance
       *  @param[in]      fp    The face preprocessor to use
       *  @param[in]      detect  An emodetector instance
       *  @param[in]      fps  Desired frame per second
       *
       */
      AGui(ACapture* capt, FacePreProcessor* fp, EmoDetector* detect, int fps, string title="AGui: Main Emotime GUI");
      
      /**
       *  @brief    Start the gui
       *
       *  @return   False if something wrong.
       *
       */
      bool run();

    protected:

      /// Face preprocessor
      FacePreProcessor* preprocessor;
      /// Emotion detector
      EmoDetector* detector;
      /// Capture instance
      ACapture* capture;
      /// Title for the main window
      string mainWinTitle;
      /// Desired frames per second
      int fps;

      /**
       *  @brief          Initialize the windows
       *
       *  @return         Returns true if the initialization succeded
       *
       */
      virtual bool init();

      /**
       *  @brief          Produce the next frame
       *
       *  @return         False if there is no next frame
       *
       */
      virtual bool nextFrame(); 
      
      /**
       *  @brief          A new frame is available
       *
       *  @param[in]      frame The new frame that will be drawn after returing
       *  @param[in]      prediction The predicted emotion
       *
       *  @return         False if something wrong
       *
       */
      virtual bool newFrame(Mat& frame, pair<Emotion, float> prediction) = 0;

  };
}

#endif

