/**
 *
 * @file    EmoDetector.h
 * @brief   Contains definition of EmoDetector and Emotion
 *
 */

#ifndef _H_EMO_DETECTOR
#define _H_EMO_DETECTOR

#include "matrix_io.h"
#include "string_utils.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <limits>

#include <opencv2/opencv.hpp>
#include "Classifier.h"

using namespace cv;

namespace emotime {

  /// Emotion identification
  enum Emotion {
    /// NEUTRAL emotion
    NEUTRAL = 0,
    /// ANGER emotion
    ANGER = 1,
    /// CONTEMPT emotion
    CONTEMPT = 2,
    /// DISGUST emotion
    DISGUST = 3,
    /// FEAR emotion
    FEAR = 4,
    /// HAPPY emotion
    HAPPY = 5,
    /// SADNESS emotion
    SADNESS = 6,
    /// SURPRISE emotion
    SURPRISE = 7,
    /// OTHERS emotion
    OTHERS = 8,
    /// UNKNOWN emotion
    UNKNOWN = 9
  };

  /**
   * @brief Retrieve the string associated with an emotion
   *
   * @param[in] emo The enum value
   *
   * @return  A string representation
   */
  std::string emotionStrings(Emotion emo);

  /**
   * @class   EmoDetector
   *
   * @brief   Generic class for performing multi-class classification using
   *          binary classifiers.
   *
   * @details
   *
   */
  class EmoDetector {

    public:

      EmoDetector();

      /**
       *  @brief          Initialize the detectors in extended mode (one detector multiple emotion)
       *
       *  @param[in]      detmap_ext A map to name -> (vector<emotion>, detector)
       *
       */
      EmoDetector(std::map<std::string, std::pair<std::vector<Emotion>, Classifier*> >& detmap_ext);

      /**
       *  @brief          Initialize the detector from classifier paths
       *
       *  @param[in]      classifiers_path  Path of various classifier files.
       *
       *  @details        Path must be in the format: emop1_emop2_emopn_vs_emon1_emon2_emonm.xml
       *                  Where emop* is the emotion recognized and emon* is the emotion not recognized.
       *
       */
      void init(std::vector<std::string>& classifiers_path);

      /**
       * Release an EmoDetector
       */
      virtual ~EmoDetector();

      /**
       * Return true if the given emotion is present
       *
       * @param[in]  name  The name to search
       *
       * @return true if the given emotion is present
       *
       */
      bool contains(std::string& name);

      /**
       *  @brief          Predict the emotion using the extended classification.
       *
       *  @param[in]      frame The image to predict
       *
       *  @return         The prediction and its result
       *
       *  @details
       */
      std::pair<Emotion, float> predictVotingOneVsAllExt(cv::Mat& frame);

      /**
       *  @brief          Predict the emotion using the best-wins approach
       *
       *  @param[in]      frame The image to predict
       *
       *  @return         The prediction and its result
       *
       *  @details
       */
      std::pair<Emotion, float> predictBestWinsOneVsAll(cv::Mat& frame);

      /**
       * @brief     Predict the class of the sample frame using a majority voting strategy.
       *
       *  @param[in]      frame The image to predict
       *
       *  @return         The prediction and its result
       *
       *  @details
       *
       */
      std::pair<Emotion, float> predictMajorityOneVsAll(cv::Mat& frame);

      /**
       * @brief Apply the default prediction method
       *
       * @param[in] frame The image to predict
       *
       * @return The predicted emotion
       */
      virtual std::pair<Emotion, float> predict(cv::Mat& frame);

    protected:

      /**
       *  @brief          Instantiate a classifier
       *
       *  @return         A new classifier
       *
       */
      virtual Classifier* createClassifier() = 0;

    private:

      /// Detectors for generic approaches (each detector matches one or more emotion)
      std::map<std::string, std::pair<std::vector<Emotion>, Classifier*> > detectors_ext;

      /**
       *  @brief          Initialize the detectors in extended mode (one detector multiple emotion)
       *
       *  @param[in]      detmap_ext A map to name -> (vector<emotion>, detector)
       *
       */
      void init(const std::map<std::string, std::pair<std::vector<Emotion>, Classifier*> >& detmap_ext);

  }; // end of EmoDetector
}

#endif
