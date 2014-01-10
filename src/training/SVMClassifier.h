/**
 * @class   SVMClassifier
 *
 * @brief   Classifier specialization using SVM.
 *
 * @details
 *
 */

#ifndef  __SVMCLASSIFIER_INC__
#define  __SVMCLASSIFIER_INC__

#include "Classifier.h"

#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>

namespace emotime {

  class SVMClassifier : public Classifier {

    public:

      SVMClassifier(double C_factor, int max_iteration, double error_margin);
      ~SVMClassifier();

      bool save(std::string outfile);

      bool load(std::string infile);

      float predict(cv::Mat& features);

    protected:

      bool doTraining(cv::Mat train_data, cv::Mat train_labels);

    private:

      /// The opencv implementation of SVM
      CvSVM svm;
      /// The SVM parameters
      CvSVMParams params;

  };

}


#endif   /* ----- #ifndef __SVMCLASSIFIER_INC__  ----- */
