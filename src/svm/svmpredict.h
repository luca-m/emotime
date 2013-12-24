/**
 *
 * @file    svmpredict.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/24/2013 10:26:52 AM
 * @brief   
 *
 * @details 
 *
 */

#ifndef _H_SVMPREDICT
#define _H_SVMPREDICT

#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include "gaborbank.h"
#include "facecrop.h"

/**
 * @brief Returns the prediction using the specified image, by calculating
 *        features required for classification.
 *
 * @param [in] svm  The trained SVM to use
 * @param [in] img  The image to predict
 *
 * @return the prediction
 *
 */
float svmpredict_predictNoPreprocess(CvSVM& svm, cv::Mat& img);

/**
 * @brief Returns the prediction using the specified image and the specified
 * filters for feature extraction. It is also able to preprocess the specified
 * image for face extraction and registration.
 *
 * @param [in] svm  The trained SVM to use
 * @param [in] img  The target image.
 * @param [in] scaleSize resizing factor for feature calculation.
 * @param [in] faceDetectConf Face detector configuration. If NULL no face
 *                            cropping will be made.
 *
 * @return the prediction
 *
 */
float svmpredict_predictPreprocess(CvSVM& svm, cv::Mat& img, cv::Size&
    scaleSize, const char* faceDetectConf);

/**
 * @brief Converts a NxM feature image to a (N*M)x1 feature vector suitable for
 *        predictions.  NOTE: it assumes to work whith CV_32C1 matrix.
 *
 * @param [in] src The NxM feature image.
 *
 * @return The new image
 */
cv::Mat svmpredict_imageToFeatVec(cv::Mat& src);

/**
 * @brief Returns the prediction using the specified image and the specified
 *        filters for feature extraction.  It is also able to preprocess the
 *        specified image for face extraction and registration.
 *
 * @param [in] svm The trained SVM to use
 * @param [in] bank The GaborKernel vector to use for feature computation.
 * @param [in] img The target image.
 * @param [in] scaleSize resizing factor for feature calculation.
 * @param [in] faceDetectConf Face detector configuration. If NULL no face
 *                            cropping will be made.
 *
 * @return the prediction
 *
 */
float svmpredict_predict(CvSVM& svm, std::vector<struct GaborKern*>& bank,
    cv::Mat& img, cv::Size & scaleSize, emotime::FaceDetector* facedetector);

#endif // _H_SVMPREDICT

