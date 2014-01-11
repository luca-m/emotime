/**
 * ACapture.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
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

