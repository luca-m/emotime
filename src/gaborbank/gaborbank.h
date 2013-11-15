/**
 *
 * */
#ifndef _H_GABORBANK
#define _H_GABORBANK


#define GABOR_WIDTH_MIN        7
#define GABOR_WIDTH_MAX        15
#define GABOR_LAMBDA_MIN       (CV_PI/32.0)
#define GABOR_LAMBDA_MAX       (CV_PI/2.0)
#define GABOR_THETA_MIN        0.001
#define GABOR_THETA_MAX        (CV_PI/2.0)
#define GABOR_DEFAULT_NWIDTH   2.0
#define GABOR_DEFAULT_NLAMBDA  3.0
#define GABOR_DEFAULT_NTHETA   4.0

#include <opencv2/opencv.hpp>

/**
 * Data structure containing the both real and imaginary part of
 * a Gabor kernel.
 * */
typedef struct GaborKern {
  //! The imaginary part
  cv::Mat real; 
  //! The imaginary part
  cv::Mat imag; 
} GaborKern;

/**
 * Gabor Kernel generator.
 *
 * Taken from OpenCV implementation and modified for output both Real and Imaginary part of the Gabor wavelet.
 *
 * See http://en.wikipedia.org/wiki/Gabor_filter for parameter detail
 *
 * */
cv::Mat gaborbank_getGaborKernel(cv::Size ksize, double sigma, double theta, double lambd, double gamma, double psi, int ktype, bool real);
/**
 * Obtain a bank of Gabor filter kernels with different orientations and frequencies using custom values
 *
 * See http://en.wikipedia.org/wiki/Gabor_filter for parameter detail
 * 
 * @param bank vector of kernel to fill.
 * @param nwidths
 * @param nlambdas
 * @param nthetas
 * */
void gaborbank_getCustomGaborBank(std::vector<struct GaborKern *> & bank, double nwidths, double nlambdas, double nthetas); 
/**
 * Obtain a bank of Gabor filter kernels with different orientations and frequencies using default values
 * 
 * @param bank vector of kernel to fill.
 * */
void gaborbank_getGaborBank(std::vector<struct GaborKern *> & bank);
/**
 * Obtain the dimension needed for pre-allocating an image suitable for being used in "gaborbank_filterImage"  
 * NOTE: GRAYSCALE IMAGE ONLY
 *
 * @param src 		the input image
 * @param bank 		the vector of filters kernel
 * */
cv::Size gaborbank_getFilteredImgSize(cv::Mat & src, std::vector<struct GaborKern *> & bank );
/**
 * Filter the input image with all the kernel specified in the bank parameter, 
 * then write all result in a single output image. 
 * NOTE: GRAYSCALE IMAGE ONLY
 *
 * @param src 		the input image
 * @param bank 		the vector of filters kernel
 * @param dest 		The output image
 * 
 * */
cv::Mat gaborbank_filterImage(cv::Mat & src, std::vector<struct GaborKern *> & bank );


#endif // _H_GABORBANK
