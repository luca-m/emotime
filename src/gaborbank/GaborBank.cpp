/**
 *
 * @file    GaborBank.cpp
 *
 * @date    01/09/2014 11:50:56 PM
 * @brief   Contains implementation for GaborBank.h
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
    #ifdef GABOR_USE_OPENCV_KERNEL
    Mat r = cv::getGaborKernel(ksize,sigma,theta,lambd,gamma,psi,ktype);
    Mat i = cv::getGaborKernel(ksize,sigma,theta,lambd,gamma,psi+CV_PI/2.0,ktype);
    r=r.mul(1.0/(1.5*sum(r)));
    i=i.mul(1.0/(1.5*sum(i)));
    return new GaborKernel(r,i); 
    #else
    double sigma_x = sigma;             // b scale factor in "Tutorial on Gabor Filters" Javier R. Movellan 
    double sigma_y = sigma/gamma;       // a scale factor in "Tutorial on Gabor Filters" Javier R. Movellan
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
        // rotating the gaussian envelope (xr,yr)
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
    #endif
  }

  void GaborBank::fillGaborBank(double nwidths, double nlambdas, double nthetas) {
    #ifdef GABOR_FORMULA
    fillGaborBankFormula(nwidths,nlambdas,nthetas);
    #else
    fillGaborBankEmpiric(nwidths,nlambdas,nthetas);
    #endif
  }

  void GaborBank::fillGaborBankFormula(double nwidths, double nlambdas, double nthetas){

    this->emptyBank();

    double _sigma;        /// Sigma of the Gaussian envelope
    double _theta;        /// Orientation of the normal to the parallel stripes of the Gabor function
    double _lambda;       /// Wavelength of sinusoidal factor
    double _gamma;        /// Spatial aspect ratio (ellipticity of the support of the Gabor function)
    double _psi;          /// Phase offset
    _gamma = kGaborGamma;
    _sigma = kGaborSigmaMin;
    _psi=kGaborPsi;

    //double bandwidth=1.6;
    double slratio;
    for (double bandwidth=kGaborBandwidthMin; bandwidth<kGaborBandwidthMax;
          bandwidth+=(kGaborBandwidthMax-kGaborBandwidthMin)/((double)(nwidths)) ) {
      slratio = (1./CV_PI)*std::sqrt(std::log(2.0)/2.0)*((std::pow(2,bandwidth)+1)/(std::pow(2,bandwidth)-1));

      #if defined(DO_LAMBDA)
          #ifndef DO_LOG_SWEEP
      for (_lambda = kGaborLambdaMin; _lambda < kGaborLambdaMax;
          _lambda += (kGaborLambdaMax-kGaborLambdaMin)/((double)(nlambdas<=0?1:nlambdas))) 
      //    _lambda=2*CV_PI/std::pow(2,1);
      //for (int _lambda_c=1;_lambda_c<(nlambdas<=1?2:nlambdas+1); // http://222.146.6.210/iccs99OLP/o1-10/o1-10.htm
      //     _lambda = CV_PI/std::pow(2,++_lambda_c))
          #else
      for (_lambda = kGaborLambdaMin; _lambda < kGaborLambdaMax;
           _lambda += std::pow(10, (log(kGaborELambdaMax)-log(kGaborELambdaMin))/((double)(nlambdas<=0?1:nlambdas)) ) )
          #endif
      #elif defined(DO_SIGMA)
      for (_sigma = kGaborSigmaMin; _sigma < kGaborSigmaMax; 
          #ifndef DO_LOG_SWEEP
           _sigma += (kGaborSigmaMax-kGaborSigmaMin)/((double)(nlambdas<=0?1:nlambdas))) 
          #else
          _sigma += std::pow(10, (log(kGaborESigmaMax)-log(kGaborESigmaMin))/((double)(nlambdas<=0?1:nlambdas)) ) ) 
          #endif
      #elif defined(DO_LAMBDA_P)
      _lambda = kGaborPaperCicles[0];
      // _lambda=2*CV_PI/std::pow(2,kGaborPaperCicles[0]);
      for (int j = 0; j < (nlambdas>kGaborPaperLambdasLen?kGaborPaperLambdasLen:nlambdas); 
           j++, _lambda = /* 2*CV_PI/std::pow(2,kGaborPaperCicles[j]) */ kGaborPaperCicles[j]) 
      #endif
          {
        #if defined(DO_LAMBDA) || defined(DO_LAMBDA_P) 
        _sigma= slratio*_lambda;
        #elif defined(DO_SIGMA)
        _lambda=_sigma/slratio;
        #endif
        //int n = ( std::ceil(2.5*_sigma/_gamma) >maxfwidth? : std::ceil(2.5*_sigma/_gamma)  );
        int n = (int)std::ceil(2.5*_sigma/_gamma); 
        cv::Size kernelSize(2*n+1, 2*n+1);

        #if defined(GABOR_DEBUG)
        std::cerr<<"INFO:bandw="<<bandwidth<<",slratio="<<slratio<<",lambda="<<_lambda<<",sigma="<<_sigma<<",ksize="<<2*n-1<<""<<std::endl;
        #endif

        for (_theta = kGaborThetaMin; _theta < kGaborThetaMax;
            _theta += (kGaborThetaMax - kGaborThetaMin)/((double)(nthetas<=0?1:nthetas))) {
          emotime::GaborKernel* kern = this->generateGaborKernel(kernelSize,
              _sigma, _theta, _lambda, _gamma, _psi, CV_32F);
          bank.push_back(kern);
        }
      }
    }
  }

  void GaborBank::fillGaborBankEmpiric(double nwidths, double nlambdas, double nthetas){
    this->emptyBank();
    double _sigma;        /// Sigma of the Gaussian envelope
    double _theta;        /// Orientation of the normal to the parallel stripes of the Gabor function
    double _lambda;       /// Wavelength of sinusoidal factor
    double _gamma;        /// Spatial aspect ratio (ellipticity of the support of the Gabor function)
    double _psi;          /// Phase offset
    _gamma = kGaborGamma; 
    _sigma = kGaborSigma;
    _psi=kGaborPsi;
    int minfwidth = kGaborWidthMin;
    int maxfwidth = kGaborWidthMax;
    int fwidth = minfwidth;
    
    double _theta_step = (kGaborThetaMax - kGaborThetaMin)/((double)(nthetas<=0?1:nthetas));
    
    for (fwidth = minfwidth; fwidth < maxfwidth; fwidth += (int)
        ((maxfwidth-minfwidth)/((double)(nwidths<=0?1:nwidths)))) {
      cv::Size kernelSize(fwidth, fwidth);
      //for (_sigma = kGaborESigmaMin; _sigma < kGaborESigmaMax;
      //    _sigma += (kGaborESigmaMax-kGaborESigmaMin)/((double)(nwidths<=0?1:nwidths))) {
      _sigma = kGaborSigma;
      #if defined(DO_LAMBDA_P)
      _lambda=kGaborPaperCicles[0];
      for (int j = 0; j < (nlambdas>kGaborPaperLambdasLen?kGaborPaperLambdasLen:nlambdas);
           _lambda=kGaborPaperCicles[j++])
      #else
      #ifndef DO_LOG_SWEEP
      for ( _lambda = kGaborELambdaMin; _lambda < kGaborELambdaMax;
            _lambda += (kGaborELambdaMax-kGaborELambdaMin)/((double)(nlambdas<=0?1:nlambdas)) )
      #else
      for ( _lambda = kGaborELambdaMin; _lambda < kGaborELambdaMax;
            _lambda += std::pow(10,(log(kGaborELambdaMax)-log(kGaborELambdaMin))/((double)(nlambdas<=0?1:nlambdas)) ) )
      //_lambda=CV_PI/std::pow(2,1);
      //for ( int _lambda_c=1; _lambda_c<(nlambdas<=1?2:nlambdas+1);  // http://222.146.6.210/iccs99OLP/o1-10/o1-10.htm
      //      _lambda = CV_PI/std::pow(2,++_lambda_c))
      #endif
      #endif
      {
         #if defined(GABOR_DEBUG)
         std::cerr<<"INFO:lambda="<<_lambda<<",sigma="<<_sigma<<",ksize="<<fwidth<<""<<std::endl;
         #endif
         _theta=kGaborThetaMin;
         for ( int _theta_c=0; _theta_c < nthetas;
               _theta += _theta_step, _theta_c++) {
             emotime::GaborKernel* kern = this->generateGaborKernel(kernelSize,
                                          _sigma, _theta, _lambda, _gamma, _psi, CV_32F);
             bank.push_back(kern);
         }
       }
     }
   }

    void GaborBank::fillDefaultGaborrBank() {
      this->fillGaborBank(kGaborDefaultNwidth,
                          kGaborDefaultNlambda, 
                          kGaborDefaultNtheta);
    }

    Size GaborBank::getFilteredImgSize(cv::Mat & src) {
      // The output image will contain all the filtered image vertically stacked.
      Size s=src.size();
      return this->getFilteredImgSize(s);
    }
    Size GaborBank::getFilteredImgSize(cv::Size & size) {
      // The output image will contain all the filtered image vertically stacked.
      Size s = Size(0,0);
      #ifndef GABOR_SHRINK
      s.height = (int) (size.height * bank.size());
      #else
      s.height = size.height;
      #endif
      s.width = size.width;
      return s;
    }

    Mat GaborBank::filterImage(cv::Mat& src) {
      Size s=Size(src.cols, src.rows);
      return filterImage(src, s);
    }
    
    Mat GaborBank::filterImage(cv::Mat & src, cv::Size & featSize) {
      unsigned short int type = CV_32F;  // CV_8U will break svm detectors
      if (src.size().width ==0 || src.size().height==0){
        std::cerr<<"[!] cannot filter image (size="<<src.size()<<")"<<std::endl;
        return Mat();
      }
      Size bankSize=this->getFilteredImgSize(featSize);
      Mat dest = Mat::zeros(bankSize.height, bankSize.width, type);
      Mat image;
      #ifdef GABOR_DEBUG
      std::cerr<<"[-] resizing image "<<image.cols<<"x"<<image.rows<<
                 " to "<<featSize.height<<"x"<<featSize.width<<
                 " and apply gabor filter bank"<<std::endl;
      #endif
      src.convertTo(image, type);
      resize(image, image, featSize, CV_INTER_AREA);
      for (unsigned int k = 0; k < bank.size(); k++) {
        emotime::GaborKernel * gk = bank.at(k);
        Mat real = gk->getReal();
        Mat freal = Mat::zeros(image.size().height, image.size().width, type);
        filter2D(image, freal, type, real);
        #ifndef GABOR_ONLY_REAL
        Mat scaled = freal;
        #else
        Mat imag = gk->getImag();
        Mat fimag = Mat::zeros(image.size().height, image.size().width, type);
        Mat magn  = Mat::zeros(image.size().height, image.size().width, type);
        filter2D(image, fimag, type, imag);
        pow(freal,2,freal);
        pow(fimag,2,fimag);
        add(fimag,freal,magn);
        sqrt(magn,magn);
        Mat scaled = magn;
        #endif
        #ifndef GABOR_SHRINK
        for (unsigned int i = 0; i<(unsigned int) featSize.height; i++) {
          for (unsigned int j = 0; j<(unsigned int)featSize.width; j++) {
            if (type == CV_32F){
              dest.at<float>(i + (k * featSize.height), j) = scaled.at<float>(i,j);
            } else if (type == CV_8U){
              dest.at<char>(i + (k * featSize.height), j) = scaled.at<char>(i,j);
            }
          }
        }
        #else
        cv::max(scaled,dest,dest);
        #endif
      }
      //double min,max;
      //minMaxIdx(dest, &min, &max);
      //convertScaleAbs(dest, dest, 255/max);
      //dest.convertTo(dest, CV_8U);
      //equalizeHist(dest,dest);
      //dest.convertTo(dest, CV_32F);
      return dest;
    }

  }
