/**
 *
 */
#include <iostream>
#include <sstream>

#include "adapredict.h"

using namespace std;
using namespace cv;

void help() {
	cout << "Usage:" << endl;
	cout << "   adapredict_cli <adaConfig> <image> [<width> <height> <faceDetectConf>]" << endl;
	cout << "Parameters:" << endl;
	cout << "   <adaConfig>   - OpenCV XML configuration file containig " << endl;
	cout << "                   the trained boosted decision trees." << endl;
	cout << "   <image>       - The input image" << endl;
	cout << "   <width>       - Width of the image, the input image will be scaled" << endl;
	cout << "   <height>      - Height of the image, the input image will be scaled" << endl;
	cout << "   <faceDetectConf>   - OpenCV cascade classifier configuration file (Haar or LBP) for face detection" << endl;
	cout << endl;
}
void banner() {
	cout << "AdaPredict Utility:" << endl;
	cout << "     Predict the an AdaBoost classifier " << endl;
}

int main( int argc, const char *argv[] ) {
	bool preprocess=false;
  if (argc < 2) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
	} else {
    if (argc == 5){
     preprocess=true; 
    }
  }
	const char *config = argv[1];
	const char *infile = argv[2];
  cv::Size s(0,0);
  const char * detectorConf;
  float prediction;
  if (preprocess){
  	s.width = abs(atoi(argv[3]));
	  s.height = abs(atoi(argv[4]));
    detectorConf = argv[5];
  }

	try {		
		Mat img = imread( infile, CV_LOAD_IMAGE_GRAYSCALE );
		CvBoost boost;
		boost.load(config);

    if( !boost.get_weak_predictors() ) {
      cerr << "ERR: Could not read the classifier " << config << endl;
      return -1;
    }
    if (preprocess){
		  prediction = adapredict_predictPreprocess( boost, img, s, detectorConf );
    }
    else{
		  prediction = adapredict_predictNoPreprocess( boost, img );
    }
    cout << "Precicted value=" << prediction << endl; 
	} catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}
  return 0;
}
