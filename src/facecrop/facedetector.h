/*
 * facedetector.h
 *
 *  Created on: Jul 23, 2013
 *      Author: stk
 */

#ifndef FACEDETECTOR_H_
#define FACEDETECTOR_H_

#include <opencv2/opencv.hpp>

using namespace cv;

namespace emotime{

class FaceDetector{

protected:
	CascadeClassifier cascade_f;
	CascadeClassifier cascade_e;
  bool doEyesRot;
  /**
	 * \brief Set face area of a face region with the coordinate of the first face found.
	 * @param img
	 * @param faceRegion
	 * @return
	 */
	virtual bool detectFace(Mat & img, Rect & faceRegion);
	virtual bool detectEyes(Mat & img, Point & eye1, Point & eye2);

public:
	FaceDetector(string face_config_file, string eye_config_file);
	FaceDetector(string face_config_file);
  FaceDetector();
	virtual ~FaceDetector();
	/**
	 *
	 * @param img
	 * @param face
	 * @return
	 */
  virtual bool detect(Mat & img , Mat & face);
};

} /* namespace facecrop */

#endif /* FACEDETECTOR_H_ */
