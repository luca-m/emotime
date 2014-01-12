/**
 *
 * @file    GaborBank.cpp
 *
 * @date    01/09/2014 11:50:56 PM
 * @brief
 *
 * @details
 *
 */

#include "GaborBank.h"

using std::max;
using std::fabs;

using cv::Size;
using cv::Mat;

namespace emotime {

  GaborBank::~GaborBank() {
    this->emptyBank();
  }

  void GaborBank::emptyBank() {
    for (std::vector<GaborKernel*>::iterator it = this->bank.begin(); it != this->bank.end(); ++it) {
      delete *it;
    }
  }

  GaborKernel* GaborBank::generateGaborKernel(cv::Size ksize, double sigma, double
      theta, double lambd, double gamma, double psi, int ktype) {

    double sigma_x = sigma;
    double sigma_y = sigma/gamma;
    int nstds = 3;
    int xmin, xmax, ymin, ymax;
    double c = cos(theta), s = sin(theta);

    if (ksize.width > 0) {
      xmax = ksize.width / 2;
    } else {
      xmax = cvRound(max(fabs(nstds * sigma_x * c), fabs(nstds * sigma_y * s)));
    }

    if (ksize.height > 0) {
      ymax = ksize.height /2;
    } else {
      ymax = cvRound(max(fabs(nstds * sigma_x * s), fabs(nstds * sigma_y * c)));
    }

    xmin = -xmax;
    ymin = -ymax;
    CV_Assert( ktype == CV_32F || ktype == CV_64F );

    Mat kernel_real(ymax - ymin + 1, xmax - xmin + 1, ktype);
    Mat kernel_img(ymax - ymin + 1, xmax - xmin + 1, ktype);

    double scale = 1;
    double ex = -0.5 / (sigma_x * sigma_x);
    double ey = -0.5 / (sigma_y * sigma_y);
    double cscale = CV_PI * 2 / lambd;

    for (int y = ymin; y <= ymax; y++) {
      for (int x = xmin; x <= xmax; x++) {
        double xr = x*c + y*s;
        double yr = -x*s + y*c;
        double v_real = 0.0;
        double v_img = 0.0;
        v_real = scale * exp(ex * xr * xr + ey * yr * yr) * cos(cscale * xr + psi);
        v_img = scale * exp(ex * xr * xr + ey * yr * yr) * sin(cscale * xr + psi);
        if (ktype == CV_32F) {
          kernel_real.at<float>(ymax - y, xmax - x) = (float) v_real;
          kernel_img.at<float>(ymax - y, xmax - x) = (float) v_img;
        } else {
          kernel_real.at<double>(ymax - y, xmax - x) = v_real;
          kernel_img.at<double>(ymax - y, xmax - x) = v_img;
        }
      }
    }
    return new GaborKernel(kernel_real, kernel_img);
  }

  void GaborBank::fillGaborBank(double nwidths, double nlambdas, double nthetas) {

    this->emptyBank();

    double _sigma;        /// Sigma of the Gaussian envelope
    double _theta;        /// Orientation of the normal to the parallel stripes of the Gabor function
    double _lambda;       /// Wavelength of sinusoidal factor
    double _gamma;        /// Spatial aspect ratio (ellipticity of the support of the Gabor function)
    double _psi;          /// Phase offset
    _gamma = sqrt(2);
    _sigma = 4.0;
    _lambda = sqrt(2);
    _psi=0;
    int fwidth;
    int minfwidth = kGaborWidthMin;
    int maxfwidth = kGaborWidthMax;


//    for (fwidth = minfwidth; fwidth < maxfwidth; fwidth += (int)
//        ((maxfwidth-minfwidth)/((double)(nwidths<=0?1:nwidths)))) {
//      cv::Size kernelSize(fwidth, fwidth);

      for (_lambda = kGaborLambdaMin; _lambda < kGaborLambdaMax;
          _lambda += (kGaborLambdaMax-kGaborLambdaMin)/((double)(nlambdas<=0?1:nlambdas))) {
        //    for (int j = 0; j < kGaborPaperLamdasLen; j++) {
        //      _lambda = kGaborPaperLambdas[j];

    float octave = 1;
    _sigma = _lambda * 1/M_PI * sqrt(log(2)/2) * (pow(2,octave)+1)/(pow(2,octave)-1);


    float limit = 0.0005;
    // x = sqrt( -2 * sigm^2 * ln(limit * sqrt(2pi * sigm^2)))
    float factor = 2.5066282746310002 * _sigma;
    int x = ceil(_sigma * 1.4142135623730951 * sqrt(-log(limit * factor)));

    if(x % 2 == 1) x++;
    cv::Size kernelSize(2*x + 1, 2*x + 1);

        for (_theta = kGaborThetaMin; _theta < kGaborThetaMax;
            _theta += (kGaborThetaMax - kGaborThetaMin)/((double)(nthetas<=0?1:nthetas))) {

          emotime::GaborKernel* kern = this->generateGaborKernel(kernelSize,
              _sigma, _theta, _lambda, _gamma, _psi, CV_32F);
          bank.push_back(kern);
        }
      }
//      }
    }

    void GaborBank::fillDefaultGaborrBank() {
      this->fillGaborBank(kGaborDefaultNwidth, kGaborDefaultNtheta,
          kGaborDefaultNlambda);
    }

    Size GaborBank::getFilteredImgSize(Mat & src) {
      // The output image will contain all the filtered image vertically stacked.
      Size s = Size(0,0);
      s.height = src.rows * bank.size();
      s.width = src.cols;
      return s;
    }

    Mat GaborBank::filterImage(Mat& src) {

      Size size(0,0);
      unsigned int i;
      unsigned int j;
      unsigned int k;

      Size s = this->getFilteredImgSize(src);
      Mat * dest = new Mat(s, CV_32F);
      Mat image;
      size.height=src.rows;
      size.width=src.cols;
      src.convertTo(image, CV_32F);

      for (k = 0; k < bank.size(); k++) {
        emotime::GaborKernel * gk = bank.at(k);
        Mat real = gk->getReal();
        Mat imag = gk->getImag();
        Mat freal = Mat(size,CV_32F);
        Mat fimag = Mat(size,CV_32F);
        Mat magn  = Mat(size,CV_32F);
        filter2D(src, freal, CV_32F, real);
        filter2D(src, fimag, CV_32F, imag);
        // Calculating Gabor magnitude
        pow(freal,2,freal);
        pow(fimag,2,fimag);
        add(fimag,freal,magn);
        sqrt(magn,magn);
        // Write all the filtered image vertically stacked.
        // TODO: use image1.copyTo ( Mat ( display, Rect(y,x,h,w) ) )
        for (i = 0; i<(unsigned int)size.height; i++) {
          for (j = 0; j<(unsigned int)size.width; j++) {
            dest->at<float>(i + (k * size.height), j) = magn.at<float>(i,j);
          }
        }
      }
      return *dest;
    }
  }
