
#ifndef _H_FACECROP
#define _H_FACECROP 

#include <string>
#include <opencv2/opencv.hpp>

#include "facedetector.h"
#include "registration.h"


/**
* Detect and crop face from an image	
* 
* @param detector 	face detector to use.
* @param img 		image containing a face
* @param cropped 	output image containing the face in face. 
* @param regist 	register output image (at the moment: equalization)
* @return true if face is detected and cropped properly
*/
bool facecrop_cropFace( facecrop::FaceDetector & detector, cv::Mat & img, cv::Mat & cropped, bool regist );

#endif // _H_FACECROP 