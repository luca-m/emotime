/**
 *
 * @file    BoostEmoDetector.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/27/2013 12:12:58 PM
 * @brief
 *
 * @details
 *
 */

#include "BoostEmoDetector.h"
#include "AdaBoostClassifier.h"

namespace emotime {

  BoostEmoDetector::BoostEmoDetector(int boost_type, double trim_weight, double max_depth) : EmoDetector() {
    this->boost_type = boost_type;
    this->trim_weight = trim_weight;
    this->max_depth = max_depth;
  }

  BoostEmoDetector::BoostEmoDetector(int boost_type, double trim_weight, double max_depth,
      std::map<std::string, std::pair<vector<Emotion>, Classifier*> >
      detmap_ext) : EmoDetector(detmap_ext) {
    this->boost_type = boost_type;
    this->trim_weight = trim_weight;
    this->max_depth = max_depth;
  }


  Classifier* BoostEmoDetector::createClassifier() {
    return new AdaBoostClassifier(this->boost_type, this->trim_weight, this->max_depth);
  }

  std::pair<Emotion, float> BoostEmoDetector::predict(cv::Mat& frame) {
    return EmoDetector::predictVotingOneVsAllExt(frame);
  }
}
