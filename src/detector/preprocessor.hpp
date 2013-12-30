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
      vector<GaborKernel*> gaborbank;
      Size imgsize;
      double nwidths;
      double nlambdas;
      double nthetas;
    public:
      FacePreProcessor(string faceDetectorConfig, string eyesDetectorConfig, int width, int height, double nwidths, double nlambdas, double nthetas){
        imgsize.width=width;
        imgsize.height=height;
        this->nwidths=nwidths; 
        this->nlambdas=nlambdas; 
        this->nthetas=nthetas;
        if (eyesDetectorConfig.size()>0){ 
          facedet=FaceDetector(faceDetectorConfig,eyesDetectorConfig); 
        }else{
          facedet=FaceDetector(faceDetectorConfig); 
        }
        gaborbank_getCustomGaborBank(gaborbank, this->nwidths, this->nlambdas, this->nthetas);
      }
      FacePreProcessor(string faceDetectorConfig, int width, int height, double nwidths, double nlambdas, double nthetas){
        FacePreProcessor(faceDetectorConfig,string(""), width, height, nwidths, nlambdas, nthetas);
      }
     ~FacePreProcessor(){
       facedet.~FaceDetector();
       for (unsigned int i=0; i<gaborbank.size();i++){
        delete gaborbank.at(i);
       }
       gaborbank.clear();
       //delete gaborbank; // should usa a class not a structure..
     }
     bool extractFace(Mat & src, Mat & out){
       Mat face;
	     bool faceFound=this->facedet.detect(src, face);
       if (!faceFound){
         #ifdef DEBUG
         cout<<"DEBUG: FacePreprocessor has found no face."<<endl;
         #endif 
         return false;
       }
       #ifdef DEBUG
       cout<<"DEBUG: FacePreprocessor has extracted a face (w="<<face.size().width<<",h="<<face.size().height<<")"<<endl;
       #endif
       face.copyTo(out); 
       return true;
     }
     bool filterImage(Mat & src, Mat & out){
       Mat resized, filtered;
       #ifdef DEBUG
       cout<<"DEBUG: FacePreprocessor is resizing (h="<<this->imgsize.height<<",w="<<this->imgsize.width<<") and filtering image. ("<<this->gaborbank.size()<<" filters)"<<endl;
       #endif 
       resize(src, resized, this->imgsize, 0, 0, CV_INTER_AREA);
       filtered=gaborbank_filterImage(resized, this->gaborbank);
       #ifdef DEBUG
       cout<<"DEBUG: FacePreprocessor is filtered image h="<<filtered.size().height<<",w="<<filtered.size().width<<endl;
       #endif 
       filtered.copyTo(out);
       return true;
     }
     bool toFeaturesVector(Mat & src, Mat & out){
        Mat feat;
        #ifdef DEBUG
        cout<<"DEBUG: FacePreprocessor is transforming filtered image to feature vector."<<endl;
        #endif 
        feat=src.reshape(1/*chan*/, 1/*rows*/);
        feat.copyTo(out);
        return true;
     }
     bool filterFeatures(Mat &src, Mat &out){
       #ifdef DEBUG
       cout<<"DEBUG: features filtering not yet implemented. Just copy."<<endl;
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

