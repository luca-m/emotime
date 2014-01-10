/**
 * @class    BoostEmoDetector
 * @ingroup  <+group+>
 *
 * @date    12/27/2013 11:59:26 AM
 *
 * @brief   <+brief+>
 *
 * @details <+description+>
 *
 */

#ifndef _H_BOST_EMO_DETECTOR
#define _H_BOST_EMO_DETECTOR 

#include "EmoDetector.h"

namespace emotime{

  class BoostEmoDetector : public EmoDetector {
    public:

      BoostEmoDetector(int boost_type, double trim_weight, double max_depth);

      BoostEmoDetector(int boost_type, double trim_weight, double max_depth,
          std::map<std::string, std::pair<vector<Emotion>, Classifier*> >
          detmap_ext);

      std::pair<Emotion, float> predict(cv::Mat& frame);

    protected:

      Classifier* createClassifier();

    private:

      int boost_type;
      double trim_weight;
      double max_depth;
  };

}

#endif
