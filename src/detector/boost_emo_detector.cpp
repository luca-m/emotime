#include "boost_emo_detector.h"
#include "adapredict.h"
#include "gaborbank.h" 

using namespace std;
using namespace cv;
using namespace emotime;

vector<struct GaborKern*> boost_emo_detector_bank;
Size boost_emo_detector_size (0,0);
FaceDetector * boost_emo_facedetector=NULL;

float boost_predict_no_preprocess(CvBoost & detector, Mat & frame){
 return detector.predict(frame, Mat(), Range::all(), false, false);  
}
float boost_predict(CvBoost & detector, Mat & frame){
  return adapredict_predict(detector, 
                            boost_emo_detector_bank, 
                            frame, 
                            boost_emo_detector_size, 
                            boost_emo_facedetector);
}

EmoDetector<CvBoost> boost_EmoDetector_create_no_preprocess( map<string, pair<Emotion, CvBoost *> > & detectors){
  EmoDetector<CvBoost> emodet = EmoDetector<CvBoost>(detectors, &boost_predict_no_preprocess);
  return emodet;
} 

EmoDetector<CvBoost> boost_EmoDetector_create( map<string, pair<Emotion, CvBoost *> > & detectors, 
                            Size size, double nwidths, double nlambdas, double nthetas, FaceDetector * facedetector){

  gaborbank_getCustomGaborBank(boost_emo_detector_bank, nwidths, nlambdas, nthetas);
  boost_emo_detector_size.width=size.width;
  boost_emo_detector_size.height=size.height; 
  boost_emo_facedetector=facedetector;
  EmoDetector<CvBoost> emodet = EmoDetector<CvBoost>(detectors, &boost_predict);
  return emodet;
}

