/**
 * @class    Classifier
 *
 * @brief   Represent a general classifier method, capable of training, saving
 *          and loading the status and predicting a class.
 *
 * @details
 *
 */

#ifndef  __CLASSIFIER_INC__
#define  __CLASSIFIER_INC__

#include <opencv2/opencv.hpp>
#include <string>

namespace emotime {

  class Classifier {

    public:
      Classifier();
      virtual ~Classifier();


      /**
       *  @brief          Train the classifier using the given csv file
       *
       *  @param[in]      csv_file  A csv file in the training format
       *
       *  @return         True if the training was successful
       *
       */
      virtual bool train(std::string csv_file);

      /**
       *  @brief          Save the trained classifier status in the given file
       *
       *  @param[in]      outfile Where to save the status
       *
       *  @return         True if the save operation succeded
       */
      virtual bool save(std::string outfile) = 0;

      /**
       *  @brief          Load a trained classifier status from the given file
       *
       *  @param[in]      infile  The file to load
       *
       *  @return         True if the load operation succeded
       */
      virtual bool load(std::string infile) = 0;

      /**
       *  @brief          Predict the given image
       *
       *  @param[in]      features  The features to predict
       *
       *  @return         A value between 0 and 1 indicating the proxximity to
       *                  a class.
       */
      virtual float predict(cv::Mat& features) = 0;

    protected:


      /**
       *  @brief          Load the training data from a CSV
       *
       *  @param[in]        csv_file The file to load
       *  @param[in,out]    sample_files_classes    Filled with the sample files path and their class
       *
       *  @return         True if the parsing was correct
       *
       *  @details
       */
      bool loadTrainData(std::string csv_file, std::vector<std::pair<std::string, int> >&
          sample_files_classes);


      /**
       *  @brief          Perform the real training given files and related classes
       *
       *  @param[in]      train_data The matrix containing the training data
       *  @param[in]     train_labels The matrix containing the data labels
       *
       *  @return         True if the training succeded
       *
       *  @details
       */
      virtual bool doTraining(cv::Mat train_data, cv::Mat train_labels) = 0;

    private:

  }; /* -----  end of class Classifier  ----- */

}


#endif   /* ----- #ifndef __CLASSIFIER_INC__  ----- */
