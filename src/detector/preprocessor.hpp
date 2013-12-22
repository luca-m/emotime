/*
 * preprocessor.hpp
 * Copyright (C) 2013 stk <stk@101337>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <limits>

#include <opencv2/opencv.hpp>

#include "facecrop.h"
#include "facedetector.h"
#include "gaborbank.h"

using namespace std;
using namespace cv;
using namespace emotime;

namespace emotime{

  /**
   * 
   * TODO: support features filters
   * */
  class FacePreProcessor{
    private:
		  FaceDetector facedet;
      vector<GaborKern*> gaborbank;
      Size imgsize;
      double nwidths;
      double nlambdas;
      double nthetas;
    public:
      FacePreProcessor(string faceDetectorConfig, int width, int height, double nwidths, double nlambdas, double nthetas){
        imgsize.width=width;
        imgsize.height=height;
        this->nwidths=nwidths; 
        this->nlambdas=nlambdas; 
        this->nthetas=nthetas; 
        facedet=FaceDetector(faceDetectorConfig, true/*equalize*/, true/*toGray*/);
        gaborbank_getCustomGaborBank(gaborbank, this->nwidths, this->nlambdas, this->nthetas);
     }
     ~FacePreProcessor(){
       facedet.~FaceDetector();
       //delete gaborbank; // should usa a class not a structure..
     }
     bool extractFace(Mat & src, Mat & out){
	     bool faceFound=facecrop_cropFace(this->facedet, src, out, true/*register/equalize*/);
       if (!faceFound){
         #ifdef DEBUG
         cout<<"DEBUG: FacePreProcessr has found no face."<<endl;
         #endif 
         return false;
       }
       #ifdef DEBUG
       cout<<"DEBUG: FacePreProcessr has extracted a face."<<endl;
       #endif 
       return true;
     }
     bool filterImage(Mat & src, Mat & out){
       Mat resized;
       #ifdef DEBUG
       cout<<"DEBUG: FacePreProcessr is resizing and filtering image."<<endl;
       #endif 
       resize(src, resized, this->imgsize, 0, 0, CV_INTER_AREA);
       out=gaborbank_filterImage(resized, this->gaborbank);
       return true;
     }
     bool toFeaturesVector(Mat & src, Mat & out){
        #ifdef DEBUG
        cout<<"DEBUG: FacePreProcessr is transforming filtered image to feature vector."<<endl;
        #endif 
        out=src.reshape(1/*chan*/, 1/*rows*/);
        return true;
     }
     bool filterFeatures(Mat &src, Mat &out){
       #ifdef DEBUG
       cout<<"DEBUG: features filtering not yet implemented. Just dont filter."<<endl;
       #endif
       src.copyTo(out);
       return true; 
     }
     bool preprocess(Mat & img, Mat & featvec){
       Mat face, filtered, fvec;
       bool res;
       res=this->extractFace(img, face);
       if (res){
         res=this->filterImage(face, filtered);
         if (res){
           res=this->toFeaturesVector(filtered, fvec);
           if (res){
             res=this->filterFeatures(fvec, featvec);
           }
         }
       }
       return res;
     }
  };

}
#endif /* !PREPROCESSOR_HPP */

