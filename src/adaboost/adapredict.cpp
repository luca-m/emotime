/**
*
*/
#include "adapredict.h"

using namespace std;
using namespace cv;
using namespace emotime;

float adapredict_predictNoPreprocess( CvBoost & boost, cv::Mat & img ){
  vector<GaborKernel*> bank;
  Size s(0,0);
  gaborbank_getGaborBank(bank);
  return adapredict_predict( boost, bank, img, s, NULL );
}

float adapredict_predictPreprocess( CvBoost & boost, cv::Mat & img, cv::Size & scaleSize, const char * faceDetectConf ){
  vector<emotime::GaborKernel*> bank;
  gaborbank_getGaborBank(bank);
  emotime::FaceDetector detector(faceDetectConf);
  return adapredict_predict(boost, bank, img, scaleSize, &detector);
}

cv::Mat adapredict_imageToFeatVec(cv::Mat & src){
  #ifdef DEBUG
  cout<<"DEBUG: converting matrix to vector"<< endl;
  #endif
  //unsigned int i,j;
  //Mat * feat_v = new Mat( 1, src.cols*src.rows , CV_32FC1 );
  //// NOTE: scan first X then Y (horizontal, vertical)
  //for (i=0; i < src.rows; i++ ){
  //  for (j=0; j < src.cols; j++ ){
  //    feat_v->at<float>(i+j, 0 ) = src.at<float>(i,j);
  //  }
  //}
  return src.reshape(1 /*chan*/, 1 /*rows*/);
}

float adapredict_predict( CvBoost & boost, std::vector<emotime::GaborKernel*> & bank, cv::Mat & img, cv::Size & scaleSize, emotime::FaceDetector * facedetector ){
  Size suggSize;
  Mat image;
  Mat scaled;
  float prediction;

  if (facedetector!=NULL) {
    #ifdef DEBUG
    cout<<"DEBUG: exrtacting face from image"<<endl;
    #endif
    // Face cropping
    facecrop_cropFace( *facedetector, img, image, true );
  } else {
    image=img;
  }
  if ( scaleSize.height!=0 && scaleSize.width!=0 ) {
    #ifdef DEBUG
    cout<<"DEBUG: resizing image to w="<<scaleSize.width<<",h="<<scaleSize.height<<endl;
    #endif
    resize(image, scaled, scaleSize, 0, 0, CV_INTER_AREA);
  } else {
    scaled=image;
  }
  Mat dest;
  if (bank.size()>0) {
    dest=gaborbank_filterImage(scaled, bank);
  } else {
    dest=scaled;
  }
  
  Mat feat = adapredict_imageToFeatVec(dest);
  #ifdef DEBUG
  cout<<"DEBUG: predicting";
  #endif
  prediction=boost.predict(feat, Mat(), Range::all(), false, false);
  #ifdef DEBUG
  cout<<" ("<<prediction<<")"<<endl;
  #endif
  feat.release();
  return prediction;
}
