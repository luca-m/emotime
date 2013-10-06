/*
 * facecrop.cpp
 *
 *  Created on: Aug 07, 2013
 *      Author: stk
 */

#include <stdlib.h>
#include <iostream>

#include "facecrop.h"

bool facecrop_cropFace( facecrop::FaceDetector & detector, cv::Mat & img, cv::Mat & cropped, bool regist ){
		Rect face(0,0,0,0);
		if ( !detector.detect(img, face) ){
			return false;
		}
		cropped=img(face);
		if (regist){
			registerImage( cropped, cropped );
		}
		return true;
}
