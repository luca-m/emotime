/**
 *
 * @file    SVMEmoDetector.h
 * @brief   Defines SVMEmoDetector
 *
 */

#ifndef  __SVMEMODETECTOR_INC__
#define  __SVMEMODETECTOR_INC__

#include "EmoDetector.h"

namespace emotime {

  /**
   * @class    SVMEmoDetector
   * @date    12/27/2013 11:59:26 AM
   *
   * @brief   EmoDetector specialization using SVMClassifier
   *
   */
  class SVMEmoDetector : public EmoDetector {
    public:

      /**
       *  @brief          Creates an SVMEmoDetector with svm parameters and
       *                  empty classifiers.
       *
       *  @param[in]      C_factor  The algorithm C factor
       *  @param[in]      max_iteration Maximum number of iteration termination criteria
       *  @param[in]      error_margin Minimum error termination criteria
       *
       *  @see SVMClassifier
       */
      SVMEmoDetector(double C_factor, int max_iteration, double error_margin);

      /**
       *  @brief          Creates an SVMEmoDetector with svm parameters and
       *                  classifiers.
       *
       *  @param[in]      C_factor  The algorithm C factor
       *  @param[in]      max_iteration Maximum number of iteration termination criteria
       *  @param[in]      error_margin Minimum error termination criteria
       *  @param[in]      detmap_ext Mapping between emotions and classifier.
       *
       *  @see SVMClassifier
       */
      SVMEmoDetector(double C_factor, int max_iteration, double error_margin,
          std::map<std::string, std::pair<vector<Emotion>, Classifier*> >
          detmap_ext);

      std::pair<Emotion, float> predict(cv::Mat& frame, std::map<Emotion,float> &votes);

    protected:

      Classifier* createClassifier();

    private:

      /// The algorithm C factor
      double C_factor;
      /// Maximum number of iteration termination criteria
      int max_iteration;
      /// Minimum error termination criteria
      double error_margin;


  }; /* -----  end of class SVMEmoDetector  ----- */

}


#endif   /* ----- #ifndef __SVMEMODETECTOR_INC__  ----- */
