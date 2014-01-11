/**
 * ACapture.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef ACAPTURE_H
#define ACAPTURE_H

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

namespace emotime{

 /**
  * @class    ACapture
  * @date    12/31/2013 10:53:55 AM
  *
  * @brief   Generic frame capture interface
  *
  * @details
  *
  */
  class ACapture{
    private:
      /// Retrieve grayscale
      bool toGrayscale;
    protected:
      /**
       * @brief
       *
       * @param[out] frm frame to fill
       * 
       * @return ture if frame has been retrieved
       * */ 
      virtual bool extractFrame(Mat & frm)=0;
    public:
      /**
       * @brief
       *
       * @param[int] Retrieve grayscal frames
       *
       * */
      ACapture(bool toGray);
      /**
       * @brief
       *
       * */
      virtual ~ACapture();
      /**
       * @brief
       *
       * @return true if capture device is ready to provide new frames
       * */
      virtual bool isReady()=0;
      /**
       * @brief
       *
       * @param[out] Frame to fill
       *
       * @return true if new frame is available
       * */
      bool nextFrame(Mat & frame);
  };



}


#endif /* !ACAPTURE_H */

