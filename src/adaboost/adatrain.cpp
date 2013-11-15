/**
 *
 */
#include "adatrain.h"
#include "../utils/matrix_io.h"

#include <iostream>

bool adatrain_trainCvMLData(CvBoost & boost, const char * csvFilePath, int nFeatures, bool validate){
		CvMLData cvml;
    #ifdef DEBUG
    cout<<"DEBUG: loading training samples and labels from "<<csvFilePath<<endl;
    #endif
		cvml.read_csv(csvFilePath);
    cvml.set_response_idx(0);
		CvTrainTestSplit cvtts(ADATRAIN_TRAINING_PORTION, true);
    if (validate){
		  cvml.set_train_test_split(&cvtts);
    }
    #ifdef DEBUG
    cout<<"DEBUG: training adaboost (using CvMLData)"<<endl;
    #endif
		boost.train(&cvml, CvBoostParams(CvBoost::REAL, nFeatures, 0, 1, false, 0), false);
    if (validate){
	  	std::vector<float> train_responses, test_responses;
	  	float fl1 = boost.calc_error(&cvml, CV_TRAIN_ERROR, &train_responses);
	  	float fl2 = boost.calc_error(&cvml, CV_TEST_ERROR, &test_responses);
	  	cout<<"INFO: Train error="<<fl1<<endl;
	  	cout<<"INFO: Test error="<<fl2<<endl;
    }
		return true;
}
bool adatrain_trainManual(CvBoost & boost, std::vector<std::string> & matrixFiles, std::vector<int> & classes){
  // See http://breckon.eu/toby/teaching/ml/examples/c++/speech_ex/svm.cpp
  if (classes.size()!=matrixFiles.size() && matrixFiles.size()==0){
      cerr<<"ERR: number of class not equals to matrix numbers or do not contain any element (size="<<matrixFiles.size()<<")"<<endl; 
      return false;
   }
  
  // Load the first file in order to detect the number of samples attribute
  Mat sample = matrix_io_load(matrixFiles.at(0));
  int nsamples = matrixFiles.size();
  int nfeatures = sample.rows*sample.cols;
  Mat train_data = Mat(0, nfeatures, CV_32FC1); // rows are 0 because of Mat.push_back usage
  Mat train_labels = Mat(0, 1, CV_32FC1);

  for (int i=0; i<matrixFiles.size();i++){
    string fpath=matrixFiles.at(i);
    int label=classes.at(i);
    sample = matrix_io_load(fpath);
    #ifdef DEBUG
    cout<<"DEBUG: matrix '"<<fpath<<"' rows="<<sample.rows<<",cols="<<sample.cols<<",type="<<sample.type()<<",CV_32FC1="<<CV_32FC1<<endl;
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
  CvBoostParams params(CvBoost::REAL, train_data.cols, 0, 1, false, 0); 
  #ifdef DEBUG
  cout<<"DEBUG: training adaboost manually (nsamples="<<train_data.rows<<", nfeatures="<<train_data.cols<<")"<<endl;
  #endif
  // http://docs.opencv.org/modules/ml/doc/boosting.html#cvboost-train
  boost.train(train_data, CV_ROW_SAMPLE, train_labels, Mat(), Mat(), Mat(), Mat(), params, false);
  return true;
}
