#include "facecrop.h"

#ifdef DEBUG 
#include <iostream>
#endif

using namespace std;

bool facecrop_cropFace( emotime::FaceDetector & detector, cv::Mat & img, cv::Mat & cropped, bool regist){
		Rect face(0, 0, 0, 0);
		if (!detector.detect(img, face)){
      #ifdef DEBUG
      err<<"DEBUG: no face detected"<<endl;
      #endif
      return false;
		}
		cropped=img(face);
		if (regist){
			registerImage(cropped, cropped);
		}
		return true;
}
