/**
 * DebugGui.h
 * Copyright (C) 2014 luca.mella@studio.unibo.it
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef DEBUGGUI_H
#define DEBUGGUI_H

#include "AGui.h"

namespace emotime{

  /**
  * @class    ADebugGui
  * @author   Luca Mella
  * @date
  *
  * @brief   GUI for debugging purpose
  *
  */
  class DebugGui: public AGui {

    public:

      /**
       *  @brief          Creates an ADebugGUI
       *
       *  @param[in]      capt  A capture instance
       *  @param[in]      fp    The face preprocessor to use
       *  @param[in]      detect  An emodetector instance
       *  @param[in]      fps  Desired frame per second
       *
       */
      DebugGui(ACapture* capt, FacePreProcessor* fp, EmoDetector* detect, int fps);

    protected:
      /// Title for face window
      string faceWinTitle;
      /// Title for features window
      string featsWinTitle;

      bool init();

      bool newFrame(Mat& frame, pair<Emotion, float> prediction);
  
  };

}

#endif /* !DEBUGGUI_H */

