/**
 *
 * @file    EmotimeGui.h
 * @brief   Definition of EmotimeGUI
 *
 */

#ifndef EMOTIMEGUI_H
#define EMOTIMEGUI_H


#include "AGui.h"
#include "WebcamCapture.h"

namespace emotime{

  /**
   * @class EmotimeGui
   *
   * @brief Gui that detects emotion.
   *
   */
  class EmotimeGui : public AGui {

    public:

      /**
       *  @brief          Creates an EmotimeGUI with a webcam capture
       *
       *  @param[in]      fp    The face preprocessor to use
       *  @param[in]      detect  An EmoDetector instance
       *  @param[in]      fps  Desired frame per second
       *
       */
      EmotimeGui(FacePreProcessor* fp, EmoDetector* detect, int fps);

       /**
        *  @brief          Creates an EmotimeGUI with a custom capture
        *
        *  @param[in]      capture    The ACapture to use
        *  @param[in]      fp    The face preprocessor to use
        *  @param[in]      detect  An EmoDetector instance
        *  @param[in]      fps  Desired frame per second
        *
        */
      EmotimeGui(ACapture * capture, FacePreProcessor* fp, EmoDetector* detect, int fps);

      ~EmotimeGui();

    protected:


      /**
       *  @brief          Prints the prediction over the frame
       *
       *  @param[in,out]      frame The frame filled with the prediction
       *  @param[in]          prediction The prediction to display
       *
       *  @returns Always returns true
       */
      bool newFrame(Mat& frame, pair<Emotion, float> prediction);

    private:

  };

}

#endif /* !EMOTIMEGUI_H */

