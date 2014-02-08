/**
 *
 * @file    VideoCapture.h
 * @brief   Contains the implementation of VideoCapture
 *
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
       * @param[in] deviceID opencv device id
       * @param[in] grayScale True if the frame should be captured in grayscale.
       *
       */
      VideoCapture(int deviceID, bool grayScale);

      /**
       * @brief Open a video from video file.
       *
       * @param[in] infile video path
       * @param[in] grayScale True if the frame should be captured in grayscale.
       */
      VideoCapture(string infile, bool grayScale);

      ~VideoCapture();


      /**
       *  @brief          Returns true if the device or file was opened
       *
       *  @return         Returns true if the device or file was opened
       *
       */
      bool isReady();

    protected:

      /**
       *  @brief          Extract a frame from the device
       *
       *  @param[in,out]      frm The frame to fill
       *
       *  @return         The result of cap.read
       *
       */
      bool extractFrame(Mat& frm);

    private:

      /// The opencv video capture
      cv::VideoCapture cap;
  };


}

#endif /* !VIDEOCAPTURE_H */

