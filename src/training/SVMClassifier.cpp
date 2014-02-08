/**
 *
 * @file    SVMClassifier.cpp
 *
 * @brief   Implementation of SVMClassifier
 *
 */

#include "SVMClassifier.h"

using std::vector;
using std::pair;
using std::string;
using std::cerr;
using std::endl;

namespace emotime {

  SVMClassifier::SVMClassifier(double C_factor, int max_iteration, double
      error_margin) {

    // Set up SVM's parameters
    this->params.svm_type    = CvSVM::C_SVC;
    this->params.kernel_type = CvSVM::LINEAR;
    this->params.C           = C_factor;
    this->params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, max_iteration,
        error_margin);
  }

  SVMClassifier::~SVMClassifier() {

  }

  bool SVMClassifier::doTraining(cv::Mat train_data, cv::Mat train_labels) {
    return this->svm.train(train_data, train_labels, cv::Mat(), cv::Mat(), this->params);
  }

  bool SVMClassifier::save(std::string outfile) {
    this->svm.save(outfile.c_str());
    return true;
  }

  bool SVMClassifier::load(std::string infile) {
    this->svm.load(infile.c_str());
    return svm.get_var_count() > 0;
  }

  float SVMClassifier::predict(cv::Mat& features) {
    return svm.predict(features);
  }

}
