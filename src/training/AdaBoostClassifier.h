/**
 * @class   AdaBoostClassifier
 *
 * @brief   Classifier specialization using AdaBoost
 *
 * @details
 *
 */

#ifndef  __ADABOOSTCLASSIFIER_INC__
#define  __ADABOOSTCLASSIFIER_INC__

#include "Classifier.h"

#include <opencv2/opencv.hpp>

namespace emotime {

  class AdaBoostClassifier : public Classifier {

    public:

      AdaBoostClassifier(int boost_type, double trim_weight, double max_depth);
      ~AdaBoostClassifier();

      bool save(std::string outfile);

      bool load(std::string infile);

      float predict(cv::Mat& img);

    protected:

      bool do_training(cv::Mat train_data, cv::Mat train_labels);

    private:

      /// The opencv implementation of AdaBoost
      CvBoost boost;
      /// Adaboost params
      CvBoostParams params;

  };

}

#endif   /* ----- #ifndef __ADABOOSTCLASSIFIER_INC__  ----- */
