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
  if (argc < 2) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	} 
	string infile = string(argv[1]);
	const char *config = argv[2];
  cv::Size size(0,0);
  int nwidths, nlambdas, nthetas;
  size.width = abs(atoi(argv[3]));
	size.height = abs(atoi(argv[4]));
  nwidths = abs(atoi(argv[5]));
  nlambdas= abs(atoi(argv[6]));
  nthetas = abs(atoi(argv[7]));
  vector<string> classifierPaths; 
  map<string, pair<Emotion, CvBoost *> > classifiers; 
  
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

    // load classifiers and try to detect the emotion they have been trained to detect
    for (size_t i=0; i<classifierPaths.size();i++){
      string clpath= classifierPaths.at(i);
      CvBoost tree = *(new CvBoost());
      #ifdef DEBUG
      cerr<<"DEBUG: loading boosted tree "<<clpath;
      #endif 
      tree.load(clpath.c_str());
      
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
      pair<Emotion,CvBoost *> value = make_pair(emo, &tree);
      pair<string, pair<Emotion,CvBoost*> > entry(emotionStrings(emo), value);
      classifiers.insert(entry);
    }

		Mat img = matrix_io_load(infile);
    Mat cropped;
		Mat scaled;
    Mat features;
    
    //#ifdef DEBUG
    //cerr<<"DEBUG: extracting face"<<endl;
    //#endif
    FaceDetector facedetector=FaceDetector(config, true, true);
    bool hasFace=facecrop_cropFace(facedetector, img, cropped, true);
    if (!hasFace){
      cerr<<"ERR: cannot detect any face in image "<<infile<<endl;
      return -4;
    } 
		resize(cropped, scaled, size, 0, 0, CV_INTER_AREA);
    
    #ifdef DEBUG
    cerr<<"DEBUG: creating feature vector"<<endl;
    #endif
    vector<struct GaborKern *> bank;
    gaborbank_getCustomGaborBank(bank, (double) nwidths, (double) nlambdas, (double) nthetas);
		features = gaborbank_filterImage(scaled, bank);
    features.reshape(1, 1); // to feature vector
    
    #ifdef DEBUG
    cerr<<"DEBUG: creating emo detector"<<endl;
    #endif
    EmoDetector<CvBoost> emodetector=boost_EmoDetector_create_no_preprocess(classifiers);
    //EmoDetector<CvBoost> emodetector=boost_EmoDetector_create(classifiers, size, nwidths, nlambdas, nthetas, NULL );
    pair<Emotion,float> prediction=emodetector.predictMayorityOneVsAll(features);

    cout<<"Emotion predicted: "<<emotionStrings(prediction.first)<<" with score "<<prediction.second<<endl;

	} catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}
  return 0;
}
