
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
* NOTE: greyscale image only
* 
* @param boost
* @param bank
* @param img
* @param cropFace 	face detector configuration. If null no face cropping will be made.
* @return the prediction
*/
float adapredict_predict( CvBoost & boost, std::vector<cv::Mat> & bank, cv::Mat & img, cv::Size scaleSize, const char * faceDetectConf );


#endif // _H_ADAPREDICT

