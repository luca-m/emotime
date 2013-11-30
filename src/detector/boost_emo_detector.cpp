#include "boost_emo_detector.h"
#include "adapredict.h"

using namespace std;
using namespace cv;
using namespace emotime;

float boost_predict(CvBoost & detector, Mat & frame){
  return detector.predict(frame, Mat(), Range::all(), false, false);  
}

EmoDetector<CvBoost> boost_EmoDetector_create( map<string, pair<Emotions, CvBoost> >  detectors);
  EmoDetector<CvBoost> emodet= EmoDetector<CvBoost>( detectors, &boost_predict);
  pair<Emotions,CvBoost> entry;
  return emodet;
}

