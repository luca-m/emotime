/**
 *
 * @file    FaceDetector.h
 *
 * @date    01/10/2014 12:26:04 AM
 * @brief   Defintion of FaceDetector
 *
 * @details
 *
 */

#ifndef FACEDETECTOR_H_
#define FACEDETECTOR_H_

//#define TRAINING_BUILD

#include <opencv2/opencv.hpp>

namespace emotime {

  /// Performance saving constant. Limit the dimension of the face retrieved. (NOT GOOD FOR TRAINING)
  const cv::Size kFaceSizeLimit = cv::Size(128,128);

  #ifdef TRAINING_BUILD
  /// Maximum rotation allowed for eye correction
  const float kMaxRotationAngle = 10.0f;
  #else
  /// Maximum rotation allowed for eye correction
  const float kMaxRotationAngle = 20.0f;
  #endif

  /**
   * @class   FaceDetector
   *
   * @brief   Face detector using Cascade classifier for face and eye.
   *
   * @details
   *
   */
  class FaceDetector {

    public:

      /**
       *  @brief          Creates a face detector with both face and eye
       *                  detection.
       *
       *  @param[in]      face_config_file  The configuration file for face detection
       *  @param[in]      eye_config_file  The configuration file for eye
       *                  detection. If the value is "none", the eye
       *                  configuration is ignored.
       *
       */
      FaceDetector(std::string face_config_file, std::string eye_config_file);

      /**
       *  @brief          Creates a face detector
       *  @param[in]      face_config_file  The configuration file for face detection
       *
       */
      FaceDetector(std::string face_config_file);

      /**
       *  @brief          Create an empty face detector. Should NOT be used!
       *
       */
      FaceDetector();

      virtual ~FaceDetector();

      /**
       * @brief Detect the face and adjust roll basing on eye position.
       *
       * @param[in]     img  The image to analyze
       * @param[in,out] face The resulting face
       *
       * @return True if a face was identified
       *
       * @details  If the eye detector was loaded and doEyesRot
       *           is true, then the face is rotated basing on eye position to
       *           compensate roll.
       *
       * @see detectFace
       * @see detectEyes
       *
       */
      virtual bool detect(cv::Mat& img, cv::Mat& face);

    protected:

      /// Cascade classifier for face
      cv::CascadeClassifier cascade_f;
      /// Cascade classifier for eyes
      cv::CascadeClassifier cascade_e;
      /// Minimum size of the face
      cv::Size faceMinSize;
      /// Perform the eye rotation?
      bool doEyesRot;

      /**
       * @brief Detect a face, setting a region with the coordinate of the first face found.
       *
       * @param[in]     img   The image to analyze
       * @param[in,out] face  The rectangle surrounding the face
       *
       * @return  True if a face was identified
       */
      virtual bool detectFace(cv::Mat& img, cv::Rect& face);

      /**
       * @brief Detects two eyes, returning their points
       *
       * @param[in]     img   The image to analyze
       * @param[in,out] eye1  First eye coordinates
       * @param[in,out] eye2  Second eye coordinates
       *
       * @return  True if the eyes were identified
       */
      virtual bool detectEyes(cv::Mat& img, cv::Point& eye1, cv::Point& eye2);

  };

} /* namespace facecrop */

#endif /* FACEDETECTOR_H_ */
