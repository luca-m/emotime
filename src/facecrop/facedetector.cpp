/**
 *
 */
#include "facedetector.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

namespace emotime{

FaceDetector::FaceDetector(){

}
FaceDetector::FaceDetector(string face_config_file, bool equalize, bool toGrayscale) {
	cascade_f.load(face_config_file);
	autoEqualize=equalize;
	autoGrayscale=toGrayscale;
	assert(!cascade_f.empty() );
}
FaceDetector::FaceDetector(string face_config_file) {
	cascade_f.load(face_config_file);
	autoEqualize=true;
	autoGrayscale=true;
	assert(	!cascade_f.empty() );
}
FaceDetector::~FaceDetector() {
  //TODO: release cascade_f
}

bool FaceDetector::detectFace(Mat & img, Rect & faceRegion) {
	vector<Rect> faces;
  #ifdef DEBUG
  cout<<"DEBUG: detecting faces"<<endl;
  #endif
	// detect faces */
	cascade_f.detectMultiScale(img, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE,
			cvSize(40, 60));
	int maxI=-1;
	int maxArea=-1;
	for (unsigned int i=0;i<faces.size();i++){
		int area=faces.at(i).width*faces.at(i).height;
		if (area>maxArea){
			maxI=i;
			maxArea=area;
		} 
	}
  // Pick the face with maximum area
	if (maxI >= 0) {
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
bool FaceDetector::detect(Mat & img, Rect & faceRegion) {
	if (img.rows == 0 || img.rows == 0){
		return false;
	}
	if (autoGrayscale){
    #ifdef DEBUG
    cout<<"DEBUG: converting in grayscale"<<endl;
    #endif
		Mat imgGray(img.size(),CV_8UC1);
		if (img.channels()>2){
			cvtColor(img, imgGray, CV_BGR2GRAY);
		}else{
			img.copyTo(imgGray);
		}
		if (autoEqualize) {
      #ifdef DEBUG
      cout<<"DEBUG: auto equalization (on image copy)"<<endl;
      #endif
			equalizeHist(imgGray, imgGray);
		}
		return this->detectFace(imgGray,faceRegion);
	}else{
		return this->detectFace(img,faceRegion);
	}
}

}/* namespace facecrop */
