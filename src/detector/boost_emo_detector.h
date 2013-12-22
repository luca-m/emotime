#ifndef _H_BOST_EMO_DETECTOR
#define _H_BOST_EMO_DETECTOR 

#include "emo_detector.hpp"
#include "facedetector.h"

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
      virtual float predict(CvBoost &detector, cv::Mat & frame){
        return detector.predict(frame, Mat(), Range::all(), false, false);  
      }
  };

}


#endif

