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

/// If defined, uses the mathematical correlation between the parameters.
/// Otherwise an empirical value of lambda is used.
//#define GABOR_FORMULA
/// If defined, uses sigma as independent value
//#define DO_SIGMA
/// If defined uses lambda as indipendend value
#define DO_LAMBDA
/// Use lambda values from http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=1384873 
//#define DO_LAMBDA_P       
/// Logarithmic sweep in cicles for lambda and sigma
//#define DO_LOG_SWEEP    // nogood

/// Some debug print
//#define GABOR_DEBUG

/// Calculate only real part instead of gabor magnitude as feature
#define GABOR_ONLY_REAL         // faster
/// Keep the maximum feature values calculated through the application of the filter 
/// bank and shrink the feature vector at the same size of the input image.
//#define GABOR_SHRINK          // nogood
/// Use opencv functions to retrieve gabor kernel (almost equal)
#define GABOR_USE_OPENCV_KERNEL // equal

#include <opencv2/opencv.hpp>
#include "GaborKernel.h"


  namespace emotime {

    /* Gabor Formula consts */

    /// Minimum bandwidth for a gabor filter
    const double kGaborBandwidthMin = 1.0;
    /// Maximum bandwidth for a gabor filter
    const double kGaborBandwidthMax = 2.0; //CV_PI/2.;//1.6;
    /// Minimum wavelength for a gabor filter
    const double kGaborLambdaMin = 4;///*16;//4;*/1./(CV_PI/6.0);
    /// Maximum wavelength for a gabor filter
    const double kGaborLambdaMax = 4;///*48;//12;*/ 1./(CV_PI/32.0);
    /// Minimum sigma for a gabor filter
    const double kGaborSigmaMin = 2.0;//1.0;
    /// Maximum sigma for a gabor filter
    const double kGaborSigmaMax = 3.0; //6.0;
    /// Minimum orientation for a gabor filter
    const double kGaborThetaMin = 0.0;
    /// Maximum orientation for a gabor filter
    const double kGaborThetaMax = (CV_PI);

    /* Gabor Misc consts*/

    /// Gabor support shape parameter (0.5 ellipse .. 1 circle)
    const double kGaborGamma= 0.5;
    /// Gabor phase offset
    const double kGaborPsi=0.0;

    /// Lambda values suggested in the paper. Used only if DO_LAMBDA_P is enabled
    /// and DO_SIGMA is disabled.
    const double kGaborPaperLambdas[] = {1./3, 1./4, 1./6, 1./8, 1./12, 1./16, 1./24, 1./36};
    const double kGaborPaperCicles[] = {3, 4, 6, 8, 12, 16, 24, 36};
    /// Length of kGaborPaperLambdas
    const int kGaborPaperLambdasLen = 8;

    /* Gabor Empiric consts */
    
    /// Empirical value for Sigma
    const double kGaborSigma= 3.1;//1.25;//CV_PI/2.;//2.0;
    /// Empirical minimum value of wavelength
    const double kGaborELambdaMin = 8.0;//10;//(CV_PI /32.);
    /// Empirical maximum value of wavelength
    const double kGaborELambdaMax = 16.0;//18;//(CV_PI /2.);
    /// Minimum sigma for a gabor filter
    const double kGaborESigmaMin = 2.0;
    /// Maximum sigma for a gabor filter
    const double kGaborESigmaMax = 4.0;
    /// Minimum width for a gabor filter
    const int kGaborWidthMin = 12;
    /// Maximum width for a gabor filter
    const int kGaborWidthMax = 20;

    
    /// Default gabor number of different with (gaborbank_getGaborBank)
    const double kGaborDefaultNwidth = 1.0;
    /// Default gabor number of different lambda (gaborbank_getGaborBank)
    const double kGaborDefaultNlambda = 5.0;
    /// Default gabor number of different theta (gaborbank_getGaborBank)
    const double kGaborDefaultNtheta = 8.0;

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
