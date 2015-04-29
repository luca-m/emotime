/**
 *
 * @file    emotimegui_cli.cpp
 * @brief   Emotime GUI command line interface
 *
 * @details
 *
 */

#include "WebcamCapture.h"
#include "BoostEmoDetector.h"
#include "SVMEmoDetector.h"
#include "matrix_io.h"
#include "EmotimeGui.h"
#include "FaceDetector.h"

#include "TrainingParameters.h"

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
const int kWidth = 48;
/// Height of the faces used during training
const int kHeight = 48;
/// N-Widths used during training
const double kNWidths = 2;
/// N-Lambdas used during training
const double kNLambdas = 5;
/// N-Thetas used during training
const double kNThetas = 4;

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
	cout << "   emotimegui_cli [-s] [-oJSONOutputFile] <faceDetecXML> <eyeDetectXML> <width> <height> <nwidths> <nlambdas> <nthetas> [<mode>] <classifier>{<classifier>}" << endl;
	cout << "Parameters:" << endl;
  cout << "   -s <optional>      - This will capture a single image from webcam and print the analysis to console." << endl;
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
  if (argc < 5) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
	}

  int argOffset = 1;

  if(!strcmp(argv[1], "-s")){
    argOffset++;
  }

  // Intializing the face detector
	string faceDetConfig(argv[argOffset]);
	string eyeDetConfig(argv[argOffset+1]);
  int width = std::atoi(argv[argOffset + 2]);
  int height = std::atoi(argv[argOffset + 3]);
  int nwidths = std::atoi(argv[argOffset + 4]);
  int nlambdas = std::atoi(argv[argOffset + 5]);
  int nthetas = std::atoi(argv[argOffset + 6]);

  cout  << "Run Configuration: " << endl 
        << "  Width: " << width << endl
        << "  Height: " << height << endl
        << "  nWidths: " << nwidths << endl
        << "  nLambdas: " << nlambdas << endl
        << "  nThetas: " << nthetas << endl;

  FacePreProcessor facepreproc(faceDetConfig, eyeDetConfig, width, height, nwidths, nlambdas, nthetas);

  // Setting the mode
  int i;
	string mode;
  mode = string(argv[argOffset + 7]);

	if (mode != "svm" && mode != "ada") {
    mode = "ada";
    i = argOffset + 7;
  } else {
    i = argOffset + 8;
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
  int fps = 120;
	try {
    EmotimeGui gui(&facepreproc, emodetector, fps);
    if(!strcmp(argv[1], "-s")){
      gui.runOnce();  
    }
    else{
      gui.run();
    }
	} catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}

  delete emodetector;
  return 0;
}
