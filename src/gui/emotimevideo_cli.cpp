/**
 *
 * @file    emotimegui_cli.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/31/2013 11:05:55 AM
 * @brief   Emotime GUI command line interface
 *
 * @details
 *
 */

#include "capture.hpp"
#include "debuggui.hpp"
#include "boost_emo_detector.h"
#include "svm_emo_detector.h"
#include "matrix_io.h"
#include "emotimegui.hpp"
#include "facedetector.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>

using namespace emotime;

using std::cout;
using std::cerr;
using std::endl;
using std::string;

/// Width of the faces used during training
const int kWidth = 32;
/// Height of the faces used during training
const int kHeight = 32;
/// N-Widths used during training
const double kNWidths = 1;
/// N-Lambdas used during training
const double kNLambdas = 5;
/// N-Thetas used during training
const double kNThetas = 8;

/**
 *  @brief          Prints the help
 *
 */
void help();

/**
 *  @brief          Prints the program banner
 *
 */
void banner();

void help()
{
	cout << "Usage:" << endl;
	cout << "   emotimegui_cli <faceDetecXML> <eyeDetectXML> [<mode>] <classifier>{<classifier>}" << endl;
	cout << "Parameters:" << endl;
	cout << "   <faceDetectXML>    - OpenCV cascade classifier configuration file (Haar or LBP) for face detection" << endl;
	cout << "   <eyeDetectXML>     - OpenCV cascade classifier configuration file (Haar or LBP) for eye detection" << endl;
	cout << "   <width>            - Width of the image, the input image will be scaled"<<endl;
	cout << "   <height>           - Height of the image, the input image will be scaled"<<endl;
	cout << "   <nwidths>          - "<<endl;
	cout << "   <nlambdas>         - "<<endl;
	cout << "   <nthetas>          - "<<endl;
	cout << "   <mode>             - ada or svm (default ada)" << endl;
	cout << "   <classifier>       - classificators list" << endl;
	cout << endl;
}

void banner()
{
	cout << "EmotimeGui Utility:" << endl;
	cout << "     GUI for emotime" << endl;
}

int main(int argc, const char* argv[])
{
  if (argc < 5) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
	}

  // Intializing the face detector
  string infile;
	string faceDetConfig = string(argv[1]);
	string eyeDetConfig = string(argv[2]);
  int width = std::atoi(argv[3]);
  int height = std::atoi(argv[4]);
  int nwidths = std::atoi(argv[5]);
  int nlambdas = std::atoi(argv[6]);
  int nthetas = std::atoi(argv[7]);

  // Setting the mode
  int i;
	string mode;
  mode = string(argv[8]);

	if (mode != "svm" && mode != "ada") {
    mode = "ada";
    i = 8;
  } else {
    i = 9;
  }

  // Setting the classifiers
  vector<string> cl_paths;
  vector<CvSVM*> classifiers_svm;
  vector<CvBoost*> classifiers_ada;

  CvSVM* cvsvm;
  CvBoost* cvboost;

  for(; i < argc; i++) {
    string clpath(argv[i]);
    if(mode == "svm") {
      // SVM loading
      cvsvm = new CvSVM();
      cvsvm->load(argv[i]);
      if(!cvsvm->get_var_count()) {
        cerr << "ERR: Could not read the classifier '" << clpath << "' (skip)" << endl;
        continue;
      }
      classifiers_svm.push_back(cvsvm);
    } else {
      // ADA loading
      cvboost = new CvBoost();
      cvboost->load(argv[i]);
      if(!cvboost->get_weak_predictors()) {
        cerr << "ERR: Could not read the classifier '" << clpath << "' (skip)" << endl;
        continue;
      }
      classifiers_ada.push_back(cvboost);
    }
    cl_paths.push_back(string(argv[i]));
  }

  cout<<"Insert the video file path: "<<endl; 
  std::getline(std::cin, infile);
  cout<<"Loading '"<<infile<<"'"<<endl;
  Video capture=Video(infile, true/*grayscale*/);
  // Creating and starting the EmotimeGUI
  int fps = 30;
  try{
    FacePreProcessor preprocessor(faceDetConfig, eyeDetConfig, width, height, nwidths, nlambdas, nthetas);
	  if(mode == "svm") {
	    SVMEmoDetector svm_det(cl_paths, classifiers_svm);
      DebugGuiSVM guiS=DebugGuiSVM(&capture, &preprocessor, &svm_det, 30);
      guiS.run();
    } else {
	    BoostEmoDetector boost_det(cl_paths, classifiers_ada);
      DebugGuiBoost guiB=DebugGuiBoost(&capture, &preprocessor, &boost_det, 30);
      guiB.run();
    }
	} catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}

  if(mode == "svm") {
    for(vector<CvSVM*>::iterator it = classifiers_svm.begin(); it !=
        classifiers_svm.end(); ++it) {
      delete *it;
    }
  } else {
    for(vector<CvBoost*>::iterator it = classifiers_ada.begin(); it !=
        classifiers_ada.end(); ++it) {
      delete *it;
    }
  }

  return 0;
}
