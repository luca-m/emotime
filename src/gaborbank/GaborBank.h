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

#define GABOR_FORMULA
#define DO_SIGMA
//#define DO_LAMBDA_P
#define GABOR_DEBUG

#include <opencv2/opencv.hpp>
#include "GaborKernel.h"


namespace emotime {

  /* Gabor Formula consts */

  /// Minimum width for a gabor filter
  const double kGaborBandwidthMin = 1.3;
  /// Maximum width for a gabor filter
  const double kGaborBandwidthMax = CV_PI/2.;//1.6;
  /// Minimum lambda for a gabor filter
  const double kGaborLambdaMin = /*16;//4;*/1./(CV_PI/2.0);
  /// Maximum lambda for a gabor filter
  const double kGaborLambdaMax = /*48;//12;*/1./(CV_PI/32.0);
  /// Minimum lambda for a gabor filter
  const double kGaborSigmaMin = 1.0;
  /// Maximum lambda for a gabor filter
  const double kGaborSigmaMax = 6.0;

  /* Gabor Misc consts*/
  /// Gabor support shape parameter (0.5 ellipse .. 1 circle)
  const double kGaborGamma= 1.0;
  /// Gabor phase offset 
  const double kGaborPsi= CV_PI/2.0;

  /// Minimum tetha for a gabor filter
  const double kGaborThetaMin = 0.0;
  /// Maximum tetha for a gabor filter
  const double kGaborThetaMax = (CV_PI);

  const double kGaborPaperLambdas[] = {1./3, 1./4, 1./6, 1./8, 1./12, 1./16, 1./24, 1./36};
  const double kGaborPaperCicles[] = {3, 4, 6, 8, 12, 16, 24, 36};
  const int kGaborPaperLamdasLen = 8;
  
  /* Gabor Empiric consts */

  /// Minimum width for a gabor filter
  const int kGaborWidthMin = 20;
  /// Maximum width for a gabor filter
  const int kGaborWidthMax = 26;
  
  const double kGaborELambdaMin = 3;//(CV_PI /32.);
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
  ///   
  const double kGaborSigma= 4.0;

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
      void fillGaborBankFormula(double nwidths, double nlambdas, double nthetas);
      void fillGaborBankEmpiric(double nwidths, double nlambdas, double nthetas);

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
      cv::Mat filterImage(cv::Mat& src, cv::Size & featSize);

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
      cv::Size getFilteredImgSize(cv::Size& size);


      /**
       *  @brief          Destroy the gabor bank
       */
      void emptyBank();
  };

}


#endif // _H_GABORBANK
