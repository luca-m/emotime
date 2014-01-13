/**
 * @class    GaborBank
 * @author
 * @date
 *
 * @brief   Representation of a gabor bank
 *
 * @details
 *
 */
#ifndef _H_GABORBANK
#define _H_GABORBANK

#include <opencv2/opencv.hpp>
#include "GaborKernel.h"

namespace emotime {

  /// Minimum width for a gabor filter
  const int kGaborWidthMin = 7;
  /// Maximum width for a gabor filter
  const int kGaborWidthMax = 17;
  /// Minimum lambda for a gabor filter
  const double kGaborLambdaMin = 3;
  /// Maximum lambda for a gabor filter
  const double kGaborLambdaMax = 12;

  const int kGaborPaperLambdas[] = {/*3,*/4,6,8,12/*,16,24*/};

  const int kGaborPaperLamdasLen = 4;

  /// Minimum tetha for a gabor filter
  const double kGaborThetaMin = 0.0;
  /// Maximum tetha for a gabor filter
  const double kGaborThetaMax = (CV_PI);

  /// Default gabor number of different with (gaborbank_getGaborBank)
  const double kGaborDefaultNwidth = 2.0;
  /// Default gabor number of different lambda (gaborbank_getGaborBank)
  const double kGaborDefaultNlambda = 3.0;
  /// Default gabor number of different theta (gaborbank_getGaborBank)
  const double kGaborDefaultNtheta = 4.0;

  class GaborBank {

    public:

      ~GaborBank();

      /**
       * @brief Genereates a bank of Gabor filter kernels with different orientations and
       *        frequencies using custom values
       *
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
      void fillGaborBank(double nwidths, double nlambdas, double nthetas);

      /**
       * @brief Generates a bank of Gabor filter kernels with different orientations
       *        and frequencies using default values
       *
       * @details This function is a wrapper for gaborbank_getCustomGaborBank(bank,
       * GABOR_DEFAULT_NWIDTH, GABOR_DEFAULT_NLAMBDA, GABOR_DEFAULT_NTHETA)
       *
       */
      void fillDefaultGaborrBank();

      /**
       * @brief Filter the input (greyscale) image with the given kernels returning
       *        the results stacked in a single image .
       *
       * @param [in]  src   the input image
       * 
       * */
      cv::Mat filterImage(cv::Mat& src);

    protected:

    private:

      /// Gabor bank
      std::vector<emotime::GaborKernel*> bank;

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
       *
       * @return A gabor kernel as real and imaginary parte: a matrix of size
       *        \p ksize of \p ktype elements.
       *
       * @details Modified OpenCV implementation for output both Real and Imaginary
       *          part of the Gabor wavelet.
       *
       * @see http://en.wikipedia.org/wiki/Gabor_filter
       *
       */
      GaborKernel* generateGaborKernel(cv::Size ksize, double sigma, double theta,
          double lambda, double gamma, double psi, int ktype);

      /**
       * @brief Obtain the dimension needed for pre-allocating an image suitable for
       *        being used in gaborbank_filterImage (grayscale image only)
       *
       * @param [in] src     the input image
       *
       * @returns The size of the filtered image.
       * */
      cv::Size getFilteredImgSize(cv::Mat& src);


      /**
       *  @brief          Destroy the gabor bank
       */
      void emptyBank();
  };

}


#endif // _H_GABORBANK
