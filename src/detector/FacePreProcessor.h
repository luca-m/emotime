/**
 * @class   FacePreProcessor
 *
 * @brief   Class for preprocessing a face before training.
 *
 * @details
 *
 */

#ifndef FACEPREPROCESSOR_H
#define FACEPREPROCESSOR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <limits>

#include <opencv2/opencv.hpp>

#include "FaceDetector.h"
#include "GaborBank.h"

namespace emotime{

  class FacePreProcessor{

    public:

      FacePreProcessor(std::string faceDetectorConfig, std::string eyesDetectorConfig,
          int width, int height, double nwidths, double nlambdas, double
          nthetas);

      FacePreProcessor(std::string faceDetectorConfig, int width, int height, double
          nwidths, double nlambdas, double nthetas);

      ~FacePreProcessor();

      /**
       *  @brief          Extract the features from the given image
       *
       *  @param[in]      src The image to analyze
       *  @param[in.out]  out The feature vector
       *
       *  @return         True if the operation succedede
       *
       */
      bool preprocess(cv::Mat& src, cv::Mat& out);

      /**
       *  @brief          Extract the face from the given image
       *
       *  @param[in]      src The image to analyze
       *  @param[in,out]  out The image to set
       *
       *  @return         True if the face was extracted
       *
       */
      bool extract_face(cv::Mat& src, cv::Mat& out);

      /**
       *  @brief          Filter the given image
       *
       *  @param[in]      src The image to filter
       *  @param[in,out]  out The image to set
       *
       *  @return         True if the image was filtered
       *
       */
      bool filter_image(cv::Mat& src, cv::Mat& out);

    private:

      /// Face detector utility
      FaceDetector facedet;
      /// Gabor bank to use
      GaborBank gaborbank;
      /// The resize measure
      cv::Size imgsize;
      /// Gabor param: nwidths
      double nwidths;
      /// Gabor param: nlambdas
      double nlambdas;
      /// Gabor param: nthetas
      double nthetas;

      /**
       *  @brief          Convert the given filtered image to a feature vector,
       *                  suitable for training.
       *
       *  @param[in]      src The filtered image
       *  @param[in,out]  out The vector to set
       *
       *  @return         True if the operation succeded
       *
       */
      bool to_features_vector(cv::Mat& src, cv::Mat& out);

      /**
       *  @brief          Initialize the FacePreProcessor
       *
       *  @param[in]      faceDetectorConfig  Config file for face detection
       *  @param[in]      eyeDetectorConfig   Config file for eye detection. If
       *                                      empty, no eye detection will be performed.
       *  @param[in]      width               The width resize
       *  @param[in]      height               The height resize
       *  @param[in]      nwidths               N-Width gabor param
       *  @param[in]      nlabmdas               N-Width gabor param
       *  @param[in]      nthetas               N-Width gabor param
       *
       */
      void init(std::string faceDetectorConfig, std::string eyesDetectorConfig, int
          width, int height, double nwidths, double nlambdas, double nthetas);
  };

}
#endif /* !FACEPREPROCESSOR_H */

