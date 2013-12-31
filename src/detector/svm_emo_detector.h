/**
  * @class    SVMEmoDetector
  * @ingroup  <+group+>
  * @author   Daniele Bellavista (daniele.bellavista@studio.unibo.it)
  * @date    12/27/2013 11:59:26 AM
  *
  * @brief   <+brief+>
  *
  * @details <+description+>
  *
  */

#ifndef  __SVM_EMO_DETECTOR_INC__
#define  __SVM_EMO_DETECTOR_INC__

#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include "emo_detector.hpp"

namespace emotime
{

  class SVMEmoDetector : public EmoDetector<CvSVM>
  {
    public:
      SVMEmoDetector() : EmoDetector()
      {

      }
      SVMEmoDetector(std::map<std::string, std::pair<Emotion, CvSVM*> > detmap)
        : EmoDetector(detmap)
      {

      }
      SVMEmoDetector(std::map<std::string, std::pair<vector<Emotion>, CvSVM*> > detmap_ext)
        : EmoDetector(detmap_ext)
      {

      }
      SVMEmoDetector(vector<string> classifier_paths, vector<CvSVM*>
          initialized_classifiers) : EmoDetector(classifier_paths,
            initialized_classifiers)
      {

      }

    protected:
      float predict(CvSVM* detector, cv::Mat& frame);

    private:


  }; /* -----  end of class SVMEmoDetector  ----- */

}


#endif   /* ----- #ifndef __SVM_EMO_DETECTOR_INC__  ----- */
