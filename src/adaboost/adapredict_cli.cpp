/**
 *
 */
#include <iostream>
#include <sstream>

#include "adapredict.h"
#include "matrix_io.h"

using namespace std;
using namespace cv;

void help() {
	cout<<"Usage:"<<endl;
	cout<<"   adapredict_cli <adaConfig> <image> [<width> <height> <nwidths> <nlambdas> <nthetas> <faceDetectConf> [<eyeDetectConf>]]"<<endl;
	cout<<"Parameters:"<<endl;
	cout<<"   <adaConfig>   - OpenCV XML configuration file containig "<<endl;
	cout<<"                   the trained boosted decision trees."<<endl;
	cout<<"   <image>       - The input image"<<endl;
	cout<<"   <width>       - Width of the image, the input image will be scaled"<<endl;
	cout<<"   <height>      - Height of the image, the input image will be scaled"<<endl;
	cout<<"   <nwidths>     - "<<endl;
	cout<<"   <nlambdas>    - "<<endl;
	cout<<"   <nthetas>     - "<<endl;
	cout<<"   <faceDetectConf>   - OpenCV cascade classifier configuration file (Haar or LBP) for face detection"<<endl;
	cout<<"   [<eyesDetectConf>]   - OpenCV cascade classifier configuration file (Haar or LBP) for face detection"<<endl;
	cout<<endl;
}
void banner() {
	cout<<"AdaPredict Utility:"<<endl;
	cout<<"     Predict the an AdaBoost classifier "<<endl;
}

int main( int argc, const char *argv[] ) {
	bool preprocess=false;
  if (argc < 2) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	}
	const char *config = argv[1];
	string infile = string(argv[2]);
  cv::Size s(0,0);
  string detectorConf;
  string *detectorConf_e = NULL;
  int nwidths, nlambdas, nthetas;
  float prediction;

  if (argc >= 9) {
    preprocess=true;
  	s.width = abs(atoi(argv[3]));
	  s.height = abs(atoi(argv[4]));
    nwidths = abs(atoi(argv[5]));
    nlambdas= abs(atoi(argv[6]));
    nthetas = abs(atoi(argv[7]));

    detectorConf = string(argv[8]);
    if (argc == 10) {
      detectorConf_e = new string(argv[9]);
    }
  }

	try {		
		Mat img = matrix_io_load(infile);
		CvBoost boost;
		boost.load(config);

    if( !boost.get_weak_predictors() ) {
      cerr<<"ERR: Could not read the classifier "<<config<<endl;
      return -1;
    }
    if (preprocess){
      vector<emotime::GaborKernel *> bank;
      gaborbank_getCustomGaborBank(bank, (double) nwidths, (double) nlambdas, (double) nthetas);
      emotime::FaceDetector *facedetector;
      if (detectorConf_e == NULL) {
        facedetector = new emotime::FaceDetector(detectorConf);
      } else {
        facedetector = new emotime::FaceDetector(detectorConf, *detectorConf_e);
      }

		  prediction = adapredict_predict(boost, bank, img, s, facedetector);
      delete facedetector;
    }
    else{
      // with default parameters
		  prediction = adapredict_predictNoPreprocess(boost, img);
    }
    cout<<"Precicted value="<<prediction<<endl; 
	} catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}
  return 0;
}
