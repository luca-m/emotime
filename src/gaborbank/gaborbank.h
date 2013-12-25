/**
 *
 * @file    gaborbank.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/04/2013 10:46:35 AM
 * @brief   Contains functions for generation of gabor filters.
 *
 * @details 
 *
 */
#ifndef _H_GABORBANK
#define _H_GABORBANK

/// Minimum width for a gabor filter
#define GABOR_WIDTH_MIN        7
/// Maximum width for a gabor filter
#define GABOR_WIDTH_MAX        15
/// Minimum lambda for a gabor filter
#define GABOR_LAMBDA_MIN       (CV_PI/32.0)
/// Maximum lambda for a gabor filter
#define GABOR_LAMBDA_MAX       (CV_PI/2.0)
/// Minimum tetha for a gabor filter
#define GABOR_THETA_MIN        0.001
/// Maximum tetha for a gabor filter
#define GABOR_THETA_MAX        (CV_PI/2.0)

/// Default gabor number of different with (gaborbank_getGaborBank)
#define GABOR_DEFAULT_NWIDTH   2.0
/// Default gabor number of different lambda (gaborbank_getGaborBank)
#define GABOR_DEFAULT_NLAMBDA  3.0
/// Default gabor number of different theta (gaborbank_getGaborBank)
#define GABOR_DEFAULT_NTHETA   4.0

#include <opencv2/opencv.hpp>

///**
// * Data structure containing the both real and imaginary part of a Gabor kernel.
// */
//typedef struct GaborKern {
//  /// The imaginary part
//  CV::mAT real;
//  /// The imaginary part
//  cv::Mat imag;
//} GaborKern;

namespace emotime{
  class GaborKernel{
    private:
      cv::Mat real;
      cv::Mat imag;
    public:
      GaborKernel(){}
      GaborKernel(cv::Mat r, cv::Mat i){
        r.copyTo(real);
        i.copyTo(imag);
      }
      ~GaborKernel(){
        real.release();
        imag.release();
      }
      cv::Mat getReal(){return real;}
      cv::Mat getImag(){return imag;} 
  };
}

/**
 * @brief: Gabor Kernel generator.
 *
 * @param [in] ksize  Desired kernel siez
 * @param [in] sigma  Sigma of the Gaussian envelope
 * @param [in] theta  Orientation of the normal to the parallel striped of a Gabor Function
 * @param [in] lambda Wavelength of the sinusoidal factor
 * @param [in] gamma  Spatial aspect ratio (ellipticity of the support)
 * @param [in] psi    Phase offset
 * @param [in] ktype  Kernel OpenVC Type (CV_32F or CV_64F)
 * @param [in] real   If true, the return value is the real part. The immaginary part it's returned otherwise.
 *
 * @return A matrix of size \p ksize of \p ktype elements, representing a Gabor
 *         kernel. If \p real is true, the real part is returned, the immaginary
 *         otherwise.
 *
 * @details Modified OpenCV implementation for output both Real and Imaginary
 *          part of the Gabor wavelet.
 *
 * @see http://en.wikipedia.org/wiki/Gabor_filter
 *
 */
cv::Mat gaborbank_getGaborKernel(cv::Size ksize, double sigma, double theta,
    double lambda, double gamma, double psi, int ktype, bool real);

/**
 * @brief Genereates a bank of Gabor filter kernels with different orientations and
 *        frequencies using custom values
 *
 * @param [in,out] bank       Vector to be filled with GaborKern
 * @param [in]     nwidths    Number of different dimension
 * @param [in]     nlambdas   Number of different wavelengths
 * @param [in]     nthetas    Number of different orientations
 *
 * @details This function generates \p nwidths * \p nlambdas * \p nthetas
 * kernels by partitioning the ranges [GABOR_WIDTH_MIN, GABOR_WITH_MAX],
 * [GABOR_LAMBDA_MIN, GABOR_LAMBDA_MAX] and [GABOR_THETA_MIN, GABOR_THETA_MAX].
 *
 * @see http://en.wikipedia.org/wiki/Gabor_filter
 * @see "Dynamics of facial expression extracted automatically from video" section 2
 * @see "Distortion Invariant Object Recognition in the Dynamic Link Architecture" section 3.H
 * @see "Jesper Juul Henriksen Thesis" at page 22  (http://covil.sdu.dk/publications/jespermaster07.pdf)
 *
 */
void gaborbank_getCustomGaborBank(std::vector<emotime::GaborKernel *> & bank,
    double nwidths, double nlambdas, double nthetas);

/**
 * @brief Generates a bank of Gabor filter kernels with different orientations
 *        and frequencies using default values
 *
 * @param [in,out] bank       Vector to be filled with GaborKern
 *
 * @details This function is a wrapper for gaborbank_getCustomGaborBank(bank,
 * GABOR_DEFAULT_NWIDTH, GABOR_DEFAULT_NLAMBDA, GABOR_DEFAULT_NTHETA)
 *
 */
void gaborbank_getGaborBank(std::vector<emotime::GaborKernel *> & bank);

/**
 * @brief Filter the input (greyscale) image with the given kernels returning
 *        the results stacked in a single image .
 *
 * @param [in]  src   the input image
 * @param [in]  bank  the GaborKern bank
 * 
 * */
cv::Mat gaborbank_filterImage(cv::Mat & src, std::vector<emotime::GaborKernel *> & bank);

#endif // _H_GABORBANK
