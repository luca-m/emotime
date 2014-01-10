/**
 *
 * @file    SVMEmoDetector.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/27/2013 12:12:58 PM
 * @brief
 *
 * @details
 *
 */

#include "SVMEmoDetector.h"
#include "SVMClassifier.h"

namespace emotime {

  SVMEmoDetector::SVMEmoDetector(double C_factor, int max_iteration, double error_margin) : EmoDetector() {
    this->C_factor = C_factor;
    this->max_iteration = max_iteration;
    this->error_margin = error_margin;
  }

  SVMEmoDetector::SVMEmoDetector(double C_factor, int max_iteration, double error_margin,
      std::map<std::string, std::pair<vector<Emotion>, Classifier*> >
      detmap_ext) : EmoDetector(detmap_ext) {
    this->C_factor = C_factor;
    this->max_iteration = max_iteration;
    this->error_margin = error_margin;
  }


  Classifier* SVMEmoDetector::createClassifier() {
    return new SVMClassifier(this->C_factor, this->max_iteration, this->error_margin);
  }

  std::pair<Emotion, float> SVMEmoDetector::predict(cv::Mat& frame) {
    return EmoDetector::predictVotingOneVsAllExt(frame);
  }
}
