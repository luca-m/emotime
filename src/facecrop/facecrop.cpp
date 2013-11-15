/*
 * facecrop.cpp
 *
 *  Created on: Aug 07, 2013
 *      Author: stk
 */

#include "facecrop.h"

#ifdef DEBUG 
#include <iostream>
#endif

using namespace std;

bool facecrop_cropFace( facecrop::FaceDetector & detector, cv::Mat & img, cv::Mat & cropped, bool regist ){
		Rect face(0, 0, 0, 0);
		if ( !detector.detect(img, face) ){
      #ifdef DEBUG
      cout<<"DEBUG: no face detected"<<endl;
      #endif
      return false;
		}
		cropped=img(face);
		if (regist){
      #ifdef DEBUG
      cout<<"DEBUG: registering image"<<endl;
      #endif
			registerImage(cropped, cropped);
		}
		return true;
}
