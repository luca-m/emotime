/**
 *
 * @file    FaceDetector.cpp
 *
 * @date    01/10/2014 12:26:04 AM
 * @brief   Implementation for FaceDetector
 *
 * @details
 *
 */

#include "FaceDetector.h"
#include <math.h>

using cv::Mat;
using cv::Point;
using cv::Size;
using cv::Rect;
using cv::CascadeClassifier;

using std::string;
using std::vector;

namespace emotime {

  FaceDetector::FaceDetector(std::string face_config_file, std::string eye_config_file){
    
    if (face_config_file.find(std::string("cbcl1"))!=std::string::npos){
      this->faceMinSize=Size(30, 60);
    } else {
      this->faceMinSize=Size(120,200); 
    } 

    cascade_f.load(face_config_file);
    if (eye_config_file != string("none")) {
      cascade_e.load(eye_config_file);
      assert(!cascade_e.empty());
      this->doEyesRot = true;
    } else {
      this->doEyesRot = false;
    }
    assert(!cascade_f.empty());
  }

  FaceDetector::FaceDetector(std::string face_config_file) {
    cascade_f.load(face_config_file);
    this->doEyesRot = false;
    assert(!cascade_f.empty());
  }

  FaceDetector::FaceDetector() {

  }

  FaceDetector::~FaceDetector() {

  }

  bool FaceDetector::detectFace(cv::Mat& img, cv::Rect& face) {
    vector<Rect> faces;
    // detect faces
    assert(!cascade_f.empty());
    cascade_f.detectMultiScale(img, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, this->faceMinSize );

    if (faces.size() == 0){
      return false;
    }
    // Pick the face with maximum area
    unsigned int maxI=-1;
    int maxArea=-1;
    int area=-1;
    for (unsigned int i=0;i<faces.size();i++){
      area=faces.at(i).width*faces.at(i).height;
      if (area>maxArea){
        maxI=i;
        maxArea=area;
      }
    }
    face.x = faces.at(maxI).x;
    face.y = faces.at(maxI).y;
    face.width = faces.at(maxI).width;
    face.height = faces.at(maxI).height;
    faces.clear();
    return true;
  }

  bool FaceDetector::detectEyes(cv::Mat& img, cv::Point& eye1, cv::Point& eye2){
    vector<Rect> eyes;
    // detect faces
    assert(!cascade_e.empty());
    // Min widths and max width are taken from eyes proportions
    cascade_e.detectMultiScale(img, eyes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE,
        Size(img.size().width/5, img.size().width/(5*2)));

    if (eyes.size() < 2) {
      eyes.clear();
      return false;
    }

    // Pick eyes with maximum area
    int val1=-1;
    int tmp=-1;
    int x,y,w,h;
    Point tmpe;
    int val2=-1;
    int area=-1;
    for (unsigned int i=0;i<eyes.size();i++){
      x=eyes.at(i).x;
      y=eyes.at(i).y;
      w=eyes.at(i).width;
      h=eyes.at(i).height;
      area=eyes.at(i).width*eyes.at(i).height;
      if (area>val1 && val1>val2){
        tmp=val1;
        tmpe.x=eye1.x;
        tmpe.y=eye1.y;
        val1=area;
        eye1.x=x+w/2;
        eye1.y=y+h/2;
        val2=tmp;
        eye2.x=tmpe.x;
        eye2.y=tmpe.y;
      }else if (area>val2 && val2>val1){
        tmp=val2;
        tmpe.x=eye2.x;
        tmpe.y=eye2.y;
        val2=area;
        eye2.x=x+w/2;
        eye2.y=y+h/2;
        val1=tmp;
        eye1.x=tmpe.x;
        eye1.y=tmpe.y;
      } else if (area>val1){ 
        // second
        val1=area;
        eye1.x=x+w/2;
        eye1.y=y+h/2;
      } else if (area>val2){
        // second
        val2=area;
        eye2.x=x+w/2;
        eye2.y=y+h/2;
      }
    }
    eyes.clear();
    return true;
  }

  bool FaceDetector::detect(cv::Mat& img, cv::Mat& face) {
    bool hasFace;
    bool hasEyes;
    Rect faceRegion;
    Mat plainFace;
    Point eye1,eye2;

    if (img.rows == 0 || img.rows == 0){
      return false;
    }

    Mat imgGray(img.size(),CV_8UC1);
    if (img.channels()>2){
      cvtColor(img, imgGray, CV_BGR2GRAY);
    }else{
      img.copyTo(imgGray);
    }

    equalizeHist(imgGray, imgGray);
    hasFace=detectFace(imgGray, faceRegion);

    if (!hasFace){
      return false;
    }
    // detect eyes and locate points
    plainFace=imgGray(faceRegion);
    if (doEyesRot){
      hasEyes=detectEyes(plainFace, eye1, eye2);
      if (hasEyes){
        // eyes are initially relative to face patch
        eye1.x+=faceRegion.x; 
        eye2.x+=faceRegion.x; 
        eye1.y+=faceRegion.y; 
        eye2.y+=faceRegion.y; 
        Point left,right,upper,lower,tribase,eyecenter;
        if (eye1.x<eye2.x){
          left=eye1;
          right=eye2;
        }else{
          left=eye2;
          right=eye1;
        }
        if (eye1.y<eye2.y){
          lower=eye1;
          upper=eye2;
        }else{
          lower=eye2;
          upper=eye1;
        }
        tribase=Point(upper.x, lower.y);
        eyecenter=Point(left.x+(right.x-left.x)/2, lower.y+(upper.y-lower.y)/2);
        // rotate image
//        float c0=std::sqrt(std::pow(tribase.x-upper.x,2)+std::pow(tribase.y-upper.y,2));
        float c1=std::sqrt(std::pow(tribase.x-lower.x,2)+std::pow(tribase.y-lower.y,2));
        float ip=std::sqrt(std::pow(upper.x-lower.x,2)  +std::pow(upper.y-lower.y  ,2));
        float angle=(left.x==lower.x?1:-1)*std::acos(c1/ip)*(180.0f/CV_PI)/2.0;
        
        if (/*std::abs(angle)<20.0 && */std::abs(angle)<kMaxRotationAngle){
          Mat rotMat = getRotationMatrix2D(eyecenter, angle, 1.0);
          warpAffine(imgGray, imgGray, rotMat, imgGray.size());
          //hasFace=detectFace(imgGray, faceRegion);
          //if (!hasFace){
          //  return false;
          //}
        }
      }
    }
    // copy equalized and rotated face to out image 
    #ifndef TRAINING_BUILD
    cv::resize(plainFace,plainFace, kFaceSizeLimit, cv::INTER_LINEAR);
    #endif
    plainFace.copyTo(face);
    equalizeHist(face, face);
    imgGray.release();
    return true;
  }

}/* namespace facecrop */
