/**
 *
 * @file    TrainingParameters.h
 * @date    01/10/2014 01:50:29 AM
 * @brief   Global parameter algorithm
 *
 */

#ifndef  __DETECTIONPARAMETERS_INC__
#define  __DETECTIONPARAMETERS_INC__

#include <opencv2/opencv.hpp>

namespace emotime {

  /// SVM C factor parameter
  const double kCfactor = 1.0;
  /// SVM max interation termination criteria
  const int kMaxIteration = 1000000;
  /// SVM error margin termination criteria
  const double kErrorMargin = 1e-6;

  /// AdaBoost algorithm type
  const int kBoostType = CvBoost::REAL;
  /// AdaBoost trim weight value
  const double kTrimWeight = 0.95;
  /// AdaBoost weak count 
  const double kWeakCount = 100;
  /// AdaBoost max algorithm depth
  const int kMaxDepth = 3;

}


#endif   /* ----- #ifndef __DETECTIONPARAMETERS_INC__  ----- */
