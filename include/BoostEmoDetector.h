/**
 *
 * @file    BoostEmoDetector.h
 * @date    02/08/2014 11:47:13 AM
 * @brief   Definition of BoostEmoDetector
 *
 */

#ifndef _H_BOST_EMO_DETECTOR
#define _H_BOST_EMO_DETECTOR 

#include "EmoDetector.h"

namespace emotime{

  /**
   * @class    BoostEmoDetector
   *
   * @date    12/27/2013 11:59:26 AM
   *
   * @brief   Emotion detector specialization using AdaBoost
   *
   */
  class BoostEmoDetector : public EmoDetector {
    public:


      /**
       *  @brief          Initialize the emodetector with boost parameters and
       *                  empty classifiers.
       *
       *  @param[in]      boost_type Type of the opencv boosting algorithm
       *  @param[in]      trim_weight The opencv trim weight value
       *  @param[in]      max_depth Algorithm max depth
       *
       *  @see AdaBoostClassifier
       *
       */
      BoostEmoDetector(int boost_type, double trim_weight, int max_depth);

      /**
       *  @brief          Initialize the emodetector with boost parameters and
       *                  classifiers.
       *
       *  @param[in]      boost_type Type of the opencv boosting algorithm
       *  @param[in]      trim_weight The opencv trim weight value
       *  @param[in]      max_depth Algorithm max depth
       *  @param[in]      detmap_ext Mapping between emotions and classifier.
       *
       *  @see AdaBoostClassifier
       *
       */
      BoostEmoDetector(int boost_type, double trim_weight, int max_depth,
          std::map<std::string, std::pair<vector<Emotion>, Classifier*> >
          detmap_ext);

      std::pair<Emotion, float> predict(cv::Mat& frame, std::map<Emotion,float> &votes);

    protected:

      Classifier* createClassifier();

    private:

      /// Type of the opencv boosting algorithm
      int boost_type;
      /// The opencv trim weight value
      double trim_weight;
      /// Algorithm max depth
      int max_depth;
  };

}

#endif
