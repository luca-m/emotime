/*
 * registration.h
 *
 *  Created on: Aug 10, 2013
 *      Author: stk
 */
#ifndef REGISTR_H_
#define REGISTR_H_

#include <opencv2/opencv.hpp>

/**
* Register a grayscale image
* @param src Source image
* @param dst Destination image
*/
void registerImage( cv::Mat & src, cv::Mat & dst );


#endif /* REGISTR_H_ */