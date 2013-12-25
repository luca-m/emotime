/*
 * debuggui.hpp
 */

#ifndef DEBUGGUI_HPP
#define DEBUGGUI_HPP

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "agui.hpp"
#include "preprocessor.hpp"
#include "emo_detector.hpp"
#include "boost_emo_detector.h"

using namespace std;
using namespace cv;
using namespace emotime;

namespace emotime{

  class DebugGuiBoost: public ADebugGui<CvBoost>{
    public:
      DebugGuiBoost(ACapture * capt, FacePreProcessor * fp, EmoDetector<CvBoost> * detect, int fps): ADebugGui<CvBoost>(capt, fp, detect, fps){
      }
  };
  /*
  class SvmGuiBoost: public ADebugGui<CvSvm>{
    public:
      SvmGuiBoost(ACapture * capt, FacePreProcessor * fp, EmoDetector<CvBoost> * detect, int fps): ADebugGui<SvSvm>(capt, fp, detect, fps){
      }
  };
 */

  class GaborGui{
    protected:
      FacePreProcessor *preprocessor;
      ACapture * capture;
      string mainWinTitle;
      string gaborWinTitle;
      Size size;
      int  nwidths;
      int  nlambdas;
      int  nthetas;
      string facedetectorconf;
      Mat currframe;
    protected:
      static void on_trackbar(int newVal, void *obj){
        #ifdef DEBUG
        cout<<"DEBUG: Parameters changed, reconfiguring.. (this@"<<obj<<")"<<endl; 
        #endif
        GaborGui * ths = (GaborGui *) obj;
        if (ths==NULL){
          return;
        }
        if (ths->preprocessor!=NULL){
          delete ths->preprocessor; 
        }
        Mat copy;
        ths->currframe.copyTo(copy);
        cout<<"DEBUG: recreating preprocessor"<<endl; 
        ths->preprocessor=new FacePreProcessor(ths->facedetectorconf, ths->size.width, ths->size.height, ths->nwidths, ths->nlambdas, ths->nthetas);
        Mat face;
        if(ths->preprocessor->extractFace(copy, face)){
          Mat gabor;
          if (ths->preprocessor->filterImage(face, gabor)){
            double min;
            double max;
            cv::minMaxIdx(gabor, &min, &max);
            cv::Mat adjMap;
            cv::convertScaleAbs(gabor, adjMap, 255/max);
            equalizeHist(adjMap, adjMap);
            Mat bigger;
            resize(adjMap,bigger,Size(adjMap.size().width*5,adjMap.size().height*5), 0, 0, CV_INTER_LINEAR);
            imshow(ths->mainWinTitle.c_str(), ths->currframe);
            imshow(ths->gaborWinTitle.c_str(), bigger);
          }
       }
      }
    public:
      GaborGui(ACapture * cap, string facedetconf){
        preprocessor=NULL;
        capture=cap;
        size=Size(48,48);
        nwidths=1;
        nlambdas=5;
        nthetas=8;
        facedetectorconf=facedetconf;
        mainWinTitle=string("GaborGui: Main Emotime Debug GUI");
        gaborWinTitle=string("GaborGui: Features");
      } 
      bool init(){
	       namedWindow(mainWinTitle.c_str(), WINDOW_NORMAL);
	       namedWindow(gaborWinTitle.c_str(), WINDOW_AUTOSIZE);
         createTrackbar("nwidths",mainWinTitle.c_str(), &nwidths, 4, GaborGui::on_trackbar, this);
         createTrackbar("nlamdas",mainWinTitle.c_str(), &nlambdas, 12, GaborGui::on_trackbar, this);
         createTrackbar("nthetas",mainWinTitle.c_str(), &nthetas, 12, GaborGui::on_trackbar, this);
      }
      bool nextFrame(){
        Mat frame;
        Mat featvector;
        if (capture->nextFrame(frame)){
          currframe.release();
          frame.copyTo(currframe);
          on_trackbar(1,this);
        }
      }
      bool run(){
        init();
        while (nextFrame()){
            waitKey(0);
        }
      }
  }; 

}

#endif /* !DEBUGGUI_HPP */

