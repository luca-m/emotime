/**
 *
 * */
#ifndef _H_GABORBANK
#define _H_GABORBANK

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
 * Obtain a bank of 40 Gabor filter kernels with different orientations and frequencies
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
