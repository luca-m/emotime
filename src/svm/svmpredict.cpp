/**
 *
 * @file    svmpredict.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/24/2013 10:36:18 AM
 * @brief
 *
 * @details
 *
 */

#include "svmpredict.h"

using namespace cv;

using std::vector;

float svmpredict_predictNoPreprocess(CvSVM& svm, cv::Mat& img)
{
  vector<struct GaborKern*> bank;
  Size s(0, 0);
  gaborbank_getGaborBank(bank);
  return svmpredict_predict(svm, bank, img, s, NULL);
}

float svmpredict_predictPreprocess(CvSVM& svm, cv::Mat& img, cv::Size&
    scaleSize, const char* faceDetectConf)
{
  vector<struct GaborKern*> bank;
  gaborbank_getGaborBank(bank);
  emotime::FaceDetector detector(faceDetectConf);
  return svmpredict_predict(svm, bank, img, scaleSize, &detector);
}

cv::Mat svmpredict_imageToFeatVec(cv::Mat & src)
{
#ifdef DEBUG
  std::cout << "DEBUG: converting matrix to vector" << std::endl;
#endif
  return src.reshape(1 /*chan*/, 1 /*rows*/);
}

float svmpredict_predict(CvSVM& svm, std::vector<struct GaborKern*>& bank,
    cv::Mat& img, cv::Size & scaleSize, emotime::FaceDetector* facedetector)
{
  Size suggSize;
  Mat image;
  Mat scaled;
  float prediction;

  if (facedetector != NULL) {
#ifdef DEBUG
    std::cout << "DEBUG: exrtacting face from image" << std::endl;
#endif
    // Face cropping
    facecrop_cropFace(*facedetector, img, image, true);
  } else {
    image = img;
  }

  if ( scaleSize.height != 0 && scaleSize.width != 0 ) {
#ifdef DEBUG
    std::cout << "DEBUG: resizing image to w = " << scaleSize.width << ", h = "
      << scaleSize.height << std::endl;
#endif
    resize(image, scaled, scaleSize, 0, 0, CV_INTER_AREA);
  } else {
    scaled = image;
  }

  Mat dest;
  if (bank.size()>0) {
    dest = gaborbank_filterImage(scaled, bank);
  } else {
    dest = scaled;
  }

  Mat feat = svmpredict_imageToFeatVec(dest);
#ifdef DEBUG
  std::cout << "DEBUG: predicting";
#endif

  std::cout << feat.size[0] << " " << feat.size[1] << std::endl;

  prediction = svm.predict(feat);

#ifdef DEBUG
  std::cout << " (" << prediction << ")" << std::endl;
#endif

  feat.release();
  return prediction;
}
