/**
*
*/
#include "adapredict.h"

using namespace std;
using namespace cv;
using namespace facecrop;

float adapredict_predictNoPreprocess( CvBoost & boost, cv::Mat & img ){
	vector<struct GaborKern*> bank;
  Size s(0,0);
	gaborbank_getGaborBank(bank);
	return adapredict_predict( boost, bank, img, s, NULL );
}

float adapredict_predictPreprocess( CvBoost & boost, cv::Mat & img, cv::Size & scaleSize, const char * faceDetectConf ){
	vector<struct GaborKern*> bank;
	gaborbank_getGaborBank(bank);
	return adapredict_predict( boost, bank, img, scaleSize, faceDetectConf );
}

cv::Mat adapredict_imageToFeatVec(cv::Mat & src){
  unsigned int i,j;
  #ifdef DEBUG
  cout<<"DEBUG: converting matrix to vector"<< endl;
  #endif
	Mat * feat_v = new Mat( src.size() , CV_32FC1 );
  // NOTE: scan first X then Y (horizontal, vertical)
	for (i=0; i < src.rows; i++ ){
		for (j=0; j < src.cols; j++ ){
			feat_v->at<float>(i+j, 0 ) = src.at<float>(i,j);
		}
	}
  return *feat_v;
}

float adapredict_predict( CvBoost & boost, std::vector<struct GaborKern*> & bank, cv::Mat & img, cv::Size & scaleSize, const char * faceDetectConf ){
	Size suggSize;
  Mat image;
	Mat scaled;
  float prediction;

	if (faceDetectConf!=NULL) {
    #ifdef DEBUG
    cout<<"DEBUG: crop face from image"<<endl;
    #endif
		// Face cropping
		FaceDetector detector(faceDetectConf);
		facecrop_cropFace( detector, img, image, true );
    detector.~FaceDetector();
	} else {
		image=img;
	}
	if ( scaleSize.height!=0 && scaleSize.width!=0 ) {
    #ifdef DEBUG
    cout<<"DEBUG: resize image"<<endl;
    #endif
		resize(image, scaled, scaleSize, 0, 0, CV_INTER_AREA);
	} else {
		scaled=image;
	}
	Mat dest = gaborbank_filterImage(scaled, bank);
	Mat feat = adapredict_imageToFeatVec(dest);
  #ifdef DEBUG
  cout<<"DEBUG: predicting"<< endl;
  #endif
  prediction = boost.predict( feat, Mat(), Range::all(), false, false);
  feat.release();
	return prediction;
}
