/*
 * registration.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: stk
 */

#include "registration.h"

 void registerImage( cv::Mat & src, cv::Mat & dst){
 	cv::equalizeHist(src,dst);
 	return;
 }
