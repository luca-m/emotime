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

#ifndef  __SVMEMODETECTOR_INC__
#define  __SVMEMODETECTOR_INC__

#include "EmoDetector.h"

namespace emotime {

  class SVMEmoDetector : public EmoDetector {
    public:

      SVMEmoDetector(double C_factor, int max_iteration, double error_margin);

      SVMEmoDetector(double C_factor, int max_iteration, double error_margin,
          std::map<std::string, std::pair<vector<Emotion>, Classifier*> >
          detmap_ext);

      std::pair<Emotion, float> predict(cv::Mat& frame);

    protected:

      Classifier* create_classifier();

    private:

      double C_factor;
      int max_iteration;
      double error_margin;


  }; /* -----  end of class SVMEmoDetector  ----- */

}


#endif   /* ----- #ifndef __SVMEMODETECTOR_INC__  ----- */
