/**
 *
 * @file    train_cli.cpp
 *
 * @date    12/04/2013 10:33:42 AM
 * @brief   CLI interface for training a Classifier
 *
 * @details 
 *
 */

#include "Classifier.h"
#include "SVMClassifier.h"
#include "AdaBoostClassifier.h"

#include "TrainingParameters.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace cv;

using std::cout;
using std::cerr;
using std::endl;

/**
 *  @brief          Prints the CLI banner
 *
 */
void banner();

/**
 *  @brief          Prints the CLI help
 *
 */
void help();

void help() {
  cout << "Usage:" << endl;
  cout << "   train_cli (svm|ada) <trainFile> <outputFile>" << endl;
  cout << "Parameters:" << endl;
	cout << "   (svm|ada)     - Training method to use" << endl;
	cout << "   <trainFile>   - File containing the training data" << endl;
	cout << "                   (CSV format, first column contains class, features in others)" << endl;
	cout << "   <outputFile>  - Output file where to store classifier" << endl;
  cout << endl;
}

void banner() {
  cout << "Train Utility:" << endl;
	cout << "   Train an classifier." << endl;
	cout << "   Train a classifier using data specified in a CSV file." << endl;
	cout << "   NOTE: the CSV file may contain sample data directly (aka read samples from CSV mode)" << endl;
	cout << "         or also contains path of the file where to find samples (images or OpenCV yml)" << endl;
  cout << "" << endl;
}

/**
 *  @brief          Main
 *
 *  @param[in]      argc
 *  @param[in]     argv
 *
 *  @returns  1
 *
 */
int main(int argc, const char* argv[]) {

  if (argc < 4) {
    banner();
    help();
    cerr << "ERR: missing parameters" << endl;
    return 1;
  }
  string method(argv[1]);
  string infile(argv[2]);
  string outfile(argv[3]);

  emotime::Classifier* classifier;
	if (method == "svm") {
    classifier = new emotime::SVMClassifier(emotime::kCfactor,
        emotime::kMaxIteration, emotime::kErrorMargin);
  } else {
    classifier = new emotime::AdaBoostClassifier(emotime::kBoostType,
        emotime::kTrimWeight, emotime::kMaxDepth);
  }

  int ret = 0;
  if (!classifier->train(infile)) {
    cerr << "Error training!" << endl;
    ret = 1;
  } else if (!classifier->save(outfile)) {
    cerr << "Error saving!" << endl;
    ret = 1;
  }

  delete classifier;
  return ret;
}
