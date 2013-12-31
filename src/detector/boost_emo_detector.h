#ifndef _H_BOST_EMO_DETECTOR
#define _H_BOST_EMO_DETECTOR 

#include "emo_detector.hpp"
#include "facedetector.h"
#include "adapredict.h"

using namespace std;
using namespace cv;
using namespace emotime;

namespace emotime{

  class BoostEmoDetector: public EmoDetector<CvBoost>{
    public:
      BoostEmoDetector(): EmoDetector<CvBoost>() {
      }
      BoostEmoDetector(map<string,pair<Emotion,CvBoost*> > detmap ): EmoDetector<CvBoost>(detmap){
      }
      BoostEmoDetector(vector<string> classifier_paths, vector<CvBoost*>
          initialized_classifiers) : EmoDetector(classifier_paths, initialized_classifiers) {
      }
      virtual float predict(CvBoost *detector, cv::Mat & frame){
        #ifdef DEBUG
        //cout<<"DEBUG: prediction with CvBoost at "<<detector<<", Frame r="<<frame.rows<<" c="<<frame.cols<<",t="<<frame.type()<<endl;
        #endif
        float pred=detector->predict(frame, Mat(), Range::all(), false, false);  
        return pred;
      }
  };

}


#endif

