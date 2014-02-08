/**
 *
 * @file    GaborGui.hpp
 * @brief   Contains the implementation and definition of GaborGui
 *
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
   * @brief Gui for visualizing gabor bank effects
   *
   */
  class GaborGui {

    protected:

      /// The frame capture device
      ACapture* capture;
      /// The title for the main window
      string mainWinTitle;
      /// The title for the gabor window
      string gaborWinTitle;
      /// The size to be used for image resizing
      Size size;
      /// nwidths param of GaborBank::fillGaborBank
      int  nwidths;
      /// nlambdas param of GaborBank::fillGaborBank
      int  nlambdas;
      /// nthetas param of GaborBank::fillGaborBank
      int  nthetas;
      /// The current gui frame
      Mat currframe;

    protected:

      /**
       * @brief OpenCV trackbar callback function
       *
       * @param[in] newVal new value
       * @param[in] obj The GaburGui object reference
       *
       */
      static void on_trackbar(int newVal, void *obj) {
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


      /**
       *  @brief          Creates a GaborGui with the given ACapture
       *
       *  @param[in]      cap The ACapture to use
       *
       */
      GaborGui(ACapture* cap) {
        capture=cap;
        size=Size(48,48);
        nwidths=2;
        nlambdas=5;
        nthetas=4;
        mainWinTitle=string("GaborGui: Gabor parameter tuning");
        gaborWinTitle=string("GaborGui: Gabor features");
      }

      /**
       *  @brief          Initialize windows and trackbars
       *
       *  @return         Always returns true
       *
       */
      bool init(){
	       namedWindow(mainWinTitle.c_str(), WINDOW_NORMAL);
	       namedWindow(gaborWinTitle.c_str(), WINDOW_AUTOSIZE);
         createTrackbar("nwidths",mainWinTitle.c_str(), &nwidths, 4, GaborGui::on_trackbar, this);
         createTrackbar("nlamdas",mainWinTitle.c_str(), &nlambdas, 12, GaborGui::on_trackbar, this);
         createTrackbar("nthetas",mainWinTitle.c_str(), &nthetas, 12, GaborGui::on_trackbar, this);
         return true;
      }


      /**
       *  @brief          Capture the next frame and reset the trackbar
       *
       *  @return         Returns false if no frame are available
       *
       */
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


      /**
       *  @brief          Produce a new frame at every keystroke
       *
       *  @return         Returns when no more frame are available. The value
       *                  is always true.
       *
       */
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

