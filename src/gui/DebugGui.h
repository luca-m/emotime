/**
 *
 * @file    DebugGui.h
 * @brief   Defines the class DebugGui
 *
 */

#ifndef DEBUGGUI_H
#define DEBUGGUI_H

#include "AGui.h"

namespace emotime{

  /**
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

      /**
       *  @brief          Initialize the GUI
       *
       *  @return         False if something wrong
       */
      bool init();

      /**
       *  @brief          Produce a new frame, showing the given prediction
       *
       *  @param[in,out]      frame The image to be filled
       *  @param[in]     prediction The prediction to show
       *
       *  @return         False if something wrong.
       *
       */
      bool newFrame(Mat& frame, pair<Emotion, float> prediction);

  };

}

#endif /* !DEBUGGUI_H */

