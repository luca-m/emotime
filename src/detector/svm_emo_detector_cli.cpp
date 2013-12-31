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
  vector<string> classifierPaths;
  map<string, pair<vector<Emotion>, CvSVM*> > classifiers;

  if (argc>=9) {
    // Read boost XML paths
    for (int i=8; i < argc;i++) {
      classifierPaths.push_back(string(argv[i]));
    }
  } else {
    cerr << "ERR: you must specify some svm" << endl;
    return -2;
  }

	try {
    // load classifiers and try to detect the emotion they have been trained to detect
    CvSVM* trsvm;
    for (size_t i = 0; i < classifierPaths.size(); i++) {
      string clpath = classifierPaths.at(i);
      trsvm = new CvSVM();
      #ifdef DEBUG
      cerr << "DEBUG: Loading svm " << clpath;
      #endif
      trsvm->load(clpath.c_str());

      if(!trsvm->get_var_count()) {
        cerr << "ERR: Could not read the classifier '" << clpath << "' (skip)" << endl;
        continue;
      }

      string fname = matrix_io_fileBaseName(clpath);
      Emotion emo = UNKNOWN;

      vector<string> emotions_list = split_string(fname, "_");
      vector<Emotion> fin_emo_list;
      fin_emo_list.reserve(emotions_list.size());
      string label = "";

      #ifdef DEBUG
      cerr << " (";
      #endif
      for(vector<string>::iterator it = emotions_list.begin(); it !=
          emotions_list.end(); it++) {
        emo = UNKNOWN;
        if (*it == "vs") {
          break;
        } else if (*it == emotionStrings(NEUTRAL)) {
          emo = NEUTRAL;
        } else if (*it == emotionStrings(ANGER)) {
          emo = ANGER;
        } else if (*it == emotionStrings(CONTEMPT)) {
          emo = CONTEMPT;
        } else if (*it == emotionStrings(DISGUST)) {
          emo = DISGUST;
        } else if (*it == emotionStrings(FEAR)) {
          emo = FEAR;
        } else if (*it == emotionStrings(HAPPY)) {
          emo = HAPPY;
        } else if (*it == emotionStrings(SADNESS)) {
          emo = SADNESS;
        } else if (*it == emotionStrings(SURPRISE)) {
          emo = SURPRISE;
        }
        if(emo != UNKNOWN) {
          if(label.size() > 0) {
            label.append("_");
          }
          label.append(emotionStrings(emo));
          fin_emo_list.push_back(emo);
          #ifdef DEBUG
          cerr << emotionStrings(emo) << ", ";
          #endif
        }
      }

      #ifdef DEBUG
      cerr << ")" << endl;
      #endif

      pair<vector<Emotion>, CvSVM*> value(fin_emo_list, trsvm);
      pair<string, pair<vector<Emotion>, CvSVM*> > entry(label, value);
      classifiers.insert(entry);
    }

    FacePreProcessor preprocessor = FacePreProcessor(config, config_e, size.width, size.height, nwidths, nlambdas, nthetas);
    SVMEmoDetector emodetector = SVMEmoDetector(classifiers);

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
  return 0;
}
