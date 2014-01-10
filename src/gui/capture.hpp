/*
 * capture.hpp
 */
#ifndef CAPTURE_HPP
#define CAPTURE_HPP

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

namespace emotime{

  /**
   * Abstract capture class with support to grayscale conversion.
   * */
  class ACapture{
    private:
      bool toGrayscale;
    protected:
      virtual bool extractFrame(Mat & frm)=0;
    public:
      ACapture(bool toGray){
        toGrayscale=toGray;
      };
      virtual ~ACapture(){};
      virtual bool isReady() = 0;
      bool nextFrame(Mat & frame){
        Mat frm;
        bool hasNext;
        hasNext=extractFrame(frm);
        if (hasNext){
          if (toGrayscale && frm.channels()>2){
		        Mat gray(frm.size(), CV_8UC1);
		        cvtColor(frm, gray, CV_BGR2GRAY);
            gray.copyTo(frame);
		      } else {
            frm.copyTo(frame);
          }
          return true;
        }
        return false;
      }  
  };
  /**
   * Video Capture class. 
   * Enable capturing video frames from devices or video files. 
   * */
  class Video: public ACapture{
    private:
      VideoCapture cap;
    protected: 
      bool extractFrame(Mat & frm){
        return cap.read(frm); 
      }
    public:
      /**
       * Open a video from input device (eg. Camera)
       * */
      Video(int deviceID, bool grayScale): ACapture(grayScale) {
        cap.open(deviceID);
      }
      /**
       * Open a video from video file.
       * */
      Video(string infile, bool grayScale): ACapture(grayScale) {
        cap.open(infile.c_str());
      }
      ~Video(){
       cap.release(); 
      }
      bool isReady(){
        if (cap.isOpened()){
          return true;
        } else {
          return true;
        }
      }
  };
  /**
   * Capture frames from a single image.
   * */
  class Image: public ACapture {
    private:
      Mat image;
    protected:
      bool extractFrame(Mat & frm){
        if (isReady()){
          image.copyTo(frm);
          return true;
        }
        return false; 
      }
    public:
      /**
       * Load an image from file.
       * */
      Image(string infile, bool grayScale): ACapture(grayScale) {
        image=imread(infile.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
      }
      bool isReady(){
        return image.cols>0 && image.rows>0;
      }
  };



  /**
   * @class    Webcam
   * @author   Daniele Bellavista (daniele.bellavista@studio.unibo.it)
   * @date    12/31/2013 10:22:15 AM
   *
   * @brief   Capture implementation using a Webcam
   *
   * @details
   *
   */
  class Webcam : public ACapture {

    public:


      /**
       *  @brief          Webcam costructor
       *
       *  @param[in]      to_grey If true, each photogram will be converted to greyscale
       *
       */
      Webcam(bool to_grey) : ACapture(to_grey) {
        this->capture = cvCaptureFromCAM(CV_CAP_ANY);
      }

    protected:

      bool extractFrame(Mat& frm) {
        IplImage* frame = cvQueryFrame(capture);
        if(frame == NULL) {
          return false;
        }
        Mat(frame).copyTo(frm);
        return true;
      }

      bool isReady() {
        return true;
      }

    private:
      /// The opencv webcam capture
      CvCapture* capture;
  };
}


#endif /* !CAPTURE_HPP */

