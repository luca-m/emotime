/**
 *
 * @file    svmpredict_cli.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/24/2013 10:53:43 AM
 * @brief
 *
 * @details
 *
 */

#include <iostream>
#include <sstream>

#include "svmpredict.h"
#include "matrix_io.h"

using namespace cv;

using std::cout;
using std::cerr;
using std::endl;

void help(){
  cout << "Usage:" << endl;
  cout << "   svmpredict_cli <svmConfig> <image> [" << "<width> <height> " <<
    "<nwidths> <nlambdas> <nthetas> <faceDetectConf>]" << endl;

  cout << "Parameters:" << endl;
  cout << "   <svmConfig>   - OpenCV XML configuration file containig " << endl;
  cout << "                   the trained boosted decision trees." << endl;
  cout << "   <image>       - The input image" << endl;
  cout << "   <width>       - Width of the image, the input image will be scaled" << endl;
  cout << "   <height>      - Height of the image, the input image will be scaled" << endl;
  cout << "   <nwidths>     - " << endl;
  cout << "   <nlambdas>    - " << endl;
  cout << "   <nthetas>     - " << endl;
  cout << "   <faceDetectConf>   - OpenCV cascade classifier configuration " <<
    "file (Haar or LBP) for face detection" << endl;
  cout << endl;
}

void banner(){
  cout << "SVMPredict Utility:" << endl;
  cout << "     Predict the an SVM classifier " << endl;
}

int main( int argc, const char *argv[] ){
  bool preprocess = false;
  if (argc<2) {
    banner();
    help();
    cerr << "ERR: missing parameters" << endl;
    return -3;
  }
  const char *config = argv[1];
  string infile = string(argv[2]);
  cv::Size s(0, 0);
  const char * detectorConf;
  int nwidths, nlambdas, nthetas;
  float prediction;

  if (argc==9) {
    preprocess = true;
    s.width = abs(atoi(argv[3]));
    s.height = abs(atoi(argv[4]));
    nwidths = abs(atoi(argv[5]));
    nlambdas = abs(atoi(argv[6]));
    nthetas = abs(atoi(argv[7]));

    detectorConf = argv[8];
  }

  try {
    Mat img = matrix_io_load(infile);
    CvSVM svm;
    svm.load(config);

    if( svm.get_support_vector_count() == 0 ) {
      cerr << "ERR: Could not read the classifier " << config << endl;
      return -1;
    }
    if (preprocess){
      vector<emotime::GaborKernel *> bank;
      gaborbank_getCustomGaborBank(bank, (double) nwidths, (double) nlambdas, (double) nthetas);
      emotime::FaceDetector facedetector =  emotime::FaceDetector(detectorConf);
      prediction = svmpredict_predict(svm, bank, img, s, &facedetector);
    } else {
      // with default parameters
      prediction = svmpredict_predictNoPreprocess(svm, img);
    }
    cout << "Precicted value = " << prediction << endl;
  } catch (int e) {
    cerr << "ERR: Exception #" << e << endl;
    return -e;
  }
  return 0;
}
