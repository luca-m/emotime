/**
 *
 * @file    FacePreProcessor.h
 * @brief   Definition of FacePreProcessor
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

  /**
   * @class   FacePreProcessor
   *
   * @brief   Class for preprocessing a face before training.
   *
   * @details
   *
   */
  class FacePreProcessor{

    public:


      /**
       *  @brief          Creates a FacePreProcessor specifying FaceDetector and GaborBank parameters.
       *
       *  @param[in]      faceDetectorConfig  The configuration file for face detection
       *  @param[in]      eyesDetectorConfig  The configuration file for eye
       *                  detection. If the value is "none", the eye
       *                  configuration is ignored.
       *  @param[in]      width Resized with of the face
       *  @param[in]      height Resized height of the face
       *  @param[in]      nwidths nwidhts parameter of GaborBank::fillGaborBank
       *  @param[in]      nlambdas nlambdas parameter of GaborBank::fillGaborBank
       *  @param[in]      nthetas nthetas parameter of GaborBank::fillGaborBank
       *
       *  @see  FaceDetector
       *  @see  GaborBank
       *
       */
      FacePreProcessor(std::string faceDetectorConfig, std::string eyesDetectorConfig,
          int width, int height, double nwidths, double nlambdas, double
          nthetas);

      /**
       *  @brief          Creates a FacePreProcessor specifying FaceDetector
       *                  and GaborBank parameters. This constructor disables the eye detection.
       *
       *  @param[in]      faceDetectorConfig  The configuration file for face detection
       *  @param[in]      width Resized with of the face
       *  @param[in]      height Resized height of the face
       *  @param[in]      nwidths nwidhts parameter of GaborBank::fillGaborBank
       *  @param[in]      nlambdas nlambdas parameter of GaborBank::fillGaborBank
       *  @param[in]      nthetas nthetas parameter of GaborBank::fillGaborBank
       *
       *  @see  FaceDetector
       *  @see  GaborBank
       *
       */
      FacePreProcessor(std::string faceDetectorConfig, int width, int height, double
          nwidths, double nlambdas, double nthetas);

      ~FacePreProcessor();

      /**
       *  @brief          Extract the features from the given image
       *
       *  @param[in]      src The image to analyze
       *  @param[in,out]  out The feature vector
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
      bool extractFace(cv::Mat& src, cv::Mat& out);

      /**
       *  @brief          Filter the given image
       *
       *  @param[in]      src The image to filter
       *  @param[in,out]  out The image to set
       *
       *  @return         True if the image was filtered
       *
       */
      bool filterImage(cv::Mat& src, cv::Mat& out);

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
      bool toFeaturesVector(cv::Mat& src, cv::Mat& out);

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

