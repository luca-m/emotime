/**
 *
 * @file    svm_emo_detector_cli.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/27/2013 12:18:32 PM
 * @brief
 *
 * @details
 *
 */

#include "svm_emo_detector.h"
#include "matrix_io.h"
#include "gaborbank.h"
#include "preprocessor.hpp"
#include "string_utils.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>

using namespace cv;
using namespace emotime;

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::pair;

void help()
{
	cout << "Usage:" << endl;
	cout << "   svm_emo_detector_cli <FaceDetecXML> <width> <height> <nwidths> <nlambdas> <nthetas> <svmXML> {<svmXML>}" << endl;
	cout << "Parameters:" << endl;
	cout << "   <image>       - The input image" << endl;
	cout << "   <faceDetectConf>   - OpenCV cascade classifier configuration file (Haar or LBP) for face detection" << endl;
	cout << "   <width>       - Width of the image, the input image will be scaled" << endl;
	cout << "   <height>      - Height of the image, the input image will be scaled" << endl;
	cout << "   <nwidths>     - " << endl;
	cout << "   <nlambdas>    - " << endl;
	cout << "   <nthetas>     - " << endl;
	cout << "   <svmXML>    - The trained svm for detecting an expression " << endl;
	cout << "                   Name format: EMOTION_* where EMOTION is one of (neutral, contempt, disgust, fear, sadness, surprise)" << endl;
	cout << endl;
}
void banner()
{
	cout << "SVMEmoDetector Utility:" << endl;
	cout << "     Detect emotions using trained svm" << endl;
}

int main(int argc, const char *argv[])
{
  if (argc < 8) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
	}
	string infile; // = string(argv[1]);
	string config = string(argv[1]);
	string config_e = string(argv[2]);
  cv::Size size(0,0);
  int nwidths, nlambdas, nthetas;
  size.width = abs(atoi(argv[3]));
	size.height = abs(atoi(argv[4]));
  nwidths = abs(atoi(argv[5]));
  nlambdas= abs(atoi(argv[6]));
  nthetas = abs(atoi(argv[7]));
  vector<string> classifier_paths;
  vector<CvSVM*> classifiers;

  if (argc >= 9) {
    // Read boost XML paths
    CvSVM* cvsvm;
    for (int i = 8; i < argc; i++) {
      string clpath(argv[i]);
      cvsvm = new CvSVM();
      cvsvm->load(argv[i]);
      if(!cvsvm->get_var_count()) {
        cerr << "ERR: Could not read the classifier '" << clpath << "' (skip)" << endl;
        continue;
      }
      classifier_paths.push_back(string(argv[i]));
      classifiers.push_back(cvsvm);
    }
  } else {
    cerr << "ERR: you must specify some svm" << endl;
    return -2;
  }

	try {

    FacePreProcessor preprocessor = FacePreProcessor(config, config_e,
        size.width, size.height, nwidths, nlambdas, nthetas);

    SVMEmoDetector emodetector = SVMEmoDetector(classifier_paths, classifiers);

    cout << "Insert the image file path: " << endl;
    while(std::getline(std::cin, infile)) {
      try {
        cout << "Processing '" << infile << "'" << endl;
		    Mat img = matrix_io_load(infile);
        Mat features;
        bool canPreprocess = preprocessor.preprocess(img, features);
        if (!canPreprocess) {
          cerr << "ERR: Cannot preprocess this image '" << infile << "'" << endl;
          continue;
        }
        pair<Emotion,float> prediction = emodetector.predictVotingOneVsAllExt(features);
        cout << "Emotion predicted: " << emotionStrings(prediction.first) << " with score " << prediction.second << endl;
        cout << "Insert the image file path: " << endl;
      } catch (int ee) {
        cerr << "ERR: Something wrong with '" << infile << "'" << endl;
      }
    }
	} catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}

  for(vector<CvSVM*>::iterator it = classifiers.begin(); it !=
      classifiers.end(); ++it) {
    delete *it;
  }

  return 0;
}
