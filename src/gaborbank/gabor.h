/*
 * gaborbank.h
 *
 *  Created on: Jul 23, 2013
 *      Author: stk
 */

#ifndef GABORBANK_H_
#define GABORBANK_H_

#include <opencv2/opencv.hpp>

/**
* Obtain a bank of Gabor filter kernels with different orientations and frequencies
* 
* @param bank vector of kernel to fill.
*/
void getGaborBank(std::vector<cv::Mat> & bank);

#endif /* GABORBANK_H_ */