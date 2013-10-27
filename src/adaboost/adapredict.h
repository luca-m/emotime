
#ifndef _H_ADAPREDICT
#define _H_ADAPREDICT

#include <opencv2/opencv.hpp>
#include "../gaborbank/gaborbank.h"
#include "../facecrop/facecrop.h"

using namespace cv;

/**
* Return the prediction using the specified image.
* It calculate features required for classification. 
* NOTE: greyscale image only
*
* @param boost
* @param img
* @return the prediction
*/
float adapredict_predictNoPreprocess( CvBoost & boost, cv::Mat & img );
/**
* Return the prediction using the specified image and the specified filters for feature extraction.
* It is also able to preprocess the specified image for face extraction and registration.
* NOTE: greyscale image only
* 
* @param boost
* @param img the target image.
* @param scaleSize new size of the image to use for feature calculation. If cv::Size(0,0) no resizing will be made.
* @param cropFace 	face detector configuration. If NULL no face cropping will be made.
* @return the prediction
*/
float adapredict_predictPreprocess( CvBoost & boost, cv::Mat & img, cv::Size & scaleSize, const char * faceDetectConf );
/**
 * Convert a NxM feature image to a (N*M)x1 feature vector suitable for predictions.
 * NOTE: it assume to work whith CV_32C1 matrix.
 *
 * @param src The NxM feature image.
 * */
cv::Mat adapredict_imageToFeatVec(cv::Mat & src);
/**
* Return the prediction using the specified image and the specified filters for feature extraction.
* It is also able to preprocess the specified image for face extraction and registration.
* NOTE: greyscale image only
* 
* @param boost
* @param bank the GaborKernel vector to use for feature computation.
* @param img the target image.
* @param scaleSize new size of the image to use for feature calculation. If cv::Size(0,0) no resizing will be made.
* @param cropFace 	face detector configuration. If NULL no face cropping will be made.
* @return the prediction
*/
float adapredict_predict( CvBoost & boost, std::vector<struct GaborKern*> & bank, cv::Mat & img, cv::Size & scaleSize, const char * faceDetectConf );

#endif // _H_ADAPREDICT

