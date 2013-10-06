/*
 * gaborbank.h
 *
 *  Created on: Jul 23, 2013
 *      Author: stk
 */

#ifndef _H_GABORBANK
#define _H_GABORBANK

#include <opencv2/opencv.hpp>


/**
* Obtain a bank of 40 Gabor filter kernels with different orientations and frequencies
* 
* @param bank vector of kernel to fill.
*/
void gaborbank_getGaborBank(std::vector<cv::Mat> & bank);

/**
* Obtain the dimension needed for pre-allocating an image suitable for being used in "gaborbank_filterImage"  
* NOTE: GRAYSCALE IMAGE ONLY
*
* @param src 		the input image
* @param bank 		the vector of filters kernel
* @param outSize 	size suggested
*/
void gaborbank_getFilteredImgSize( cv::Mat src, std::vector<cv::Mat> & bank , cv::Size & outSize );

/**
* Filter the input image with all the kernel specified in the bank parameter, 
* then write all result in a single output image. 
* NOTE: GRAYSCALE IMAGE ONLY
*
* @param src 		the input image
* @param bank 		the vector of filters kernel
* @param dest 		The output image
* 
*/
void gaborbank_filterImage( cv::Mat & src, std::vector<cv::Mat> & bank, cv::Mat & dest );


#endif // _H_GABORBANK