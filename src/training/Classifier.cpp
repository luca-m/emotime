/**
 *
 * @file    Classifier.cpp
 *
 * @brief   The Classifier implementation
 *
 * @details
 *
 */

#include "Classifier.h"
#include "matrix_io.h"

#include <fstream>
#include <iostream>

using std::ifstream;
using std::cerr;
using std::endl;

using std::string;
using std::vector;
using std::pair;

using cv::Mat;

namespace emotime {

  Classifier::Classifier() {

  }

  Classifier::~Classifier() {

  }

  bool Classifier::train(std::string csv_file) {
    vector<pair<string, int> > files_classes;
    if (!this->loadTrainData(csv_file, files_classes)) {
      return false;
    }
    if (files_classes.size() == 0){
      cerr << "ERR: empty samples!" << endl;
      return false;
    }

    Mat sample = matrix_io_load(files_classes.at(0).first);
    int nfeatures = sample.rows * sample.cols;
    Mat train_data = Mat(0, nfeatures, CV_32FC1);
    Mat train_labels = Mat(0, 1, CV_32FC1);

    for(size_t i = 0; i < files_classes.size(); i++) {
      string fpath = files_classes.at(i).first;

      sample = matrix_io_load(fpath);
      Mat sample_float;

      if (sample.type() != CV_32FC1) {
        sample.convertTo(sample_float, CV_32FC1);
      } else {
        sample_float=sample;
      }
      train_data.push_back(sample_float.reshape(1, 1)); // reshape(int channels, int rows)
      train_labels.push_back((float)files_classes.at(i).second);
    }

    return this->doTraining(train_data, train_labels);
  }


  bool Classifier::loadTrainData(string csv_file,
      std::vector<std::pair<std::string, int> >& sample_files_classes) {
    try {
      ifstream csvf(csv_file.c_str());
      string sampleF;
      string sampleC;
      int sclass;
      while (getline(csvf, sampleC, ',')) {
        getline(csvf, sampleF, '\n');
        if (sampleC == "N") {
          sclass = 0;
        } else {
          sclass = 1;
        }
        sample_files_classes.push_back(std::make_pair(sampleF, sclass));
      }
      return true;
    } catch (int e) {
      cerr << "ERR: Something wrong during csv loading #" << e << endl;
      return false;
    }
  }
}
