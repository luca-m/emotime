/**
 *
 * @file    GaborBank.h
 * @author
 * @date
 * @brief   Contains class and constant for generation of gabor banks.
 *
 */

#ifndef _H_GABORBANK
#define _H_GABORBANK

/// If defined, uses the mathematical correlation between the parameters (references below).
/// Otherwise an empirical value of lambda is used.
#define GABOR_FORMULA
/// Uses sigma as independent value in gabor bank calculation
#define DO_SIGMA
/// Uses lambda as independent value in gabor bank calculation
//#define DO_LAMBDA
/// Uses lambdas from paper  value in gabor bank calculation (both formula and empiric)
//#define DO_LAMBDA_P 
/// Uses lambdas linear sweep (empiric only, default logarithmic sweep)
//#define DO_LAMBDA_L


//#define DO_LAMBDA_P
//#define GABOR_DEBUG

#include <opencv2/opencv.hpp>
#include "GaborKernel.h"


namespace emotime {

  /* Gabor Formula consts */

  /// Minimum bandwidth for a gabor filter
  const double kGaborBandwidthMin = 1.3;
  /// Maximum bandwidth for a gabor filter
  const double kGaborBandwidthMax = CV_PI/2.;//1.6;
  /// Minimum wavelength for a gabor filter
  const double kGaborLambdaMin = /*16;//4;*/1./(CV_PI/2.0);
  /// Maximum wavelength for a gabor filter
  const double kGaborLambdaMax = /*48;//12;*/1./(CV_PI/32.0);
  /// Minimum sigma for a gabor filter
  const double kGaborSigmaMin = 1.0;
  /// Maximum sigma for a gabor filter
  const double kGaborSigmaMax = 6.0;
  /// Minimum orientation for a gabor filter
  const double kGaborThetaMin = 0.0;
  /// Maximum orientation for a gabor filter
  const double kGaborThetaMax = (CV_PI);

  /* Gabor Misc consts*/

  /// Gabor support shape parameter (0.5 ellipse .. 1 circle)
  const double kGaborGamma= 0.9;
  /// Gabor phase offset
  const double kGaborPsi= CV_PI/2.0;

  /// Lambda values suggested in the paper. Used only if DO_LAMBDA_P is enabled
  /// and DO_SIGMA is disabled.
  const double kGaborPaperLambdas[] = {1./3, 1./4, 1./6, 1./8, 1./12, 1./16, 1./24, 1./36};
  /// Length of kGaborPaperLambdas
  const int kGaborPaperLamdasLen = 8;
  /// Unused
  const double kGaborPaperCicles[] = {3, 4, 6, 8, 12, 16, 24, 36};
  // Gabor Filter threads to be used in multithreaded mode
  const double kGaborFilterThreads = 3;

  /* Gabor Empiric consts */

  /// Minimum width for a gabor filter
  const int kGaborWidthMin = 20;
  /// Maximum width for a gabor filter
  const int kGaborWidthMax = 26;

  /// Empirical minimum value of wavelength
  const double kGaborELambdaMin = 3;//(CV_PI /32.);
  /// Empirical maximum value of wavelength
  const double kGaborELambdaMax = 32;//(CV_PI /2.);

  
  /// Default gabor number of different with (gaborbank_getGaborBank)
  const double kGaborDefaultNwidth = 1.0;
  /// Default gabor number of different lambda (gaborbank_getGaborBank)
  const double kGaborDefaultNlambda = 5.0;
  /// Default gabor number of different theta (gaborbank_getGaborBank)
  const double kGaborDefaultNtheta = 8.0;
  /// Minimum sigma for a gabor filter
  const double kGaborESigmaMin = 1.0;
  /// Maximum sigma for a gabor filter
  const double kGaborESigmaMax = 5.0;
  /// Empirical value for Sigma
  const double kGaborSigma= 2.6;
  /// Empirical value for Lambda
  const double kGaborLambda= CV_PI/4.0;

  /**
   * @class    GaborBank
   * @author
   * @date
   *
   * @brief   Representation of a gabor bank.
   *
   * @details A GaborBank is a collection of gabor filters, with different
   *          bandwidth, size, sigma, wavelength (lambda) and orientation (theta).
   *          Parameters are subdivided between a minimum value and a maximum value.
   *          At compile time it can be configured to use different approaches to generate
   *          the bank:
   *            - GABOR_FORMULA: enables the use of the mathematical
   *              correlation between the parameters using bandwidth, theta and
   *              lambda or sigma as independent values. Otherwise an empirical
   *              value of sigma (kGaborSigma) it's used and different values of
   *              size, lambda and theta are used.
   *            - DO_SIGMA: uses sigma as independent value, Otherwise lambda is
   *              used. If lambda is used and DO_LAMBDA_P is defined,
   *              kGaborPaperLambdas are used.
   *
   */
  class GaborBank {

