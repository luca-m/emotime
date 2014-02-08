/**
 *
 * @file    AdaBoostClassifier.h
 * @brief   Definition of AdaBoostClassifier
 *
 */

#ifndef  __ADABOOSTCLASSIFIER_INC__
#define  __ADABOOSTCLASSIFIER_INC__

#include "Classifier.h"

#include <opencv2/opencv.hpp>

namespace emotime {

  /**
   * @class   AdaBoostClassifier
   *
   * @brief   Classifier specialization using AdaBoost
   *
   * @details
   *
   */
  class AdaBoostClassifier : public Classifier {

    public:


      /**
       *  @brief          Create a new AdaBoostClassifier specifying opencv
       *                  boost type, trim weight and max depth
       *
       *  @param[in]      boost_type Type of the opencv boosting algorithm
       *  @param[in]      trim_weight The opencv trim weight value
       *  @param[in]      max_depth Algorithm max depth
       *
       *
       *  @see CvBoostParams::CvBoostParams
       */
      AdaBoostClassifier(int boost_type, double trim_weight, double max_depth);
      ~AdaBoostClassifier();

      bool save(std::string outfile);

      bool load(std::string infile);

      float predict(cv::Mat& img);

    protected:

      bool doTraining(cv::Mat train_data, cv::Mat train_labels);

    private:

      /// The opencv implementation of AdaBoost
      CvBoost boost;
      /// Adaboost params
      CvBoostParams params;

  };

}

#endif   /* ----- #ifndef __ADABOOSTCLASSIFIER_INC__  ----- */
