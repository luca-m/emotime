/**
 *
 * @file    AdaBoostClassifier.cpp
 *
 * @date    01/09/2014 11:15:57 PM
 * @brief   Implementation for AdaBoostClassifier
 *
 * @details
 *
 */

#include "AdaBoostClassifier.h"


using std::vector;
using std::pair;
using std::string;
using std::cerr;
using std::endl;

namespace emotime {

  AdaBoostClassifier::AdaBoostClassifier(int boost_type, double trim_weight, int max_depth) {
    this->params.boost_type = boost_type;
    this->params.weak_count = 0;
    this->params.weight_trim_rate = trim_weight;
    this->params.max_depth = max_depth;
    this->params.use_surrogates = false;
    this->params.priors = NULL;
  }

  AdaBoostClassifier::~AdaBoostClassifier() {

  }

  bool AdaBoostClassifier::save(std::string outfile) {
    this->boost.save(outfile.c_str());
    return true;
  }

  bool AdaBoostClassifier::load(std::string infile) {
    this->boost.load(infile.c_str());
    return this->boost.get_weak_predictors() != NULL;
  }

  float AdaBoostClassifier::predict(cv::Mat& features) {
    return this->boost.predict(features, cv::Mat(), cv::Range::all(), false, false);
  }

  bool AdaBoostClassifier::doTraining(cv::Mat train_data, cv::Mat train_labels) {
    this->params.weak_count = train_data.cols;
    return this->boost.train(train_data, CV_ROW_SAMPLE, train_labels,
        cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(), this->params, false);
  }
}
