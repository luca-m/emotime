/**
 *
 * @file    svmtrain.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/05/2013 10:44:35 PM
 * @brief   
 *
 * @details 
 *
 */

#ifndef  __SVMTRAIN_INC__
#define  __SVMTRAIN_INC__


#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>

#define SVM_MAX_ITERATIONS 10000
#define SVM_ERROR_MARGIN 1e-6

/**
* @brief      Train a SVM classifieer using samples data from a CSV file (using CvMLData).
*
* @param [in] svm 			  The CvSVM object to train
* @param [in] csvFilePath	The path of the CSV file containing training data (
*                         P,val,val,val,... \n N,val,val,val.. ) 
* @param [in] nFeatures		Number of features to consider
* @param [in] validate    Split dataset and perform trained model validation.
*
* @return                 true if training was successfull
*/
bool svmtrain_trainCvMLData(CvSVM& svm, const char * csvFilePath, int nFeatures, bool validate);

/**
* @brief      Train a SVM classifier with the given training files and classes.
*
* @param [in] svm 			    The CvSVM object to train
* @param [in] matrixFiles   List of files where to load feature matrix
* @param [in] classes		    Classes vector containing "P" or "N" strings
* @return    true if training was successfull
*/
bool svmtrain_trainManual(CvSVM& svm, std::vector<std::string> & matrixFiles, std::vector<int> & classes);

#endif   /* ----- #ifndef __SVMTRAIN_INC__  ----- */
