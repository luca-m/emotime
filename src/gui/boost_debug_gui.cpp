#include "boost_emo_detector.h"
#include "matrix_io.h"
#include "gaborbank.h"
#include "facecrop.h"
#include "preprocessor.hpp"
#include "debuggui.hpp"
#include "capture.hpp"

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
	cout<<"   boost_emo_detector_cli <FaceDetecXML> <width> <height> <nwidths> <nlambdas> <nthetas> [<boostXML> ..] "<<endl;
	cout<<"Parameters:"<<endl;
	cout<<"   <faceDetectConf>   - OpenCV cascade classifier configuration file (Haar or LBP) for face detection"<<endl;
	cout<<"   <width>       - Width of the image, the input image will be scaled"<<endl;
	cout<<"   <height>      - Height of the image, the input image will be scaled"<<endl;
	cout<<"   <nwidths>     - "<<endl;
	cout<<"   <nlambdas>    - "<<endl;
	cout<<"   <nthetas>     - "<<endl;
	cout<<"   <boostXML>    - The trained boosted tree for detecting an expression "<<endl;
	cout<<"                   Name format: EMOTION_* where EMOTION is one of (neutral, contempt, disgust, fear, sadness, surprise)"<<endl;
	cout<<endl;
}
void banner() {
	cout<<"BoostDebugGui Utility:"<<endl;
	cout<<"     Read video in stdin"<<endl;
}

int main( int argc, const char *argv[] ) {
  if (argc < 7) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	} 
	string infile; //= string(argv[1]);
	const char *config = argv[1];
  cv::Size size(0,0);
  int nwidths, nlambdas, nthetas;
  size.width = abs(atoi(argv[2]));
	size.height = abs(atoi(argv[3]));
  nwidths = abs(atoi(argv[4]));
  nlambdas= abs(atoi(argv[5]));
  nthetas = abs(atoi(argv[6]));
  vector<string> classifierPaths; 
  map<string, pair<Emotion, CvBoost *> > classifiers; 
  
  if (argc>=8){
    // Read boost XML paths
    for (int i=7; i<argc;i++){
      classifierPaths.push_back(string(argv[i]));
    }  
  } else {
    cerr<<"ERR: you must specify some boosted trees"<<endl;
    return -2; 
  }

	try {
    // load classifiers and try to detect the emotion they have been trained to detect
    CvBoost * tree;
    for (size_t i=0; i<classifierPaths.size();i++){
      string clpath= classifierPaths.at(i);
      tree = new CvBoost();
      #ifdef DEBUG
      cerr<<"DEBUG: Loading boosted tree "<<clpath;
      #endif 
      tree->load(clpath.c_str());
      if(!tree->get_weak_predictors()) {
        cerr<<"ERR: Could not read the classifier '"<<clpath<<"' (skip)"<<endl;
        continue;
      }
      
      string fname = matrix_io_fileBaseName(clpath);
      Emotion emo=UNKNOWN;

      if (fname.find(emotionStrings(NEUTRAL))==0){
        emo=NEUTRAL;
      } else if (fname.find(emotionStrings(ANGER))==0){
        emo=ANGER;
      } else if (fname.find(emotionStrings(CONTEMPT))==0){
        emo=CONTEMPT;
      } else if (fname.find(emotionStrings(DISGUST))==0){
        emo=DISGUST;
      } else if (fname.find(emotionStrings(FEAR))==0){
        emo=FEAR;
      } else if (fname.find(emotionStrings(HAPPY))==0){
        emo=HAPPY;
      } else if (fname.find(emotionStrings(SADNESS))==0){
        emo=SADNESS;
      } else if (fname.find(emotionStrings(SURPRISE))==0){
        emo=SURPRISE;
      }
      #ifdef DEBUG
      cerr<<" ("<<emotionStrings(emo)<<")"<<endl;
      #endif 
      pair<Emotion,CvBoost *> value = make_pair(emo, tree);
      pair<string, pair<Emotion,CvBoost*> > entry(emotionStrings(emo), value);
      classifiers.insert(entry);
    }

    FacePreProcessor preprocessor=FacePreProcessor(string(config), size.width, size.height, nwidths, nlambdas, nthetas);
    BoostEmoDetector emodetector=BoostEmoDetector(classifiers);
   
    cout<<"Insert the image file path: "<<endl; 
    while(std::getline(std::cin, infile)){
      try {
          Video capture=Video(infile, true/*grayscale*/);
          DebugGuiBoost gui=DebugGuiBoost(&capture, &preprocessor, &emodetector, 0);
          gui.run();
        } catch (int ee){
        cerr<<"ERR: Something wrong with '"<<infile<<"'"<<endl;
      }
    }
	} catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}
  return 0;
}
