#ifndef _H_BOST_EMO_DETECTOR
#define _H_BOST_EMO_DETECTOR 

#include "emo_detector.hpp"
#include <opencv2/ml/ml.hpp>
using namespace std;
using namespace cv;
using namespace emotime;

namespace emotime{

  class BoostEmoDetector: public EmoDetector<CvBoost>{
    public:
      BoostEmoDetector() : EmoDetector(){
      }
      BoostEmoDetector(std::map<std::string, std::pair<Emotion, CvBoost*> > detmap)
        : EmoDetector(detmap) {
      }
      BoostEmoDetector(std::map<std::string, std::pair<vector<Emotion>, CvBoost*> > detmap_ext) : EmoDetector(detmap_ext){
      }
      BoostEmoDetector(vector<string> classifier_paths, vector<CvBoost*> initialized_classifiers) : EmoDetector(classifier_paths, initialized_classifiers){
      }

      pair<Emotion, float> predict(cv::Mat& frame);

    protected:
      float predict(CvBoost* detector, cv::Mat& frame);

    private:
  
  };

}


#endif

