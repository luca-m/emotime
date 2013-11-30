#ifndef _H_BOST_EMO_DETECTOR
#define _H_BOST_EMO_DETECTOR 

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "emo_detector.hpp"

using namespace std;
using namespace cv;
using namespace emotime;

/**
 * Prediction function to use in EmoDetector
 * 
 * NOTE: no precalculation on sample frame is performed here
 * 
 * @param 
 * @param 
 * @return 
 * */
float boost_predict(CvBoost detector, cv::Mat frame);
/**
 * Create an EmoDetector using CvBoost as 2-class detectors
 *
 * @param
 * @return
 * */
EmoDetector<CvBoost> boost_EmoDetector_create(std::map<std::string, std::pair<emotime::Emotion, CvBoost> > & detectors);

#endif

