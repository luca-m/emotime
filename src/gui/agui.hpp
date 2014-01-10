/*
 * agui.hpp
 * Copyright (C) 2013 stk <stk@101337>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef AGUI_HPP
#define AGUI_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>

#include "capture.hpp"
#include "EmoDetector.h"
#include "FacePreProcessor.h"

using std::stringstream;
using std::string;
using std::pair;

using namespace emotime;

namespace emotime {


  /**
  * @class    AGui
  * @author   Daniele Bellavista (daniele.bellavista@studio.unibo.it)
  * @date    12/31/2013 10:53:55 AM
  *
  * @brief   Generic GUI
  *
  * @details
  *
  */
  class AGui {

    public:

      /**
       *  @brief          Creates an AGui
       *
       *  @param[in]      capt  A capture instance
       *  @param[in]      fp    The face preprocessor to use
       *  @param[in]      detect  An emodetector instance
       *  @param[in]      fps  Desired frame per second
       *
       */
      AGui(ACapture* capt, FacePreProcessor* fp, EmoDetector* detect, int fps) {
        capture = capt;
        preprocessor = fp;
        detector = detect;
        mainWinTitle = string("AGui: Main Emotime GUI");
        this->fps = fps;
      }

      /**
       *  @brief    Start the gui
       *
       *  @return   False if something wrong.
       *
       */
      bool run() {
        if(!init()) {
          return false;
        }
        while (nextFrame()) {
          int key;
          if (fps <= 0) {
            key = waitKey(0);
          } else {
            key = waitKey((int) 1000.0 / fps);
          }
          if((key & 0xFF) == 27) {
            break;
          }
        }
        return true;
      }

    protected:

      /// Face preprocessor
      FacePreProcessor* preprocessor;
      /// Emotion detector
      EmoDetector* detector;
      /// Capture instance
      ACapture* capture;
      /// Title for the main window
      string mainWinTitle;
      /// Desired frames per second
      int fps;

      /**
       *  @brief          Initialize the windows
       *
       *  @return         Returns true if the initialization succeded
       *
       */
      virtual bool init() {
        namedWindow(mainWinTitle.c_str(), WINDOW_NORMAL);
        return true;
      }

      /**
       *  @brief          Produce the next frame
       *
       *  @return         False if there is no next frame
       *
       */
      virtual bool nextFrame() {
        Mat frame;
        Mat featvector;
        if (capture->nextFrame(frame)) {
          if (preprocessor->preprocess(frame, featvector)) {
            pair<Emotion, float> prediction = detector->predict(featvector);
            if (!newFrame(frame, prediction)) {
              return false;
            }
            imshow(mainWinTitle.c_str(), frame);
          }
          return true;
        } else {
          return false;
        }
      }


      /**
       *  @brief          A new frame is available
       *
       *  @param[in]      frame The new frame that will be drawn after returing
       *  @param[in]      prediction The predicted emotion
       *
       *  @return         False if something wrong
       *
       */
      virtual bool newFrame(Mat& frame, pair<Emotion, float> prediction) = 0;

  };

  /**
  * @class    ADebugGui
  * @author
  * @date
  *
  * @brief   GUI for debugging purpose
  *
  */
  class ADebugGui : public AGui {

    public:

      /**
       *  @brief          Creates an ADebugGUI
       *
       *  @param[in]      capt  A capture instance
       *  @param[in]      fp    The face preprocessor to use
       *  @param[in]      detect  An emodetector instance
       *  @param[in]      fps  Desired frame per second
       *
       */
      ADebugGui(ACapture* capt, FacePreProcessor* fp, EmoDetector*
          detect, int fps) : AGui(capt, fp, detect, fps) {
        faceWinTitle = string("AGui: Face");
        featsWinTitle = string("AGui: Features");
      }

    protected:
      /// Title for face window
      string faceWinTitle;
      /// Title for features window
      string featsWinTitle;

      bool init() {
        if (!AGui::init()) {
          return false;
        }
        //namedWindow(faceWinTitle.c_str(), CV_WINDOW_AUTOSIZE);
        namedWindow(featsWinTitle.c_str(), CV_WINDOW_AUTOSIZE);
        return true;
      }

      bool newFrame(Mat& frame, pair<Emotion, float> prediction) {
        Mat copy;
        frame.copyTo(copy);
        stringstream ss;
        ss << "Emotion=" << emotionStrings(prediction.first) << ", Score=" << prediction.second;
        string osd = ss.str();

        cv::putText(frame, osd.c_str(), Point(80,60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar::all(0));
        // QT only
        //displayOverlay(mainWinTitle.c_str(), osd.c_str(), 2000);

        Mat face;
        if (AGui::preprocessor->extractFace(copy, face)) {
          Mat gabor;
          if (AGui::preprocessor->filterImage(face,gabor)){
            double min;
            double max;
            cv::minMaxIdx(gabor, &min, &max);
            cv::Mat adjMap;
            cv::convertScaleAbs(gabor, adjMap, 255 / max);
            Mat bigger;
            resize(adjMap,bigger,Size(adjMap.size().width*3,adjMap.size().height*3), 0, 0, CV_INTER_LINEAR);
            equalizeHist(bigger,bigger);
            imshow(featsWinTitle.c_str(), bigger);
          }
        }
        return true;
      }
  };
}


#endif /* !AGUI_HPP */