    public:

      ~GaborBank();

      /**
       * @brief If GABOR_FORMULA is defined, it calls fillGaborBankFormula,
       *        otherwise it calls fillGaborBankEmpiric.
       *
       * @param [in]  nwidthsOrBandwith    Depends on the method called
       * @param [in]  nlambdasOrSigma      Number of different wavelengths or sigmas (depends on DO_SIGMA)
       * @param [in]  nthetas              Number of different orientations
       *
       */
      void fillGaborBank(double nwidthsOrBandwith, double nlambdasOrSigma, double nthetas);

      /**
       * @brief Genereates a bank of Gabor filter kernels with different bandwidth, orientations and
       *        wavelength using custom values.
       *
       * @param [in]     nBandwith    Number of different bandwidth
       * @param [in]     nlambdasOrSigma   Number of different wavelengths or sigma (depends on DO_SIGMA).
       * @param [in]     nthetas    Number of different orientations
       *
       * @details This function generates \p nwidths * \p nlambdasOrSigma * \p nthetas
       *          kernels by partitioning the ranges [kGaborBandwidthMin, kGaborBandwidthMax],
       *          [kGaborLambdaMin, kGaborLambdaMax] and [kGaborThetaMin, kGaborThetaMax]. If
       *          DO_SIGMA is defined, the range [kGaborSigmaMin,
       *          kGaborSigmaMax] is partitioned instead of [kGaborLambdaMin, kGaborLambdaMax].
       *
       *
       * @see http://en.wikipedia.org/wiki/Gabor_filter
       * @see "Dynamics of facial expression extracted automatically from video" section 2
       * @see "Distortion Invariant Object Recognition in the Dynamic Link Architecture" section 3.H
       * @see "Jesper Juul Henriksen Thesis" at page 22  (http://covil.sdu.dk/publications/jespermaster07.pdf)
       *
       */
      void fillGaborBankFormula(double nBandwith, double nlambdasOrSigma, double nthetas);

      /**
       * @brief Genereates a bank of Gabor filter kernels with different size, orientations and
       *        wavelength using custom values
       *
       * @param [in]     nwidths    Number of different dimension
       * @param [in]     nlambdas   Number of different wavelengths
       * @param [in]     nthetas    Number of different orientations
       *
       * @details This function uses 
       */
      void fillGaborBankEmpiric(double nwidths, double nlambdas, double nthetas);

      /**
       * @brief Generates a bank of Gabor filter kernels with different orientations
       *        and frequencies using default values
       *
       * @details This function is a wrapper for fillGaborBank with parameters
       *          \p bank, kGaborDefaultNWidth, kGaborDefaultNLambda,
       *          kGaborDefaultNTheta.
       *
       */
      void fillDefaultGaborrBank();

      /**
       * @brief Calls filterImage(cv::Mat& src, cv::Size& featsize) with
       *        featsize equals to \p src size.
       *
       * @param [in]  src   the input image
       *
       * @returns The features extracted from the image.
       */
      cv::Mat filterImage(cv::Mat& src);

      /**
       * @brief Filter the input (greyscale) image with the given kernels returning
       *        the results stacked in a single image .
       *
       * @param [in]  src   the input image
       * @param [in]  featSize final size of the returned image.
       *
       * @returns The resized features extracted from the image.
       *
       */
      cv::Mat filterImage(cv::Mat& src, cv::Size& featSize);

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
       * @brief Calls getFilteredImgSize(cv::Size& size) with \p src size.
       *
       * @param [in] src     the input image
       *
       * @returns The size of the filtered image.
       */
      cv::Size getFilteredImgSize(cv::Mat& src);

      /**
       * @brief Obtain the dimension needed for pre-allocating an image suitable for
       *        being used in gaborbank_filterImage (grayscale image only)
       *
       * @param [in] size     the input image size
       *
       * @returns The size of the filtered image.
       * */
      cv::Size getFilteredImgSize(cv::Size& size);


      /**
       *  @brief          Destroy the gabor bank
       */
      void emptyBank();
  };

}


#endif // _H_GABORBANK
