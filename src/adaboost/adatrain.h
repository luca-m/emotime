
#ifndef _H_ADATRAIN
#define _H_ADATRAIN

#include <string>
#include <opencv2/opencv.hpp>

#define ADATRAIN_TRAINING_PORTION 0.7f

// #if defined(WIN32) || defined(_WIN32) 
// #define ADATRAIN_PATH_SEPARATOR "\\" 
// #else 
// #define ADATRAIN_PATH_SEPARATOR "/" 
// #endif 

using namespace std;
using namespace cv;

/**
* Train a AdaBoost detector using data from a CSV file.
*
* @param boost 			The CvBoost object to train
* @param csvFilePath	The path of the CSV file containing training data ( P,val,val,val,... \n N,val,val,val.. ) 
* @param nFeatures		Number of features to consider
* @param printErr		Print training error in STDERR
* @return true if training is successfull
*/
bool adaboost_trainCSV(CvBoost & boost, const char * csvFilePath, int nFeatures, bool printErr );

#endif // _H_ADATRAIN