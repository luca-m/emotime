#ifndef _H_BOST_EMO_DETECTOR
#define _H_BOST_EMO_DETECTOR 

#include "emo_detector.hpp"
#include "facedetector.h"

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
float boost_predict(CvBoost & detector, cv::Mat & frame);


EmoDetector<CvBoost> boost_EmoDetector_create_no_preprocess( map<string, pair<Emotion, CvBoost *> > & detectors);
/**
 * Create an EmoDetector using CvBoost as 2-class detectors
 *
 * @param
 * @param
 * @param
 * @param 
 * @param
 * @param
 *
 * @return
 * */
//EmoDetector<CvBoost> boost_EmoDetector_create(std::map<std::string, std::pair<emotime::Emotion, CvBoost *> > & detectors);
EmoDetector<CvBoost> boost_EmoDetector_create( map<string, pair<Emotion, CvBoost *> > & detectors, 
                                               Size size, double nwidths, double nlambdas, double nthetas, 
                                               FaceDetector * facedetector);

#endif

