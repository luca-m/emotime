/**
 *
 * @file    gaborbank.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/04/2013 10:50:32 AM
 * @brief   Implementation of gaborbank.h
 *
 * @details 
 *
 */

#include "gaborbank.h"

using namespace cv;
using namespace std;

using emotime::GaborKernel;
using emotime::kGaborWidthMin;
using emotime::kGaborWidthMax;
using emotime::kGaborLambdaMin;
using emotime::kGaborLambdaMax;
using emotime::kGaborThetaMin;
using emotime::kGaborThetaMax;
using emotime::kGaborDefaultNwidth;
using emotime::kGaborDefaultNlambda;
using emotime::kGaborDefaultNtheta;
using emotime::kGaborPaperLambdas;
using emotime::kGaborPaperLamdasLen;

/**
 * @brief Obtain the dimension needed for pre-allocating an image suitable for
 *        being used in gaborbank_filterImage (grayscale image only)
 *
 * @param [in] src     the input image
 * @param [in] bank    the vector of filters kernel
 *
 * @returns The size of the filtered image.
 * */
cv::Size gaborbank_getFilteredImgSize(cv::Mat & src, std::vector<struct GaborKern *> & bank );

Mat gaborbank_getGaborKernel(Size ksize, double sigma, double theta, double
    lambd, double gamma, double psi, int ktype, bool real) {

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
  Mat kernel(ymax - ymin + 1, xmax - xmin + 1, ktype);

  double scale = 1;
  double ex = -0.5 / (sigma_x * sigma_x);
  double ey = -0.5 / (sigma_y * sigma_y);
  double cscale = CV_PI * 2 / lambd;

  for (int y = ymin; y <= ymax; y++) {
    for (int x = xmin; x <= xmax; x++) {
      double xr = x*c + y*s;
      double yr = -x*s + y*c;
      double v = 0.0;
      if (real) {
        v = scale * exp(ex * xr * xr + ey * yr * yr) * cos(cscale * xr + psi);
      } else {
        v = scale * exp(ex * xr * xr + ey * yr * yr) * sin(cscale * xr + psi);
      }
      if (ktype == CV_32F) {
        kernel.at<float>(ymax - y, xmax - x) = (float) v;
      } else {
        kernel.at<double>(ymax - y, xmax - x) = v;
      }
    }
  }
  return kernel;
}

void gaborbank_getCustomGaborBank(std::vector<emotime::GaborKernel *> & bank,
    double nwidths, double nlambdas, double nthetas) {

  #ifdef DEBUG
  cout << "DEBUG: generating gabor bank with nwidths=" << nwidths <<
    ",nlambdas=" << nlambdas << ",nthetas=" << nthetas << endl;
  #endif

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

  for (fwidth = minfwidth; fwidth < maxfwidth; fwidth += (int)
      ((maxfwidth-minfwidth)/((double)(nwidths<=0?1:nwidths)))) {
    cv::Size kernelSize(fwidth, fwidth);

    //for ( _lambda=GABOR_LAMBDA_MIN; _lambda<GABOR_LAMBDA_MAX;
    //    _lambda+=(GABOR_LAMBDA_MAX-GABOR_LAMBDA_MIN)/((double)(nlambdas<=0?1:nlambdas)) ) {
    for (int j = 0; j < kGaborPaperLamdasLen; j++) {
      _lambda = kGaborPaperLambdas[j];
      for (_theta = kGaborThetaMin; _theta < kGaborThetaMax;
          _theta += (kGaborThetaMax - kGaborThetaMin)/((double)(nthetas<=0?1:nthetas)) ) {
        emotime::GaborKernel * kern = new emotime::GaborKernel(gaborbank_getGaborKernel(kernelSize, _sigma, _theta, _lambda, _gamma, _psi, CV_32F, true), gaborbank_getGaborKernel(kernelSize, _sigma, _theta, _lambda, _gamma, _psi, CV_32F, false) );
        bank.push_back(kern);
      }
    }
  }
}

void gaborbank_getGaborBank(std::vector<emotime::GaborKernel *> & bank) {
  gaborbank_getCustomGaborBank(bank, kGaborDefaultNwidth, kGaborDefaultNtheta,
      kGaborDefaultNlambda);
  return;
}

Size gaborbank_getFilteredImgSize(cv::Mat & src, std::vector<emotime::GaborKernel *> & bank) {

  #ifdef DEBUG
  cout << "DEBUG: calculating correct size for filtered image." << endl; 
  #endif

  // The output image will contain all the filtered image vertically stacked.
  Size s = Size(0,0); 
  s.height = src.rows * bank.size();
  s.width = src.cols;
  return s;
}

Mat gaborbank_filterImage(cv::Mat & src, std::vector<emotime::GaborKernel *> & bank) {

  #ifdef DEBUG
  cout << "DEBUG: process the image with bank of " << bank.size() << " gabor filters" << endl;
  #endif

  Size size(0,0);
  unsigned int i;
  unsigned int j;
  unsigned int k;

  Size s = gaborbank_getFilteredImgSize(src,bank);
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
