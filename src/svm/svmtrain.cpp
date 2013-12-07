/**
 *
 * @file    svmtrain.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/07/2013 01:06:39 AM
 * @brief   
 *
 * @details 
 *
 */

#include "svmtrain.h"
#include "matrix_io.h"

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

using cv::Mat;

bool svmtrain_trainCvMLData(CvSVM& svm, const char * csvFilePath, int
    nFeatures, bool validate)
{
  CvMLData cvml;
#ifdef DEBUG
  cout << "DEBUG: loading training samples and labels from " << csvFilePath << endl;
#endif
  cerr << "ERROR: SVM doesn't support CvML traning" << endl;
  return false;
//  cvml.read_csv(csvFilePath);
//  cvml.set_response_idx(0);
//
//  // Set up SVM's parameters
//  CvSVMParams params;
//  params.svm_type    = CvSVM::C_SVC;
//  params.kernel_type = CvSVM::LINEAR;
//  params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
//
//#ifdef DEBUG
//  cout << "DEBUG: training svm (using CvMLData)" << endl;
//#endif
//  svm.train(trainingDataMat, labelsMat, Mat(), Mat(), params);
}

bool svmtrain_trainManual(CvSVM& svm, std::vector<std::string> &
    matrixFiles, std::vector<int> & classes)
{

  if (classes.size() != matrixFiles.size() && matrixFiles.size()==0){
    cerr << "ERR: number of class not equals to matrix numbers or do not contain any element (size=" << matrixFiles.size() << ")" << endl; 
    return false;
  }

  // Load the first file in order to detect the number of samples attribute
  Mat sample = matrix_io_load(matrixFiles.at(0));
  int nfeatures = sample.rows*sample.cols;
  Mat train_data = Mat(0, nfeatures, CV_32FC1); // rows are 0 because of Mat.push_back usage
  Mat train_labels = Mat(0, 1, CV_32FC1);

  for(size_t i = 0; i < matrixFiles.size(); i++) {
    string fpath = matrixFiles.at(i);
    sample = matrix_io_load(fpath);
#ifdef DEBUG
    cout << "DEBUG: matrix '" << fpath << "' rows=" << sample.rows << ",cols=" << sample.cols << ",type=" << sample.type() << ",CV_32FC1=" << CV_32FC1 << endl;
#endif
    Mat sample_float;
    if (sample.type()!=CV_32FC1) {
      sample.convertTo(sample_float, CV_32FC1);
    } else {
      sample_float=sample;
    }
    train_data.push_back(sample_float.reshape(1, 1)); // reshape(int channels, int rows)
    train_labels.push_back(classes.at(i)); 
  }

#ifdef DEBUG
  cout << "DEBUG: training svm manually (nsamples=" << train_data.rows << ", nfeatures=" << train_data.cols << ")" << endl;
#endif

  // Set up SVM's parameters
  CvSVMParams params;
  params.svm_type    = CvSVM::C_SVC;
  params.kernel_type = CvSVM::LINEAR;
  params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

  svm.train(train_data, train_labels, Mat(), Mat(), params);
  return true;
}
