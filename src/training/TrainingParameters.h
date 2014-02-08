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
  const double kCfactor = 0.5;
  /// SVM max interation termination criteria
  const int kMaxIteration = 1000000;
  /// SVM error margin termination criteria
  const double kErrorMargin = 1e-6;

  /// AdaBoost algorithm type
  const int kBoostType = CvBoost::GENTLE;
  /// AdaBoost trim weight value
  const double kTrimWeight = 0.0;
  /// AdaBoost max algorithm depth
  const int kMaxDepth = 1;

}


#endif   /* ----- #ifndef __DETECTIONPARAMETERS_INC__  ----- */
