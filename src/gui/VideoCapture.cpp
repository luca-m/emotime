/**
 * VideoCapture.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "VideoCapture.h"

namespace emotime{

  VideoCapture::VideoCapture(int deviceID, bool grayScale): ACapture(grayScale) {
    cap.open(deviceID);
  }
      
  VideoCapture::VideoCapture(string infile, bool grayScale): ACapture(grayScale) {
    cap.open(infile.c_str());
  }
  
  VideoCapture::~VideoCapture() {
    cap.release();
  }
  
  bool VideoCapture::isReady() {
    if (cap.isOpened()) {
      return true;
    } else {
      return true;
    }
  }

}


