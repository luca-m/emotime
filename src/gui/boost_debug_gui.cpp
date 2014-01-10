
#include "BoostEmoDetector.h"
#include "matrix_io.h"
#include "GaborBank.h"
#include "FacePreProcessor.h"
#include "debuggui.hpp"
#include "capture.hpp"

#include "DetectionParameters.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>

using namespace std;
using namespace cv;
using namespace emotime;

void help() {
  cout<<"Usage:"<<endl;
  cout<<"   boost_emo_detector_cli <faceDetectXML> <eyeDetectXML> <width> <height> <nwidths> <nlambdas> <nthetas> [<boostXML> ..] "<<endl;
  cout<<"Parameters:"<<endl;
  cout<<"   <faceDetectXML>   - OpenCV cascade classifier configuration file (Haar or LBP) for face detection"<<endl;
  cout<<"   <eyeDetectXML>    - OpenCV cascade classifier configuration file (Haar or LBP) for eye detection"<<endl;
  cout<<"   <width>           - Width of the image, the input image will be scaled"<<endl;
  cout<<"   <height>          - Height of the image, the input image will be scaled"<<endl;
  cout<<"   <nwidths>         - "<<endl;
  cout<<"   <nlambdas>        - "<<endl;
  cout<<"   <nthetas>         - "<<endl;
  cout<<"   <boostXML>        - The trained boosted tree for detecting an expression "<<endl;
  cout<<"                       Name format: EMOTION_* where EMOTION is one of (neutral, contempt, disgust, fear, sadness, surprise)"<<endl;
  cout<<endl;
}
void banner() {
  cout<<"BoostDebugGui Utility:"<<endl;
  cout<<"     Read video in stdin"<<endl;
}

int main( int argc, const char *argv[] ) {
  if (argc<8) {
    banner();
    help();
    cerr<<"ERR: missing parameters"<<endl;
    return -3;
  } 
  string infile; //= string(argv[1]);
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

  if (argc>=9){
    // Read boost XML paths
    for (int i=8; i<argc;i++){
      classifierPaths.push_back(string(argv[i]));
    }  
  } else {
    cerr<<"ERR: you must specify some boosted trees"<<endl;
    return -2; 
  }

  try {
    FacePreProcessor preprocessor = FacePreProcessor(config, config_e, size.width, size.height, nwidths, nlambdas, nthetas);

    BoostEmoDetector emodetector(kBoostType, kTrimWeight, kMaxDepth);
    emodetector.init(classifierPaths);

    cout<<"Insert the image file path: "<<endl; 
    while(std::getline(std::cin, infile)) {
      try {
        Video capture=Video(infile, true/*grayscale*/);
        ADebugGui gui = ADebugGui(&capture, &preprocessor, &emodetector, 30);
        gui.run();
      } catch (int ee) {
        cerr<<"ERR: Something wrong with '"<<infile<<"'"<<endl;
      }
    }
  } catch (int e) {
    cerr<<"ERR: Exception #"<<e<<endl;
    return -e;
  }
  return 0;
}
