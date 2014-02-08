/**
 * @file    ACapture.cpp
 * @brief   Implementation of ACapture
 *
 */

#include "ACapture.h"

namespace emotime{

  ACapture::ACapture(bool toGray){
        toGrayscale=toGray;
  }

  ACapture::~ACapture(){}

  bool ACapture::nextFrame(Mat & frame){
    Mat frm;
    bool hasNext;
    hasNext=extractFrame(frm);
    if (hasNext){
      if (toGrayscale && frm.channels()>2){
        Mat gray(frm.size(), CV_8UC1);
        cvtColor(frm, gray, CV_BGR2GRAY);
        gray.copyTo(frame);
      } else {
        frm.copyTo(frame);
      }
      return true;
    }
    return false;
  }  

}

