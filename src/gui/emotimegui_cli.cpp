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
#include "BoostEmoDetector.h"
#include "SVMEmoDetector.h"
#include "matrix_io.h"
#include "emotimegui.hpp"
#include "FaceDetector.h"

#include "DetectionParameters.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>

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

void help() {
	cout << "Usage:" << endl;
	cout << "   emotimegui_cli <faceDetecXML> <eyeDetectXML> <width> <height> <nwidths> <nlambdas> <nthetas> [<mode>] <classifier>{<classifier>}" << endl;
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

void banner() {
	cout << "EmotimeGui Utility:" << endl;
	cout << "     GUI for emotime" << endl;
}

int main(int argc, const char* argv[]) {
  if (argc < 5) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
	}

  // Intializing the face detector
	string faceDetConfig(argv[1]);
	string eyeDetConfig(argv[2]);
  int width = std::atoi(argv[3]);
  int height = std::atoi(argv[4]);
  int nwidths = std::atoi(argv[5]);
  int nlambdas = std::atoi(argv[6]);
  int nthetas = std::atoi(argv[7]);
  FacePreProcessor facepreproc(faceDetConfig, eyeDetConfig, width, height, nwidths, nlambdas, nthetas);

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
  EmoDetector* emodetector;

  for(; i < argc; i++) {
    cl_paths.push_back(string(argv[i]));
  }

  if (mode == "svm") {
    emodetector = new SVMEmoDetector(kCfactor, kMaxIteration, kErrorMargin);
  } else {
    emodetector = new BoostEmoDetector(kBoostType, kTrimWeight, kMaxDepth);
  }
  emodetector->init(cl_paths);

  // Creating and starting the EmotimeGUI
  int fps = 30;
	try {
    EmotimeGUI gui(&facepreproc, emodetector, fps);
    gui.run();
	} catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}

  delete emodetector;
  return 0;
}
