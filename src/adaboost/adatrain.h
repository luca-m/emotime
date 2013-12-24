
#ifndef _H_ADATRAIN
#define _H_ADATRAIN

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

#define ADATRAIN_TRAINING_PORTION 0.9f
#define BOOST_ALGORITHM CvBoost::GENTLE 

using namespace std;
using namespace cv;


/**
* Train a AdaBoost detector using samples data from a CSV file (using CvMLData).
* Note: train/validation set split is 0.7 by default.
*
* @param boost 			  The CvBoost object to train
* @param csvFilePath	The path of the CSV file containing training data ( P,val,val,val,... \n N,val,val,val.. ) 
* @param nFeatures		Number of features to consider
* @param validate     Split dataset and perform trained model validation.
* @return             true if training is successfull
*/
bool adatrain_trainCvMLData(CvBoost & boost, const char * csvFilePath, int nFeatures, bool validate );
/**
* Train a AdaBoost detectot preparing train vectors e validation vectors of salmples manutally.
*
* @param boost 			  The CvBoost object to train
* @param matrixFiles  List of files where to load feature matrix
* @param classes		  Classes vector containing "P" or "N" strings
* @return             true if training is successfull
*/
bool adatrain_trainManual(CvBoost & boost, std::vector<std::string> & matrixFiles, std::vector<int> & classes);

#endif // _H_ADATRAIN
