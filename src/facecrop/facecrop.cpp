/**
 * @Deprecated
 * */
#include "facecrop.h"

#ifdef DEBUG 
#include <iostream>
#endif

using namespace std;
using namespace cv;

bool facecrop_cropFace(emotime::FaceDetector & detector, cv::Mat & img, cv::Mat & cropped, bool regist){
		Rect face(0, 0, 0, 0);
		if (!detector.detect(img, face)){
      #ifdef DEBUG
      cerr<<"DEBUG: no face detected"<<endl;
      #endif
      return false;
		}
    Mat fimg=img(face);
    #ifdef DEBUG
    Rect facedraw;
    facedraw.x=face.x-1;
    facedraw.y=face.y-1;
    facedraw.width=face.width+2;
    facedraw.height=face.height+2;
    rectangle(img, facedraw, Scalar(0, 0, 0));
    #endif
		fimg.copyTo(cropped);
		if (regist){
			registerImage(cropped, cropped);
		}
		return true;
}
