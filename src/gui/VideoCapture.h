/**
 * VideoCapture.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include "ACapture.h"

namespace emotime{
  /**
   * @class    VideoCapture
   *
   * @brief   Capture implementation using opencv VideoCapture
   *
   * @details
   *
   */
  class VideoCapture: public ACapture {

    public:

      /**
       * @brief Open a video from input device (eg. Camera)
       *
       * @param[in] devide number
       * @param[in] capture grayscale
       *
       * */
      VideoCapture(int deviceID, bool grayScale); 
      
      /**
       * @brief Open a video from video file.
       *
       * @param[in] video path
       * @param[in] capture grayscale
       * */
      VideoCapture(string infile, bool grayScale);
      
      ~VideoCapture();

      bool isReady();

    protected:

      bool extractFrame(Mat& frm) {
        return cap.read(frm);
      }

    private:
      cv::VideoCapture cap;
  };


}

#endif /* !VIDEOCAPTURE_H */

