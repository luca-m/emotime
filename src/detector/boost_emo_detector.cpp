#include "boost_emo_detector.h"

using namespace emotime;

float BoostEmoDetector::predict(CvBoost* detector, cv::Mat& frame) {
  return detector->predict(frame);
}
pair<Emotion, float> BoostEmoDetector::predict(cv::Mat& frame) {
  return EmoDetector<CvBoost>::predictVotingOneVsAllExt(frame);
}
