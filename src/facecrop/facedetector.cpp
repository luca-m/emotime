/*
 * facedetector.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: stk
 */

#include "facedetector.h"

using namespace std;

namespace facecrop {

bool FaceDetector::detectFace(Mat & img, Rect & faceRegion) {
	vector<Rect> faces;
	bool hasFace = false;
	/* detect faces */
	cascade_f.detectMultiScale(img, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE,
			cvSize(40, 60));

	int maxI=-1;
	int maxArea=std::numeric_limits<int>::min();
	for (int i=0;i< faces.size();i++){
		int area=faces.at(i).width * faces.at(i).height;
		if ( area > maxArea){
			maxI=i;
		} 
	}

	if (maxI > 0) {
		faceRegion.x = faces.at(maxI).x;
		faceRegion.y = faces.at(maxI).y;
		faceRegion.width = faces.at(maxI).width;
		faceRegion.height = faces.at(maxI).height;
		return true;
	} else {
		faceRegion.x = 0;
		faceRegion.y = 0;
		faceRegion.width = 0;
		faceRegion.height = 0;
		return false;
	}
}

FaceDetector::FaceDetector(string face_config_file) {
	cascade_f.load(face_config_file);
	autoEqualize=true;
	autoGrayscale=true;
	assert(	!cascade_f.empty() );
}

FaceDetector::~FaceDetector() {
}

bool FaceDetector::detect(Mat & img , Rect & faceRegion) {
	if (img.rows == 0 || img.rows == 0){
		return false;
	}
	if (autoGrayscale){
		Mat imgGray(img.size(),CV_8UC1);
		cvtColor(img, imgGray, CV_BGR2GRAY);
		if ( autoEqualize ) {
			equalizeHist(imgGray, imgGray);
		}
		return this->detectFace(imgGray,faceRegion);
	}else{
		return this->detectFace(img,faceRegion);
	}
}

}/* namespace facecrop */
