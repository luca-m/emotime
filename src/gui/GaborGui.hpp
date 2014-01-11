/**
 * GaborGui.hpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef GABORGUI_H
#define GABORGUI_H

#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>

#include "ACapture.h"
#include "GaborBank.h"

namespace emotime{

  /**
   * @class GaburGui
   *
   * @brief Gui for visualizing gabor bank effects
   *
   * @description
   *
   * */
  class GaborGui {
    
    protected:
      
      /// The frame capture device
      ACapture* capture;
      ///
      string mainWinTitle;
      ///
      string gaborWinTitle;
      ///
      Size size;
      ///
      int  nwidths;
      ///
      int  nlambdas;
      ///
      int  nthetas;
      ///
      Mat currframe;
    
    protected:
 
      /**
       * @brief OpenCV trackbar callback function
       *
       * @param[in] new value
       * @param[in] The GaburGui object reference
       *
       * */ 
      static void on_trackbar(int newVal, void *obj){
        GaborBank gaborbank;
        Mat frame;
        Mat gaborframe;
        Mat resized;
        Mat scaled;
        Mat magnified;
        double min;
        double max;
        #ifdef DEBUG
        cout<<"DEBUG: Parameters changed, reconfiguring.. (this@"<<obj<<")"<<endl; 
        #endif
        GaborGui* ths = (GaborGui*) obj;
        if (ths==NULL){
          return;
        }
        Mat copy;
        ths->currframe.copyTo(frame);
        resize(ths->currframe, resized, ths->size, 0, 0, CV_INTER_AREA); 
        cout<<"DEBUG: recreating gabor filter bank "<<ths->nwidths<<","<<ths->nlambdas<<","<<ths->nthetas <<endl;
        gaborbank=GaborBank();
        gaborbank.fillGaborBank(ths->nwidths, ths->nlambdas, ths->nthetas);
        gaborframe=gaborbank.filterImage(resized);
        cout<<"DEBUG: preparing for visualization"<<endl;
        minMaxIdx(gaborframe, &min, &max);
        convertScaleAbs(gaborframe, scaled, 255/max);
        equalizeHist(scaled, scaled);
        resize(scaled, magnified, Size(scaled.size().width*5, scaled.size().height*5), 0, 0, CV_INTER_LINEAR);
        
        imshow(ths->mainWinTitle.c_str(), ths->currframe);
        imshow(ths->gaborWinTitle.c_str(), magnified);
      }

    public:
      
      GaborGui(ACapture* cap) {
        capture=cap;
        size=Size(48,48);
        nwidths=2;
        nlambdas=5;
        nthetas=4;
        mainWinTitle=string("GaborGui: Gabor parameter tuning");
        gaborWinTitle=string("GaborGui: Gabor features");
      } 
      
      bool init(){
	       namedWindow(mainWinTitle.c_str(), WINDOW_NORMAL);
	       namedWindow(gaborWinTitle.c_str(), WINDOW_AUTOSIZE);
         createTrackbar("nwidths",mainWinTitle.c_str(), &nwidths, 4, GaborGui::on_trackbar, this);
         createTrackbar("nlamdas",mainWinTitle.c_str(), &nlambdas, 12, GaborGui::on_trackbar, this);
         createTrackbar("nthetas",mainWinTitle.c_str(), &nthetas, 12, GaborGui::on_trackbar, this);
         return true;
      }
      
      bool nextFrame(){
        Mat frame;
        Mat featvector;
        if (capture->nextFrame(frame)){
          currframe.release();
          frame.copyTo(currframe);
          on_trackbar(0, this);
          return true;
        } else{ 
          return false;
        }
      }
      
      bool run(){
        init();
        while (nextFrame()){
            waitKey(0);
        } 
        return true;

      }
  }; 

}

#endif /* !GABORGUI_H */

