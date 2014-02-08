/**
 * @file    ACapture.h
 * @brief   Definition of ACapture
 *
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
  * @class   ACapture
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
       * @brief Extracts the frame
       *
       * @param[in,out] frm frame to fill
       * 
       * @return true if frame has been retrieved
       * */ 
      virtual bool extractFrame(Mat & frm)=0;
    public:
      /**
       * @brief Creates an ACaputre specifying if the capture must be performed
       * in grayscale.
       *
       * @param[in] toGray Retrieve grayscal frames
       *
       * */
      ACapture(bool toGray);

      virtual ~ACapture();
      /**
       * @brief Returns true if the device is ready to provide new frames
       *
       * @return true if capture device is ready to provide new frames
       * */
      virtual bool isReady()=0;
      /**
       * @brief Fills the given image with a new frame.
       *
       * @param[in,out] frame Frame to fill
       *
       * @return true if new frame is available
       * */
      bool nextFrame(Mat & frame);
  };



}


#endif /* !ACAPTURE_H */

