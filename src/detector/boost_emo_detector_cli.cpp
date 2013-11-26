#include "boost_emo_detector.h"
#include "matrix_io.h"
#include "gaborbank.h"
#include "facecrop.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>

using namespace std;
using namespace cv;
using namespace emotime;

void help() {
	cout<<"Usage:"<<endl;
	cout<<"   boost_emo_detector_cli <image> <FaceDetecXML> <width> <height> <nwidths> <nlambdas> <nthetas> [<boostXML> ..] "<<endl;
	cout<<"Parameters:"<<endl;
	cout<<"   <image>       - The input image"<<endl;
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
	cout<<"BoostEmoDetector Utility:"<<endl;
	cout<<"     Detect emotions using boosted trees"<<endl;
}

int main( int argc, const char *argv[] ) {
	bool preprocess=false;
  if (argc < 2) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	} 
	string infile = string(argv[1]);
	const char *config = argv[2];
  cv::Size size(0,0);
  const char * detectorConf;
  int nwidths, nlambdas, nthetas;
  float prediction;
  size.width = abs(atoi(argv[3]));
	size.height = abs(atoi(argv[4]));
  nwidths = abs(atoi(argv[5]));
  nlambdas= abs(atoi(argv[6]));
  nthetas = abs(atoi(argv[7]));
  vector<string> classifierPaths; 
  map<string, pair<emotime::Emotions,CvBoost> > classifiers; 
  
  if (argc > 9){
    // Read boost XML paths
    for (int i=8; i<argc;i++){
      classifierPaths.push_back(string(argv[i]));
    }  
  } else {
    cerr<<"ERR: you must specify some boosted trees"<<endl;
    return -2; 
  }

	try {

    // load classifiers and try to detect the emotion they have been trained to detect
    for (size_t i=0; i<classifierPaths.size();i++){
      string clpath= classifierPaths.at(i);
      size_t found = string::npos;
      CvBoost tree = CvBoost();
      tree.load(clpath.c_str());
      if ( (found=clpath.find(string("neutral")))==0){
        classifiers.insert( pair<string, pair<Emotions,CvBoost> >(string("neutral"), make_pair(Neutral,tree)) );
      } else 
      if ( (found=clpath.find(string("anger")))==0){
        classifiers.insert( pair<string, pair<Emotions,CvBoost> >(string("anger"), make_pair(Anger,tree)) );
      } else  
      if ( (found=clpath.find(string("contempt")))==0){
        classifiers.insert( pair<string, pair<Emotions,CvBoost> >(string("contempt"), make_pair(Contempt,tree)) );
      } else  
      if ( (found=clpath.find(string("disgust")))==0){
        classifiers.insert( pair<string, pair<Emotions,CvBoost> >(string("distgust"), make_pair(Disgust,tree)) );
      } else  
      if ( (found=clpath.find(string("fear")))==0){
        classifiers.insert( pair<string, pair<Emotions,CvBoost> >(string("fear"), make_pair(Fear,tree)) );
      } else  
      if ( (found=clpath.find(string("happy")))==0){
        classifiers.insert( pair<string, pair<Emotions,CvBoost> >(string("happy"), make_pair(Happy,tree)) );
      } else  
      if ( (found=clpath.find(string("sadness")))==0){
        classifiers.insert( pair<string, pair<Emotions,CvBoost> >(string("sadness"), make_pair(Sadness,tree)) );
      } else  
      if ( (found=clpath.find(string("surprise")))==0){
        classifiers.insert( pair<string, pair<Emotions,CvBoost> >(string("surprise"), make_pair(Surprise,tree)) );
      } 
    }

		Mat img = matrix_io_load(infile);
    Mat cropped;
		Mat scaled;
    Mat features;
    // Extract the face
    #ifdef DEBUG
    cerr<<"DEBUG: extracting face"<<endl;
    #endif
    FaceDetector facedetector=FaceDetector(detectorConf);
    bool hasFace=facecrop_cropFace( facedetector, img, cropped, true);
    if (!hasFace){
      cerr<<"ERR: cannot detect any face in image "<<infile<<endl;
      return -4;
    } 
		resize(cropped, scaled, size, 0, 0, CV_INTER_AREA);
    // Calculate feature vector
    #ifdef DEBUG
    cerr<<"DEBUG: creating feature vector"<<endl;
    #endif
    vector<struct GaborKern *> bank;
    gaborbank_getCustomGaborBank(bank, (double) nwidths, (double) nlambdas, (double) nthetas);
		features = gaborbank_filterImage(scaled, bank);
    features.reshape(1, 1);
    #ifdef DEBUG
    cerr<<"DEBUG: creating emo detector"<<endl;
    #endif
    EmoDetector<CvBoost> emodetector=boost_EmoDetector_create(classifiers);
    pair<Emotions,float> prediction= emodetector.predictMayorityOneVsAll(features);

    cout<<"Emotion predicted: "<<prediction.first<<" with score "<<prediction.second<<endl;

	} catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}
  return 0;
}
