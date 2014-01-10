/**
 *
 * @file    DetectionParameters.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    01/10/2014 01:50:29 AM
 * @brief   
 *
 * @details 
 *
 */

#ifndef  __DETECTIONPARAMETERS_INC__
#define  __DETECTIONPARAMETERS_INC__

#include <opencv2/opencv.hpp>

namespace emotime {

  const double kCfactor = 0.5;
  const int kMaxIteration = 1000000;
  const double kErrorMargin = 1e-6;

  const int kBoostType = CvBoost::GENTLE;
  const double kTrimWeight = 0.0;
  const double kMaxDepth = 1.0;

}


#endif   /* ----- #ifndef __DETECTIONPARAMETERS_INC__  ----- */
