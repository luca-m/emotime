/**
 * @class   FaceDetector
 *
 * @brief   Face detector using Cascade classifier for face and eye.
 *
 * @details
 *
 */

#ifndef FACEDETECTOR_H_
#define FACEDETECTOR_H_

#include <opencv2/opencv.hpp>

namespace emotime {

  class FaceDetector {

    public:
      FaceDetector(std::string face_config_file, std::string eye_config_file);
      FaceDetector(std::string face_config_file);
      FaceDetector();
      virtual ~FaceDetector();

      /**
       * @brief Detect the face
       *
       * @param[in]     img  The image to analyze
       * @param[in,out] face The resulting face
       *
       * @return True if a face was identified
       *
       */
      virtual bool detect(cv::Mat& img, cv::Mat& face);

    protected:

      /// Cascade classifier for face
      cv::CascadeClassifier cascade_f;
      /// Cascade classifier for eyes
      cv::CascadeClassifier cascade_e;

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
