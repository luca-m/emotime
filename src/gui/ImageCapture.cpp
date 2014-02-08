/**
 *
 * @file    ImageCapture.cpp
 * @brief   Contains the implementation of ImageCapture
 *
 */

#include "ImageCapture.h"

namespace emotime{

  ImageCapture::ImageCapture(string infile, bool grayScale): ACapture(grayScale) {
        image=imread(infile.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
  }
  
  bool ImageCapture::isReady(){
    return image.cols>0 && image.rows>0;
  }

  bool ImageCapture::extractFrame(Mat & frm){
    if (isReady()){
      image.copyTo(frm);
      return true;
    } else {
      return false;
    }
  }

}


