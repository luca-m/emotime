/**
 *
 * @file    svm_emo_detector.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/27/2013 12:12:58 PM
 * @brief   
 *
 * @details 
 *
 */

#include "svm_emo_detector.h"

using namespace emotime;

float SVMEmoDetector::predict(CvSVM* detector, cv::Mat& frame)
{
  return detector->predict(frame);
}
