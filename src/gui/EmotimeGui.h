/**
 * EmotimeGui.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef EMOTIMEGUI_H
#define EMOTIMEGUI_H


#include "AGui.h"
#include "WebcamCapture.h"

namespace emotime{
 
  /**
   * @class EmotimeGui
   *
   * @brief Emotime sample webcam gui. 
   *
   * @description
   *
   * */ 
  class EmotimeGui : public AGui {

    public:

      /**
       * @brief EmotimeGui creator with webcam 
       *
       * @param[in]
       * @param[in] 
       * @param[in] 
       *
       * */
      EmotimeGui(FacePreProcessor* fp, EmoDetector* detect, int fps) : AGui(new WebcamCapture(true), fp, detect, fps, "Emotime!"){}
      /**
       * @brief EmotimeGui creator
       *
       * @param[in]
       * @param[in] 
       * @param[in] 
       * @param[in] 
       *
       * */
      EmotimeGui(ACapture * capture, FacePreProcessor* fp, EmoDetector* detect, int fps) : AGui(capture, fp, detect, fps, "Emotime!"){}

      ~EmotimeGui();

    protected:

      bool newFrame(Mat& frame, pair<Emotion, float> prediction);

    private:
  
  };  

}

#endif /* !EMOTIMEGUI_H */

