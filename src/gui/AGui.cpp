/**
 *
 * @file    AGui.cpp
 * @brief   Contains the implementation for AGui
 *
 */

#include "AGui.h"


volatile int quit_signal=0;
#ifdef __unix__
#include <signal.h>
  extern "C" void quit_signal_handler(int signum) {
    if (quit_signal!=0) exit(0); // just exit already
      quit_signal=1;
    cout << "Will quit at next camera frame (repeat to kill now)" << endl;
}
#endif

namespace emotime{

  AGui::AGui(ACapture* capt, FacePreProcessor* fp, EmoDetector* detect, int fps, string title) {
       capture = capt;
       preprocessor = fp;
       detector = detect;
       mainWinTitle = title;
       this->fps = fps;
     }

  bool AGui::run() {
    if(!init()) {
      return false;
    }
    #ifdef __unix__
      signal(SIGINT,quit_signal_handler); // listen for ctrl-C
    #endif
    while (true) {
      nextFrame(false);
      if (quit_signal) exit(0);
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

  bool AGui::runOnce() {
    if(!init()) {
      return false;
    }
   
    clock_t temp;
    temp = clock () + 3 * CLOCKS_PER_SEC ;

    while (clock() < temp) {
      nextFrame(false);
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

    while(!nextFrame(true)){}

    return true;
  }

  bool AGui::init() {
    namedWindow(mainWinTitle.c_str(), WINDOW_NORMAL);
    return true;
  }

  bool AGui::nextFrame(bool verbose) {
    Mat frame;
    Mat featvector;
    if (capture->nextFrame(frame)) {
      if (preprocessor->preprocess(frame, featvector)) {
        std::map<Emotion,float> votes;
        pair<Emotion, float> prediction = detector->predict(featvector, votes);
        if (!newFrame(frame, prediction)) {
          return false;
        }
        if(verbose){
          ofstream outFile;
          outFile.open ("Outfile.json");
          outFile << "{" << endl;
          for( map<Emotion, float>::iterator ii=votes.begin(); ii!=votes.end(); ++ii){
            std::cout << emotionStrings(ii->first) << ": " << ii->second << endl; 
            outFile << emotionStrings(ii->first) << ": " << ii->second << "," << endl; 
          }
          outFile << "}" << endl;
          outFile.close();
        }
        imshow(mainWinTitle.c_str(), frame);
      }
      return true;
    } else {
      return false;
    }
  }

}

